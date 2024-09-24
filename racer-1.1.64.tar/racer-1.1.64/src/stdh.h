// standart header - precompiled
#ifndef _PRECOMPILED_HEADER_INCLUDED_
#define _PRECOMPILED_HEADER_INCLUDED_

#include <string>
using namespace std;

// includy standartni
#include <sys/stat.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
//#include <io.h>
#include <allegro.h>
//#include <winalleg.h>
//#include <windows.h>  

//#define WIN_RGB(r,g,b)        ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define WIN_RGB(r,g,b)          makecol(r,g,b)


// on the very top, since they are used everywhere
typedef uint8_t  BYTE;
typedef int16_t WORD;
typedef int32_t   DWORD;


// includy moje
#include "halleg.h"

#include "HMP3.H"
#include "HDSound.h"

// globalni promene - deklarovanve stdh.cpp
extern HScreen hscreen;

// defines
// matika
#define  PI  3.14  // pi
#define DPI  6.28  // dve pi
#define PI2  1.57  // pi / 2


//#define WINRGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
//#define WINRGB(r,g,b)          ((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)))
#define WINRGB(r,g,b)            makecol(r,g,b)

// pro casovac
extern volatile int tmr; // 10-ms timer

#define MAX_MOUSE_CUR  2
extern BITMAP*  b_mouse[MAX_MOUSE_CUR];					           // bitmapa kurzoru mysi :)

const int mpos[MAX_MOUSE_CUR][2] = {
	{1,1}, {16,16}
};



// pro chybova hlaseni
extern char errch[50];

// popis grafiky
extern DWORD  DF_X;  // x-ove rozliseni obrazovky == 640
extern DWORD  DF_Y;  // y-ove rozliseni obrazovky == 480
extern DWORD  DF_GAMEX; // rozliseni pri hre
extern DWORD  DF_GAMEY; // hmm
extern DWORD  DF_GAMEBPP; // hloubka barev pri hre
extern DWORD  DF_SVETLA; // typ svetel
extern DWORD  DF_STINY;  // typ stinu
extern double DF_GAME_SPEED;  // rychlost hry 1 == 100%

#define BOOL  int32_t
#ifndef TRUE 
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

extern BOOL DF_INTRO; // zda se ma poustet intro
extern BOOL DF_ZVUK; // zda se ma byt zvuk
extern BOOL DF_HUDBA; // zda ma byt hudba
extern BOOL DF_FULLSCREEN; // zda ma byt fullscreen
extern int  DF_OBTIZNOST;  // index obtiznosti 0 - 2

extern BOOL DF_NOCDISABLE; // zakaze noc
extern BOOL DF_SHADOWDISABLE; // zakaze stiny
extern BOOL DF_SMOGDISABLE; // zakaze smog
extern BOOL DF_SMYKDISABLE;  // zakaze smyky

extern DWORD DF_BPP; // barevna hloubka

#define S_STATIC  0
#define S_DINAMIC 1

#define DF_MENUX   640
#define DF_MENUY   480

// reseni zavislosti na windowsss
 
typedef int HFONT;    // prozatim
typedef int COLORREF; // taky zatim

typedef struct {
	int cx;
	int cy;
} SIZE;

typedef struct {
	int left;
	int top;
	int right;
	int bottom;
} RECT;

typedef struct {
	DWORD x;
	DWORD y;
} POINT;


//LINUX
void ZeroMemory(void *mem, int sz);

bool PtInRect(RECT *rect, POINT point);

//LINUX
//void  FindFile(const char *file);
string locateFile(const string& file);
void goHomeRacer();


// muj vlastni typ - obdelnik - souradnice vrcholu
// index 0 je pravy horni roh, 1 levy horni, 2 levy dolni, 3 pravy dolni - jde jako uhel ( proti smeru hodinovych rucicek )
typedef POINT MYRECT[4];

bool ReadFromKeyB(char* s, int max);
void TTDraw(BITMAP *bmp, int x, int y, COLORREF rgb, const char* c, HFONT font, int align = 0);
void TTGetDimension(const char *c, HFONT font2, int &width, int &height);
BITMAP *rload_bmp(const char *c);
BITMAP *fload_bitmap(const char *c);
void LogMessage(const char *s);

void rload_snd(const char *c, HDSoundBuffer *buffer);

#ifdef _DEBUG // mam svoje vlastni makro ASSERT
#ifdef ASSERT
#undef ASSERT
#endif
#define ASSERT(condition)     { if (!(condition)) {char ass[50]; sprintf(ass, "%s, %d",__FILE__, __LINE__);  MessageBox(NULL, ass, "ASSERT", MB_OK); }}
#endif


#endif // _PRECOMPILED_HEADER_INCLUDED_
