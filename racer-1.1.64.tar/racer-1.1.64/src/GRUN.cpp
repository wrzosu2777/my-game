// GRun.cpp: implementation of the GRun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdh.h"

#include "TButton.h"

#include "GCar.h"
#include "GView.h"
#include "GObject.h"
#include "GFile.h"
#include "GMap.h"
#include "GRun.h"
#include "trate.h"

#include "lang.h"

#include <iostream>
using namespace std;

extern	HFONT   g_font12B;
extern	HFONT   g_font12;

/*////////////////////////////////////////////////////////////////////////////////
	NULL-ovaci konstruktor
*/
GRun::GRun()
{
	int i;
	m_game_speed = 2; // rychlost 50 snimku za sekundu

	b_obris = NULL;

	for (i = 0; i < MAX_SMOGS_BITMAPS; i++)
		b_smogs[i] = NULL;

	for (i = 0; i < MAX_OBJECTS_BITMAPS; i++) 
		b_objects[i] = NULL;

	NullBitmaps(&m_tabs[0], MAX_TABS);
	NullBitmaps(&b_kola[0], MAX_KOL_BMP);

	m_carsc = 0;
	m_stin  = true;
	m_bonusy_enabled = false;
	m_vyrazovacka = false;

	m_map.m_run = this;

	for(i = 0; i < MAX_RACING_CARS; i++)
			m_cars[i].m_map =  &m_map;

}

/*////////////////////////////////////////////////////////////////////////////////
	Rusi ruzne dinamicke obekty
*/
GRun::~GRun()
{
	Destroy();
}

/*////////////////////////////////////////////////////////////////////////////////
	Hlavni fce, diky ketre bezi samotna hra ( vsechno to kloubi dohromady )
*/
void GRun::Run()
{
	LogMessage("GRun::Run()...\n");

	if (DF_SHADOWDISABLE)
		m_stin = false;

   clear_keybuf();
	hscreen.m_mouse = false;
	m_semafor = SEMAFOR_CERVENA;

	m_paused  = false;
	m_dokncen = false;
	bool m_exit = false;
	do {

		// smazeme log file
//		FILE *f = fopen("racer.log", "wt");
//		fclose(f);

//		LogMessage("Start Loop\n");

		tmr = 4; // 40 ms; -> 25 refreshu za sekundu

		// zobrazeni
//		LogMessage("---- Drawing ----\n");
		Draw();

//		LogMessage("---- Sound Update ----\n");
		UpdateSound();

		if (!m_paused && !m_dokncen) {
			// nastaveni ridice
//			LogMessage("---- Input ----\n");
			Input();
			// a jedem
//			LogMessage("---- Move ----\n");
			Move();
//			LogMessage("---- Crash ----\n");
			Crash();
	
//			LogMessage("---- Timer waiting ----\n");
			while (tmr > 0) ; // Sleep(1); - LINUX - nevim, jak se dela delay

			if (m_semafor > 0) m_semafor--;

//			LogMessage("---- Test End ----\n");
			if (TestEnd()) {
				m_dokncen = true;
				ComputeFinalPos();
			}
			else
				if (key[KEY_ESC]) m_paused = true;
		} 
		else {
			if (m_paused) {
				if (key[KEY_A] || key[KEY_Y]) m_exit = true;
				if (key[KEY_N]) m_paused = false;
			}
			else
				if (key[KEY_ESC]) m_exit = true;
		}

//		LogMessage("End Loop\n");

   } while (!m_exit);
	SimulateRaceToEnd();
	ComputeFinalPos();

	LogMessage("GRun::Run() OK\n");
}

/*////////////////////////////////////////////////////////////////////////////////
	Rusi vsechny bitmapy
*/
void GRun::Destroy()
{
	int i;

	// Ruseni aut
	for (i = 0; i < MAX_CARS_BITMAPS; i++) {
		if (b_cars[i] != NULL) destroy_bitmap(b_cars[i]);
		b_cars[i] = NULL;
	}
	for (i = 0; i < MAX_CARS_POHAR_BITMAPS; i++) {
		if (b_carsPohar[i] != NULL) destroy_bitmap(b_carsPohar[i]);
		b_carsPohar[i] = NULL;
	}

	
	DestroyBitmaps(&m_tabs[0], MAX_TABS);
	DestroyBitmaps(&b_kola[0], MAX_KOL_BMP);


	if (b_obris != NULL) destroy_bitmap(b_obris);
	b_obris = NULL;
	// ruseni smogu
	for (i = 0; i < MAX_SMOGS_BITMAPS; i++) {
		if (b_smogs[i] != NULL) destroy_bitmap(b_smogs[i]);
		b_smogs[i] = NULL;
	}
	for (i = 0; i < MAX_SMOGS_CBITMAPS; i++) {
		if (b_smogsC[i] != NULL) destroy_bitmap(b_smogsC[i]);
		b_smogsC[i] = NULL;
	}

	for (i = 0; i < MAX_SMYK_BMP; i++) {
		if (b_smyk[i] != NULL) destroy_bitmap(b_smyk[i]);
		b_smyk[i] = NULL;
	}

	// Ruseni objektu
	for (i = 0; i < MAX_OBJECTS_BITMAPS; i++) {
		if (b_objects[i] != NULL) destroy_bitmap(b_objects[i]);
		b_objects[i] = NULL;
	}

	DestroyBitmaps((BITMAP**)&b_carlights, MAX_CARS_LIGHTS);

	m_view.Destroy();
	m_map.Destroy();

	for (i = 0; i < MAX_RACING_CARS; i++)
		m_cars[i].Destroy();
}

