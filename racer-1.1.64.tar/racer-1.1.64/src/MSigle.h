// MSigle.h: interface for the MSigle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSIGLE_H__43F1E9B0_08A7_4B0F_AB45_65A16EC08595__INCLUDED_)
#define AFX_MSIGLE_H__43F1E9B0_08A7_4B0F_AB45_65A16EC08595__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TButton.h"
#include "MTrain.h"
#include "MKariera.h"
#include "MPohar.h"


extern	BITMAP *g_menuGround;
extern	HFONT   g_font18;
extern	HFONT   g_font12;
extern	HFONT   g_font24;


class MSigle  
{
public:
	MSigle();
	virtual ~MSigle();

	int Select();
	void Run();  // spousteci metoda (a hlavni smycka)

	enum {  // vybery v hlavnim menu
	  e_zpet   = 0, // konec
	  e_train,     // 
	  e_kariera,
	  e_pohar,
	  e_vyrazovacka
	};

private :
	int  Loop();
	void LoadGFX();
	void Destroy();  // v pripade, ze je zapnute setreni pameti, tak uvolnuje bitmapy jakmile uzivatel v menu nekam klikne

	TButton kariera;
	TButton trenink;
	TButton pohar;
	TButton vyrazovacka;
	TButton zpet;

	BITMAP  *m_tabulka;

	MKariera m_kariera;
	MTrain   m_train;
	MPohar   m_pohar;

};

#endif // !defined(AFX_MSIGLE_H__43F1E9B0_08A7_4B0F_AB45_65A16EC08595__INCLUDED_)
