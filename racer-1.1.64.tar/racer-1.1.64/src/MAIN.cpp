
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include <time.h> // pro random
#include "stdh.h"

#include "resource.h"

#include "keygen.h"

#include "cars.h"
#include "objects.h"
#include "player.h"
#include "drivers.h"

// Gincludes
#include "GCar.h"
#include "GView.h"
#include "GObject.h"
#include "GMap.h"
#include "GRun.h"
#include "GMap.h"

// Menu
#include "MMain.h"
#include "MTrain.h"

// Intro
#include "HDShow.h"

#include <iostream>
using namespace std;


// #include <process.h>

void timer_procedure()
{
	if (tmr > 0) tmr--;
}

END_OF_FUNCTION(timer_procedure);


//HINSTANCE gfxdll;
//HINSTANCE sfxdll;

//long  hicon;
//HWND   hwnd;

void InitData();
void SaveData();
void AppInit(int card);
void AppRun();
void AppClose();
void LoadCars();
void LoadDrivers();
void LoadADrivers();
void LoadCup();
void SaveDrivers();
void SetupDialog();

void Intro();

int main(int argc, char *argv[])
{
   allegro_init();
	
//	system("cd /home/berny/Projects/Racer/done");
//	chdir("/home/berny/Projects/Racer/done");
	goHomeRacer(); // change to ~/.racer

	cout << "Allegro inited" << endl;
	
/*
	if (!TestKeys()) {
		allegro_message("Hra, nebo jeji casti byly pravdepodobne poskozeny pri prenosu\n");
		return 0;
	}
	*/
/*
	char    buf[100];
	char   *pc;
	strcpy(buf, argv[0]);
	pc = strrchr(buf,'\\');
	if (pc != NULL) {
		pc++;
		*pc = '\0';
		SetCurrentDirectory(buf);
	}
*/	

	InitData();
	cout << "Data inited" << endl;
	

/*
	if((gfxdll = LoadLibrary("GFX.dll")) == NULL) exit(0);;
	if((sfxdll = LoadLibrary("SFX.dll")) == NULL) exit(0);;
*/		
	set_window_title("Racer");
/*	
	hwnd = win_get_window();
	hicon = (long)LoadIcon(gfxdll, "ICON");
	SetClassLong(hwnd, GCL_HICON, hicon);
*/
//	if (argc == 2 && !strcmp(argv[1], "-?"))
//		SetupDialog();


/*
	if ((argc == 2 && (!strcmp(argv[1], "-I") || !strcmp(argv[1], "-i"))) || DF_INTRO)
		Intro();
*/
//	exit(0);
	
	
	cout << "Loading obects" << endl;
	LoadOparams();
	cout << "Loading cars" << endl;
	LoadCars();
	cout << "Data loaded" << endl;


	if (argc == 3 && !strcmp(argv[1], "-GENERATE")) {
		GRun trun;
		trun.GenerateMap(argv[2]);
	   set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		cout << "Generovani mapy probehlo uspesne";
		exit(0);
	}


	if ((argc == 2 && (!strcmp(argv[1], "-W") || !strcmp(argv[1], "-w"))) || !DF_FULLSCREEN)
		AppInit(GFX_AUTODETECT_WINDOWED);
	else
		AppInit(GFX_AUTODETECT);

	cout << "Window created" << endl;
// 	SetClassLong(hwnd, GCL_HICON, (long)hicon);
	
	cout << "Running game" << endl;
	AppRun();
	
	
	cout << "Closing game" << endl;
	AppClose();

	SaveData();

	cout << "Good bye :-)" << endl;

   return 0;
}

END_OF_MAIN();

const char gfxdirect[7][20] = {
	"gfx/cars/", "gfx/gw/","gfx/menu/",
	"gfx/", "gfx/other/", "", "maps/"
};

void upcaser (char *c)
{
	while (*c != '\0') {
		if (*c <= 'z' && *c >= 'a') *c += 'A' - 'a';
		c++;
	}
}