/*////////////////////////////////////////////////////////////////////////////////
	zaridi nacteni bitmap ( vraci false pri neuspechu )
*/
bool GRun::LoadGFX()
{
	LogMessage("Nacitani GRUN::GFX..");

	Destroy(); // radsi to nejrve zlikvidujem ( pro jistotu )
	int i,j;
	char s[80];

/////////////////////
//	  NACTENI AUT   //
/////////////////////
	for (i = 0; i < MAX_CARS; i++)
		for (int j = 0; j < MAX_RACING_CARS; j++) 
			if (i == m_cars[j].m_ID && b_cars[i*3] == NULL) {
				sprintf(s, "%s", Cparams[i].file);
		 		ASSERT(b_cars[i*3] == NULL);
				b_cars[i*3] = fload_bitmap(s);
				if (!b_cars[i*3]) {
					strcpy(errch, s);
					return false;
				}
				sprintf(s, "%s_n", Cparams[i].file);
		 		ASSERT(b_cars[i*3+1] == NULL);
				b_cars[i*3+1] = fload_bitmap(s);
				if (!b_cars[i*3+1]) {
					strcpy(errch, s);
					return false;
				}
				CreateShadow(i);
				if (Cparams[i].poharb != -1) {
					sprintf(s, "%sp", Cparams[i].file);
			 		ASSERT(b_carsPohar[Cparams[i].poharb] == NULL);
					b_carsPohar[Cparams[i].poharb] = fload_bitmap(s);
					if (!b_carsPohar[Cparams[i].poharb]) {
						strcpy(errch, s);
						return false;
					}
					sprintf(s, "%sp_n", Cparams[i].file);
			 		ASSERT(b_carsPohar[Cparams[i].poharb+1] == NULL);
					b_carsPohar[Cparams[i].poharb+1] = fload_bitmap(s);
					if (!b_carsPohar[Cparams[i].poharb+1]) {
						strcpy(errch, s);
						return false;
					}
				}
			}

// cernoch
	ASSERT(b_obris == NULL);
	b_obris = create_bitmap(DF_CAR_X, DF_CAR_Y);
	clear(b_obris); // vycernani


/////////////////////////
//	  NACTENI SMOGU + smyk //
/////////////////////////
	for (i = 0; i < MAX_SMOGS_BITMAPS; i++) {
		sprintf(s, "kour%dm", i+1);
		ASSERT(b_smogs[i] == NULL);
		b_smogs[i] = fload_bitmap(s);
	   if (!b_smogs[i]) {
	      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	      cout << s << endl;
	      return false;
		}
	}

	for (i = 0; i < MAX_SMOGS_CBITMAPS; i++) {
		ASSERT(b_smogsC[i] == NULL);
		b_smogsC[i] = create_bitmap(32, 32);
	}

	clear_to_color(b_smogsC[0], makecol(128, 100, 100));
	clear_to_color(b_smogsC[1], makecol(200, 200, 150));
	clear_to_color(b_smogsC[2], makecol( 80,  80,  80));
	clear_to_color(b_smogsC[3], makecol(180, 210, 230));

	if (m_noc) for (i = 0; i < MAX_SMOGS_CBITMAPS; i++) hfade_sprite(b_smogsC[i]);

	for (i = 0; i < MAX_SMYK_BMP; i++) {
		sprintf(s, "smyk%d", i);
		ASSERT(b_smyk[i] == NULL);
		b_smyk[i] = fload_bitmap(s);
//		b_smyk[i] = create_bitmap(8, 8);
//		clear_to_color(b_smyk[i], makecol(50, 50, 50));
		if (m_noc && b_smyk[i] != NULL) hfade_sprite(b_smyk[i]);
	}

	for (i = 0; i < MAX_KOL_BMP; i++) {
		ASSERT(b_kola[i] == NULL);
		sprintf(s, "kol%d", i);
		b_kola[i] = fload_bitmap(s);
	}
///////////////////////////
//	  NACTENI SVETEL     //
/////////////////////////
	ASSERT(b_carlights[0] == NULL);
	b_carlights[0] = create_bitmap(160,160);
	ASSERT(b_carlights[1] == NULL);
	b_carlights[1] = create_bitmap(160,160);
	clear_to_color(b_carlights[1], makecol(255,255,255));
	ASSERT(b_carlights[2] == NULL);
	b_carlights[2] = fload_bitmap("clight");
   if (!b_carlights[2]) {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      cout << "Missing clight" << endl;
      return false;
	}
/////////////////////////
//	  NACTENI OBJEKTU   //
/////////////////////////
	for( i = 0; i < MAX_OBJECTS; i++)
		if (Oparams[i] != NULL && m_map.IsObjIndexInMap(Oparams[i]->INDEX)) {

			// stin
			if (Oparams[i]->type != OBJECT_LOW && Oparams[i]->type != OBJECT_NONE) {
				sprintf(s,"%s_s",Oparams[i]->file);
				ASSERT(b_objects[Oparams[i]->bindex] == NULL);
				b_objects[Oparams[i]->bindex] = fload_bitmap(s);
//				if (!b_objects[Oparams[i]->bindex]) {
//					set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
//					allegro_message("Chybi %s",s);
//					return false;
//				}
			}

			// svetlo
			if (Oparams[i]->light) {
				sprintf(s,"%s_l",Oparams[i]->file);
				ASSERT(b_objects[Oparams[i]->bindex+1] == NULL);
				b_objects[Oparams[i]->bindex+1] = fload_bitmap(s);
				if (!b_objects[Oparams[i]->bindex+1]) {
					set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
					allegro_message("Missing %s",s);
					return false;
				}
			}

			// objekt
			for (j = 0; j < Oparams[i]->anims; j++) {
				sprintf(s,"%s_%d",Oparams[i]->file, j);
				ASSERT(b_objects[Oparams[i]->bindex+2+j] == NULL);
				b_objects[Oparams[i]->bindex+2+j] = fload_bitmap(s);
				if (m_noc) hfade_sprite(b_objects[Oparams[i]->bindex+2+j]);
				if (!b_objects[Oparams[i]->bindex+2+j]) {
					set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
					allegro_message("Missing %s",s);
					return false;
				}
			}
			// rozjety
			for (j = 0; j < Oparams[i]->canims; j++) 
			if (Oparams[i]->type == OBJECT_SOFT) {
				sprintf(s,"%s_B%d",Oparams[i]->file, j);
				ASSERT(b_objects[Oparams[i]->bindex+2+Oparams[i]->anims+j] == NULL);
				b_objects[Oparams[i]->bindex+2+Oparams[i]->anims+j] = fload_bitmap(s);
				if (m_noc) hfade_sprite(b_objects[Oparams[i]->bindex+2+Oparams[i]->anims+j]);
				if (!b_objects[Oparams[i]->bindex+2+Oparams[i]->anims+j]) {
					set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
					allegro_message("Missing %s",s);
					return false;
				}
			}
		}

/////////////////////
// tabulky
		for (i =0; i < MAX_TABS; i++) {
			sprintf(s,"tab%d", i);
			ASSERT(m_tabs[i] == NULL);
			m_tabs[i] = fload_bitmap(s);
		}

/////////////////////
// nacteni animaci //
/////////////////////
	
/////////////
// Ostatni //
/////////////
	if (!m_map.LoadGFX(m_noc)) return false;
	if (!m_view.LoadGFX()) return false;
	for (i = 0; i < m_carsc; i++)
		if (!m_cars[i].LoadGFX(m_noc)) return false;

	LogMessage("OK\n");
	return true;
}

