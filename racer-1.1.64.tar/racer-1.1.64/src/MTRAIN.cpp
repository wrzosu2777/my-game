// MTrain.cpp: implementation of the MTrain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MTrain.h"
#include "GRun.h"
#include "cars.h"
#include "lang.h"
#include "trate.h"
#include "player.h"
#include "drivers.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MTrain::MTrain()
{
	m_tratID = 1;
	m_noc = false;

	m_carID[0] = 0;
	m_carID[1] = 0;

	m_kol = 3;
	m_oponentu = 8;
	m_cat = 0;
	m_vyrazovacka = false;

	m_bGar = NULL;
	m_bTrat = NULL;
	m_bTabulka = NULL;
	m_bTabulka2 = NULL;
}

MTrain::~MTrain()
{
	Destroy();
}

void MTrain::LoadCar()
{
	char s[100];

	if(m_bGar != NULL) {
		destroy_bitmap(m_bGar);
		m_bGar = NULL;
	}

	sprintf(s, "%s_B", Cparams[m_carID[m_cID]].file);
	ASSERT(m_bGar == NULL);
	m_bGar = fload_bitmap(s);

}

void MTrain::LoadGFX()
{
	Destroy();

	ASSERT(m_bTabulka == NULL);
	m_bTabulka = fload_bitmap("TR");

	ASSERT(m_bTabulka2 == NULL);
	m_bTabulka2 = fload_bitmap("HM_1");
}

void MTrain::Destroy()
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

void MTrain::Run(int hracu)
{
	int ret;
	m_duel = false;

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

	for(;;){
		// vyber z menu
		if (m_cID < 2)
			ret = Select();
		else 
			ret = SelectTrat();

		switch (ret) {
		case e_back :
			if (--m_cID < 0) {
				Destroy();
				return;
			}
			if (m_cID == 1 && m_hraci == 1) m_cID = 0;
			break;
		case e_next :
			if (m_cID == 0 && m_hraci == 2)
				m_cID = 1;
			else m_cID = 2;
			break;
		case  e_start :
			StartRace();
//			Destroy();
//			return;
			break;
		}
	}
}

int MTrain::Select()
{
	LoadCar();

	m_zpet.Init(30, 463, LNG_ZPET, g_font18);
	m_OK.Init(610, 463, LNG_OK, g_font18);
	m_L1.Init(191, 294, "<", g_font18);
	m_R1.Init(445, 294, ">", g_font18);

	int ret = Loop();

	m_zpet.Destroy();
	m_OK.Destroy();
	m_L1.Destroy();
	m_R1.Destroy();

	return ret;
}

int MTrain::Loop()
{
	char s[100];
	clear_keybuf();
	for(;;){
		draw_sprite(hscreen.m_back, g_menuGround, 0, 0);
		draw_sprite(hscreen.m_back, m_bTabulka, 133, 76);
		draw_sprite(hscreen.m_back, m_bGar, 153, 97);
		
		sprintf(s, "%s (%c)", Cparams[m_carID[m_cID]].name, Cats[Cparams[m_carID[m_cID]].kategorie]);
		TTDraw(hscreen.m_back, 320, 294, WIN_RGB(255, 255, 255), s, g_font18);
		for (int i = 0; i < 6; i++)
			TTDraw(hscreen.m_back, 153, 310 + i*12, WIN_RGB(255, 255, 255), Cparams[m_carID[m_cID]].line[i], g_font12, 1); 
		TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), LNG_VYBERAUTA, g_font24);

		if (m_hraci == 2)
			TTDraw(hscreen.m_back, 320, 45, WIN_RGB(255, 255, 255), a_player[m_cID].name, g_font18);
		
		m_zpet.Draw();
		m_OK.Draw();
		m_L1.Draw();
		m_R1.Draw();

		hscreen.Flip();

		if (m_zpet.Update()) return e_back;
		if (m_OK.Update()) return e_next;

		if (m_L1.Update()) {
			if (--m_carID[m_cID] <  0) m_carID[m_cID] = MAX_CARS-1;
			LoadCar();
		}
		if (m_R1.Update()) {
			if (++m_carID[m_cID] >= MAX_CARS) m_carID[m_cID] = 0;
			LoadCar();
		}

		if (keypressed()) {
			int key = readkey() >> 8;
			if (key == KEY_LEFT) 
			{
				if (--m_carID[m_cID] <  0) m_carID[m_cID] = MAX_CARS-1;
				LoadCar();
			}
			if (key == KEY_RIGHT) {
				if (++m_carID[m_cID] >= MAX_CARS) m_carID[m_cID] = 0;
				LoadCar();
			}
			if (key == KEY_ESC)   return e_back;
			if (key == KEY_ENTER) return e_next;
		}
	}
}