BITMAP *fload_bitmap(const char *c)
{

	char tc[150];
//	char s[100];
	
	char tmpc[50];
	string path;	

	strncpy(tmpc, c, 50);
	upcaser(tmpc);
	
	//cout << "loading - " << c << " -> " << tmpc << endl;
	
	//LINUX tmpc - upcase
	
	BITMAP *tmp = NULL; // = rload_bmp(tc);

	if (tmp == NULL) {
		for (int i = 0; i < 7; i++) {
			snprintf(tc, 150, "%s%s", gfxdirect[i], tmpc);
			path = locateFile(tc);
			tmp = hload_bitmap(path.c_str());
			if (tmp != NULL) break;
		}
	}	
	
	if (tmp == NULL) {
		for (int i = 0; i < 7; i++) {
			snprintf(tc, 150, "%s%s.BMP", gfxdirect[i], tmpc);
			path = locateFile(tc);
			tmp = hload_bitmap(path.c_str());
			if (tmp != NULL) break;
		}
	}
	
	if (tmp == NULL)
		printf("Bitmapa %s - nebyla nikde nalezena\n", c);


	return tmp;
}

void LogMessage(const char *s)
{
//	printf("%s", s);
/*	static int runs = 0;
	FILE *f;
	if (runs++ == 0) {
		f = fopen("racer.log", "wt");
		fclose(f);
	}

	f = fopen("racer.log", "at");

	fprintf(f, s);

	fclose(f);
	*/
}

BITMAP *rload_bmp(const char *c)
{
	//LINUX
	return NULL;
	
	/*
	HBITMAP bmp = LoadBitmap(gfxdll, c);
	BITMAP* bm = convert_hbitmap_to_bitmap(bmp);
	DeleteObject(bmp);
	return bm;
	*/
}

void rload_snd(const char *c, HDSoundBuffer *buffer)
{
//LINUX -- :-)
//	buffer->LoadFromRes(c, sfxdll);
}

void TTGetDimension(const char *c, HFONT font2, int &width, int &height)
{	
	width  = text_length(font, c);
	height = text_height(font);	
}

// align 0 = center, 1 = left, 2 = right;
void TTDraw(BITMAP *bmp, int x, int y, COLORREF rgb, const char *c, HFONT font2, int align)
{
//	SIZE sz;
//	int ln = strlen(c);	

	switch (align) {
	case 0 : textout_centre_ex(bmp, font, c, x, y, rgb, -1); break; // canter
	case 2 : // right
		{
			int width = text_length(font, c);	
			textout_ex(bmp, font, c, x-width, y, rgb, -1);
		}
		break;
	case 1 : // left
		textout_ex(bmp, font, c, x, y, rgb, -1);
	}

	

	//LINUX - TOHLE JE KLICOVA VEC!!!
	
/*
	HDC dc = win_get_dc(screen);
	if (dc == NULL) return;

	HFONT dc_f = SelectObject(dc, font);

	GetTextExtentPoint(dc, c, ln, &sz);

	SelectObject(dc, dc_f);

   win_release_dc(screen, dc);

	BITMAP *tbmp = create_video_bitmap(sz.cx, sz.cy);

	if (tbmp == NULL) 
		return;

	switch (align) {
	case 0 : x -= sz.cx/2; y -= sz.cy/2; break; // center
	case 1 : break; // left
	case 2 : x -= sz.cx; break; // right
	}

	blit(bmp, tbmp, x, y, 0, 0, sz.cx, sz.cy);

	dc = win_get_dc(tbmp);
	SetTextColor(dc, rgb);
	SetBkMode(dc, TRANSPARENT);
	dc_f = SelectObject(dc, font);
	TextOut(dc, 0, 0, c, ln);
	SelectObject(dc, dc_f);
   win_release_dc(tbmp, dc);

	blit(tbmp, bmp, 0, 0, x, y, sz.cx, sz.cy);

	destroy_bitmap(tbmp);
*/

/*
   
     HDC dc, dcs;
	  SIZE sz;
	  int ln = strlen(c);

	  dcs = win_get_dc(screen);
	  dc = CreateCompatibleDC(dcs);

	  HFONT dc_f = SelectObject(dc, font);

	  GetTextExtentPoint(dc, c, ln, &sz);

	  HBITMAP hbmp = CreateCompatibleBitmap(dcs, sz.cx, sz.cy);
	  HBITMAP dc_b = SelectObject(dc, hbmp);

//	  draw_to_hdc(dc, hscreen.m_back, 0, 0);
	  blit_to_hdc(hscreen.m_back, dc, x-sz.cx/2, y-sz.cy/2, 0, 0, sz.cx, sz.cy);

	  SetTextColor(dc, rgb);
	  SetBkMode(dc, TRANSPARENT);
//	  SetTextAlign(dc, TA_CENTER);
	  TextOut(dc, 0, 0, c, ln);

	  blit_from_hdc(dc, hscreen.m_back, 0, 0, x-sz.cx/2, y-sz.cy/2, sz.cx, sz.cy);

	  SelectObject(dc, dc_f);
	  SelectObject(dc, dc_b);
	  DeleteObject(hbmp);
	  DeleteDC(dc);
     win_release_dc(screen, dcs);
*/
/*     HDC dc;
	  SIZE sz;
	  int ln = strlen(c);

	  dc = win_get_dc(hscreen.m_back);

	  HFONT dc_f = SelectObject(dc, font);
	  GetTextExtentPoint(dc, c, ln, &sz);
	  SetTextColor(dc, rgb);
	  SetBkMode(dc, TRANSPARENT);
	  TextOut(dc, x-sz.cx/2, y-sz.cy/2, c, ln);
	  SelectObject(dc, dc_f);

     win_release_dc(hscreen.m_back, dc);
*/
}