/*////////////////////////////////////////////////////////////////////////////////
	Nacte novou trat
*/
bool GRun::NewRace(const char *file)
{
	printf("\nNacitani GRun:NewRace.. %s", file);

	m_carsc = 1;
	m_cars[0].m_active = true;
	m_bonusy_enabled = false;
	m_vyrazovacka = false;

	GFile gfile;
	if(!gfile.OpenFile(file)) {
		printf("\nOppening failed\n");
		gfile.CloseFile();
		return false;
	}
	if(!gfile.LoadData(&m_map)) {
		printf("\nLoading failed\n");
		gfile.CloseFile();
		return false;
	}

	gfile.CloseFile();
	
	printf(" Loaded\n");


	return true;
}

/*////////////////////////////////////////////////////////////////////////////////
	Zajisti vykresleni (+ flip screen)
*/
void GRun::Draw()
{
	char s[100];

	acquire_bitmap(hscreen.m_back);
	m_view.UpdateCamera(this);
	m_view.Draw(this);
	if (m_hracu == 2) {
		m_view2.UpdateCamera(this);
		m_view2.Draw(this);
	}

	if (m_paused) {
		set_trans_blender(0, 0, 0, 128);
		draw_trans_sprite(hscreen.m_back, m_tabs[1], hscreen.m_w/2-m_tabs[1]->w/2, hscreen.m_h/2-m_tabs[1]->h/2);
		TTDraw(hscreen.m_back, hscreen.m_w/2, hscreen.m_h/2-10, WIN_RGB(255, 255, 255), LNG_OPRAVDUKONEC, g_font12B);
		TTDraw(hscreen.m_back, hscreen.m_w/2, hscreen.m_h/2+10, WIN_RGB(255, 255, 255), LNG_AN, g_font12B);
	}

	int i;
	if (m_dokncen) {
		set_trans_blender(0, 0, 0, 128);

		int top = hscreen.m_h/2 - 13 * m_carsc;

		draw_trans_sprite(hscreen.m_back, m_tabs[3], hscreen.m_w/2-m_tabs[3]->w/2, top-m_tabs[3]->h);

		for (i = 0; i < m_carsc; i++) {

			draw_trans_sprite(hscreen.m_back, m_tabs[0], hscreen.m_w/2-m_tabs[0]->w/2, top+i*26);

			int ind = 0;
			for (int j = 0; j < m_carsc; j++)
				if (m_cars[j].m_fpos == i) ind = j;
			//int avs = m_cars[ind].m_avs;
			//sprintf(s, "%s", m_cars[ind].m_dname, avs);
			sprintf(s, "%s", m_cars[ind].m_dname);
			if (ind < m_hracu) {
				TTDraw(hscreen.m_back, hscreen.m_w/2, top+i*26, WIN_RGB(255, 255, 0), s, g_font12B);
				TTDraw(hscreen.m_back, hscreen.m_w/2, top+i*26+10, WIN_RGB(255, 255, 0), m_cars[ind].m_par.name, g_font12);
			}
			else {
				TTDraw(hscreen.m_back, hscreen.m_w/2, top+i*26, WIN_RGB(255, 255, 255), s, g_font12B);
				TTDraw(hscreen.m_back, hscreen.m_w/2, top+i*26+10, WIN_RGB(255, 255, 255), m_cars[ind].m_par.name, g_font12);
			}
		}

		draw_trans_sprite(hscreen.m_back, m_tabs[2], hscreen.m_w/2-m_tabs[2]->w/2, top+i*26);
		TTDraw(hscreen.m_back, hscreen.m_w/2, top+i*26+10, WIN_RGB(255, 255, 255), LNG_UKONCENI, g_font12);
	}

	release_bitmap(hscreen.m_back);
	hscreen.Flip();
}

/*////////////////////////////////////////////////////////////////////////////////
	Zajisti pohnuti vsemi objkety
*/
void GRun::Move()
{
	LogMessage("map\n");
	m_map.Move();

	LogMessage("cars\n");
	if (m_semafor <= SEMAFOR_ZELENA) 
		for (int i = 0; i < m_carsc; i++)
			m_cars[i].Move(&m_map);

	LogMessage("vyrazovacka test\n");
	TestVyrazovacka();
}

/*////////////////////////////////////////////////////////////////////////////////
	Testuje narazy
*/
void GRun::Crash()
{
	int k = 0, // promena cyklu
		 l = 0; // pocet narazeni
	
	// nechame 3* pocitat narazy, nebo dokud neni bez narazu
	for( k = 0; k < 3; k++) {
   //  test auta X objekty
//		LogMessage("map crash\n");
		l = m_map.Crashig(this);
	// test mezi narazy aut
//		LogMessage("car crash\n");
		l += CarCrash();
		if (l == 0) break;
	}
}

