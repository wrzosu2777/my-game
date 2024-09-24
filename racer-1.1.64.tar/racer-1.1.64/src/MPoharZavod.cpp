// MPoharZavod.cpp: implementation of the MPoharZavod class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MPoharZavod.h"
#include "GRun.h"
#include "cars.h"
#include "lang.h"
#include "trate.h"
#include "player.h"
#include "drivers.h"

const int Pohar_body[10] = {
	10, 8, 6 ,4, 3, 2, 1, 0, 0, 0,
};

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

MPoharZavod::MPoharZavod()
{
	m_bTabulka = NULL;
	m_button = NULL;
	m_sel = -1;

	GRun::NullBitmaps(&m_bPoharky[0], 6);
	GRun::NullBitmaps(&m_bTrats[0], CUP_MAX);

	m_bTrat = NULL;
	m_bGround = NULL;
}

MPoharZavod::~MPoharZavod()
{
	Destroy();
}


void MPoharZavod::LoadGFX()
{
	Destroy();

	ASSERT(m_bTabulka == NULL);
	m_bTabulka = fload_bitmap("TR");

	GRun::LoadBitmaps(&m_bPoharky[0], 6, 0, "poh%d");

	for (int i = 0; i < g_maxcup; i++)
		m_bTrats[i] = fload_bitmap(g_cup[i].bitmap);

	ASSERT(m_button == NULL);
	m_button = fload_bitmap("servs");
	CreateGround(Cparams[a_player[0].car].kategorie);
}

void MPoharZavod::Destroy()
{
	if(m_bTabulka != NULL) {
		destroy_bitmap(m_bTabulka);
		m_bTabulka = NULL;
	}

	if(m_bTrat != NULL) {
		destroy_bitmap(m_bTrat);
		m_bTrat = NULL;
	}

	GRun::DestroyBitmaps(&m_bPoharky[0], 6);

	GRun::DestroyBitmaps(&m_bTrats[0], CUP_MAX);

	if(m_button != NULL) {
		destroy_bitmap(m_button);
		m_button = NULL;
	}
	if (m_bGround != NULL) {
		destroy_bitmap(m_bGround);
		m_bGround = NULL;
	}

}

void MPoharZavod::Run()
{
	int ret;

	m_sel = -1;
	SelNext();


	LoadGFX();
	m_trat = 0;

	while (m_trat == 0)
	{
		if (SelectTrat() == e_back) {
			Destroy();
			return;
		}

		PreparePlayers();
	
		for(;;){
			if (m_trat < g_cup[m_sel].trati)
				ret = SelectRace();
			else {
				SelectVysledky();
				EndRace();
				break;
			}

			if (ret == e_back)
			{
				if (m_trat == 0)
					a_player[0].money += g_cup[m_sel].startovne;
				break;
			}

			StartRace();
			m_trat++;
		}
	}
	Destroy();
	return;
}



bool MPoharZavod::StartRace()
{
	GRun run; // promena typu GRun musi byt jen jedna !!!!!!

	Destroy();


	run.PrepareScreen();

	// trat
	if(!run.NewRace(Tparams[g_cup[m_sel].trat[m_trat]].file)) {
//		run.Destroy();
		allegro_message("Nepodarilo se nacist trat !!"); // SMULA !!!!
		return false;
	}


	// nastaveni hracu
	int i;
	for(i = 0; i < 1+g_cup[m_sel].oponentu; i++)
		run.m_cars[i].SetCar(&m_players[i]);




	run.PrepareRace(m_noc, true, g_cup[m_sel].oponentu+1, 1, g_cup[m_sel].kol);


	run.m_vyrazovacka = (g_cup[m_sel].vyrazovacka==1);

	hmp3.PlayGame();
	run.Run();
	hmp3.PlayMenu();

	// rusni bitmap
	run.Destroy();

	run.RestoreScreen();

	run.LoopVysledky();

	if (run.m_dokncen) {
		for (i = 0; i < run.m_carsc; i++) 
			m_players[i].body += Pohar_body[run.m_cars[i].m_fpos];
	}
	else {
		for (i = 1; i < run.m_carsc; i++)
			m_players[i].body += Pohar_body[run.m_cars[i].m_fpos];
	}

	for (i = 0; i < g_cup[m_sel].oponentu+1; i++)
		m_players[i].poradi = -1;

	int max, in = 0;
	for (i = 0; i < g_cup[m_sel].oponentu+1; i++) {
		max = -1;
		for (int j = 0; j < g_cup[m_sel].oponentu+1; j++)
			if (m_players[j].body > max && m_players[j].poradi == -1) {
				max = m_players[j].body;
				in = j;
			}
		m_players[in].poradi = i;
	}

	LoadGFX();
	return run.m_dokncen;
}