void TESTZERO(int X)
{
    if (X==0) { cerr << " loading config failed" << endl; }
}

void InitData() 
{
	strcpy(a_player[0].name, "Berny");
	strcpy(a_player[1].name, "Beza");

	FILE *f = fopen("racer.cfg", "rb");
	if (f != NULL) {
        TESTZERO(fread(&a_player[0].name, sizeof(char)*20, 1, f));
        TESTZERO(fread(&a_player[1].name, sizeof(char)*20, 1, f));
        TESTZERO(fread(&DF_GAMEX, sizeof(DF_GAMEX), 1, f));
        TESTZERO(fread(&DF_GAMEY, sizeof(DF_GAMEY), 1, f));
        TESTZERO(fread(&DF_SVETLA, sizeof(DF_SVETLA), 1, f));
        TESTZERO(fread(&DF_STINY, sizeof(DF_STINY), 1, f));
        TESTZERO(fread(&DF_INTRO, sizeof(DF_INTRO), 1, f));
        TESTZERO(fread(&DF_ZVUK, sizeof(DF_ZVUK), 1, f));
        TESTZERO(fread(&DF_HUDBA, sizeof(DF_HUDBA), 1, f));
        TESTZERO(fread(&DF_FULLSCREEN, sizeof(DF_FULLSCREEN), 1, f));
        TESTZERO(fread(&hmp3.m_volume, sizeof(DWORD), 1, f));
        TESTZERO(fread(&dsound.m_volume, sizeof(DWORD), 1, f));
        TESTZERO(fread(&DF_OBTIZNOST, sizeof(DF_OBTIZNOST), 1, f));
        TESTZERO(fread(&DF_NOCDISABLE, sizeof(DF_NOCDISABLE), 1, f));
        TESTZERO(fread(&DF_SHADOWDISABLE, sizeof(DF_SHADOWDISABLE), 1, f));
        TESTZERO(fread(&DF_SMOGDISABLE, sizeof(DF_SMOGDISABLE), 1, f));
        TESTZERO(fread(&DF_SMYKDISABLE, sizeof(DF_SMYKDISABLE), 1, f));
        TESTZERO(fread(&DF_BPP, sizeof(DF_BPP), 1, f));
		fclose(f);
	}
}

void SaveData()
{
	FILE *f = fopen("racer.cfg", "wb");
	if (f != NULL) {
		fwrite(&a_player[0].name, sizeof(char)*20, 1, f);
		fwrite(&a_player[1].name, sizeof(char)*20, 1, f);
		fwrite(&DF_GAMEX, sizeof(DF_GAMEX), 1, f);
		fwrite(&DF_GAMEY, sizeof(DF_GAMEY), 1, f);
		fwrite(&DF_SVETLA, sizeof(DF_SVETLA), 1, f);
		fwrite(&DF_STINY, sizeof(DF_STINY), 1, f);
		fwrite(&DF_INTRO, sizeof(DF_INTRO), 1, f);
		fwrite(&DF_ZVUK, sizeof(DF_ZVUK), 1, f);
		fwrite(&DF_HUDBA, sizeof(DF_HUDBA), 1, f);
		fwrite(&DF_FULLSCREEN, sizeof(DF_FULLSCREEN), 1, f);
		fwrite(&hmp3.m_volume, sizeof(DWORD), 1, f);
		fwrite(&dsound.m_volume, sizeof(DWORD), 1, f);
		fwrite(&DF_OBTIZNOST, sizeof(DF_OBTIZNOST), 1, f);
		fwrite(&DF_NOCDISABLE, sizeof(DF_NOCDISABLE), 1, f);
		fwrite(&DF_SHADOWDISABLE, sizeof(DF_SHADOWDISABLE), 1, f);
		fwrite(&DF_SMOGDISABLE, sizeof(DF_SMOGDISABLE), 1, f);
		fwrite(&DF_SMYKDISABLE, sizeof(DF_SMYKDISABLE), 1, f);
		fwrite(&DF_BPP, sizeof(DF_BPP), 1, f);
		fclose(f);
	}
}