/*////////////////////////////////////////////////////////////////////////////////
	Testuje narazy mezi auty a vraci jejich pocet
*/
int GRun::CarCrash()
{
	int l = 0;
	for(int i = 0; i < m_carsc; i++)
		for(int j = 0; j < m_carsc; j++)
			if (i != j) 
				if (m_cars[i].Crash(&m_map, &m_cars[j])) l++;
	return l;
}

/*////////////////////////////////////////////////////////////////////////////////
	Nacte pole bitmap (z LoadGFX)
*/
bool GRun::LoadBitmaps(BITMAP **pole, int kolik, int zacatek, const char *file)
{
	char s[60];
	int i;

	for( i = 0; i < kolik; i++){
		// objekt
		snprintf(s, 59, file, i+zacatek);
		ASSERT(*pole == NULL);
		*pole = fload_bitmap(s);
		if (!*pole) {
			cout << "Missing " << s << endl;
			return false;
		}
		pole++;
	}
	return true;
}

/*////////////////////////////////////////////////////////////////////////////////
	Rusi pole bitmap (z Destroy)
*/
void GRun::DestroyBitmaps(BITMAP **pole, int kolik)
{
	int i;
	for( i = 0; i < kolik; i++, pole++){
		if (*pole == NULL) continue;
		destroy_bitmap(*pole);
		*pole = NULL;
	}
}

void GRun::CreateShadow(int ID)
{
	ASSERT(b_cars[ID*3+2] == NULL);
   b_cars[ID*3+2] = create_bitmap(DF_CAR_X, DF_CAR_Y);

	int col = makecol(255, 0, 255);
	clear_to_color(b_cars[ID*3+2], col);
	for (int i = 0; i < DF_CAR_X; i++)
		for( int j = 0; j < DF_CAR_Y; j++) 
			if (getpixel(b_cars[ID*3], i, j) != col) putpixel(b_cars[ID*3+2], i, j, 0);
			
}

void GRun::GenerateMap(char *file)
{
	hscreen.Install();

	set_color_depth(16);
	screen = create_bitmap(1, 1);

//	set_gfx_mode(GFX_DIRECTX_WIN, 100, 100, 0, 0);


	if(!NewRace(file)) {
	   set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Failed loading track %s", file);
		exit(1);
	}

	m_noc = false;
	bool stin = true;

// grafika
	if(!LoadGFX()) {
	   set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Failed loading graphics");
		exit(1);
	}
	

	DF_X = m_map.m_sx; DF_Y = m_map.m_sy;
	int m_Cx = DF_X/2, m_Cy = DF_Y/2;

	BITMAP *bma, *bmb;
	bma = create_bitmap(m_map.m_sx, m_map.m_sy);
	bmb = create_bitmap(m_map.m_sx/2, m_map.m_sy/2);


	m_map.Draw(bma, bmb, m_Cx, m_Cy, stin);



	m_map.DrawObj(bma, m_Cx, m_Cy,  OBJ_HI_GROUNG, stin);
	m_map.DrawObj(bma, m_Cx, m_Cy,  OBJ_HI_LAY, stin);


 // cast nad autem
	m_map.DrawObj(bma, m_Cx, m_Cy,  OBJ_HI_NORMAL, stin);
	m_map.DrawObj(bma, m_Cx, m_Cy,  OBJ_HI_TOP, stin);
	m_map.DrawObj(bma, m_Cx, m_Cy,  OBJ_HI_FLY, stin);
	
	stretch_sprite(bmb, bma, 0, 0, m_Cx, m_Cy);

	char c[40];
	sprintf(c, "%s_gen.bmp", file);
	PALETTE pal;
	save_bmp(c, bmb, pal);


	if (bmb != NULL) destroy_bitmap(bmb);
	if (bma != NULL) destroy_bitmap(bma);
	if (screen != NULL) destroy_bitmap(screen);

	// rusni bitmap
	Destroy();
}

bool GRun::PrepareRace(bool noc, bool stin, int aut, int hracu, int kol)
{
	LogMessage("GRun::PrepareRace...\n");

	if (DF_NOCDISABLE) noc = false;

	m_noc   = noc;
	m_hracu = hracu;
	m_kol   = kol;

	LogMessage("GRun::PrepareRace...Startovni pozice hracu. \n");
	int mpos = m_map.GetMaxStartPos();
	if (aut > mpos) aut = mpos;

// auta
	int x, y, angle, i, j;

	int spos[MAX_RACING_CARS];
	for(i = 0; i < aut; i++) {
		spos[i] = -1;
	}

	if (hracu > 0) 
		spos[GetHracStartPos(aut)] = 0;

	if (hracu == 2) {
		do {
			j = GetHracStartPos(aut);
		}while(spos[j] != -1);
		spos[j] = 1;
	}

	LogMessage("GRun::PrepareRace...Startovni pozice pocitacu. \n");
	for (i = hracu; i < aut; i++) {
		do {
			j = rand()%aut;
		}while(spos[j] != -1);
		spos[j] = i;
	}

	LogMessage("GRun::PrepareRace...Souradnice startovnich pozic. \n");
	for(i = 0; i < aut; i++) {
		if (m_map.GetStartPos(i, &x, &y, &angle)) {
			j = spos[i];
			m_cars[j].m_active = true;
			if (j < hracu)
				m_cars[j].Init(angle, x, y, false, &m_map);
			else
				m_cars[j].Init(angle, x, y, true, &m_map);
		}
		else break;
	}

	m_carsc = i;


	LogMessage("GRun::PrepareRace...Grafika. \n");
// grafika
	if(!LoadGFX()) {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Failed loading gfx !\n Missing file %s", errch); // SMULA !!!!
		exit(1);
	}

	LogMessage("GRun::PrepareRace...Zaverecny init. \n");
	m_view.Init(&m_map, 0);
	if (m_hracu == 2) {
		m_view2.Init(&m_map, 1);
		m_view2.m_rect.left = 0; m_view2.m_rect.top = 0;
		m_view2.m_rect.right = DF_GAMEX/2; m_view2.m_rect.bottom = DF_GAMEY;
		m_view.m_rect.left = DF_GAMEX/2; m_view.m_rect.top = 0;
		m_view.m_rect.right = DF_GAMEX; m_view.m_rect.bottom = DF_GAMEY;
	}
	else {
		m_view.m_rect.left = 0; m_view.m_rect.top = 0;
		m_view.m_rect.right = DF_GAMEX; m_view.m_rect.bottom = DF_GAMEY;
	}

	for (i = 0; i < MAX_KOL_RES; i++)
		for (j = 0; j < MAX_CHP; j++) 
			m_pos[i][j] = 0;


	return true;

	LogMessage("GRun::PrepareRace.. OK\n");
}