void MTrain::StartRace()
{
	int i, j, u;
	GRun run; // promena typu GRun musi byt jen jedna !!!!!!

	Destroy();


	run.PrepareScreen();

	// trat
	if(!run.NewRace(Tparams[m_tratID].file)) {
//		run.Destroy();
		allegro_message("Nepodarilo se nacist trat !!"); // SMULA !!!!
		return;
	}


	if (m_duel) {
		run.m_cars[0].SetCar(&a_player[0]);
		run.m_cars[1].SetCar(&a_player[1]);
	}
	else 
	{
		run.m_cars[0].SetCar(m_carID[0], a_player[0].name, -1);
		if (m_hraci == 2)
			run.m_cars[1].SetCar(m_carID[1], a_player[1].name, -1);
	}

	for (j = 0; j < g_drivers; j++)
		g_driver[j].used = false;

	int cid;
	for(i = m_hraci; i < m_oponentu+m_hraci; i++) {
			while (Cparams[(cid = rand()%MAX_CARS)].kategorie != m_cat);

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

			run.m_cars[i].SetCar(cid, g_driver[u].name, u);
		}



	run.PrepareRace(m_noc, true, m_oponentu+m_hraci, m_hraci, m_kol);


	run.m_vyrazovacka = m_vyrazovacka;

	hmp3.PlayGame();
	run.Run();
	hmp3.PlayMenu();

	// rusni bitmap
	run.Destroy();

	run.RestoreScreen();

	run.LoopVysledky();


	LoadGFX();
}

int MTrain::SelectTrat()
{

	LoadTrat();

	m_zpet.Init(30, 463, LNG_ZPET, g_font18);
	m_OK.Init(610, 463, LNG_STAR, g_font18);
	m_L1.Init(191, 294, "<", g_font18,KEY_LEFT);
	m_R1.Init(445, 294, ">", g_font18,KEY_RIGHT);
	m_OponentiL.Init(259, 325, "<", g_font18,KEY_LEFT);
	m_OponentiR.Init(379, 325, ">", g_font18,KEY_RIGHT);
	m_TridaL.Init(259, 345, "<", g_font18,KEY_LEFT);
	m_TridaR.Init(379, 345, ">", g_font18,KEY_RIGHT);
	m_DenL.Init(259, 365, "<", g_font18,KEY_LEFT);
	m_DenR.Init(379, 365, ">", g_font18,KEY_RIGHT);
	m_KolL.Init(259, 385, "<", g_font18,KEY_LEFT);
	m_KolR.Init(379, 385, ">", g_font18,KEY_RIGHT);

	int ret = LoopTrat();

	m_zpet.Destroy();
	m_OK.Destroy();
	m_L1.Destroy();
	m_R1.Destroy();
	m_TridaL.Destroy();
	m_TridaR.Destroy();
	m_OponentiL.Destroy();
	m_OponentiR.Destroy();
	m_DenL.Destroy();
	m_DenR.Destroy();
	m_KolL.Destroy();
	m_KolR.Destroy();
	return ret;
}


