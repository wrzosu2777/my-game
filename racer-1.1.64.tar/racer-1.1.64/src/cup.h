#ifndef _CUP_INCLUDED_
#define _CUP_INCLUDED_

#define MAX_DOTACE_PLACE 6  // bude dotovno jen prvnich 6 mist

#define CUP_FILE "data/cup.dat"

#ifndef CUP_MAX
#define CUP_MAX        100  // maximalni pocet zavodu v cupu (pro vsechny kategorie)
#endif

#define MAX_CUP_TRATI    8  // maximalni pocet trati na 1 zavod

typedef struct {
	char name[30];   // jmeno zavodu
	int  kategorie;  // kategori  zavodu
	int  trat[MAX_CUP_TRATI];       // trat
	int  trati;      // pocet platnych v trat[]
	int  kol;        // pocet kol zavodu
	int  oponentu;   // pocet oponentu (asi bude vzdy max)
	int  dotace[MAX_DOTACE_PLACE]; // penezni dotace zavodu
	int  upgr_min;  // maximalni upgradovanost pocitace
	int  upgr_max;  // maximalni upgradovanost pocitace
	int  startovne; // startovne
	BOOL protinam;       // zda je zavod v noci
	BOOL vyrazovacka; // zda je zavod vyrazovaci
	char bitmap[30];  // jmeno bitmapky k zavodu
	int  body;        // pocet bodu
	int  dbody;       // bodovy rozdil
	int  obtiznost;   // 0 - 2 amateri, poloprofi, profi
} st_cup;

/*
typedef struct {
	char name[30];   // jmeno zavodu
	int  kategorie;  // kategori  zavodu
	int  trat[MAX_CUP_TRATI];       // trat
	int  trati;      // pocet platnych v trat[]
	int  kol;        // pocet kol zavodu
	int  oponentu;   // pocet oponentu (asi bude vzdy max)
	int  dotace[MAX_DOTACE_PLACE]; // penezni dotace zavodu
	int  upgr_min;  // maximalni upgradovanost pocitace
	int  upgr_max;  // maximalni upgradovanost pocitace
	int  startovne; // startovne
} st_cup2;
*/
// pozn. zavod 0 je zavod proti autorum - kategorie == -1
extern st_cup    g_cup[CUP_MAX];
extern int       g_maxcup;

#endif
