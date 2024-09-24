// MHistorie.h: interface for the MHistorie class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHISTORIE_H__7AA83949_D647_4E96_8218_EC55AABCE661__INCLUDED_)
#define AFX_MHISTORIE_H__7AA83949_D647_4E96_8218_EC55AABCE661__INCLUDED_

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


class MHistorie  
{
public:
	MHistorie();
	virtual ~MHistorie();

	void CreateGround();
	int  GetZavod(int z);
	void SelNext();

	int LoopTrat();
	int SelectTrat();
	void Run();

private:

	int m_cat; // aktualni kategorie
	int m_sel; // pomocna promena

	void Destroy();
	void LoadGFX();


	BITMAP *m_bPoharky[6];
	BITMAP *m_bTrats[CUP_MAX];
	BITMAP *m_bGround;


	st_cup m_cup[CUP_MAX];
	int    m_maxcup;

	TButton m_zpet;
	TButton m_L1;
	TButton m_R1;
};

#endif // !defined(AFX_MHISTORIE_H__7AA83949_D647_4E96_8218_EC55AABCE661__INCLUDED_)
