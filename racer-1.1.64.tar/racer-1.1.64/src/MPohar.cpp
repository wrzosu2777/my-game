// MPohar.cpp: implementation of the MPohar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GRun.h"
#include "cars.h"
#include "lang.h"
#include "trate.h"
#include "player.h"
#include "drivers.h"
#include "MPohar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int Pohar_body[10] = {
	10, 8, 6 ,4, 3, 2, 1, 0, 0, 0,
};

MPohar::MPohar()
{

	m_carID = 0;
	GetNextCar();

	m_noc = false;

	m_trati = MAX_TRATI;
	m_kol = 5;
	m_oponentu = 4;

	m_bonus = 10000; // penez za bonus


	m_bGar = NULL;
	m_bTrat = NULL;
	m_bTabulka = NULL;
	m_bTabulka2 = NULL;
}

MPohar::~MPohar()
{
	Destroy();
}

void MPohar::LoadCar()
{
	char s[100];

	if(m_bGar != NULL) {
		destroy_bitmap(m_bGar);
		m_bGar = NULL;
	}

	sprintf(s, "%sP_b", Cparams[m_carID].file);
	m_bGar = fload_bitmap(s);

}

void MPohar::LoadGFX()
{
	Destroy();

	ASSERT(m_bTabulka == NULL);
	m_bTabulka = fload_bitmap("TR");

	ASSERT(m_bTabulka2 == NULL);
	m_bTabulka2 = fload_bitmap("HM_1");
}

void MPohar::Destroy()
{
	if(m_bGar != NULL) {
		destroy_bitmap(m_bGar);
		m_bGar = NULL;
	}
	if(m_bTrat != NULL) {
		destroy_bitmap(m_bTrat);
		m_bTrat = NULL;
	}
	if(m_bTabulka != NULL) {
		destroy_bitmap(m_bTabulka);
		m_bTabulka = NULL;
	}
	if(m_bTabulka2 != NULL) {
		destroy_bitmap(m_bTabulka2);
		m_bTabulka2 = NULL;
	}
}

void MPohar::Run(int hracu)
{
	int ret;

	m_cID = 0;

	LoadGFX();

	m_hraci = hracu;

	if (m_hraci == 0) {
		Destroy();
		return;
	}
	if (m_hraci == 3) {
		Destroy();
		return;
	}

	if (Select() == e_back) {
		Destroy();
		return;
	}

	PrepareCup();

	for(;;){
		// vyber z menu
		if (m_cID < m_trati) ret = SelectTrat();
		else break; // dokonceno

		switch (ret) {
		case e_back :
			Destroy();
			return;
			break;
		case  e_start :
			if (!StartRace()) {
				Destroy();
				return;
			}
			m_cID++;
			break;
		}
	}

	SelectVysledky();

	Destroy();
}

int MPohar::Select()
{
	LoadCar();

	m_zpet.Init(30, 463, LNG_ZPET, g_font18);
	m_OK.Init(610, 463, LNG_OK, g_font18);
	m_L1.Init(191, 294, "<", g_font18, KEY_LEFT);
	m_R1.Init(445, 294, ">", g_font18, KEY_RIGHT);
	m_OponentiL.Init(259, 325, "<", g_font18, KEY_LEFT);
	m_OponentiR.Init(379, 325, ">", g_font18, KEY_RIGHT);
	m_TratiL.Init(259, 345, "<", g_font18, KEY_LEFT);
	m_TratiR.Init(379, 345, ">", g_font18, KEY_RIGHT);
	m_BonusL.Init(259, 365, "<", g_font18, KEY_LEFT);
	m_BonusR.Init(379, 365, ">", g_font18, KEY_RIGHT);
	m_KolL.Init(259, 385, "<", g_font18, KEY_LEFT);
	m_KolR.Init(379, 385, ">", g_font18, KEY_RIGHT);


	int ret = Loop();

	m_zpet.Destroy();
	m_OK.Destroy();
	m_L1.Destroy();
	m_R1.Destroy();
	m_OponentiL.Destroy();
	m_OponentiR.Destroy();
	m_TratiL.Destroy();
	m_TratiR.Destroy();
	m_BonusL.Destroy();
	m_BonusR.Destroy();
	m_KolL.Destroy();
	m_KolR.Destroy();

	return ret;
}

