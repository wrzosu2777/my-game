// MAutori.h: interface for the MAutori class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAUTORI_H__7077CE2D_966E_4054_B7FF_84ECF82B3B41__INCLUDED_)
#define AFX_MAUTORI_H__7077CE2D_966E_4054_B7FF_84ECF82B3B41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TButton.h"

extern	BITMAP *g_menuGround;
extern	HFONT   g_font18;
extern	HFONT   g_font12;
extern	HFONT   g_font12B;
extern	HFONT   g_font24;


class MAutori  
{
public:
	MAutori();
	virtual ~MAutori();
	int Select();
	void Run();  // spousteci metoda (a hlavni smycka)

	enum {  // vybery v hlavnim menu
	  e_zpet   = 0, // konec
	};

private :
	int  Loop();
	void LoadGFX();
	void Destroy();  // v pripade, ze je zapnute setreni pameti, tak uvolnuje bitmapy jakmile uzivatel v menu nekam klikne

	TButton zpet;
	TButton m_berny;
	TButton m_beza;
	TButton m_hippo;

	BITMAP  *m_tabulka;


};

#endif // !defined(AFX_MAUTORI_H__7077CE2D_966E_4054_B7FF_84ECF82B3B41__INCLUDED_)
