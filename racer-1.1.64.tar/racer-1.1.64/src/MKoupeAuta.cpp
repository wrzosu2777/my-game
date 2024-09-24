// MKoupeAuta.cpp: implementation of the MKoupeAuta class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MKoupeAuta.h"
#include "lang.h"
#include "cars.h"
#include "GRun.h"
#include "cup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MKoupeAuta::MKoupeAuta()
{
	m_tabulka = NULL;
	m_bGar = NULL;
	m_maxcat = 3;
	GRun::NullBitmaps(&m_tuning[0], MAX_TUNING_BITMAPS);
}

MKoupeAuta::~MKoupeAuta()
{
	Destroy();
}

void MKoupeAuta::LoadGFX()
{
	Destroy();

	ASSERT(m_tabulka == NULL);
	m_tabulka = fload_bitmap("TR");
	GRun::LoadBitmaps(&m_tuning[0], MAX_TUNING_BITMAPS, 0, "Tuning%d");
	
}

void MKoupeAuta::Run(st_player *player)
{
	m_pPlayer = player;
	m_Player = *player;

	m_car = m_Player.car;
	if (m_car == -1) m_car = 0;

	int ret;

	Init();

	// vyber z menu
	if((ret = Select()) == e_OK) 
		*m_pPlayer = m_Player;
	
}

void MKoupeAuta::Destroy()
{
	if(m_tabulka != NULL) {
		destroy_bitmap(m_tabulka);
		m_tabulka = NULL;
	}
	GRun::DestroyBitmaps(&m_tuning[0], MAX_TUNING_BITMAPS);
	
	if(m_bGar != NULL) {
		destroy_bitmap(m_bGar);
		m_bGar = NULL;
	}
}

int MKoupeAuta::Select()
{
	LoadGFX();
	LoadCar();

	m_zpet.Init(30, 463, LNG_ZPET, g_font18);
	m_OK.Init(610, 463, LNG_OK, g_font18);
	m_koupit.Init(320, 380, LNG_KOUPIT_VUZ, g_font12B);
	m_prodat.Init(320, 380, LNG_PRODAT_VUZ, g_font12B);
	m_L1.Init(191, 294, "<", g_font18);
	m_R1.Init(445, 294, ">", g_font18);

	int ret = Loop();

	m_zpet.Destroy();
	m_OK.Destroy();
	m_L1.Destroy();
	m_R1.Destroy();

	Destroy();

	return ret;
}


int MKoupeAuta::Loop()
{
    int selected = -1;  // This is a new hack for using keyboard to select things...
    int buttons_cnt  = 3;
    int old_mouse_x = mouse_x;
    int old_mouse_y = mouse_y;
    TButton *buttons[] = {&m_zpet, &m_OK, &m_koupit};
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
		draw_sprite(hscreen.m_back, m_tabulka, 133, 76);
		draw_sprite(hscreen.m_back, m_bGar, 153, 97);
		
		sprintf(s, "%s (%c)", Cparams[m_car].name, Cats[Cparams[m_car].kategorie]);
		TTDraw(hscreen.m_back, 320, 294, WIN_RGB(255, 255, 255), s, g_font18);

		TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), LNG_KOUPE_VOZU, g_font24);
//		if (m_Player.car != -1)
//			TTDraw(hscreen.m_back, 320, 45, WIN_RGB(255, 255, 255), Cparams[m_Player.car].name, g_font18);

		TTDraw(hscreen.m_back, 155, 315, WIN_RGB(255, 255, 255), LNG_ZRYCHLENI, g_font12B, 1);
		TTDraw(hscreen.m_back, 155, 329, WIN_RGB(255, 255, 255), LNG_MAXSPEED, g_font12B, 1);
		TTDraw(hscreen.m_back, 155, 343, WIN_RGB(255, 255, 255), LNG_ZATACENI, g_font12B, 1);

		DrawTuning(290, 315, GRun::m_u_acc, GRun::GetUpgradedAcc(m_car, &m_Player.upgrades[0], 0), GRun::GetMaxUpgradedAcc(m_car));
		DrawTuning(290, 329, GRun::m_u_speed, GRun::GetUpgradedSpeed(m_car, &m_Player.upgrades[0], 0), GRun::GetMaxUpgradedSpeed(m_car));
		DrawTuning(290, 343, GRun::m_u_turning, GRun::GetUpgradedTurning(m_car, &m_Player.upgrades[0], 0), GRun::GetMaxUpgradedTurning(m_car));

		sprintf(s, "%.1f / %.1f s", GRun::GetAccToSto(GRun::GetUpgradedAcc(m_car, &m_Player.upgrades[0], 0), GRun::GetUpgradedSpeed(m_car, &m_Player.upgrades[0], 0)), GRun::GetAccToSto(GRun::GetMaxUpgradedAcc(m_car), GRun::GetMaxUpgradedSpeed(m_car)));
		TTDraw(hscreen.m_back, 430 , 315, WIN_RGB(255, 255, 255), s, g_font12, 1);
		sprintf(s, "%.0f / %.0f km/h", GRun::GetSpeedKmH(GRun::GetUpgradedSpeed(m_car, &m_Player.upgrades[0], 0)), GRun::GetSpeedKmH(GRun::GetMaxUpgradedSpeed(m_car)));
		TTDraw(hscreen.m_back, 430 , 329, WIN_RGB(255, 255, 255), s, g_font12, 1);
		sprintf(s, "%.1f / %.1f", GRun::GetUpgradedTurning(m_car, &m_Player.upgrades[0], 0), GRun::GetMaxUpgradedTurning(m_car));
		TTDraw(hscreen.m_back, 430 , 343, WIN_RGB(255, 255, 255), s, g_font12, 1);


		sprintf(s, LNG_CENA, Cparams[m_car].cena);
		if (m_Player.money >= Cparams[m_car].cena)
			TTDraw(hscreen.m_back, 320, 365, WIN_RGB(255, 255, 255), s, g_font12B);
		else
			TTDraw(hscreen.m_back, 320, 365, WIN_RGB(255, 0, 0), s, g_font12B);

		sprintf(s, LNG_PENIZE, m_Player.money);
		TTDraw(hscreen.m_back, 320, 60, WIN_RGB(255, 255, 255), s, g_font12);



		m_zpet.Draw();
		if (!m_Player.cars[m_car].owned) m_koupit.Draw();
		m_OK.Draw();
		m_L1.Draw();
		m_R1.Draw();

		hscreen.Flip();
	

