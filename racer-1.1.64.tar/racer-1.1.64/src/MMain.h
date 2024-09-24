// MMain.h: interface for the SMMenu class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _M_MAIN_INCLUDE 
#define _M_MAIN_INCLUDE

#include "TButton.h"
#include "MSigle.h"
#include "MDouble.h"
#include "MNastaveni.h"
#include "MAutori.h"
#include "GRun.h"

class MMain  
{
public:
	void Demo();
	MMain();
	virtual ~MMain();

	int Select();
	void Run();  // spousteci metoda (a hlavni smycka)

	enum {  // vybery v hlavnim menu
	  e_exit   = 0, // konec
	  e_train,     // 
	  e_single,
	  e_about,
	  e_nastaveni,
	  e_demo,
	};

private :
	int Loop();
	void LoadGFX();
	void Destroy();  // v pripade, ze je zapnute setreni pameti, tak uvolnuje bitmapy jakmile uzivatel v menu nekam klikne

	int     m_carindex;

	TButton autori;
	TButton kariera;
	TButton trenink;
	TButton nastaveni;
	TButton konec;

	BITMAP  *m_tabulka;

	MDouble    m_doubleplay;
	MSigle     m_single;
	MNastaveni m_nastaveni;
	MAutori    m_autori;

	GRun       run;
    

};

#endif // #ifndef _M_MAIN_INCLUDE 