int MPohar::Loop()
{
    int selected = -1;  // This is a new hack for using keyboard to select things...
    int selections = 7;
    int buttons_cnt  = 12;
    int old_mouse_x = mouse_x;
    int old_mouse_y = mouse_y;
    TButton *buttons[] = {&m_L1, &m_R1, &m_OponentiL, &m_OponentiR, &m_TratiL, &m_TratiR, &m_BonusL, &m_BonusR, &m_KolL, &m_KolR, &m_zpet, &m_OK};
    unselect_all_buttons(buttons, buttons_cnt);
    
	char s[100];
	clear_keybuf();
	for(;;){
        
        // mouse update
        if (mouse_x != old_mouse_x || mouse_y != old_mouse_y)
        {
            unselect_all_buttons(buttons, buttons_cnt);
            selected = -1;
            old_mouse_x = mouse_x;
            old_mouse_y = mouse_y;
        }
        
        acquire_bitmap(hscreen.m_back);
        
		draw_sprite(hscreen.m_back, g_menuGround, 0, 0);
		draw_sprite(hscreen.m_back, m_bTabulka, 133, 76);
		draw_sprite(hscreen.m_back, m_bGar, 153, 97);
		
		sprintf(s, "%s ()", Cparams[m_carID].name);
		TTDraw(hscreen.m_back, 320, 294, WIN_RGB(255, 255, 255), s, g_font18);
		TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), LNG_VYBERAUTA_POHAR, g_font24);

		switch (m_oponentu){
		case 0:
			sprintf(s, LNG_OPP_0);
			break;
		case 1:
			sprintf(s, LNG_OPP_1);
			break;
		case 2:
		case 3:
		case 4:
			sprintf(s, LNG_OPP_24, m_oponentu);
			break;
		default:
			sprintf(s, LNG_OPP, m_oponentu);
			break;
		}
		TTDraw(hscreen.m_back, 320, 325, WIN_RGB(255, 255, 255), s, g_font12);

		switch (m_trati){
		case 1:
			sprintf(s, LNG_TRAT_1);
			break;
		case 2:
		case 3:
		case 4:
			sprintf(s, LNG_TRAT_24, m_trati);
			break;
		default:
			sprintf(s, LNG_TRATE, m_trati);
			break;
		}
		TTDraw(hscreen.m_back, 320, 345, WIN_RGB(255, 255, 255), s, g_font12);

		sprintf(s, "%d %s", m_bonus, LNG_CZK);
		TTDraw(hscreen.m_back, 320, 365, WIN_RGB(255, 255, 255), s, g_font12);


		switch (m_kol){
		case 1:
			sprintf(s, LNG_KOL_1);
			break;
		case 2:
		case 3:
		case 4:
			sprintf(s, LNG_KOL_24, m_kol);
			break;
		default:
			sprintf(s, LNG_KOL, m_kol);
			break;
		}
		TTDraw(hscreen.m_back, 320, 385, WIN_RGB(255, 255, 255), s, g_font12);


		m_zpet.Draw();
		m_OK.Draw();
		m_L1.Draw();
		m_R1.Draw();
		m_OponentiL.Draw();
		m_OponentiR.Draw();
		m_TratiL.Draw();
		m_TratiR.Draw();
		m_BonusL.Draw();
		m_BonusR.Draw();
		m_KolL.Draw();
		m_KolR.Draw();

		hscreen.Flip();

        int inkey = 0;
        if (keypressed())
        {
            inkey = readkey() >> 8;
            if (inkey == KEY_ESC) return e_back;
            if (inkey == KEY_UP)
            {
                unselect_all_buttons(buttons, buttons_cnt);
                if (--selected < 0)
                    selected = selections-1;
                if (selected < 5)
                {
                    buttons[2*selected]->m_keyboard_selected = true;
                    buttons[2*selected+1]->m_keyboard_selected = true;
                }
                else
                {
                    buttons[selected+5]->m_keyboard_selected = true;
                }
            }
            if (inkey == KEY_DOWN || inkey == KEY_TAB)
            {
                unselect_all_buttons(buttons, buttons_cnt);
                selected = (selected+1)%selections;
                if (selected < 5)
                {
                    buttons[2*selected]->m_keyboard_selected = true;
                    buttons[2*selected+1]->m_keyboard_selected = true;
                }
                else
                {
                    buttons[selected+5]->m_keyboard_selected = true;
                }
                
            }
        }
        
	
		if (m_zpet.Update(inkey)) return e_back;
		if (m_OK.Update(inkey)) return e_next;

		if (m_L1.Update(inkey)) {
			GetPrevCar();
			LoadCar();
		}
		if (m_R1.Update(inkey)) {
			GetNextCar();
			LoadCar();
		}
		if (m_OponentiL.Update(inkey))
			if (m_oponentu + m_hraci > 2) m_oponentu--;
		if (m_OponentiR.Update(inkey))
			if (m_oponentu + m_hraci < 6) m_oponentu++;


		if (m_BonusL.Update(inkey))
			if (m_bonus > 20000) m_bonus -= 10000;
		if (m_BonusR.Update(inkey))
			if (m_bonus < 100000) m_bonus += 10000;
		if (m_TratiL.Update(inkey))
			if (m_trati > 3) m_trati--;
		if (m_TratiR.Update(inkey))
			if (m_trati < MAX_TRATI) m_trati++;
		if (m_KolL.Update(inkey))
			if (m_kol > 1) m_kol--;
		if (m_KolR.Update(inkey))
			if (m_kol < MAX_KOL) m_kol++;
	}
}


