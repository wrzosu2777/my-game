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
 *  Sub     : Include funkci obrazovky
 *
 *  File    : HScreen.cpp
 *  
 *  Projekt : HAllegro
 *  
 *  Autor   : Bernard Lidicky [berny@prog.cz]
 *
 *  Datum   : 9.7.2000
 *
 */      

#ifndef _HALLEG_SCREEN_
#define _HALLEG_SCREEN_

#include <allegro.h>
/*
#include <winalleg.h>
#include <windows.h>  
#define DIRECTDRAW_VERSION 0x0300
#include <ddraw.h>
*/
// #include <allegro.h> // allegro - pro jistotu

#define H_SCREEN_NOINIT  0  // neinicializovano
#define H_SCREEN_BUFFER  1  // mod se zadnim bufferem
#define H_SCREEN_2PAGES  2  // mod se dvemmi video strankami 
#define H_SCREEN_2VABUF  3  // dve videostranky a zadni buffer


class HScreen  
{
public:
	void SetBBuffer();
	void Set2Pages();
	void SetMouse(int bitmap);
	void UpdateMouse();
	HScreen();
	virtual ~HScreen();

	void AABufToBack();
	void SetFade(int speed, int rest, int speed8);
	void FadeUp();
	void FadeDown();
	void ReleaseBack();
	void AcquireBack();
	void Install();
	void SaveScreen(char *file);
	void Destroy();
	void Flip(bool VSYNC = false);
	bool Init(int w = 640, int h = 480, int bpp = 16, int fliptype = 1, int card = GFX_AUTODETECT, bool aabuffer = false);

	int GetStav();

	BITMAP* GetVisible(); // viditelna   stranka
	BITMAP* m_back;       // neviditelna stranka
	BITMAP* m_aabuffer;   // bitmapa na antialiasing - ma POLOVICNI rozmery opritu obrazovce

	/// aby se pokazdy nemuselo volat to sileny makro
	int   m_w;   // sirka obrazovky
	int   m_h;   // vyska obrazovky
	int   m_bpp; // barevna hloubka
	int   m_card;

	bool  m_fps_draw; // zda se maji zobrazovat FPS
	bool  m_aa_enabled; // pokud je false, tak se v AABufToBack vola stretch sprite misto antialiasingu


	bool m_mouse;        // zda se ma mysi zobrazit
	BITMAP *m_mousebmp;  // obrazek mysi
	int     m_mousebmps; // pocet obrazku mysi

	RGB_MAP   m_rgb_map; // rgb mapa palety (tvori se automaticky)
	COLOR_MAP m_color_map;
	PALETTE m_pal;     // aktualni palete

	bool m_fad_auto;    // zda ma fungovat auto fadeup
	bool m_fad_allowed; // pokud date false, zadna fade rutina nebude fungovat ( pokud budete chtit ve hre mit moznost vypnout fading, staci zmenit tuhle promenou )
	int  m_fad_speed;   // jemnost fadingu
	int  m_fad_speed8;  // ryclost sednuti v 8BPP - (1 slowest - 64 total)
	int  m_fad_rest;    // rychlost (cas v ms, jak dlouho se ma cekat)


private:
   BITMAP *m_page1, *m_page2; // dve stranky na page flipping
	BITMAP *m_activepage;      // ukazatel na aktivni stranku pri 2PB
	BITMAP *m_buffer;          // pokud se nazdari, pouziva se 1 zadni buffer

	int     m_mouseactive;     // aktualni obrazek mysi

	int     m_stav;				// zda jiz bylo inicialozovano, a jaky rezim bez (back buffer, two video pages)
	int     m_saves;           // pocet ulozeni do souboru
	int     m_last;            // pro pocitani fps - pocet kreslenych snimku
	long    m_timer;
	double  m_fps;             // frames per second

private:
	bool FlipCreate2PB();
	bool FlipCreateBB();
	bool FlipCreate2P();

	void static SwitchCallBack();
	int  m_repare_needed; // zda je potreba Restore()

	// volaji se v zavislosti na barevne hloubce
	void FadeUpTruecolor();
	void FadeUp8BPP();
	void FadeDown8BPP();
	void FadeDownTruecolor();
	bool m_faded; // zda je obrazovka zacernala

	// opraveni videopameti
	void Repare();
	void ToBack();

	// DIRECT X
public:
	void ReleaseHBitmap();
	void ActualHBitmap();

//	HBITMAP  m_hbitmap;
//	HDC      m_hdc;


	void DFlip();
/*	
	LPDIRECTDRAW         m_ddraw;
   LPDIRECTDRAWSURFACE  m_dscreen;
	LPDIRECTDRAWSURFACE  m_dback;
*/
//	HWND  m_hwnd;
	int m_hwnd;

};

extern HScreen *hscreen_self_pointer;


#endif // #ifndef _H_SCREEN_
