// MHistorie.cpp: implementation of the MHistorie class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHistorie.h"
#include "GRun.h"
#include "lang.h"
#include "trate.h"
#include "player.h"

const int SelTrat[8][5] = {
	{103, 75,  182,  1,  72,  }, 
	{103, 168, 182,  1,  72,  }, 
	{103, 260, 182,  1,  72,  }, 
	{103, 354, 182,  1,  72,  }, 
	{466, 75 , 460,  2,  545, }, 
	{466, 168, 460,  2,  545, }, 
	{466, 260, 460,  2,  545, }, 
	{466, 354, 460,  2,  545, },
//	  bitmapa text,atext,pohx
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MHistorie::MHistorie()
{
	m_sel = -1;

	GRun::NullBitmaps(&m_bPoharky[0], 6);
	GRun::NullBitmaps(&m_bTrats[0], CUP_MAX);

	m_bGround = NULL;

	FILE *f = fopen(CUP_FILE, "rb");
	if (f) {
		g_maxcup = fread(g_cup, sizeof(st_cup), CUP_MAX, f);
		fclose(f);
	}

}

MHistorie::~MHistorie()
{
	Destroy();

}



void MHistorie::LoadGFX()
{
	Destroy();


	GRun::LoadBitmaps(&m_bPoharky[0], 6, 0, "poh%d");

	for (int i = 0; i < g_maxcup; i++)
		m_bTrats[i] = fload_bitmap(g_cup[i].bitmap);

	CreateGround();
}

void MHistorie::Destroy()
{
	GRun::DestroyBitmaps(&m_bPoharky[0], 6);

	GRun::DestroyBitmaps(&m_bTrats[0], CUP_MAX);

	if (m_bGround != NULL) {
		destroy_bitmap(m_bGround);
		m_bGround = NULL;
	}

}

void MHistorie::Run()
{
	m_cat = Cparams[a_player[0].car].kategorie;

	LoadGFX();
	SelectTrat();
	Destroy();
}




int MHistorie::SelectTrat()
{

	m_zpet.Init(30, 463, LNG_ZPET, g_font18);

	m_L1.Init(270, 45, "<", g_font18);
	m_R1.Init(370, 45, ">", g_font18);


	int ret = LoopTrat();


	m_zpet.Destroy();
	m_L1.Destroy();
	m_R1.Destroy();
	return ret;
}

int MHistorie::LoopTrat()
{
	char s[100];
	clear_keybuf();
	for(;;){
		hscreen.AcquireBack();
		draw_sprite(hscreen.m_back, m_bGround, 0, 0);
		TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), LNG_HIST_NADPIS, g_font24);

		sprintf(s, LNG_TRIDA, Cats[m_cat]);
		TTDraw(hscreen.m_back, 320, 45, WIN_RGB(255, 255, 255), s, g_font12B);

		sprintf(s, LNG_HIST_BODY, a_player[0].kbodu);
		TTDraw(hscreen.m_back, 320, 60, WIN_RGB(255, 255, 255), s, g_font12B);


		m_zpet.Draw();
		m_L1.Draw();
		m_R1.Draw();

		hscreen.ReleaseBack();
		hscreen.Flip();

		if (m_zpet.Update()) return 0;

		if (m_L1.Update()) {
			if (--m_cat < 0)
            {
				m_cat = 3;
            }
            CreateGround();
		}
		if (m_R1.Update()) {
			if (++m_cat > 3)
            {
				m_cat = 0;
            }
            CreateGround();
		}
		if (keypressed()) {
			int key = readkey() >> 8;
			if (key == KEY_ESC)   return 0;
			if (key == KEY_ENTER) return 0;
			if (key == KEY_LEFT)  {
				if (--m_cat < 0) m_cat = 3;
				CreateGround();
			}
			if (key == KEY_RIGHT) {
			if (++m_cat > 3)
            {
				m_cat = 0;
            }
            CreateGround();
			}
		}

	}
}


void MHistorie::SelNext()
{
	while (g_cup[++m_sel].kategorie != m_cat)
		if (g_maxcup-1 <= m_sel) m_sel = -1;
}


int MHistorie::GetZavod(int z)
{
	int old = m_sel;
	m_sel = -1;
	for (int i = -1; i < z; i++) SelNext();
	int ret = m_sel;
	m_sel = old;
	return ret;
}


void MHistorie::CreateGround()
{
	char s[100];
	int zavod;


	if (m_bGround != NULL) {
		destroy_bitmap(m_bGround);
		m_bGround = NULL;
	}
	m_bGround = fload_bitmap("menu");

		for (int z = 0; z < 8; z++) {
			zavod = GetZavod(z);
			int textx = SelTrat[z][2];
			int texty = SelTrat[z][1];
			int texta = SelTrat[z][3];

			if (m_bTrats[zavod] != NULL)
				draw_sprite(m_bGround, m_bTrats[zavod], SelTrat[z][0], SelTrat[z][1]);


			if (a_player[0].best[zavod].vitezstvi > 0)
				hmasked_spritetcm(m_bGround, m_bPoharky[a_player[0].best[zavod].obtiznost],  m_bPoharky[5], SelTrat[z][4], SelTrat[z][1]+25);

			TTDraw(m_bGround, textx, texty+5, WIN_RGB(255, 255, 255), g_cup[zavod].name, g_font12B, texta);

			if (a_player[0].best[zavod].jeto > 0) {

				sprintf(s, LNG_HIST_DOKONCEN, a_player[0].best[zavod].jeto);
				TTDraw(m_bGround, textx, texty+20, WIN_RGB(255, 255, 255), s, g_font12, texta);

				sprintf(s, LNG_HIST_VITEZSTVI, a_player[0].best[zavod].vitezstvi);
				TTDraw(m_bGround, textx, texty+31, WIN_RGB(255, 255, 255), s, g_font12, texta);

				sprintf(s, LNG_HIST_NEJLIP, a_player[0].best[zavod].best+1);
				TTDraw(m_bGround, textx, texty+42, WIN_RGB(255, 255, 255), s, g_font12, texta);
	
				sprintf(s, LNG_HIST_PRUMPORADI,  1 + a_player[0].best[zavod].poradi / a_player[0].best[zavod].jeto);
				TTDraw(m_bGround, textx, texty+53, WIN_RGB(255, 255, 255), s, g_font12, texta);
			}
		}

}