int MPoharZavod::SelectTrat()
{

	m_zpet.Init(30, 463, LNG_ZPET, g_font18);
	m_OK.Init(610, 463, LNG_STAR, g_font18);

	int i;
	for (i = 0; i < 8; i++)
		butt[i].Init(SelTrat[i][0], SelTrat[i][1], 70, 69, -1, -1, -1, NULL);

	int ret = LoopTrat();

	for (i = 0; i < 8; i++)
		butt[i].Destroy();


	m_zpet.Destroy();
	m_OK.Destroy();
	return ret;
}

int MPoharZavod::LoopTrat()
{
	int zavod;
	char s[100];
	m_trat = 0;
	clear_keybuf();
	for(;;){
		hscreen.AcquireBack();
		draw_sprite(hscreen.m_back, m_bGround, 0, 0);
		TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), LNG_VYBERZAVODUKARIERY, g_font24);

		sprintf(s, LNG_TRIDA, Cats[Cparams[a_player[0].car].kategorie]);
		TTDraw(hscreen.m_back, 320, 45, WIN_RGB(255, 255, 255), s, g_font12B);

		for (int z = 0; z < 8; z++) {
			zavod = GetZavod(z);
		//	zavod = 0;
			if (m_sel == zavod)
				draw_sprite(hscreen.m_back, m_button, SelTrat[z][0], SelTrat[z][1]);
		}


		sprintf(s, LNG_PENIZE, a_player[0].money);
		TTDraw(hscreen.m_back, 320, 463, WIN_RGB(255, 255, 255), s, g_font12);



		m_zpet.Draw();
		m_OK.Draw();

		hscreen.ReleaseBack();
		hscreen.Flip();

        if (keypressed()) {
            int key = readkey() >> 8;
            if (key == KEY_ESC)   return e_back;
            if (key == KEY_ENTER)
                if (a_player[0].money >= g_cup[m_sel].startovne)
                    return e_start;
            if (key == KEY_UP)
            {
                m_sel--;
                if (m_sel < 0) m_sel = 7;
            }
            if (key == KEY_DOWN || key == KEY_TAB)
            {
                m_sel = (m_sel+1)%8;
            }
        }

        
		for (int i = 0; i < 8; i++)
			if (butt[i].Update()) m_sel = GetZavod(i);


		if (m_zpet.Update()) return e_back;
		if (m_OK.Update()) 
			if (a_player[0].money >= g_cup[m_sel].startovne)
				return e_start;

	}
}


void MPoharZavod::SelNext()
{
	while (g_cup[++m_sel].kategorie != Cparams[a_player[0].car].kategorie)
		if (g_maxcup-1 <= m_sel) m_sel = -1;
}

void MPoharZavod::SelPrev()
{
	if (0 == m_sel) m_sel = g_maxcup;
	while (g_cup[--m_sel].kategorie != Cparams[a_player[0].car].kategorie)
		if (0 == m_sel) m_sel = g_maxcup;
}

int MPoharZavod::SelectRace()
{

	LoadTrat();

	m_zpet.Init(30, 463, LNG_ZPET, g_font18);
	m_OK.Init(610, 463, LNG_STAR, g_font18);

	int ret = LoopRace();

	m_zpet.Destroy();
	m_OK.Destroy();
	return ret;
}

