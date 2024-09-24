#ifndef _FYLE_TYPE_INCLUDE_
#define _FYLE_TYPE_INCLUDE_

///////////////////////
// Struktura souboru :
//
// st_GFile_Header
// st_GFile_Header.road    * st_GFile_Road
// st_GFile_Header.objects * st_GFile_Object
// EOF
//
///////////////////////
#define FILE_SIGN             2156 // nahodne, avsak nemene cislo
#define MAX_OBJECTS_ON_MAP   10000 // maximapli pocet objektu v mape
#define MAX_ROADS_ON_MAP      1000 // maximalni pocet zatacek silnice
#define MAX_ROADS_TYPES          5 // maximalni pocet zatacek silnice

typedef struct{
	DWORD   sign;    // signatura - musi byt == FILE_SIGN
	BYTE   ver;     // urcuje verzi - musi byt == 1
	DWORD   x;       // rozmer trate X (v pixelech)
	DWORD   y;       // rozmer trate Y (v pixelech)
	DWORD   road[MAX_ROADS_TYPES];    // pocet bodu silnice
	DWORD   objects; // pocet objektu
	DWORD   bonusu; // obrazek urcujici typ trate
	POINT  cam_start; // kde ma zacit kamera
	POINT  cam_end;   // kde ma kamera koncit
} st_GFile_Header;

typedef struct{
	DWORD   sign;    // signatura - musi byt == FILE_SIGN
	BYTE   ver;     // urcuje verzi - musi byt == 1
	DWORD   x;       // rozmer trate X (v pixelech)
	DWORD   y;       // rozmer trate Y (v pixelech)
	DWORD   road[MAX_ROADS_TYPES];    // pocet bodu silnice
	DWORD   objects; // pocet objektu
	DWORD   bonusu; // obrazek urcujici typ trate
	POINT  cam_start; // kde ma zacit kamera
	POINT  cam_end;   // kde ma kamera koncit
} st_GFile_Header2;


typedef struct{
	DWORD      ID;       // index
	DWORD      x;       // souradnice X (v pixelech)
	DWORD      y;       // souradnice Y (v pixelech)
	// pozn. - typeB a typeA se nepouzivaji
	DWORD   typeA;      // typ cesty A           \         pusobuje, ze se cesta muze
	DWORD     IDA;      // index naslednika A      \ ___   pri smery sizdy <- vetvit 
	DWORD   typeB;      // typ cesty B             /
	DWORD     IDB;      // index naslednika B    /
} st_GFile_Road;


typedef struct{
	DWORD     INDEX;    // cislo objektu v Oparams
	DWORD        ID;    // cislo objekltu v sestavene databazi objektu
	DWORD         x;    // souradnice X (v pixelech)
	DWORD         y;    // souradnice Y (v pixelech)
	double   alpha;    // pocatecni natoceni ( ve stupnich )
	double turning;    // rychlost zataceni
	double   speed;    // rychlost pohybu
	double  angles;    // rychlost toceni ( 0 == nerotuje )
} st_GFile_Object;

typedef struct{
	DWORD    circle_size;
	DWORD    slower_distanc;
	DWORD    slower_angle;
	double slower_speed;
	DWORD    break_distanc;
	DWORD    break_angle;
	double break_speed;
	DWORD    speed_off_angle;
	double speed_off_speed;
	DWORD    hbreak_angle;
	double hbreak_speed;
} st_GFile_Driving;

#endif //#ifndef _FYLE_TYPE_INCLUDE_
