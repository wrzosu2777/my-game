/*  Hippo Games - 2000 (C) - http://www.prog.cz/hippo/
 *    __________________________________________________
 *   /\     __  __    _____   ______   ______   ______  \
 *   \ \   /\ \|\ \  /\_  _\ /\  __ \ /\  __ \ /\  __ \  \
 *    \ \  \ \ \_\ \ \//\ \/ \ \ \_\ \\ \ \_\ \\ \ \ \ \  \
 *     \ \  \ \  __ \  \ \ \  \ \  __/ \ \  __/ \ \ \ \ \  \
 *      \ \  \ \ \/\ \  \_\ \_ \ \ \/   \ \ \/   \ \ \_\ \  \
 *       \ \  \ \_\ \_\ /\____\ \ \_\    \ \_\    \ \_____\  \
 *        \ \  \/_/\/_/ \/____/  \/_/     \/_/     \/_____/   \
 *         \ \_________________________________________________\
 *          \/_________________________________________________/
 *                           
 *  
 *  Sub     : Implementation of HScreen class, wraper for Allegro screen
 *
 *  File    : HScreen.cpp
 *  
 *  Projekt : HAllegro
 *  
 *  Autor   : Bernard Lidicky [berny@prog.cz]
 *
 *  Datum   : 5.7.2000
 *
 */      


#include "halleg.h"
#include <string.h>
#include <stdio.h>
#include <time.h>


/* HScreen::GetStav() : 
 * NULLing all
 */
HScreen::HScreen()
{
	m_stav = 0;
	m_page1  = NULL;
	m_page2  = NULL;
	m_buffer = NULL;
	m_aabuffer = NULL;
	m_back   = NULL;
	m_w   = 0;
	m_h   = 0;
	m_bpp = 0;

	m_mouse     = false;
	m_mousebmp  = NULL;
	m_mousebmps = 0;
	m_mouseactive = 0;

	m_saves = 0;
	m_fps = 0;
	m_last = 0;
	m_repare_needed = -1;
	m_fps_draw = true;

	m_fad_auto  =  true;  // zda ma fungovat auto fadeup
	m_fad_speed  =    40; // jemnost fadingu
	m_fad_speed8 =     4; // ryclost fade v 8BPP
	m_fad_rest   =     0;  // rychlost (cas v ms, jak dlouho se ma cekat)
	m_fad_allowed =  true;


	m_last  = 1;
	m_timer = time(NULL);
}

/* HScreen::~HScreen() : 
 * empty destructor
 */
HScreen::~HScreen() 
{

}

/* HScreen::GetVisible() : 
 * returns pointer to visile BITMAP
 */
BITMAP* HScreen::GetVisible()
{
	if (m_stav == H_SCREEN_2PAGES)
		if (m_page1 == m_back) return m_page2;
		else return m_page1;
	else return screen;
}

/* HScreen::GetStav() : 
 * returns m_stav
 */
int HScreen::GetStav()
{
	return m_stav;
}

/* HScreen::Init(int w, int h, int bpp, int card) : 
 * sets set_gfx_mode and set_color_depth, creates m_back and makes black screen
 */
bool HScreen::Init(int w, int h, int bpp, int fliptype, int card, bool aabuffer)
{
	Destroy();

	// chnges resolution
#ifdef _DEBUG
	set_color_depth(32);
	m_card = GFX_DIRECTX_WIN;
   if(set_gfx_mode(GFX_DIRECTX_WIN, w, h, 0, 0) != 0)
#else
	set_color_depth(bpp);
	m_card = card;
   if(set_gfx_mode(card, w, h, 0, 0) != 0)
#endif
		return false; // failed

	m_w   = w;
	m_h   = h;
	m_bpp = bpp;


	// nastaveni modu zadni obrazovky
	switch (fliptype) {
	case H_SCREEN_2VABUF :
		FlipCreate2PB();
		break;
	case H_SCREEN_2PAGES :
		FlipCreate2P();
		break;
	case H_SCREEN_BUFFER :
	default:
		FlipCreateBB();
		break;
	}


	if (m_stav == H_SCREEN_NOINIT)
		if(!FlipCreateBB()) return false;


	set_color_conversion(COLORCONV_TOTAL);

	if (bpp == 8) {
		hprepare_pal();

		get_palette(m_pal);
		create_rgb_table(&m_rgb_map, m_pal, NULL);
		rgb_map = &m_rgb_map;

		set_trans_blender(0, 0, 0, 128);
		create_blender_table(&m_color_map, m_pal, NULL);
		color_map = &m_color_map;
	}

	// filling screen by black..
	clear_to_color(m_back, makecol(0, 0, 0));
	Flip();
	clear_to_color(m_back, makecol(0, 0, 0));

	m_faded = false;

	if (aabuffer) {
		if((m_aabuffer = create_bitmap(w/2, h/2)) == NULL) return false;
	}
	else m_aabuffer = NULL;


	// it's done :)
	return true;
}