int MPoharZavod::LoopRace()
{
	char s[100];
	int i, j;
	m_noc = (rand()%2==1);

	clear_keybuf();
	for(;;){
		draw_sprite(hscreen.m_back, g_menuGround, 0, 0);
		draw_sprite(hscreen.m_back, m_bTabulka, 133, 76);
		draw_sprite(hscreen.m_back, m_bTrat, 153, 97);

		TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), g_cup[m_sel].name, g_font24);

		TTDraw(hscreen.m_back, 150, 290, WIN_RGB(255, 255, 255), Tparams[g_cup[m_sel].trat[m_trat]].name, g_font12B, 1);
		if (g_cup[m_sel].trati > m_trat+1)
			TTDraw(hscreen.m_back, 150, 305, WIN_RGB(128, 128, 128), Tparams[g_cup[m_sel].trat[m_trat+1]].name, g_font12B, 1);
		if (g_cup[m_sel].trati > m_trat+2)
			TTDraw(hscreen.m_back, 150, 320, WIN_RGB(128, 128, 128), Tparams[g_cup[m_sel].trat[m_trat+2]].name, g_font12B, 1);

		if (m_trat != 0) {
			for (i = 0; i < g_cup[m_sel].oponentu+1 && i < 7; i++) {
				int id = 0;
				for (j = 0; j < g_cup[m_sel].oponentu+1; j++)
					if (m_players[j].poradi == i) id = j;
				if (id < 1) {
					TTDraw(hscreen.m_back, 350, 290+14*i, WIN_RGB(255, 255, 0), m_players[id].name, g_font12, 1);
					sprintf(s, "%d", m_players[id].body);
					TTDraw(hscreen.m_back, 460, 290+14*i, WIN_RGB(255, 255, 0), s, g_font12, 2);
				}
				else {
					TTDraw(hscreen.m_back, 350, 290+14*i, WIN_RGB(255, 255, 255), m_players[id].name, g_font12, 1);
					sprintf(s, "%d", m_players[id].body);
					TTDraw(hscreen.m_back, 460, 290+14*i, WIN_RGB(255, 255, 255), s, g_font12, 2);
				}
			}
		}
		else {
			for (i = 0; i < g_cup[m_sel].oponentu+1 && i < 6; i++) {
				sprintf(s, "%d",i+1);
				TTDraw(hscreen.m_back, 350, 290+14*i, WIN_RGB(255, 255, 255), s, g_font12, 1);
				sprintf(s, "%d %s", g_cup[m_sel].dotace[m_players[i].poradi], LNG_CZK);
				TTDraw(hscreen.m_back, 480, 290+14*i, WIN_RGB(255, 255, 255), s, g_font12, 2);
			}
		}

		switch (g_cup[m_sel].oponentu){
		case 0:
			sprintf(s, LNG_OPP_0);
			break;
		case 1:
			sprintf(s, LNG_OPP_1);
			break;
		case 2:
		case 3:
		case 4:
			sprintf(s, LNG_OPP_24, g_cup[m_sel].oponentu);
			break;
		default:
			sprintf(s, LNG_OPP, g_cup[m_sel].oponentu);
			break;
		}
		TTDraw(hscreen.m_back, 150, 366, WIN_RGB(255, 255, 255), s, g_font12, 1);


		if (m_noc)
			TTDraw(hscreen.m_back, 150, 380, WIN_RGB(255, 255, 255), LNG_NOC, g_font12, 1);
		else
			TTDraw(hscreen.m_back, 150, 380, WIN_RGB(255, 255, 255), LNG_DEN, g_font12, 1);

		if (g_cup[m_sel].vyrazovacka)
			TTDraw(hscreen.m_back, 150, 338, WIN_RGB(255, 255, 255), LNG_VYRAZOVACKA, g_font12, 1);

		switch (g_cup[m_sel].kol){
		case 1:
			sprintf(s, LNG_KOL_1);
			break;
		case 2:
		case 3:
		case 4:
			sprintf(s, LNG_KOL_24, g_cup[m_sel].kol);
			break;
		default:
			sprintf(s, LNG_KOL, g_cup[m_sel].kol);
			break;
		}
		TTDraw(hscreen.m_back, 150, 352, WIN_RGB(255, 255, 255), s, g_font12, 1);



		m_zpet.Draw();
		m_OK.Draw();

		hscreen.Flip();

		if (m_zpet.Update()) return e_back;
		if (m_OK.Update()) return e_start;

		if (keypressed()) {
			int key = readkey() >> 8;
			if (key == KEY_ESC)   return e_back;
			if (key == KEY_ENTER) return e_start;
		}
	}
}

