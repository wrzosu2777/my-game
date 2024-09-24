#ifndef _PLAYER_HEADER_
#define _PLAYER_HEADER_

#ifndef MAX_UPGRADES
#define MAX_UPGRADES 7
#endif
#define MAX_UPGRADED 5

#ifndef CUP_MAX
#define CUP_MAX        100  // maximalni pocet zavodu v cupu (pro vsechny kategorie)
#endif

#include "car_params.h"

typedef struct {
	int  best;       // nejlepsi umisteni
	int  obtiznost;  // na jakou obtiznost
	int  vitezstvi;  // pocet vytezstvi na dane trati
	int  jeto;       // kolikrat danou traty jel;
	int  poradi;     // soucet vsech poradi - slouzi pro vypocet prumerneho poradi
} st_player_cup;


typedef struct {
	bool owned;      // zda auto vlastni
	int  upgrades[MAX_UPGRADES]; // a jak je upgradeovane
} st_player_car;

typedef struct {
	char   name[20];
	int    money;
	int    car;
	int    upgrades[MAX_UPGRADES];
	int    body;
	int    gpID; // ID v g_players
	int    poradi; // pri psani vyledku
	double nspeed;  // ucinnost upgradu rychlosti 0..1
	double nacc;    // ucinnost upgradu akcelerace 0..1
	double nbreaks; // ucinnost brzd 0..1
	double ntear;   // ucinnost zataceni 0..1
	int    kbodu;   // pocet bodu v kariere
	st_player_cup   best[CUP_MAX];  // nejlepsi umisteni v zavodech
	st_player_car   cars[MAX_CARS_TOTAL]; // vlastnena auta
} st_player;

#define MAX_PLAYERS 20
extern st_player g_player[MAX_PLAYERS];
extern int g_players;
extern st_player a_player[MAX_PLAYERS];

#endif
