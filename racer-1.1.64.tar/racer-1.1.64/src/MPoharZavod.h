// MPoharZavod.h: interface for the MPoharZavod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPOHARZAVOD_H__019B00F7_B22E_4F25_B93E_63EA1AA29160__INCLUDED_)
#define AFX_MPOHARZAVOD_H__019B00F7_B22E_4F25_B93E_63EA1AA29160__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cup.h"

#include "TButton.h"
#include "MButton.h"
#include "player.h"

extern	BITMAP *g_menuGround;
extern	HFONT   g_font18;
extern	HFONT   g_font12;
extern	HFONT   g_font12B;
extern	HFONT   g_font24;

#define MAX_POHAR_PLAYERS 10

class MPoharZavod  
{
public:
	void CreateGround(int cat);
	void LoadTrat();
	int  GetZavod(int z);
	void EndRace();
	void PreparePlayers();
	void SelPrev();
	void SelNext();
	MPoharZavod();
	virtual ~MPoharZavod();

	int LoopTrat();
	int SelectTrat();
	bool StartRace();
	void Run();

	int SelectRace();
	int LoopRace();

	void SelectVysledky();
	void LoopVysledky();

	enum {  // vybery v menu
	  e_back   = 0, // konec
	  e_start,      // a jedem
	  e_next,
	};

private:
	void Destroy();
	void LoadGFX();

	BITMAP *m_bTabulka;

	BITMAP *m_bTrat;

	BITMAP *m_bPoharky[6];
	BITMAP *m_bTrats[CUP_MAX];
	BITMAP *m_button; // pro aktualne m_sel
	BITMAP *m_bGround;


	int    m_sel;

	bool   m_noc;

	TButton m_zpet;
	TButton m_OK;
	TButton m_L1;
	TButton m_R1;

	MButton butt[8];


	st_player m_players[MAX_POHAR_PLAYERS];
	int     m_trat; // aktualni trat zavodu
};

#endif // !defined(AFX_MPOHARZAVOD_H__019B00F7_B22E_4F25_B93E_63EA1AA29160__INCLUDED_)