int MTrain::LoopTrat()
{
    int selected = 0;  // This is a new hack for using keyboard to select things...
    int selections_cnt  = 5;
    int buttons_cnt  = 10;
    int old_mouse_x = mouse_x;
    int old_mouse_y = mouse_y;
    TButton *buttons[] = {&m_L1, &m_R1, &m_OponentiL, &m_OponentiR, &m_TridaL, &m_TridaR,  &m_DenL, &m_DenR, &m_KolL, &m_KolR};
    unselect_all_buttons(buttons, buttons_cnt);
    buttons[2*selected]->m_keyboard_selected = true;
    buttons[2*selected+1]->m_keyboard_selected = true;
    
	m_cat = Cparams[m_carID[0]].kategorie;
	char s[100];
	clear_keybuf();
	for(;;){
        // mouse update
        if (mouse_x != old_mouse_x || mouse_y != old_mouse_y)
        {
            //unselect_all_buttons(buttons, buttons_cnt);
            //selected = 0;
            //old_mouse_x = mouse_x;
            //old_mouse_y = mouse_y;
            
        }
        
		draw_sprite(hscreen.m_back, g_menuGround, 0, 0);
		draw_sprite(hscreen.m_back, m_bTabulka, 133, 76);
		draw_sprite(hscreen.m_back, m_bTrat, 153, 97);

		if (m_vyrazovacka == false)
			TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), LNG_NASTAVENIZAVODU, g_font24);
		else
			TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), LNG_NASTAVENIVYRAZOVACKY, g_font24);
		TTDraw(hscreen.m_back, 320, 294, WIN_RGB(255, 255, 255), Tparams[m_tratID].name, g_font18);
		TTDraw(hscreen.m_back, 320, 309, WIN_RGB(255, 255, 255), Tparams[m_tratID].popis, g_font12);

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

		sprintf(s, LNG_TRIDA, Cats[m_cat]);
		TTDraw(hscreen.m_back, 320, 345, WIN_RGB(255, 255, 255), s, g_font12);


		if (m_noc)
			TTDraw(hscreen.m_back, 320, 365, WIN_RGB(255, 255, 255), LNG_NOC, g_font12);
		else
			TTDraw(hscreen.m_back, 320, 365, WIN_RGB(255, 255, 255), LNG_DEN, g_font12);


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
		m_TridaL.Draw();
		m_TridaR.Draw();
		m_OponentiL.Draw();
		m_OponentiR.Draw();
		m_DenL.Draw();
		m_DenR.Draw();
		m_KolL.Draw();
		m_KolR.Draw();

		hscreen.Flip();

        int inkey = 0;
        if (keypressed())
        {
            inkey = readkey() >> 8;
            if (inkey == KEY_ESC)   return e_back;
            if (inkey == KEY_ENTER) return e_start;
            
            if (inkey == KEY_UP)
            {
                unselect_all_buttons(buttons, buttons_cnt);
                if (--selected < 0)
                    selected = selections_cnt-1;
                buttons[2*selected]->m_keyboard_selected = true;
                buttons[2*selected+1]->m_keyboard_selected = true;
            }
            if (inkey == KEY_DOWN  || inkey == KEY_TAB)
            {
                unselect_all_buttons(buttons, buttons_cnt);
                selected = (selected+1)%selections_cnt;
                buttons[2*selected]->m_keyboard_selected = true;
                buttons[2*selected+1]->m_keyboard_selected = true;
            }
        }
        
        
		if (m_zpet.Update()) return e_back;
		if (m_OK.Update()) return e_start;

		if (m_L1.Update(inkey)) {
			if (--m_tratID < 0) m_tratID = MAX_TRATI-1;
			LoadTrat();
		}
		
		if (m_R1.Update(inkey)) {
			if (++m_tratID >= MAX_TRATI) m_tratID = 0;
			LoadTrat();
		}

		if (m_TridaL.Update(inkey))
			if (--m_cat < 0) m_cat = 3;
		
		if (m_TridaR.Update(inkey))
			if (++m_cat > 3) m_cat = 0;

		if (m_OponentiL.Update(inkey)) if (m_oponentu > 0)  m_oponentu--;
		if (m_OponentiR.Update(inkey)) if (m_oponentu < MAX_RACING_CARS - m_hraci) m_oponentu++;


		if (m_DenL.Update(inkey)) m_noc = !m_noc;
		if (m_DenR.Update(inkey)) m_noc = !m_noc;

		if (m_KolL.Update(inkey)) if (m_kol > 1)  m_kol--;
		if (m_KolR.Update(inkey)) if (m_kol < MAX_KOL) m_kol++;
	}
}

void MTrain::LoadTrat()
{
	char s[100];

	if(m_bTrat != NULL) {
		destroy_bitmap(m_bTrat);
		m_bTrat = NULL;
	}

	sprintf(s, "%sB", Tparams[m_tratID].file);
	ASSERT(m_bTrat == NULL);
	m_bTrat = fload_bitmap(s);
}

void MTrain::RunDuel()
{
	int ret;
	m_duel = true;

	m_cID = 0;

	LoadGFX();

	m_hraci = 2;


	for(;;){
		// vyber z menu
		ret = SelectTrat();

		switch (ret) {
		case e_back :
				Destroy();
				return;
			break;
		case  e_start :
			StartRace();
			break;
		}
	}
}
