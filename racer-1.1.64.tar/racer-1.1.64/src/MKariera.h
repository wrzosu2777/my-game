// MKariera.h: interface for the MKariera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MKARIERA_H__2F95FC77_7ECF_4E12_B073_9449320A11EB__INCLUDED_)
#define AFX_MKARIERA_H__2F95FC77_7ECF_4E12_B073_9449320A11EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TButton.h"

extern	BITMAP *g_menuGround;
extern	HFONT   g_font18;
extern	HFONT   g_font12;
extern	HFONT   g_font24;

#include "MAutoservis.h"
#include "MKoupeAuta.h"
#include "MPoharZavod.h"
#include "MHistorie.h"


class MKariera  
{
public:
	static void UpdateAPlayers();
	void SavePlayer();
	void InitPlayer();
	MKariera();
	virtual ~MKariera();
	int Select();
	void Run();  // spousteci metoda (a hlavni smycka)

	enum {  // vybery v hlavnim menu
	  e_zpet   = 0, // konec
	  e_zavod,
	  e_koupe,
	  e_servis,
	  e_historie,
	};

private :
	int  Loop();
	void LoadGFX();
	void Destroy();  // v pripade, ze je zapnute setreni pameti, tak uvolnuje bitmapy jakmile uzivatel v menu nekam klikne

	TButton zavod;
	TButton koupe;
	TButton servis;
	TButton historie;
	TButton zpet;

	MAutoservis m_autoservis;
	MKoupeAuta  m_koupeauta;
	MPoharZavod m_zavody;
	MHistorie   m_historie;

	BITMAP  *m_tabulka;

};

#endif // !defined(AFX_MKARIERA_H__2F95FC77_7ECF_4E12_B073_9449320A11EB__INCLUDED_)
