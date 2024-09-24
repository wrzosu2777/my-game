// GRun.h: interface for the GRun class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _G_RUN_INCLUDE_
#define _G_RUN_INCLUDE_

#include "GClasses.h"
#include "GView.h"
#include "GCar.h"
#include "GMap.h"

#define MAX_RACING_CARS  10

#define	SVETLO_DEN  0
#define	SVETLO_NOC_HRUBE 1
#define	SVETLO_NOC_JEMNE 2

#define SEMAFOR_CERVENA  170
#define SEMAFOR_ORANZOVA 110
#define SEMAFOR_ZELENA    50
#define SEMAFOR_NENI       0

#define TIME_TO_END      -25

#define MAX_TABS 7

class GRun  // hlavni trida hry - MUZE BYT JEN JEDNA INSTANCE TETO TRIDY !!!!!!!
{
public:
	void SimulateRaceToEnd();
	void LoopVysledky();
	int GetHracStartPos(int aut);
	void TestVyrazovacka();
	void UpdateSound();
	void Demo();
	void RestoreScreen();
	void PrepareScreen();
	void ComputeFinalPos();
	void Input();
	bool TestEnd();
	bool PrepareRace(bool noc, bool stin, int aut, int hracu, int kol);
	void GenerateMap(char *file);
	void CreateShadow(int ID);
	int  CarCrash();
	void Crash();
	void Move();
	void Draw();
	bool NewRace(const char *file );
	bool LoadGFX();
	void Destroy();
	void Run();
	GRun();
	virtual ~GRun();

	GView   m_view;                  // kamera
	GView   m_view2;                  // kamera2
	GCar    m_cars[MAX_RACING_CARS]; // vsechna auta
	int     m_carsc;                 // pocet aut na trati
	GMap    m_map;                   // mapa objektu
	DWORD   m_game_speed;            // urcuje rychlost hry v milisekundach
	int     m_hracu;

	int     m_semafor;
	int     m_kol;
	int	  m_pos[MAX_KOL_RES][MAX_CHP];
	bool    m_vyrazovacka;


	bool    m_stin;						// zda se maji kreslit i stiny
	bool    m_noc;                   // zda se jede v noci

	bool    m_dokncen;               //  zda byl zavod dokoncen, nebo ho uzivatel stornoval

	bool    m_paused;

	BITMAP *m_tabs[MAX_TABS];

	bool    m_bonusy_enabled;

// static
	static bool LoadBitmaps(BITMAP **pole, int kolik, int zacatek, const char *file);
	static void DestroyBitmaps(BITMAP **pole, int kolik);
	static void  NullBitmaps(BITMAP **pole, int kolik);

	static double GetUpgradedTurning(int car, int *kolik, double nkoef);
	static double GetUpgradedBreaks(int car, int *kolik, double nkoef);
	static double GetUpgradedAcc(int car, int *kolik, double nkoef);
	static double GetUpgradedSpeed(int car, int *kolik, double nkoef);

	static double GetMaxUpgradedTurning(int car);
	static double GetMaxUpgradedBreaks(int car);
	static double GetMaxUpgradedAcc(int car);
	static double GetMaxUpgradedSpeed(int car);

	static double GetAccToSto(double acc, double maxspeed);
	static double GetSpeedKmH(double speed);
	static double GetBreaksClass(double breaks);


	static void ComputeMaxUpgrades();

	static double m_u_speed;
	static double m_u_acc;
	static double m_u_turning;
	static double m_u_breaks;
};

#endif // #ifndef _G_RUN_INCLUDE_
