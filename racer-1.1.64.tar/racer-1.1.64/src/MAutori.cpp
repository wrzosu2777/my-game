// MAutori.cpp: implementation of the MAutori class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MAutori.h"
#include "lang.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MAutori::MAutori()
{
	m_tabulka = NULL;
}

MAutori::~MAutori()
{
	Destroy();
}

void MAutori::LoadGFX()
{
	Destroy();

	ASSERT(m_tabulka == NULL);
	m_tabulka = fload_bitmap("AUTORI");
	
}

void MAutori::Run()
{
	Select();
}

void MAutori::Destroy()
{
	if(m_tabulka != NULL) {
		destroy_bitmap(m_tabulka);
		m_tabulka = NULL;
	}
}


int MAutori::Select()
{
	LoadGFX();


	zpet.Init(30, 463, LNG_ZPET, g_font18);


//	hscreen.m_mouse = true;

	int ret = Loop();

	zpet.Destroy();
	
	return ret;
}

int MAutori::Loop()
{
	clear_keybuf();
	for(;;){
		acquire_bitmap(hscreen.m_back);

		draw_sprite(hscreen.m_back, g_menuGround, 0, 0);
//		draw_sprite(hscreen.m_back, m_tabulka, 133, 76);

		TTDraw(hscreen.m_back, 320, 200, WIN_RGB(255, 255, 255), "Bernard Lidicky - bernard@alderan.cz ", g_font12B, 0);
		TTDraw(hscreen.m_back, 320, 220, WIN_RGB(255, 255, 255), "Zdenek Boswart - 2zdeny@seznam.cz", g_font12B, 0);
		TTDraw(hscreen.m_back, 320, 240, WIN_RGB(255, 255, 255), "hippo.nipax.cz", g_font12B, 0);
		

		zpet.Draw();
		release_bitmap(hscreen.m_back);

	   hscreen.Flip();

		if (zpet.Update()) return e_zpet;

		if (keypressed())
			if (readkey()) return e_zpet;

	}
}