bool MPohar::StartRace()
{
	UpgradeOpponents(); // oponenti chteji taky upgradovat :)

	GRun run; // promena typu GRun musi byt jen jedna !!!!!!

	Destroy();

	run.PrepareScreen();

	// trat
	if(!run.NewRace(Tparams[m_tratID[m_cID]].file)) {
		allegro_message("Nepodarilo se nacist trat !!"); // SMULA !!!!
		return false;
	}


	int i;
	for (i = 0; i < m_oponentu+m_hraci; i++) {
		run.m_cars[i].SetCar(&m_players[i]);
		if (i >= m_hraci)
			run.m_cars[i].m_poharove = true;
	}


	run.PrepareRace(m_noc, true, m_oponentu+m_hraci, m_hraci, m_kol);

	run.m_bonusy_enabled = true; // pustime bonusy
//	allegro_message("Race ready %d", m_carID[0]);


//	hscreen.SetBBuffer();
	hmp3.PlayGame();
	run.Run();
	hmp3.PlayMenu();
//	hscreen.Set2Pages();

	// rusni bitmap
	run.Destroy();

	run.RestoreScreen();

	run.LoopVysledky();

	LoadGFX();

	if (run.m_dokncen) {
		for (i = 0; i < run.m_carsc; i++) {
			m_players[i].body += Pohar_body[run.m_cars[i].m_fpos];
			m_players[i].money += m_bonus*run.m_cars[i].m_bonusu;
		}

		for (i = 0; i < m_oponentu+m_hraci; i++)
			m_players[i].poradi = -1;


		int max, in = 0;
		for (i = 0; i < m_oponentu+m_hraci; i++) {
			max = -1;
			for (int j = 0; j < m_oponentu+m_hraci; j++)
				if (m_players[j].body > max && m_players[j].poradi == -1) {
					max = m_players[j].body;
					in = j;
				}
			m_players[in].poradi = i;
		}
	}



	return run.m_dokncen;
}

int MPohar::SelectTrat()
{
	LoadTrat();
	m_noc = (rand()%2) == 1;

	m_zpet.Init(30, 463, LNG_ZPET, g_font18);
	m_OK.Init(610, 463, LNG_STAR, g_font18);
	m_Autoservis.Init(320, 463, LNG_AUTOSERVIS,g_font18);

	int ret = LoopTrat();

	m_zpet.Destroy();
	m_OK.Destroy();
	m_Autoservis.Destroy();

	return ret;
}

