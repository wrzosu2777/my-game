// MTrain.h: interface for the MTrain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MTRAIN_H__671A1620_2912_11D4_8F75_E47AA252471C__INCLUDED_)
#define AFX_MTRAIN_H__671A1620_2912_11D4_8F75_E47AA252471C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TButton.h"

extern	BITMAP *g_menuGround;
extern	HFONT   g_font18;
extern	HFONT   g_font12;
extern	HFONT   g_font12B;
extern	HFONT   g_font24;


class MTrain  
{
public:
	void RunDuel();
	void LoadTrat();
	int LoopTrat();
	int SelectTrat();
	void StartRace();
	int Select();
	void Run(int hracu);
	MTrain();
	virtual ~MTrain();

	enum {  // vybery v menu
	  e_back   = 0, // konec
	  e_start,      // a jedem
	  e_next,
	};

	bool    m_vyrazovacka;
	bool    m_duel;

private:
	void Destroy();
	void LoadGFX();
	void LoadCar();
	int  Loop();

	int     m_cID;

	int     m_tratID;
	int     m_carID[2];
	bool    m_noc;
	int     m_kol;
	int     m_oponentu;
	int     m_hraci;
	int     m_cat; // kategorie oponentu

	BITMAP *m_bGar;
	BITMAP *m_bTrat;
	BITMAP *m_bTabulka;
	BITMAP *m_bTabulka2;


	TButton m_zpet;
	TButton m_OK;
	TButton m_L1;
	TButton m_R1;
	TButton m_TridaL;
	TButton m_TridaR;
	TButton m_OponentiL;
	TButton m_OponentiR;
	TButton m_DenL;
	TButton m_DenR;
	TButton m_KolL;
	TButton m_KolR;
};

#endif // !