bool GRun::TestEnd()
{
	if (m_cars[0].m_kol < m_kol && m_cars[0].m_active) return false;
	if (m_hracu == 2 && m_cars[1].m_kol < m_kol && m_cars[1].m_active) return false;
	return true;
}

#define CON(x) (x != 0)

void GRun::Input()
{
		LogMessage("autodrivers\n");
		for(int i = m_hracu; i < m_carsc; i++)
			m_cars[i].AutoDriver(&m_map);

		LogMessage("players\n");
		if (m_hracu > 0)
			m_cars[0].SetDriver(CON(key[KEY_UP])||CON(key[KEY_F]), CON(key[KEY_DOWN])||CON(key[KEY_V]), CON(key[KEY_LEFT]), CON(key[KEY_RIGHT]), CON(key[KEY_SPACE]), CON(key[KEY_ENTER]));
		if (m_hracu == 2)
			m_cars[1].SetDriver(CON(key[KEY_W]), CON(key[KEY_S]), CON(key[KEY_A]), CON(key[KEY_D]), CON(key[KEY_LSHIFT]), CON(key[KEY_LCONTROL]));

//	for(int i = 0; i < m_carsc; i++)
//		m_cars[i].AutoDriver(&m_map);

}

void GRun::ComputeFinalPos()
{
	int i, ID, mpos = -1;
	if (m_dokncen) {
		for (i = 0; i < m_carsc; i++)
			if (m_cars[i].m_kol == m_kol) {
				if ((m_cars[i].m_fpos = m_cars[i].m_pos[m_kol][0]) > mpos)
					mpos = m_cars[i].m_fpos;
			}
		for (;;) {
			ID = -1;
			for (i = 0; i < m_carsc; i++)
				if (m_cars[i].m_fpos == -1 && 
					(ID == -1 || m_cars[i].m_kol*10000+m_cars[i].m_chp*100+10-m_cars[i].m_pos[m_cars[i].m_kol][m_cars[i].m_chp]  >
									m_cars[ID].m_kol*10000+m_cars[ID].m_chp*100+10-m_cars[ID].m_pos[m_cars[ID].m_kol][m_cars[ID].m_chp])) ID = i;
			if (ID == -1) break;
			m_cars[ID].m_fpos = ++mpos;
		}
	}
	else {
		for (i = m_hracu; i < m_carsc; i++)
			if (m_cars[i].m_kol == m_kol) {
				if ((m_cars[i].m_fpos = m_cars[i].m_pos[m_kol][0]) > mpos)
					mpos = m_cars[i].m_fpos;
			}
		for (;;) {
			ID = -1;
			for (i = m_hracu; i < m_carsc; i++)
				if (m_cars[i].m_fpos == -1 && 
					(ID == -1 || m_cars[i].m_kol*10000+m_cars[i].m_chp*100+10-m_cars[i].m_pos[m_cars[i].m_kol][m_cars[i].m_chp]  >
									m_cars[ID].m_kol*10000+m_cars[ID].m_chp*100+10-m_cars[ID].m_pos[m_cars[ID].m_kol][m_cars[ID].m_chp])) ID = i;
			if (ID == -1) break;
			m_cars[ID].m_fpos = ++mpos;
		}
		for (i = 0; i < m_hracu; i++)
			m_cars[i].m_fpos = ++mpos;
	}
}

void GRun::PrepareScreen()
{
	hscreen.m_mouse = false;
	if (DF_GAMEX != DF_MENUX)
		hscreen.Init(DF_GAMEX, DF_GAMEY, hscreen.m_bpp, H_SCREEN_BUFFER, hscreen.m_card, true); // zkusime 16BPP

	clear(hscreen.m_back);

	TTDraw(hscreen.m_back, DF_GAMEX/2, DF_GAMEY/2, WIN_RGB(255, 255, 255), LNG_LOADING, g_font12);
	hscreen.Flip();
}

void GRun::RestoreScreen()
{
	if (DF_GAMEX != DF_MENUX)
		hscreen.Init(DF_MENUX, DF_MENUY, hscreen.m_bpp, H_SCREEN_BUFFER, hscreen.m_card, true); // zkusime 16BPP
	hscreen.m_mouse = true;
}

void GRun::NullBitmaps(BITMAP **pole, int kolik)
{
	for(int i = 0; i < kolik; i++){
		*pole = NULL;
		pole++;
	}
}

void GRun::Demo()
{


   clear_keybuf();
	hscreen.m_mouse = false;
	m_semafor = SEMAFOR_CERVENA;

	m_paused  = false;
	m_dokncen = false;
	m_hracu = 0;

	m_vyrazovacka = ((rand()%2) == 1);

	do {

//		FILE *f = fopen("racer.log", "wt");
//		fclose(f);

//		LogMessage("\n---- Start Loop ----\n");

		tmr = 4; // 40 ms; -> 25 refreshu za sekundu

		// zobrazeni
//		LogMessage("\n---- Draw ----\n");
		Draw();

//		LogMessage("\n---- Update Sound ----\n");
		UpdateSound();

		// nastaveni ridice
//		LogMessage("\n---- Input ----\n");
		Input();

		// a jedem
//		LogMessage("\n---- Move ----\n");
		Move();

//		LogMessage("\n---- Crash ----\n");
		Crash();
	
//		LogMessage("\n---- Timer ----\n");
		while (tmr > 0) ; //Sleep(1); // LINUX

		if (m_semafor > 0) m_semafor--;

//		LogMessage("\n---- Test End ----\n");
		if (TestEnd()) break;

//		LogMessage("\n---- End Loop ----\n");
   } while (!keypressed());

}