void AppInit(int card)
{
   install_keyboard();
	install_timer();
	install_mouse();



	int  i;
	char c[50];


	set_mouse_speed(20, 20);


	// pripraveni timeru
	LOCK_VARIABLE(tmr);
	LOCK_FUNCTION(timer_procedure);
	install_int(timer_procedure, 10);

	srand(time(NULL));
	// H_SCREEN_BUFFER
	// H_SCREEN_2PAGES
	hscreen.Install();
	hscreen.m_fps_draw = 0;

	if (card == GFX_AUTODETECT_WINDOWED) {
		if (!hscreen.Init(DF_MENUX, DF_MENUY, 32, H_SCREEN_BUFFER, GFX_AUTODETECT_WINDOWED, true))
			exit(0);
	}
	else
		if (!hscreen.Init(DF_MENUX, DF_MENUY, DF_BPP, H_SCREEN_BUFFER, GFX_AUTODETECT, true)) {
				set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
				allegro_message("Failed to set up resolution \n %d*%d*%d", DF_MENUX, DF_MENUY, DF_BPP); // SMULA !!!!
				exit(0);
			}
/*
	if (hscreen.m_card == GFX_DIRECTX_WIN) {
		RECT r, r2;
		GetWindowRect(hwnd, &r);
		GetWindowRect(GetDesktopWindow(), &r2);
		SetWindowPos(hwnd, HWND_TOP,  (r2.right-(r.right-r.left))/2, (r2.bottom-(r.bottom-r.top))/2, 0 ,0,SWP_NOSIZE|SWP_NOZORDER);
	}
*/
	LoadDrivers();  // nacteni ridicu (hracu)
	LoadADrivers(); // nacteni auto ridicu
	LoadCup();      // nacte pohar

	for (i = 0 ; i < MAX_MOUSE_CUR; i++) {
		sprintf( c,"MYS%d.BMP",i);
		b_mouse[i] = fload_bitmap(c);
		hscreen.m_mousebmps = MAX_MOUSE_CUR;
		hscreen.m_mousebmp = b_mouse[0];
	}



	hscreen.m_mouse = true;

//	OutputDebugString("Allegro inicialized\n");

	if (DF_HUDBA)
		hmp3.Create();
	hmp3.PlayMenu();

//	if (DF_ZVUK)
//		dsound.Create();



	g_menuGround = fload_bitmap("MENU.BMP");

	cout << "App inited" << endl;
	
/*  LINUX !!!!	
	
	g_font18  = CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		                 PROOF_QUALITY, DEFAULT_PITCH, "Arial");

	g_font12 = CreateFont(13, 0, 0, 0, 500, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		                 PROOF_QUALITY, DEFAULT_PITCH, "Arial");
	g_font12B = CreateFont(14, 0, 0, 0, 800, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		                 PROOF_QUALITY, DEFAULT_PITCH, "Arial");
	g_font24 = CreateFont(23, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		                 PROOF_QUALITY, DEFAULT_PITCH, "Arial");

*/
	//hscreen.m_fps_draw = 1;
}

void AppRun()
{
	MMain menu;
	cout << "menu running" << endl;
	menu.Run();
}

void AppClose()
{
	SaveDrivers();


	destroy_bitmap(g_menuGround);
/*	
	if (g_font18) {
		DeleteObject(g_font18);
		g_font18 = NULL;
	}
	if (g_font12) {
		DeleteObject(g_font12);
		g_font12 = NULL;
	}
	if (g_font24) {
		DeleteObject(g_font24);
		g_font24 = NULL;
	}
*/
	hmp3.CleanUp();

//	OutputDebugString("Shutting down\n");

	// shutting down - bitmapy se musi znicit jeste pred sundanim algra :( (nelze pouzit obycejne destruktory)

	hscreen.Destroy();

	show_mouse(NULL);
	for (int i = 0 ; i < MAX_MOUSE_CUR; i++)
	   destroy_bitmap(b_mouse[i]);

	clear_keybuf();
	allegro_exit();

//	FreeLibrary(gfxdll);
//	FreeLibrary(sfxdll);

//	OutputDebugString("Done\n");

}