/* HScreen::Flip() : 
 * makes visible what is in m_back
 */
void HScreen::Flip(bool VSYNC)
{
		// restore obrazovky
	if (m_repare_needed == 0) {
		if (m_stav == H_SCREEN_2PAGES) Repare();
		else ToBack();
	}
	if (m_repare_needed > -1) {
		m_repare_needed--;
		return;
	}

//	if (IsIconic(m_hwnd))
//		return;

	if (m_stav == 0)
#ifdef _DEBUG
	{
		allegro_message("Zkusili jste zavolat proceduru flip bez predchozi uspesne inicializace");
		return;
	}
#else
		return;
#endif


	if (m_faded && m_fad_auto && m_fad_allowed) {
		FadeUp();
		return;
	}


	// kresleni mysi
	if (m_mouse /*&& m_mousebmp*/) {
//		poll_mouse();
		draw_sprite(m_back, m_mousebmp+sizeof(BITMAP *)*m_mouseactive, mouse_x, mouse_y);
//		show_mouse(m_back);
	}

	// ulozeni obrazku
//	if(key[KEY_C] && key[KEY_ALT]) SaveScreen(NULL);


	// update FPS
	m_last++;
	int dt = time(NULL) - m_timer;
	if (dt > 0) {
		m_fps = m_last / dt;
		m_timer += dt;
		m_last = 0;
	}
	if (m_fps_draw) 
		textprintf_ex(m_back, font, m_w-80, 25, makecol(200,200,200), -1, "%.2f FPS",m_fps);



	switch (m_stav) {
	// page flipping with back buffer
	case H_SCREEN_2VABUF :
		acquire_bitmap(m_activepage); // zrychli praci s obrazovkou
		draw_sprite(m_activepage, m_buffer, 0, 0);
		release_bitmap(m_activepage);

		// pockani na vertikalni zatmeni
		if (VSYNC) vsync();

		show_video_bitmap(m_activepage);

		if (m_activepage == m_page1) m_activepage = m_page2;      
		else m_activepage = m_page1;
		break;

	// page flipping mode
	case H_SCREEN_2PAGES :
		// pockani na vertikalni zatmeni
		if (VSYNC) vsync();

		show_video_bitmap(m_back);

		if (m_back == m_page1) m_back = m_page2;      
		else m_back = m_page1;



		break;
    // 1 back buffer mode
	case H_SCREEN_BUFFER :
		acquire_screen(); // zrychli praci s obrazovkou

		// pockani na vertikalni zatmeni
		if (VSYNC) vsync();
		draw_sprite(screen, m_buffer, 0, 0);
//		hemboss(screen, m_buffer);

		release_screen();
		break;
	}


	show_mouse(NULL);
	poll_mouse();



//	clear(m_back);

	return;
}

/* HScreen::Destroy() : 
 * destroy all alocated bitmaps
 */
void HScreen::Destroy()
{
	if (m_page1 != NULL) {
		destroy_bitmap(m_page1);
		m_page1 = NULL;
	}

	if (m_page2 != NULL) {
		destroy_bitmap(m_page2);
		m_page2 = NULL;
	} 

	if (m_buffer != NULL) {
		destroy_bitmap(m_buffer);
		m_buffer = NULL;
	}

	if (m_aabuffer != NULL) {
		destroy_bitmap(m_aabuffer);
		m_aabuffer = NULL;
	}

	m_back = NULL;
	m_stav = 0;
}

