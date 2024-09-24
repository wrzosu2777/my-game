// MDouble.h: interface for the MDouble class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDOUBLE_H__74BF44C4_F070_4DF6_A1ED_C279900A253E__INCLUDED_)
#define AFX_MDOUBLE_H__74BF44C4_F070_4DF6_A1ED_C279900A253E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TButton.h"
#include "MPohar.h"
#include "MTrain.h"

extern	BITMAP *g_menuGround;
extern	HFONT   g_font18;
extern	HFONT   g_font12;
extern	HFONT   g_font24;


class MDouble  
{
public:
	MDouble();
	virtual ~MDouble();
	int Select();
	void Run();  // spousteci metoda (a hlavni smycka)

	enum {  // vybery v hlavnim menu
	  e_zpet   = 0, // konec
	  e_train,     // 
	  e_pohar,
	  e_duel,
	  e_vyrazovacka
	};

private :
	int  Loop();
	void LoadGFX();
	void Destroy();  // v pripade, ze je zapnute setreni pameti, tak uvolnuje bitmapy jakmile uzivatel v menu nekam klikne

	TButton duel;
	TButton trenink;
	TButton pohar;
	TButton vyrazovacka;
	TButton zpet;

	BITMAP  *m_tabulka;

	MPohar  m_pohar;
	MTrain  m_train;
};

#endif // !defined(AFX_MDOUBLE_H__74BF44C4_F070_4DF6_A1ED_C279900A253E__INCLUDED_)
