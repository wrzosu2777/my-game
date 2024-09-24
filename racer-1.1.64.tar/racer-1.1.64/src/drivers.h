#ifndef _DRIVERS_HEADER_
#define _DRIVERS_HEADER_

#define DRIVERS_FILE  "data/drivers.dat"

typedef struct {

	char   name[20];  // jmeno
	DWORD    car;       // preferovane auto
	DWORD    kategorie; // preferovana kategorie
	char   vycpat[1];
	double speed;     // kolik procent rychlost 1 == 100%
	double turning;   // kolik pridava k zataceni (def == 1)

	bool   used;      // zda jiz bylo jmeno pouzito - pri nacitani jmen na zavody

	char   vycpat2[7];
	
} st_driver;

#define MAX_DRIVERS 150
extern st_driver g_driver[MAX_DRIVERS];
extern DWORD       g_drivers;

#endif
