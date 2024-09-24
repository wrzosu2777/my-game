// MKoupeAuta.h: interface for the MKoupeAuta class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MKOUPEAUTA_H__E94455B6_80A9_4FCB_BCC4_AD135982E59E__INCLUDED_)
#define AFX_MKOUPEAUTA_H__E94455B6_80A9_4FCB_BCC4_AD135982E59E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "TButton.h"
#include "player.h"


extern	BITMAP *g_menuGround;
extern	HFONT   g_font18;
extern	HFONT   g_font12;
extern	HFONT   g_font12B;
extern	HFONT   g_font24;

#ifndef MAX_TUNING_BITMAPS
#define MAX_TUNING_BITMAPS 5
#endif


class MKoupeAuta  
{
public:
	void SelPrevCar();
	void SelNextCar();
	void Init();
	void DrawTuning(int x, int y, double max, double me, double memax);
	MKoupeAuta();
	virtual ~MKoupeAuta();
	int Select();
	void Run(st_player *player);  // spousteci metoda (a hlavni smycka)

	enum {  // vybery v hlavnim menu
	  e_zpet   = 0, // konec
	  e_OK,     // 
	};

private :
	int  Loop();
	void LoadGFX();
	void Destroy();  // v pripade, ze je zapnute setreni pameti, tak uvolnuje bitmapy jakmile uzivatel v menu nekam klikne
	void LoadCar();


	TButton m_koupit;
	TButton m_prodat;
	TButton m_zpet;
	TButton m_OK;

	TButton m_L1;
	TButton m_R1;

	int m_car;

	int m_maxcat;

	st_player *m_pPlayer;
	st_player m_Player;

	BITMAP  *m_tuning[MAX_TUNING_BITMAPS];
	BITMAP  *m_bGar;
	BITMAP  *m_tabulka;

};

#endif // !defined(AFX_MKOUPEAUTA_H__E94455B6_80A9_4FCB_BCC4_AD135982E59E__INCLUDED_)