double GRun::GetUpgradedSpeed(int car, int *kolik, double nkoef)
{
	st_car_upgrade *cu = &Cparams[car].upgrade[0];
	double koef = 0;

	for (int i = 0; i < MAX_UPGRADES; i++) {
		for (int j = 0; j < *kolik; j++)
			koef += cu->speed;
		kolik++;
		cu++;
	}

	return (koef*nkoef+1)*Cparams[car].max_speed;
}


double GRun::GetUpgradedAcc(int car, int *kolik, double nkoef)
{
	st_car_upgrade *cu = &Cparams[car].upgrade[0];
	double koef = 0;

	for (int i = 0; i < MAX_UPGRADES; i++) {
		for (int j = 0; j < *kolik; j++)
			koef += cu->acc;
		kolik++;
		cu++;
	}

	return (nkoef*koef+1)*Cparams[car].acc;
}

double GRun::GetUpgradedBreaks(int car, int *kolik, double nkoef)
{
	st_car_upgrade *cu = &Cparams[car].upgrade[0];
	double koef = 0;

	for (int i = 0; i < MAX_UPGRADES; i++) {
		for (int j = 0; j < *kolik; j++)
			koef += cu->breakes;
		kolik++;
		cu++;
	}

	return (1-koef*nkoef)*Cparams[car].breakes;
}

double GRun::GetUpgradedTurning(int car, int *kolik, double nkoef)
{
	st_car_upgrade *cu = &Cparams[car].upgrade[0];
	double koef = 0;

	for (int i = 0; i < MAX_UPGRADES; i++) {
		for (int j = 0; j < *kolik; j++)
			koef += cu->turning;
		kolik++;
		cu++;
	}

	return (koef*nkoef+1)*Cparams[car].turning;
}



double GRun::m_u_speed = 0;
double GRun::m_u_acc = 0;
double GRun::m_u_turning = 0;
double GRun::m_u_breaks = 0;

void GRun::ComputeMaxUpgrades()
{
	m_u_speed = 0;
	m_u_acc = 0;
	m_u_turning = 0;
	m_u_breaks = 1;

	double skoef = 1;
	double akoef = 1;
	double tkoef = 1;
	double bkoef = 1;	

//	allegro_message(" Start FOR\n");
	for (int n = 0; n < MAX_CARS; n++) {
//	for (int n = 0; n < 7; n++) {
		
//		allegro_message("car : %s\n %s.bmp\n", Cparams[n].name, Cparams[n].file);
//		allegro_message("speed : %f\n", Cparams[n].max_speed);
		
//		for (int i = 0; i < 64; i++)
//			allegro_message("%c", Cparams[n].name[i]);
/*
	printf("\n%s\n", Cparams[n].name);
	printf("%f     ", Cparams[n].max_speed);  // maximalni rychlost smerem vpred
	printf("%f     ", Cparams[n].max_speedB); // maximalni rychlost smerem vzad (je zaporne cislo)
	printf("%f\n", Cparams[n].acc);        // akcelerace

	printf("%f    ", Cparams[n].breakes); // sila brzd (0..1) 1 == nebrzbi, 0 == hned stoji (fuj)
	printf("%f    ", Cparams[n].hbreak);  // sila rucni brzdy viz. ^
	printf("%f    ", Cparams[n].hbreaks); // sila rucni brzd pri zataceni ( smyku )
	printf("%f  \n", Cparams[n].autoslow); // automaticke zopomalovani = (0..1> 1 == nezpomaluje

	
	printf("%f    ", Cparams[n].weight);  // hmotnost auta ( dulzite pri narazech ) (zatin nenarazi)
	
	printf("%f    ", Cparams[n].tearing); // prilavos k povrchu v zatackach ( = (0..1> - 1 == totalni prilnavost, 0 == totalni klouzavost )
	
	printf("%f    ", Cparams[n].turning); // rychlost zataceni (meneni uhle otoceni) 

	printf("%d", Cparams[n].light);

	printf("\n%s\n ", Cparams[n].file); // jmeno souboru

	printf("%d   ", Cparams[n].razeni);    // pocet radicich stupnu

	printf("%d   ", Cparams[n].pohar);  // zda se s autem da jezdit tovarni pohar
	printf("%d   ", Cparams[n].poharb); // index sede bitmapy pri poharu

	printf("%d   ", Cparams[n].kategorie); // kategorie auta A = 0, B = 1,...
	printf("%d   ", Cparams[n].cena);
	
	int    rt;        // typ trati po ktere jede pocitac 0 - 4
	int    zvuk;      // zvuk motoru

	// pozice kol
	POINT  kola[4]; // 0 - leve horni, 1 - prave horni, 2 - leve dolni, 3 - prave dolni

	// MAX_UPGRADES == 7
	st_car_upgrade upgrade[MAX_UPGRADES]; // vsechny upgrady

	

		for (int i = 0; i < 6; i++) {
			printf("\n");
			for (int j = 0; j < 90; j++)
				allegro_message("%c", Cparams[n].line[i][j]);
			printf("\n");
		}
	

	printf("\n %d ", Cparams[n].nahon_predni);
	printf("%d \n\n------\n\n", Cparams[n].nahon_zadni);


		for (int i = 0; i < MAX_UPGRADES; i++) {
			printf("%d %d\n", Cparams[n].upgrade[0].max, Cparams[n].upgrade[0].cena);
		}
*/
		
		skoef = 1;
		akoef = 1;
		tkoef = 1;
		bkoef = 1;
		
		
/*		st_car_upgrade *cu = &Cparams[n].upgrade[0];
		
		for (int i = 0; i < MAX_UPGRADES; i++) {
			allegro_message(" %d\n", cu->max);
			for (int j = 0; j < cu->max-1; j++) {
				tkoef += cu->turning;
				akoef += cu->acc;
				skoef += cu->speed;
				bkoef -= cu->breakes;
			}
			cu++;
		}
*/
//		allegro_message(" Start FOR\n");

		for (int i = 0; i < MAX_UPGRADES; i++) {
//			allegro_message(" %d\n", Cparams[n].upgrade[i].max);
			for (int j = 0; (j < Cparams[n].upgrade[i].max-1) && j < 4; j++) {
				tkoef += Cparams[n].upgrade[i].turning;
				akoef += Cparams[n].upgrade[i].acc;
				skoef += Cparams[n].upgrade[i].speed;
				bkoef -= Cparams[n].upgrade[i].breakes;
			}
//			cu++;
		}
//		allegro_message("\n");

		if (Cparams[n].max_speed * skoef > m_u_speed) m_u_speed = Cparams[n].max_speed * skoef;
		if (Cparams[n].acc * akoef > m_u_acc) m_u_acc = Cparams[n].acc * akoef;
		if (Cparams[n].turning * tkoef > m_u_turning) m_u_turning = Cparams[n].turning * tkoef;
		if (Cparams[n].breakes * bkoef < m_u_breaks) m_u_breaks = Cparams[n].breakes * bkoef;
	}
}