//      Sleep(10);
        int inkey = 0;
        if (keypressed())
        {
            inkey = readkey() >> 8;
            if (inkey == KEY_ESC) return e_zpet;
            if (inkey == KEY_UP)
            {
                unselect_all_buttons(buttons, buttons_cnt);
                if (--selected < 0)
                    selected = buttons_cnt-1;
                buttons[selected]->m_keyboard_selected = true;
            }
            if (inkey == KEY_DOWN || inkey == KEY_TAB)
            {
                unselect_all_buttons(buttons, buttons_cnt);
                selected = (selected+1)%buttons_cnt;
                buttons[selected]->m_keyboard_selected = true;
            }
            if (inkey == KEY_LEFT)  SelPrevCar();
            if (inkey == KEY_RIGHT) SelNextCar();
        }

        
		if (m_zpet.Update(inkey)) return e_zpet;
		if (m_OK.Update(inkey)) return e_OK;

		if (m_L1.Update()) 
			SelPrevCar();

		if (m_R1.Update()) 
			SelNextCar();

		if (!m_Player.cars[m_car].owned)	{
			if (m_koupit.Update(inkey)) {
				if (m_Player.money >= Cparams[m_car].cena) {
						m_Player.money -= Cparams[m_car].cena; // - Cparams[m_Player.car].cena/2;
						m_Player.car = m_car;
						m_Player.cars[m_car].owned = true;
						ZeroMemory(&m_Player.upgrades[0], MAX_UPGRADES*sizeof(int));
						ZeroMemory(&m_Player.cars[m_car].upgrades[0], MAX_UPGRADES*sizeof(int));
					}
			}
		}

        

	}
}


void MKoupeAuta::LoadCar()
{
	char s[100];

	if(m_bGar != NULL) {
		destroy_bitmap(m_bGar);
		m_bGar = NULL;
	}

	sprintf(s, "%s_B", Cparams[m_car].file);
	ASSERT(m_bGar == NULL);
	m_bGar = fload_bitmap(s);

}

void MKoupeAuta::DrawTuning(int x, int y, double max, double me, double memax)
{
	draw_sprite(hscreen.m_back, m_tuning[0], x, y);
	stretch_sprite(hscreen.m_back, m_tuning[1], 5+x, 1+y, memax*120/max, 8);
	draw_sprite(hscreen.m_back, m_tuning[2], 4+x+memax*120/max, y+1);
	stretch_sprite(hscreen.m_back, m_tuning[3], 5+x, 1+y, me*120/max, 8);
	draw_sprite(hscreen.m_back, m_tuning[4], 4+x+me*120/max, y+1);
}


void MKoupeAuta::Init()
{
	m_maxcat = 3;
	bool podm = true;
	int tested;
	for (int i = 3; i > -1; i--) {
		podm = true;
		tested = 0;
		for (int j = 0; j < g_maxcup; j++) {
			if (g_cup[j].kategorie == i && m_Player.best[j].vitezstvi == 0)
				podm = false;
			if (g_cup[j].kategorie == i)
				tested++;
		}

		if (podm == true && tested != 0)
			m_maxcat = i - 1;
	}
//	m_maxcat = -1;
}

void MKoupeAuta::SelNextCar()
{
	do {
		if (m_maxcat == -1)
			if (++m_car > MAX_CARS-1) m_car = 0;
			else;
		else
			if (++m_car > MAX_CARS-3) m_car = 0;
			else;
	} while (m_maxcat > Cparams[m_car].kategorie);
	LoadCar();
}

void MKoupeAuta::SelPrevCar()
{
	do {
		if (--m_car <  0)
		{
			if (m_maxcat == -1) m_car = MAX_CARS-1;
			else m_car = MAX_CARS-3;
		}
	} while (m_maxcat > Cparams[m_car].kategorie);
	LoadCar();
}
