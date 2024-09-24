
#include "stdh.h"
#include "cars.h"
#include "objects.h"
#include "player.h"
#include "drivers.h"
#include "cup.h"

HScreen hscreen;

#ifdef WIN32
int   errno;     // sice nepouzivam, ale jinak bude unresolved exernal pri linkovani :)
#endif

char  errch[50]; // chybove hlaseni

int  DF_X = 640; // x-ove rozliseni obrazovky == 640
int  DF_Y = 480;  // y-ove rozliseni obrazovky == 480

int  DF_GAMEX   = 640; // rozliseni pri hre
int  DF_GAMEY   = 480; // hmm
int  DF_GAMEBPP =  16; // hmm
int  DF_SVETLA = S_DINAMIC; // typ svetel
int  DF_STINY  = S_DINAMIC;  // typ stinu

BOOL DF_INTRO = TRUE; // zda se ma poustet intro
BOOL DF_ZVUK  = TRUE; // zda se ma byt zvuk
BOOL DF_HUDBA = TRUE; // zda ma byt hudba
BOOL DF_FULLSCREEN = TRUE; // zda ma byt fullscreen
int  DF_OBTIZNOST =  2;    // index obtiznosti 0 - 4
double DF_GAME_SPEED = 1.0;  // rychlost hry 1 == 100%

BOOL DF_NOCDISABLE = FALSE; // zakaze noc
BOOL DF_SHADOWDISABLE = FALSE; // zakaze stiny
BOOL DF_SMOGDISABLE = FALSE; // zakaze smog
BOOL DF_SMYKDISABLE = FALSE;  // zakaze smyky

int  DF_BPP = 16; // bitova hloubka


volatile int  tmr = 0; // 10-ms timer

// nacteni je v Run::LoadGFX();
BITMAP*  b_cars[MAX_CARS_BITMAPS];       // bitmapy aut
BITMAP*  b_carsPohar[MAX_CARS_POHAR_BITMAPS]; // zesedle bitmapy aut pro pohar
BITMAP*  b_smogs[MAX_SMOGS_BITMAPS];     // bitmapy kourem aut (50*50)
BITMAP*  b_smogsC[MAX_SMOGS_CBITMAPS];    // bitmapy s kourem aut (32*32) - barva
BITMAP*  b_smyk[MAX_SMYK_BMP];           // bitmapa smyku
BITMAP*  b_kola[MAX_KOL_BMP];           // bitmapa smyku

BITMAP*  b_objects[MAX_OBJECTS_BITMAPS];  // bitmapy k objektum
BITMAP*  b_carlights[MAX_CARS_LIGHTS];    // svetla aut

BITMAP*  b_obris;					           // bitmapa velikosti auticak (50*100) cerne barvy
BITMAP*  b_mouse[MAX_MOUSE_CUR];					           // bitmapa kurzoru mysi :)

BITMAP *g_menuGround;
HFONT   g_font18;
HFONT   g_font12;
HFONT   g_font12B;
HFONT   g_font24;

st_player g_player[MAX_PLAYERS];
int       g_players;
st_player a_player[MAX_PLAYERS];

st_cup    g_cup[CUP_MAX];
int       g_maxcup;


int MAX_CARS;
st_GCar_params  Cparams[MAX_CARS_TOTAL];

st_driver g_driver[MAX_DRIVERS];
int       g_drivers = 0;


bool isprint(char c)
{
	return ((c >= 'A' && c <= 'Z') || c == ' ' || (c >= 'a' && c <= 'z'));
}

bool ReadFromKeyB(char* s, int max) {
	if(!keypressed())  return false;
	int sc;
	char c;
	sc = readkey();
	 c = sc & 0xff;
	sc >>= 8;
	if (sc == KEY_BACKSPACE) {
		if (strlen(s) > 0) {
			s[strlen(s)-1] = '\0';
		}
		else return false;
	}
	else 
		if ((int)strlen(s) < max && isprint(c)) {
			int len = strlen(s);
			s[len] = c;
			s[len+1] = '\0';
		}
		else return false;
	return true;
}

//LINUX
void ZeroMemory(void *mem, int sz)
{
	// ted si nemuzu vzpomenout
	unsigned char *c;
	c = (unsigned char*) mem;
	
	for (int i = 0; i < sz; i++, c++) *c = 0;
}

bool PtInRect(RECT *rect, POINT point)
{
	return (rect->left < point.x && rect->right > point.x &&
	        rect->top  < point.y && rect->bottom > point.y);
}

////////////////////////////////////////////////////////////////////////////////////////////


//LINUX

#include <fstream>
#include <iostream>

inline bool exist(const char* file)
{
	std::ifstream ifs(file);
	if (!ifs) return false;
	return true;
}

char working_dir[PATH_MAX] = ".";

/**
 * \brief Change the working directory to ~/.racer
 */
void goHomeRacer()
{
	if (getcwd(working_dir, PATH_MAX-1) == 0)
    {
        cerr << "Failed getting current working directory" << endl;
    }

	string racerhome;
	if (getenv("HOME")) racerhome = getenv("HOME");
	else racerhome = ".";
	racerhome.append("/.racer");

	int rv = mkdir(racerhome.c_str(), 0777);	
	if ( !(rv == 0 || rv == EEXIST) )			
		cerr << "Problem with path " << racerhome << endl;
		
	if (chdir(racerhome.c_str()) != 0)
    {
        cerr << "Failed to change directory to " << racerhome << endl;
    }
}



/**
 * \brief Paths with 
 */
const char* PATHS_UNIX[] = 
{
	".", "/usr/share/racer",  "/usr/local/share/racer", "/usr/share/games/racer", 
        "/usr/local/share/games/racer", /* INSTALL_DIR, */  0,
};

const char* SUBDIRS[] =  // not used at the moment
{
	".", "maps",  "gfx", "data", 0,
};


string locateFile(const string& file)
{
	if (file == "") return file;
	
	string f;		
	for (int i = 0; PATHS_UNIX[i] != 0; ++i) 
	{
		f.clear();
		f.append( PATHS_UNIX[i]).append("/").append(file);
					
		if ( exist(f.c_str()) ) return f;
	}

	f.clear();
	f.append(working_dir).append("/").append(file);
	if ( exist(f.c_str()) ) return f;

//	cerr << "Locating " << file << "failed" << endl;

	return file;
}