double GRun::GetMaxUpgradedSpeed(int car)
{
	st_car_upgrade *cu = &Cparams[car].upgrade[0];
	double koef = 0;

	for (int i = 0; i < MAX_UPGRADES; i++) {
		for (int j = 0; (j < cu->max-1) && (j < 4); j++)
			koef += cu->speed;
		cu++;
	}

	return (koef+1)*Cparams[car].max_speed;
}


double GRun::GetMaxUpgradedAcc(int car)
{
	st_car_upgrade *cu = &Cparams[car].upgrade[0];
	double koef = 0;

	for (int i = 0; i < MAX_UPGRADES; i++) {
		for (int j = 0; j < cu->max-1 && (j < 4); j++)
			koef += cu->acc;
		cu++;
	}

	return (koef+1)*Cparams[car].acc;
}

double GRun::GetMaxUpgradedBreaks(int car)
{
	st_car_upgrade *cu = &Cparams[car].upgrade[0];
	double koef = 0;

	for (int i = 0; i < MAX_UPGRADES; i++) {
		for (int j = 0; j < cu->max-1 && (j < 4); j++)
			koef += cu->breakes;
		cu++;
	}

	return (1-koef)*Cparams[car].breakes;
}

double GRun::GetMaxUpgradedTurning(int car)
{
	st_car_upgrade *cu = &Cparams[car].upgrade[0];
	double koef = 0;

	for (int i = 0; i < MAX_UPGRADES; i++) {
		for (int j = 0; j < cu->max-1 && (j < 4); j++)
			koef += cu->turning;
		cu++;
	}

	return (koef+1)*Cparams[car].turning;
}

void GRun::UpdateSound()
{
	int i;
	int dst, ds2, x, y;

	if (DF_ZVUK == FALSE) return;

	if (!m_paused && !m_dokncen) {
		// Auta
		for (i = 0; i < m_carsc; i++) {
			dst = abs(m_cars[0].m_x-m_cars[i].m_x) + abs(m_cars[0].m_y-m_cars[i].m_y);
			x = m_cars[0].m_x;
			y = m_cars[0].m_y;
			if (m_hracu == 2)
				if ((ds2 = abs(m_cars[1].m_x-m_cars[i].m_x) + abs(m_cars[1].m_y-m_cars[i].m_y)) < dst) {
					dst = ds2;
					x = m_cars[1].m_x;
					y = m_cars[1].m_y;
				}
			m_cars[i].UpdateSound(x, y, dst);
		}
		// Objekty
		for (i = 0; i < m_map.m_cobjects; i++) 
			if (m_map.m_objects[i]->m_par.sound) {
				dst = abs(m_cars[0].m_x-m_map.m_objects[i]->m_x) + abs(m_cars[0].m_y-m_map.m_objects[i]->m_y);
				x = m_cars[0].m_x;
				y = m_cars[0].m_y;
				if (m_hracu == 2)
					if ((ds2 = abs(m_cars[1].m_x-m_map.m_objects[i]->m_x) + abs(m_cars[1].m_y-m_map.m_objects[i]->m_y)) < dst) {
						dst = ds2;
						x = m_cars[1].m_x;
						y = m_cars[1].m_y;
				}
				m_map.m_objects[i]->UpdateSound(x, y, dst);
			}
	}
	else {
		for (i = 0; i < m_carsc; i++)
			m_cars[i].StopSound();
		for (i = 0; i < m_map.m_cobjects; i++)
			m_map.m_objects[i]->StopSound();
	}

}

void GRun::TestVyrazovacka()
{
	if (m_vyrazovacka == false) return;

	int mink = m_kol+5;
	int cnt  = 0;
	int id   = -1;
	int ac = 0;
	for (int i = 0; i < m_carsc; i++) {
		if (m_cars[i].m_active == false) continue;
		if (m_cars[i].m_kol < mink) {
			mink = m_cars[i].m_kol;
			id = i;
			cnt = 1;
		}
		else
			if (m_cars[i].m_kol == mink) cnt++;
		ac++;
	}

	if (cnt == 1 && ac > 2)
		m_cars[id].m_active = false;
}