void  LoadCars()
{
	string path = locateFile(CPARAMS_FILE);
	FILE *f = fopen(path.c_str(), "rb");
	if (f) {
		MAX_CARS = fread(Cparams, sizeof(st_GCar_params), MAX_CARS_TOTAL, f);
		fclose(f);
		cout << "cars found : " << MAX_CARS << endl;
		// LINUX !!!!!!!!!!!!
		GRun::ComputeMaxUpgrades();
	}

}

void  LoadADrivers()
{
	string path = locateFile(DRIVERS_FILE);
	FILE *f = fopen(path.c_str(), "rb");
	if (f) {
		g_drivers = fread(g_driver, sizeof(st_driver), MAX_DRIVERS, f);
		fclose(f);
	}
	
/*	
	printf("\n\n\n");
//	for ( int i = 0; i < g_drivers; i++ ) {
	for ( int i = 0; i < 4; i++ ) {
		
		for(int j = 0; j < 20; j++) {
			printf("%d - %c\n", j, g_driver[i].name[j]);
		}
		
	printf("\n%s  ", g_driver[i].name);  // jmeno
	printf("%d  ", g_driver[i].car);       // preferovane auto
	printf("%d  ", g_driver[i].kategorie); // preferovana kategorie
	printf("%f  ", g_driver[i].speed);     // kolik procent rychlost 1 == 100%
	printf("%f  \n\n", g_driver[i].turning);   // kolik pridava k zataceni (def == 1)
//	bool   used;      // zda jiz bylo jmeno pouzito - pri nacitani jmen na zavody
	}
	printf("\n\n\n");
*/	
}

void LoadCup()
{
	string path = locateFile(CUP_FILE);
	FILE *f = fopen(path.c_str(), "rb");
	if (f) {
		g_maxcup = fread(g_cup, sizeof(st_cup), CUP_MAX, f);
		fclose(f);
	}
}

void Intro()
{
//	return;
/*
	set_color_depth(32);
   set_gfx_mode(GFX_DIRECTX_WIN, 640, 480, 0, 0);
*/
/*
	HDShow show;
	show.Create();
	show.OpenClip("intro.avi");
	show.Play(false);
	show.ToggleFullScreen();
	GUID tf = TIME_FORMAT_MEDIA_TIME;
	show.pMS->GetTimeFormat(&tf);
	LONGLONG time;
	show.pMS->GetDuration(&time);
	int st = time/10000;
	Sleep(st);
	show.CloseClip();
*/	
}

void LoadDrivers()
{

//   struct _finddata_t c_file;

	//LINUX
	
    /* Find first .c file in current directory */
	g_players = 0;
 	FILE *f;
	

	char c[255];
	if (getcwd(c , 255) != 0)
    {
        cerr << "Cannot get current working directory" << endl;
    }
	
	DIR *md = opendir(c);
	if ( md == NULL ) return;
		
	dirent *d;
	
	while ( (d = readdir(md)) != NULL ) {
			if (d->d_name[strlen(d->d_name)-1] != 't') continue; // check for *.plt'
			if ((f = fopen(d->d_name, "rb")) != NULL) {
				if (fread(&g_player[g_players], sizeof(st_player), 1, f) == 1) {
					g_player[g_players].gpID = g_players;
					g_players++;
				}
				fclose(f);
			}
	}

	closedir(md);
	
/*	
	long hFile;
	if( (hFile = _findfirst( "*.plt", &c_file )) != -1L ) {
		do {
			if ((f = fopen(c_file.name, "rb")) != NULL) {
				if (fread(&g_player[g_players], sizeof(st_player), 1, f) == 1) {
					g_player[g_players].gpID = g_players;
					g_players++;
				}
				fclose(f);
			}
      } while( _findnext( hFile, &c_file ) == 0 );
       _findclose( hFile );
	}
*/	
}
void SaveDrivers()
{
	char s[100];
	FILE *f;
	for (int i = 0; i < g_players; i++) {
		snprintf(s, 100, "%s.plt", g_player[i].name);
		if ((f = fopen(s, "wb")) != NULL) {
			fwrite(&g_player[i], sizeof(st_player), 1, f);
			fclose(f);
		}
	}
}