int MPohar::LoopTrat()
{

	char s[100];
	int i, j;
	clear_keybuf();
	for(;;){
		draw_sprite(hscreen.m_back, g_menuGround, 0, 0);
		draw_sprite(hscreen.m_back, m_bTabulka, 133, 76);
		draw_sprite(hscreen.m_back, m_bTrat, 153, 97);
		sprintf(s, LNG_ZAVOD_CISLO, m_cID+1);
		TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), s, g_font24);

		int od = m_cID;
		if (od > m_trati - 3) od = m_trati-3;
		for (i = od; i < od+3; i++)
			if (m_cID == i)
				TTDraw(hscreen.m_back, 150, 290+15*(i-od), WIN_RGB(255, 255, 255), Tparams[m_tratID[i]].name, g_font12B, 1);
			else
				TTDraw(hscreen.m_back, 150, 290+15*(i-od), WIN_RGB(128, 128, 128), Tparams[m_tratID[i]].name, g_font12B, 1);

		for (i = 0; i < m_oponentu+m_hraci; i++) {
			int id = 0;
			for (j = 0; j < m_oponentu+m_hraci; j++)
				if (m_players[j].poradi == i) id = j;
			if (id < m_hraci) {
				TTDraw(hscreen.m_back, 350, 290+14*i, WIN_RGB(255, 255, 0), m_players[id].name, g_font12, 1);
				sprintf(s, "%d", m_players[id].body);
				TTDraw(hscreen.m_back, 480, 290+14*i, WIN_RGB(255, 255, 0), s, g_font12, 2);
			}
			else {
				TTDraw(hscreen.m_back, 350, 290+14*i, WIN_RGB(255, 255, 255), m_players[id].name, g_font12, 1);
				sprintf(s, "%d", m_players[id].body);
				TTDraw(hscreen.m_back, 480, 290+14*i, WIN_RGB(255, 255, 255), s, g_font12, 2);
			}
		}

		switch (m_oponentu){
		case 0:
			sprintf(s, LNG_OPP_0);
			break;
		case 1:
			sprintf(s, LNG_OPP_1);
			break;
		case 2:
		case 3:
		case 4:
			sprintf(s, LNG_OPP_24, m_oponentu);
			break;
		default:
			sprintf(s, LNG_OPP, m_oponentu);
			break;
		}
		TTDraw(hscreen.m_back, 150, 368, WIN_RGB(255, 255, 255), s, g_font12, 1);


		if (m_noc)
			TTDraw(hscreen.m_back, 150, 380, WIN_RGB(255, 255, 255), LNG_NOC, g_font12, 1);
		else
			TTDraw(hscreen.m_back, 150, 380, WIN_RGB(255, 255, 255), LNG_DEN, g_font12, 1);


		switch (m_kol){
		case 1:
			sprintf(s, LNG_KOL_1);
			break;
		case 2:
		case 3:
		case 4:
			sprintf(s, LNG_KOL_24, m_kol);
			break;
		default:
			sprintf(s, LNG_KOL, m_kol);
			break;
		}
		TTDraw(hscreen.m_back, 150, 356, WIN_RGB(255, 255, 255), s, g_font12, 1);


		m_zpet.Draw();
		m_OK.Draw();
		m_Autoservis.Draw();

		hscreen.Flip();

		if (m_zpet.Update()) return e_back;
		if (m_OK.Update()) return e_start;

		if (m_Autoservis.Update()) {
			m_autoservis.Run(&m_players[0], true);
			if (m_hraci == 2)
				m_autoservis.Run(&m_players[1], true);
		}

		if (keypressed()) {
			int key = readkey() >> 8;
			if (key == KEY_ESC)   return e_back;
			if (key == KEY_ENTER) return e_start;
		}


	}
}

void MPohar::LoadTrat()
{
	char s[100];

	if(m_bTrat != NULL) {
		destroy_bitmap(m_bTrat);
		m_bTrat = NULL;
	}

	sprintf(s, "%sB", Tparams[m_tratID[m_cID]].file);
	ASSERT(m_bTrat == NULL);
	m_bTrat = fload_bitmap(s);
}