void MPoharZavod::PreparePlayers()
{
	a_player[0].money -= g_cup[m_sel].startovne;
	a_player[0].body = 0;
	m_players[0] = a_player[0];
	m_players[0].poradi = 0;

	for (int j = 0; j < g_drivers; j++)
		g_driver[j].used = false;


	for(int i = 1; i < 1+g_cup[m_sel].oponentu; i++) {
		if (g_cup[m_sel].protinam) {
			m_players[i].car = MAX_CARS - i;
			strcpy(m_players[i].name, g_driver[i-1].name);
			m_players[i].gpID = -1-i;
		}
		else  {

			int u = -1;
			while (u == -1) {
				u = rand()%g_drivers;
				if (g_driver[u].used || 
					(g_driver[u].kategorie != g_cup[m_sel].kategorie &&  g_driver[u].kategorie != -1))
					u = -1;
				else
					g_driver[u].used = true;
			}
			strcpy(m_players[i].name, g_driver[u].name);
			m_players[i].gpID = -2 - u;

			if (g_driver[u].car != -1)
				m_players[i].car = g_driver[u].car;
			else
				while (Cparams[(m_players[i].car = rand()%(MAX_CARS-2))].kategorie != g_cup[m_sel].kategorie);

/*			while (Cparams[(m_players[i].car = rand()%MAX_CARS)].kategorie != g_cup[m_sel].kategorie);

			int cid = m_players[i].car;
			int u = -1;
			for (j = 0; j < g_drivers; j++)
				if (g_driver[j].car == cid && g_driver[j].used == false) {
					u = j;
					g_driver[u].used = true;
					break;
				}

			while (u == -1) {
				u = rand()%g_drivers;
				if (g_driver[u].used || 
					(g_driver[u].car != cid &&  g_driver[u].car != -1) ||
					(g_driver[u].kategorie != Cparams[cid].kategorie &&  g_driver[u].kategorie != -1))
					u = -1;
				else
					g_driver[u].used = true;
			}
			strcpy(m_players[i].name, g_driver[u].name);
			m_players[i].gpID = -2 - u;
*/
		}
		m_players[i].nacc = 1;
		m_players[i].nbreaks = 0;
		m_players[i].nspeed = 1;
		m_players[i].ntear = 1;
		m_players[i].body = 0;
		m_players[i].poradi = i;

		for (int j = 0; j < MAX_UPGRADES; j++) {
			m_players[i].upgrades[j] = g_cup[m_sel].upgr_min + rand()%(g_cup[m_sel].upgr_max+1-g_cup[m_sel].upgr_min);
			// omezeni upgradu
			if (m_players[i].upgrades[j] > Cparams[m_players[i].car].upgrade[j].max - 1)
				m_players[i].upgrades[j] = Cparams[m_players[i].car].upgrade[j].max - 1;
		}
	}
}

void MPoharZavod::EndRace()
{
		a_player[0].money += g_cup[m_sel].dotace[m_players[0].poradi];

		if (m_players[0].poradi == 0)
			a_player[0].best[m_sel].vitezstvi++;

		if (m_players[0].poradi <= m_players[0].best[m_sel].best || m_players[0].best[m_sel].best == -1) {
			a_player[0].best[m_sel].best = m_players[0].poradi;
			if (DF_OBTIZNOST > m_players[0].best[m_sel].obtiznost)
				a_player[0].best[m_sel].obtiznost = DF_OBTIZNOST;
		}

		a_player[0].best[m_sel].poradi += m_players[0].poradi;
		a_player[0].best[m_sel].jeto++;
		a_player[0].kbodu += g_cup[m_sel].body + DF_OBTIZNOST * g_cup[m_sel].dbody;
}

void MPoharZavod::SelectVysledky()
{

	m_OK.Init(320, 463, LNG_OK, g_font18);
	

	LoopVysledky();

	m_OK.Destroy();
}

