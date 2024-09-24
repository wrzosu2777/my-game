// MAutoservis.h: interface for the MAutoservis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAUTOSERVIS_H__0A6FB050_56DA_4FE7_8C7F_29A953227C74__INCLUDED_)
#define AFX_MAUTOSERVIS_H__0A6FB050_56DA_4FE7_8C7F_29A953227C74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TButton.h"
#include "MButton.h"
#include "player.h"

extern	BITMAP *g_menuGround;
extern	HFONT   g_font18;
extern	HFONT   g_font12;
extern	HFONT   g_font12B;
extern	HFONT   g_font24;

#define MAX_VISIBLE_UPGR 4

#ifndef MAX_TUNING_BITMAPS
#define MAX_TUNING_BITMAPS 5
#endif

class MAutoservis  
{
public:
	void DrawTuning(int x, int y, double max, double me, double memax);
	MAutoservis();
	virtual ~MAutoservis();

	int Select();
	void Run(st_player *player, bool showname);  // spousteci metoda (a hlavni smycka)

	enum {  // vybery v hlavnim menu
	  e_zpet   = 0, // konec
	  e_OK,     // 
	};

private :

	bool m_showname;

	int  Loop();
	void LoadGFX();
	void Destroy();  // v pripade, ze je zapnute setreni pameti, tak uvolnuje bitmapy jakmile uzivatel v menu nekam klikne

	void saveCar();
	void nextCar(int delta);

	TButton OK;
	TButton zpet;
	TButton koupit;

	TButton accL;
	TButton accR;
	TButton speedL;
	TButton speedR;
	TButton breaksL;
	TButton breaksR;
	TButton turningL;
	TButton turningR;
	TButton carL;
	TButton carR;


	MButton butt[MAX_UPGRADES];

	int m_selected;

	BITMAP  *m_tabulka;
	BITMAP  *m_auto;

	BITMAP  *m_upgrades[MAX_UPGRADES][MAX_UPGRADED];
	BITMAP  *m_button;
	BITMAP  *m_visible[MAX_UPGRADES][MAX_VISIBLE_UPGR];
	BITMAP  *m_upgr;
	BITMAP  *m_upZel;
	BITMAP  *m_upRed;
	BITMAP  *m_tuning[MAX_TUNING_BITMAPS];

	st_player *m_pPlayer;
	st_player m_Player;
};

#endif // !defined(AFX_MAUTOSERVIS_H__0A6FB050_56DA_4FE7_8C7F_29A953227C74__INCLUDED_)
