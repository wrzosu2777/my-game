// MNastaveni.h: interface for the MNastaveni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNASTAVENI_H__3EEE716F_40A5_4ECC_8066_DED8EB0ACB85__INCLUDED_)
#define AFX_MNASTAVENI_H__3EEE716F_40A5_4ECC_8066_DED8EB0ACB85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TButton.h"

extern	BITMAP *g_menuGround;
extern	HFONT   g_font18;
extern	HFONT   g_font12;
extern	HFONT   g_font12B;
extern	HFONT   g_font24;

class MNastaveni  
{
public:
	MNastaveni();
	virtual ~MNastaveni();
	int Select();
	void Run();  // spousteci metoda (a hlavni smycka)

	enum {  // vybery v hlavnim menu
	  e_zpet   = 0, // konec
	  e_OK,
	};

private :
	int  Loop();
	void LoadGFX();
	void Destroy();  // v pripade, ze je zapnute setreni pameti, tak uvolnuje bitmapy jakmile uzivatel v menu nekam klikne

	TButton hrac1;
	TButton hrac1l;
	TButton hrac1r;
	TButton hrac2;
	TButton hrac2l;
	TButton hrac2r;
	TButton hudbal;
	TButton hudbar;
	TButton zvukl;
	TButton zvukr;
	TButton stinl;
	TButton stinr;
	TButton svetlol;
	TButton svetlor;
	TButton rozlisenil;
	TButton rozlisenir;
	TButton obtiznostl;
	TButton obtiznostr;
	TButton rychlostl;
	TButton rychlostr;
	TButton zpet;
	TButton OK;

	BITMAP  *m_tabulka[3];
	BITMAP  *m_volume[3];

	int  m_pl;

};

#endif // !defined(AFX_MNASTAVENI_H__3EEE716F_40A5_4ECC_8066_DED8EB0ACB85__INCLUDED_)