void MPoharZavod::LoopVysledky()
{
	char s[100];
	int i,j;
	for(;;){
		draw_sprite(hscreen.m_back, g_menuGround, 0, 0);
		draw_sprite(hscreen.m_back, m_bTabulka, 133, 76);

		TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), LNG_VYSLEDKY_POHARU, g_font24);


		m_OK.Draw();

		int top = 240 - 16*g_cup[m_sel].oponentu;

		for (i = 0; i < g_cup[m_sel].oponentu+1; i++) {
			int id = 0;
			for (j = 0; j < g_cup[m_sel].oponentu+1; j++)
				if (m_players[j].poradi == i) id = j;
			if (id < 1) {
				TTDraw(hscreen.m_back, 173, top+30*i, WIN_RGB(255, 255, 0), m_players[id].name, g_font18, 1);
				if (i < 6)
					sprintf(s, "%d %s", g_cup[m_sel].dotace[m_players[id].poradi], LNG_CZK);
				else
					sprintf(s, "0 %s", LNG_CZK);
				TTDraw(hscreen.m_back, 467, top+30*i, WIN_RGB(255, 255, 0), s, g_font18, 2);
				sprintf(s, "%d", m_players[id].body);
				TTDraw(hscreen.m_back, 320, top+30*i, WIN_RGB(255, 255, 0), s, g_font18, 2);
			}
			else {
				TTDraw(hscreen.m_back, 173, top+30*i, WIN_RGB(255, 255, 255), m_players[id].name, g_font18, 1);
				if (i < 6)
					sprintf(s, "%d %s", g_cup[m_sel].dotace[m_players[id].poradi], LNG_CZK);
				else
					sprintf(s, "0 %s", LNG_CZK);
				TTDraw(hscreen.m_back, 467, top+30*i, WIN_RGB(255, 255, 255), s, g_font18, 2);
				sprintf(s, "%d", m_players[id].body);
				TTDraw(hscreen.m_back, 320, top+30*i, WIN_RGB(255, 255, 255), s, g_font18, 2);
			}
		}


		hscreen.Flip();

	
		if (m_OK.Update()) return;

		if (keypressed()) {
			int key = readkey() >> 8;
			if (key == KEY_ESC)   return;
			if (key == KEY_ENTER) return;
		}

	}
}

int MPoharZavod::GetZavod(int z)
{
	int old = m_sel;
	m_sel = -1;
	for (int i = -1; i < z; i++) SelNext();
	int ret = m_sel;
	m_sel = old;
	return ret;
}

void MPoharZavod::LoadTrat()
{
	char s[100];

	if(m_bTrat != NULL) {
		destroy_bitmap(m_bTrat);
		m_bTrat = NULL;
	}

	sprintf(s, "%sB", Tparams[g_cup[m_sel].trat[m_trat]].file);
	ASSERT(m_bTrat == NULL);
	m_bTrat = fload_bitmap(s);
}

void MPoharZavod::CreateGround(int cat)
{
	char s[100];
	char s2[100];
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

			TTDraw(m_bGround, textx, texty+20, WIN_RGB(255, 255, 255), uobtiznost[g_cup[zavod].obtiznost], g_font12, texta);

			int dotace = 0;
			for (int i = 0; i < 6; i++) dotace += g_cup[zavod].dotace[i];
			sprintf(s, LNG_DOTACE, dotace);
			TTDraw(m_bGround, textx, texty+42, WIN_RGB(255, 255, 255), s, g_font12, texta);



			sprintf(s, LNG_STARTOVNE, g_cup[zavod].startovne);
			if (a_player[0].money >= g_cup[zavod].startovne)
				TTDraw(m_bGround, textx, texty+31, WIN_RGB(255, 255, 255), s, g_font12, texta);
			else
				TTDraw(m_bGround, textx, texty+31, WIN_RGB(255, 0, 0), s, g_font12, texta);


			switch (g_cup[zavod].kol){
			case 1:
				sprintf(s2, LNG_KOL_1);
				break;
			case 2:
			case 3:
			case 4:
				sprintf(s2, LNG_KOL_24, g_cup[zavod].kol);
				break;
			default:
				sprintf(s2, LNG_KOL, g_cup[zavod].kol);
				break;
			}

			switch (g_cup[zavod].trati){
			case 1:
				sprintf(s, LNG_TRAT_1);
				break;
			case 2:
			case 3:
			case 4:
				sprintf(s, LNG_TRAT_24, g_cup[zavod].trati);
				break;
			default:
				sprintf(s, LNG_TRATE, g_cup[zavod].trati);
				break;
			}

			strcat(s, ", ");
			strcat(s, s2);
			TTDraw(m_bGround, textx, texty+53, WIN_RGB(255, 255, 255), s, g_font12, texta);
		}

}