/* HScreen::SaveScreen(char *file) : 
 * ulozi obrazovku do souboru
 * small - zda se ma ulozit 320*240, ci 640 na 480
 */
void HScreen::SaveScreen(char *file)
{
	char mfile[50];
	if (file != NULL) 
		strcpy(mfile, file);
	else 
		sprintf(mfile,"myal%.4d.bmp",m_saves++);

	PALETTE pal;
	get_palette(pal);
	save_bitmap(mfile, screen, pal);
}

/* HScreen::Repare() : 
 * pri alt+tab se muze porouchat jeden surface - snaha o opraveni
 * problem ve win32
 */
void HScreen::Repare()
{
//	if (m_stav != H_SCREEN_2PAGES) return;

	if (m_page1 != NULL) destroy_bitmap(m_page1);
	if (m_page2 != NULL) destroy_bitmap(m_page2);
	m_page1 = create_video_bitmap(m_w, m_h);
	m_page2 = create_video_bitmap(m_w, m_h);
	m_back = m_page1;
	
	if (m_page1 == NULL || m_page2 == NULL) ToBack();
	m_repare_needed = false;
}

/* HScreen::ToBack() : 
 * prepne do modu s zadni bitmapou
 */
void HScreen::ToBack()
{
	if (m_buffer != NULL) destroy_bitmap(m_buffer);
   m_buffer = create_bitmap(m_w, m_h);
	m_stav = H_SCREEN_BUFFER;
	m_back = m_buffer;
	m_repare_needed = false;
}

/* HScreen::Install() : 
 * nastavy switch call-back
 */
void HScreen::Install()
{
	// pro switch calback
	hscreen_self_pointer = this;
	set_display_switch_callback(SWITCH_IN, SwitchCallBack);

	// jpg registrace
	register_bitmap_file_type("JPG", load_jpg, NULL);

//	m_hwnd = win_get_window();
}

/* HScreen::SwitchCallBack() : 
 * zpracovani callbacku - restor obrazovek
 */
void HScreen::SwitchCallBack()
{
	hscreen_self_pointer->m_repare_needed = 10;
}

/* HScreen::AcquireBack() : 
 * zastavi zadni bitmapu - vhodne pri pouziti videobitmap - nutne pouzit spolu s ReleaseBack
 */
void HScreen::AcquireBack()
{
//	acquire_bitmap(m_back);
}

/* HScreen::ReleaseBack() : 
 * releasne zadni bitmapu
 */
void HScreen::ReleaseBack()
{
//	release_bitmap(m_back);
}

/* HScreen::FadeDown()
 * zcerna obrazovku
 */
void HScreen::FadeDown()
{
	if (!m_fad_allowed) return;

	if (m_bpp == 8) 
		FadeDown8BPP();
	else
		FadeDownTruecolor();

	m_faded = true;
}

/* HScreen::FadeDown()
 * zpatky vysvetli obrazovku obrazovku
 */
void HScreen::FadeUp()
{
	m_faded = false;

	if (m_fad_allowed) {
		if (m_bpp == 8) 
			FadeUp8BPP();
		else
			FadeUpTruecolor();
	}
	else
		Flip();
}

void HScreen::FadeDownTruecolor()
{
	BITMAP *tmp = create_bitmap(640, 480);

	acquire_screen();
	blit(screen, tmp, 0, 0, 0, 0, 640, 480); // nemuzeme blitnout 
	release_screen();
	
	int l = m_fad_speed;
	while (l < 256) {
		AcquireBack();
		fade_bitmap(m_back, tmp, 0, 0, l);
		ReleaseBack();
		l += m_fad_speed;
		Flip();
		rest(m_fad_rest);
	}

	// pro jistotu az do cerna
	AcquireBack();
	clear_to_color(m_back, makecol(0,0,0));
	ReleaseBack();
	Flip();
	AcquireBack();
	clear_to_color(m_back, makecol(0,0,0));
	ReleaseBack();
}