int GRun::GetHracStartPos(int aut)
{
	if (aut == 1) return 0;

	int ret = 0;
	int k;
	switch (DF_OBTIZNOST) {
	case 0 :
		k = aut/3;
		if (k < 2) k = 2;
		ret = rand()%k;
		break;
	case 1:
		k = 2*aut/3;
		if (k < 2) k = 2;
		ret = rand()%k;
		break;
	case 2:
		if ( aut == 0 ) ret = 0;
		else ret = rand()%aut;
		break;
	case 3:
		k = 2*aut/3;
		if (k < 2) k = 2;
		ret = rand()%k + aut-k;
		break;
	case 4:
		k = aut/3;
		if (k < 2) k = 2;
		ret = rand()%k + aut-k;
		break;
	}
	if (ret < 0) ret = 0;
	return ret;
}

double GRun::GetAccToSto(double acc, double maxspeed)
{
	double speed = 0;
	int tm = 0;
	while (speed < 10) {
		speed += acc * (1-pow(speed,0.2)/pow(maxspeed,0.2));
		tm++;
	}

	return tm*0.10 + 1.8;
}

double GRun::GetSpeedKmH(double speed)
{
	return speed*10;
}

double GRun::GetBreaksClass(double breaks)
{
	breaks -= m_u_breaks;
	return 1 + 10*(1-breaks/(1-m_u_breaks));
}

extern HFONT   g_font18;
extern HFONT   g_font12;
extern HFONT   g_font12B;
extern HFONT   g_font24;
extern BITMAP  *g_menuGround;

void GRun::LoopVysledky()
{
	LoadBitmaps(&m_tabs[0], MAX_TABS, 0, "tab%d");
	set_trans_blender(0, 0, 0, 128);

	char s[100];
	if (!m_dokncen) return;


	BITMAP *btmp;
	int i, j, k;
	btmp = create_bitmap(DF_MENUX, DF_MENUY);


	draw_sprite(btmp, g_menuGround, 0, 0);


	int top = hscreen.m_h/2 - m_carsc*15 + 5;

	draw_trans_sprite(btmp, m_tabs[4], hscreen.m_w/2-m_tabs[4]->w/2, top-m_tabs[4]->h);

	TTDraw(btmp, 170, top-20,  WIN_RGB(255, 255, 255), LNG_RUN_PORADI, g_font12, 0);

	TTDraw(btmp, 238, top-20,  WIN_RGB(255, 255, 255), LNG_RUN_JMENOAUTO, g_font12, 0);

	TTDraw(btmp, 328, top-15,  WIN_RGB(255, 255, 255), LNG_RUN_MAX, g_font12, 0);
	TTDraw(btmp, 328, top-25, WIN_RGB(255, 255, 255), LNG_RUN_RYCHLOST, g_font12, 0);

	TTDraw(btmp, 405, top-15,  WIN_RGB(255, 255, 255), LNG_RUN_PRUMERNA, g_font12, 0);
	TTDraw(btmp, 405, top-25, WIN_RGB(255, 255, 255), LNG_RUN_RYCHLOST, g_font12, 0);

	TTDraw(btmp, 480, top-20,  WIN_RGB(255, 255, 255), LNG_RUN_CAS, g_font12, 0);

	for (j = 0; j < m_carsc; j++) {

		draw_trans_sprite(btmp, m_tabs[5], hscreen.m_w/2-m_tabs[5]->w/2, top+30*j);

		i = 0;
		for (k = 0; k < m_carsc; k++) {
			if (m_cars[k].m_fpos == j)
				i = k;
		}

		int color = WIN_RGB(255, 255, 255);
		if (i < m_hracu) color = WIN_RGB(255, 255, 0);

		sprintf(s, "%d", m_cars[i].m_fpos+1);
		TTDraw(btmp, 170, top+31*j+5,  color, s, g_font12B, 0);
	
		TTDraw(btmp, 203, top+31*j-7+5,   color, m_cars[i].m_dname, g_font12B, 1);
		TTDraw(btmp, 203, top+31*j+3+5, color, m_cars[i].m_par.name, g_font12, 1);
	
		sprintf(s, "%.0f Km/h", m_cars[i].m_max*10);
		TTDraw(btmp, 328, top+31*j-7+5,  color, s, g_font12, 0);

		sprintf(s, "%.0f Km/h", m_cars[i].m_avs);
		TTDraw(btmp, 405, top+31*j-7+5,  color, s, g_font12, 0);

		sprintf(s, "%d:%02d.%d s", ((int)m_cars[i].m_times)/60, ((int)(m_cars[i].m_times)%60), ((int)(m_cars[i].m_times*10)%10));
		TTDraw(btmp, 520, top+31*j-7+5,  color, s, g_font12, 2);
	}

	draw_trans_sprite(btmp, m_tabs[6], hscreen.m_w/2-m_tabs[6]->w/2, top+30*j);



	TButton OK;
	OK.Init(320, 460, LNG_OK, g_font18);
	clear_keybuf();
	for (;;) {

		draw_sprite(hscreen.m_back, btmp, 0, 0);
		OK.Draw();
		hscreen.Flip();

		if (OK.Update()) break;
		if (keypressed()) {
			readkey();
			break;
		}
	}
	if (btmp != NULL) destroy_bitmap(btmp);

	DestroyBitmaps(&m_tabs[0], MAX_TABS);
}

void GRun::SimulateRaceToEnd()
{
	if (m_dokncen == false) return;
	bool done = true;
	int i = 5000, k;
	m_cars[0].m_autodriver = true;
	m_cars[1].m_autodriver = true;
	for (k = 0; k < m_carsc; k++) 
		if (m_cars[k].m_show == POS_SHOW_STILL) m_cars[k].m_active = false;

	do {
//		Draw();
		done = true;
		for(int i = 0; i < m_carsc; i++)
			m_cars[i].AutoDriver(&m_map);
		m_map.Move();
		for (k = 0; k < m_carsc; k++) {
			if (m_cars[k].m_show == POS_SHOW_STILL) m_cars[k].m_active = false;
			if (m_cars[k].m_show != POS_SHOW_STILL && m_cars[k].m_active) done = false;
			m_cars[k].Move(&m_map);
		}
		Crash();

		TestVyrazovacka();
	} while (done == false && i-- > 0);
}
