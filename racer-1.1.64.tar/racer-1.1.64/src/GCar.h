// GCar.h: interface for the SCar class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _G_CAR_INCLUDE_
#define _G_CAR_INCLUDE_

#define MAX_KOL         20
#define MAX_KOL_RES     22
#define MAX_CHP         10
#define POS_SHOW        75
#define POS_SHOW_STILL  -1
// includy
#include "GClasses.h"
#include "cars.h"
#include "player.h"

#define RYCHLOST_KOURE_ANIM 1
#define MAX_KOURE 8
typedef struct {
	int x;
	int y;
	int cas;
	int typ;
} st_kour;

#define NARAZ_ODHODVLEVO  1
#define NARAZ_ODHODVPRAVO 2
#define NARAZ_OTOCVPRAVO  4
#define NARAZ_OTOCVLEVO   8


// vlastni trida zapouzdrujici auta
class GCar  
{
public:
	void DrawName(BITMAP *dest, int x, int y);
	void DrawDBG(GRun *run, BITMAP *dest, int x, int y);
	void ZkusOdrazit(int tm, GMap *map);
	void ChangeZvyrazneni();
	bool IsDiffColl(int x, int y);
	void DrawZvyrazneni(BITMAP *dest, int x, int y);
	void CreateObrys();
	int m_kola_predni;
	int m_kola_zadni;

	void StopSound();
	void UpdateSound(int x, int y, int dst);
	void DrawLight(BITMAP *lmap, int x, int y);
	void UpdateShadow();
  // aktualni nastaveni auta
   double m_angle;   // uhel auta ( 0 == doprava vodorovne, dokola je 256 )
	double m_angler;  // uhel skutecneho pohybu ( pri zatackach to muze trochu klouzat )
	double m_alpha;   // uhel m_angle, ale ve stupnich !!!
	double m_alphaRad;  // uhel m_angler, ale v radianech

	double m_x, m_y;  // pozice auta 
	double m_speed;   // rychlost
	double m_gspeed;  // rychlost * DF_GAME_SPEED
	double m_otacky;

	double m_distance;   // ujeta vzdalenost  soucet speed
	double m_time;       // cas jizdy do cile (pocet framu)
	double m_distancekm; // ujeta vzdalenost km
	double m_times;      // cas jizdy do cile sekund
	double m_avs;        // prumerna rychlost km/h
	double m_max;        // max rychlost km/h

	bool   m_active; // zda je auticko aktivni ( kdyz neni, tak ani nezavodi )
	bool   m_autodriver;

  // pozadavky ridice
	bool  m_fwd;    // jizda vpred
	bool  m_back;   // jizda vzad (brzda)
	bool  m_left;   // zataceni vlevo
	bool  m_right;  // zataceni vpravo
	bool  m_hbreak; // rucni brzda 
	bool  m_crasch;  // ridic pozaduje postaveji vozu zpet na silnici

	int   m_stopped; // doba, kolik framu jiz auto stoji
	int   m_turninig; // jak dlouho uz zataci

	int   m_iqnext;  // dalsi chek point, kam se snazi nase auto dostat
	int   m_lstchp;  // dalsi chek point, kam se snazi nase auto dostat
 
//testovaci - pro UI (rozumej umelou inteligenci )
	int  m_TSTdistance;     // index n mape, kam se chceme dostat
	int  m_TSTdistanceOld;  // index n mape, odkud jedeme
	int  m_TSTdistanceNext; // index n mape, kam pojedeme
	int  m_TSTangle;

	int             m_ID;  // id carparamsu
	st_GCar_params  m_par; // parametry auta
	bool            m_poharove; // je-li true, pouziva se zesedla bitmapa auta

	GMap  *m_map;
  // init & nastavovaci fce 
	GCar();
	virtual ~GCar();
	void Init(int angle, int x, int y, bool autodriver, GMap *map);

	void SetCar(int ID, const char* name, int adriver);
	void SetCar(st_player *player);

	void SetDriver(bool fwd, bool back, bool left, bool right, bool hbreak, bool crash);
	void AutoDriver(GMap *map);

  // graficke fce 
	bool LoadGFX(bool svetlo);
	void UpdateSprite();
	void Destroy();
	void DrawSmog(BITMAP *dest, int x, int y);
	void DrawCar(BITMAP *dest, int x, int y, int svetlo);
	void DrawShadow(BITMAP *dest, int x, int y);
	bool m_svetlo;


	BITMAP *m_bSprite; // narotovana bitmapa auticka ( pouziva se pri tesu narazu a pri kresleni )
	double  m_spriteAngle; // uhel b_Sprite ( aby se nemusel stale prekreslovat ) 

	BITMAP *m_bKola[3]; //bitmapa kol auticka (i se zatocenyma kolama)


	BITMAP *m_bShadow; // narotovana bitmapa auticka ( pouziva se pri tesu narazu a pri kresleni )
	BITMAP *m_bZvyrazneni; // zluty obrys okolo auta
	double  m_shadowAngle; // uhel b_Sprite ( aby se nemusel stale prekreslovat ) 

	HDSoundBuffer m_smotor;  // zvuk motoru
	HDSoundBuffer m_ssmyk;  // zvuk motoru

	st_kour m_kour[4][MAX_KOURE]; // leve a prave zadni kolo

  // pohybove fce 
	void Move(GMap *map);

  // fce pro narazy ( tech je )
   MYRECT m_rect;    // vrcholy rotvane bitmapy auta (a s otocenim)
   MYRECT m_rect2;   // stredy hran rotvane bitmapy auta (a s otocenim)
   MYRECT m_rectkol; // stredy hran rotvane bitmapy auta (a s otocenim)
	MYRECT m_rold;    // minula pozice kol

	int    m_ground[4];  // typ povrchu pod vsemi 4 koly;

   RECT   m_recth; // vrcholy ohranicujiciho obdelika ( maximaplni a minimalni hodnoty z  m_rect - slouzi pro rychlejsi vypocet narazu)
	void   CountRect(); 
	void   SetEagle(double alpha);
	bool   PtInRect(int x, int y);
	bool   PtInRect(POINT pt);
	bool	 Crash(GObject * ob);
	bool   Crash(GMap *map, GCar *car);
	bool   Hit(GObject *ob);
	bool	 Hit(GCar *car);

 // zavody
	int   m_kol;  // pocet ujetych kol
	int   m_chp;  // dosazene check pointy
	int   m_pos[MAX_KOL_RES][MAX_CHP]; // mapa mezipozic

	char  m_dname[50];  // jmeno ridice
	int   m_adriver;    // id autodriveru;

	int   m_fpos; // final position
	int   m_show;

	int   m_bonusu; // kolik posbiralo auto bonusu

};

#endif // #ifndef _G_CAR_INCLUDE_