void HScreen::FadeDown8BPP()
{
	int col = m_rgb_map.data[0][0][0];
	fade_out(m_fad_speed8);
	vsync();
	clear_to_color(GetVisible(), col);
	clear_to_color(m_back, col);
	set_palette(m_pal);
}

void HScreen::FadeUpTruecolor()
{
	BITMAP *tmp = create_bitmap(m_w, m_h);

	AcquireBack();
	blit(m_back, tmp, 0, 0, 0, 0, m_w, m_h);
	ReleaseBack();

	int l = 255;
	while (l > 0) {
		fade_bitmap(m_back, tmp, 0, 0, l);
		l -= m_fad_speed;
		Flip();
	}


	AcquireBack();
	blit(tmp, m_back, 0, 0, 0, 0, m_w, m_h);
	ReleaseBack();
	Flip();
	/*
	AcquireBack();
	blit(tmp, m_back, 0, 0, 0, 0, m_w, m_h);
	ReleaseBack();
	*/
}

void HScreen::FadeUp8BPP()
{
	Flip();
	fade_in(m_pal, m_fad_speed8);
	set_palette(m_pal);
}

void HScreen::SetFade(int speed, int rest, int speed8)
{
	m_fad_speed   = speed;
	m_fad_rest    = rest;
	m_fad_speed8  = speed8;
}

void HScreen::AABufToBack()
{
#ifdef _DEBUG
	if (m_aabuffer == NULL) {
		allegro_message("You tryed to call HScreen::AABufToBack() without initing m_aabuffer - see Init()");
		return;
	}
#endif
/*	if (key[KEY_R]) {
		aa_stretch_sprite(m_back, m_aabuffer, 0, 0, 640, 480);
		return;
	}
*/
	if (m_aa_enabled)
		hanti_alias(m_back, m_aabuffer);
	else
		stretch_sprite(m_back, m_aabuffer, 0, 0, m_w, m_h);

}

/* HScreen::FlipCreateBB()
 * vytvori back buffer
 */
bool HScreen::FlipCreateBB()
{
	m_buffer = create_bitmap(m_w, m_h);

	if (m_buffer == NULL)
		return false;   // failed in creating back buffer

	m_back = m_buffer;
	m_stav = H_SCREEN_BUFFER;
	return true;	
}

bool HScreen::FlipCreate2P()
{
	// zkusime videopamet pro flipping
   m_page1 = create_video_bitmap(m_w, m_h);
   m_page2 = create_video_bitmap(m_w, m_h);

   if ((m_page1 == NULL) || (m_page2 == NULL)) 
	   return false;

	m_back = m_page2;
	m_stav = H_SCREEN_2PAGES;
	return true;
}

bool HScreen::FlipCreate2PB()
{
   m_page1 = create_video_bitmap(m_w, m_h);
   m_page2 = create_video_bitmap(m_w, m_h);
   m_buffer = create_bitmap(m_w, m_h);

   if ((m_page1 == NULL) || (m_page2 == NULL) || (m_buffer == NULL)) 
	   return false;

	m_activepage = m_page2;
	m_back = m_buffer;
	m_stav = H_SCREEN_BUFFER;
	return true;

}

void HScreen::UpdateMouse()
{
	if (++m_mouseactive >= m_mousebmps) m_mouseactive = 0;
}

void HScreen::SetMouse(int bitmap)
{
	if((m_mouseactive = bitmap) > m_mousebmps) m_mouseactive = 0;
}

void HScreen::Set2Pages()
{
	if (m_stav == H_SCREEN_2PAGES) return;

	m_stav = H_SCREEN_2PAGES;

	if (screen == m_page1) m_back = m_page2;      
	else m_back = m_page1;

}

void HScreen::SetBBuffer()
{
	if (m_stav == H_SCREEN_BUFFER) return;

	if (m_back == screen) {
		bool m = m_mouse;
		m_mouse = false;
		Flip();      
		m_mouse = m;
	}

	m_stav = H_SCREEN_BUFFER;

	m_back = m_buffer;
}


void HScreen::DFlip()
{
}

void HScreen::ActualHBitmap()
{

}

void HScreen::ReleaseHBitmap()
{

}
