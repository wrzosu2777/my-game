// MPohar.h: interface for the MPohar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPOHAR_H__BF716BDB_33D1_4289_8A50_2D82BAEE44D1__INCLUDED_)
#define AFX_MPOHAR_H__BF716BDB_33D1_4289_8A50_2D82BAEE44D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TButton.h"
#include "player.h"
#include "trate.h"
#include "MAutoservis.h"



extern	BITMAP *g_menuGround;
extern	HFONT   g_font18;
extern	HFONT   g_font12;
extern	HFONT   g_font12B;
extern	HFONT   g_font24;


#define MAX_POHAR_PLAYERS 10

class MPohar  
{
public:
	MPohar();
	virtual ~MPohar();
public:
	void UpgradeOpponents();
	void LoopVysledky();
	void SelectVysledky();
	void PrepareCup();
	void GetPrevCar();
	void GetNextCar();
	void LoadTrat();
	int LoopTrat();
	int SelectTrat();
	bool StartRace();
	int Select();
	void Run(int hracu);

	int m_cID;

	enum {  // vybery v menu
	  e_back   = 0, // konec
	  e_start,      // a jedem
	  e_next,
	};


private:
	void Destroy();
	void LoadGFX();
	void LoadCar();
	int  Loop();

	BITMAP *m_bGar;
	BITMAP *m_bTrat;
	BITMAP *m_bTabulka;
	BITMAP *m_bTabulka2;


	TButton m_zpet;
	TButton m_OK;
	TButton m_L1;
	TButton m_R1;
	TButton m_OponentiL;
	TButton m_OponentiR;
	TButton m_KolL;
	TButton m_KolR;
	TButton m_BonusL;
	TButton m_BonusR;
	TButton m_TratiL;
	TButton m_TratiR;
	TButton m_Autoservis;


	// poharova data
	st_player m_players[MAX_POHAR_PLAYERS];
	int       m_bonus; // penez za bonus

	int     m_tratID[MAX_TRATI];
	int     m_trati;
	int     m_carID;
	bool    m_noc;
	int     m_kol;
	int     m_oponentu;
	int     m_hraci;

	MAutoservis m_autoservis;
};

#endif // !defined(AFX_MPOHAR_H__BF716BDB_33D1_4289_8A50_2D82BAEE44D1__INCLUDED_)