void MPohar::GetNextCar()
{
	do {
		if (++m_carID >= MAX_CARS) m_carID = 0;
	} while (Cparams[m_carID].pohar == false);
}

void MPohar::GetPrevCar()
{
	do {
		if (--m_carID < 0) m_carID = MAX_CARS-1;
	} while (Cparams[m_carID].pohar == false);
}

void MPohar::PrepareCup()
{
	int i, j;
	for (i = 0; i < m_trati; i++) {
		int ID;
		do {
			ID = rand()%MAX_TRATI;
			for (j = 0; j < i; j++)
				if (m_tratID[j] == ID) break;
		} while(j < i && m_tratID[j] == ID);
		m_tratID[i] = ID;
	}

	for (j = 0; j < g_drivers; j++)
		g_driver[j].used = false;

	for (i = 0; i < MAX_POHAR_PLAYERS; i++) {
		m_players[i].car  = m_carID + rand()%2;
		m_players[i].body = 0;
		m_players[i].poradi = i;
		m_players[i].money = m_bonus * 3;
		m_players[i].nacc = 1;
		m_players[i].nbreaks = 1;
		m_players[i].nspeed = 1;
		m_players[i].ntear = 1;
		m_players[i].gpID = -1;
		ZeroMemory(&m_players[i].upgrades[0], sizeof(int)*MAX_UPGRADES);

		int cid = m_players[i].car;
		if (i >= m_hraci) {
			int u;
			u = -1;
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
		}
	}

	strcpy(m_players[0].name, a_player[0].name);
	if (m_hraci == 2)
		strcpy(m_players[1].name, a_player[1].name);
}

void MPohar::SelectVysledky()
{

	m_OK.Init(320, 463, LNG_OK, g_font18);
	

	LoopVysledky();

	m_OK.Destroy();
}

void MPohar::LoopVysledky()
{
	char s[100];
	int i,j;
	clear_keybuf();
	for(;;){
		draw_sprite(hscreen.m_back, g_menuGround, 0, 0);
		draw_sprite(hscreen.m_back, m_bTabulka, 133, 76);

		TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), LNG_VYSLEDKY_POHARU, g_font24);


		m_OK.Draw();

		int top = 240 - 16*(m_oponentu+m_hraci);

		for (i = 0; i < m_oponentu+m_hraci; i++) {
			int id = 0;
			for (j = 0; j < m_oponentu+m_hraci; j++)
				if (m_players[j].poradi == i) id = j;
			if (id < m_hraci) {
				TTDraw(hscreen.m_back, 173, top+30*i, WIN_RGB(255, 255, 0), m_players[id].name, g_font18, 1);
				sprintf(s, "%d", m_players[id].body);
				TTDraw(hscreen.m_back, 467, top+30*i, WIN_RGB(255, 255, 0), s, g_font18, 2);
			}
			else {
				TTDraw(hscreen.m_back, 173, top+30*i, WIN_RGB(255, 255, 255), m_players[id].name, g_font18, 1);
				sprintf(s, "%d", m_players[id].body);
				TTDraw(hscreen.m_back, 467, top+30*i, WIN_RGB(255, 255, 255), s, g_font18, 2);
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

void MPohar::UpgradeOpponents()
{
	for (int i = m_hraci; i < m_hraci+m_oponentu; i++) {
		m_players[i].nbreaks = 0; // nechceme, aby nam to zbytecne moc brzdilo :)
		for (int j = 0; j < 20; j++) { // nahodne zkusime upradovat 20 veci. Kdyz na to nebude, tak holt nebude
			int sel = rand()%MAX_UPGRADES;
			if (m_players[i].upgrades[sel] < Cparams[m_players[i].car].upgrade[sel].max - 1) 
				if (Cparams[m_players[i].car].upgrade[sel].cena <= m_players[i].money) {
					m_players[i].money -= Cparams[m_players[i].car].upgrade[sel].cena;
					m_players[i].upgrades[sel]++;
				}
		}
	}
}
