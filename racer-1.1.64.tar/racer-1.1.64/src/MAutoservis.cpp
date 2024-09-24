// MAutoservis.cpp: implementation of the MAutoservis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MAutoservis.h"
#include "lang.h"
#include "cars.h"
#include "GRun.h"

#define TUNING_PERCENT 0.1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const int PosServis[MAX_UPGRADES][10] = {
	{66,  206, 190, 204, -1,  -1,  -1, -1, -1, -1,},    // motor
	{159, 80,  189, 206, -1,  -1,  -1, -1, -1, -1,},    // zataceni
	{154, 333, 204, 188, 169, 249, -1, -1, -1, -1,},    // pneu1
	{360, 333, 368, 182, 351, 259, -1, -1, -1, -1,},    // pneu2
	{360, 80,  219, 201, 185, 268, 382, 203, 366, 280}, // brzdy
	{259, 333, 189, 206, -1,  -1,  -1, -1, -1, -1,},    // podvozek
	{259, 80,  241, 230, -1,  -1,  -1, -1, -1, -1,},    // vybaveni
};

MAutoservis::MAutoservis()
{
	m_selected = 0;

	int i,j;

	m_tabulka = NULL;
	m_auto = NULL;
	m_button = NULL;
	m_upgr = NULL;
	m_upZel = NULL;
	m_upRed = NULL;
	for (i = 0; i < MAX_UPGRADES; i++)
		for (j = 0; j < MAX_UPGRADED; j++) 
				m_upgrades[i][j] = NULL;

	for (i = 0; i < MAX_UPGRADES; i++)
		for (j = 0; j < MAX_VISIBLE_UPGR; j++) 
				m_visible[i][j] = NULL;

	GRun::NullBitmaps(&m_tuning[0], MAX_TUNING_BITMAPS);
}

MAutoservis::~MAutoservis()
{
	Destroy();
}

void MAutoservis::LoadGFX()
{
	Destroy();

	ASSERT(m_tabulka == NULL);
	m_tabulka = fload_bitmap("servis0");
	ASSERT(m_auto == NULL);
	m_auto = fload_bitmap("servis1");
	ASSERT(m_button == NULL);
	m_button = fload_bitmap("servs");

	ASSERT(m_upgr == NULL);
	m_upgr = fload_bitmap("upgrade");
	ASSERT(m_upZel == NULL);
	m_upZel = fload_bitmap("upgrade_Zel");
	ASSERT(m_upRed == NULL);
	m_upRed = fload_bitmap("upgrade_Cer");
	
	GRun::LoadBitmaps(&m_tuning[0], MAX_TUNING_BITMAPS, 0, "Tuning%d");


	char s[100];
	int i,j;
	for (i = 0; i < MAX_UPGRADES; i++)
		for (j = 0; j < MAX_UPGRADED; j++) {
			sprintf(s, "serv%d_%d", i, j);
			ASSERT(m_upgrades[i][j] == NULL);
			m_upgrades[i][j] = fload_bitmap(s);
		   if (!m_upgrades[i][j]) {
			   set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		      allegro_message("%s", s);
				exit(1);
			}
		}

	for (i = 0; i < MAX_UPGRADES; i++)
		for (j = 0; j < MAX_VISIBLE_UPGR; j++) {
			sprintf(s, "servZ%d_%d", i, j);
			ASSERT(m_visible[i][j] == NULL);
			m_visible[i][j] = fload_bitmap(s);
		}
}

void MAutoservis::Run(st_player *player, bool showname)
{
	m_showname = showname;
	m_pPlayer = player;
	m_Player = *player;

	int ret;

	// vyber z menu
	if((ret = Select()) == e_OK) 
		*m_pPlayer = m_Player;
	
}

void MAutoservis::Destroy()
{
	int i,j;

	if(m_tabulka != NULL) {
		destroy_bitmap(m_tabulka);
		m_tabulka = NULL;
	}
	if(m_auto != NULL) {
		destroy_bitmap(m_auto);
		m_auto = NULL;
	}
	if(m_button != NULL) {
		destroy_bitmap(m_button);
		m_button = NULL;
	}
	if(m_upgr != NULL) {
		destroy_bitmap(m_upgr);
		m_upgr = NULL;
	}
	if(m_upZel != NULL) {
		destroy_bitmap(m_upZel);
		m_upZel = NULL;
	}
	if(m_upRed != NULL) {
		destroy_bitmap(m_upRed);
		m_upRed = NULL;
	}

	for (i = 0; i < MAX_UPGRADES; i++)
		for (j = 0; j < MAX_UPGRADED; j++) 
			if (m_upgrades[i][j] != NULL) {
				destroy_bitmap(m_upgrades[i][j]);
				m_upgrades[i][j] = NULL;
			}

	for (i = 0; i < MAX_UPGRADES; i++)
		for (j = 0; j < MAX_VISIBLE_UPGR; j++) 
			if (m_visible[i][j] != NULL) {
				destroy_bitmap(m_visible[i][j]);
				m_visible[i][j] = NULL;
			}

	GRun::DestroyBitmaps(&m_tuning[0], MAX_TUNING_BITMAPS);

}

int MAutoservis::Select()
{
	LoadGFX();

	zpet.Init(30, 463, LNG_ZPET, g_font18);
	OK.Init(610, 463, LNG_OK, g_font18);
	koupit.Init(560, 180, LNG_KOUPIT, g_font12B);

	breaksL.Init(490, 218, "-", g_font12, KEY_LEFT);
	breaksR.Init(631, 218, "+", g_font12, KEY_RIGHT);
	accL.Init(490, 243, "-", g_font12, KEY_LEFT);
	accR.Init(631, 243, "+", g_font12, KEY_RIGHT);
	speedL.Init(490, 268, "-", g_font12, KEY_LEFT);
	speedR.Init(631, 268, "+", g_font12, KEY_RIGHT);
	turningL.Init(490, 293, "-", g_font12, KEY_LEFT);
	turningR.Init(631, 293, "+", g_font12, KEY_RIGHT);
	carL.Init(220, 45, "<", g_font12, KEY_LEFT);
	carR.Init(420, 45, ">", g_font12, KEY_RIGHT);

	for (int i = 0; i < MAX_UPGRADES; i++)
		butt[i].Init(PosServis[i][0], PosServis[i][1], 70, 69, -1, -1, -1, NULL);

//	hscreen.m_mouse = true;

	int ret = Loop();

	Destroy();
	zpet.Destroy();
	OK.Destroy();
	koupit.Destroy();
	for (int i = 0; i < MAX_UPGRADES; i++)
		butt[i].Destroy();

	return ret;
}

int MAutoservis::Loop()
{
    int selected = -1;  // This is a new hack for using keyboard to select things...
    int selections = 8;
    int buttons_cnt  = 13;
    int old_mouse_x = mouse_x;
    int old_mouse_y = mouse_y;
    TButton *buttons[] = {&breaksL, &breaksR, &accL, &accR, &speedL, &speedR, &turningL, &turningR, &carL, &carR, &zpet, &OK, &koupit};
    unselect_all_buttons(buttons, buttons_cnt);
    int blue;
    
    
	int i = 0;
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
        
        
		int cena = Cparams[m_Player.car].upgrade[m_selected].cena*pow(1.5, m_Player.upgrades[m_selected]);

		acquire_bitmap(hscreen.m_back);

		draw_sprite(hscreen.m_back, g_menuGround, 0, 0);
		draw_sprite(hscreen.m_back, m_tabulka, 480, 0);
		draw_sprite(hscreen.m_back, m_auto, 147, 162);

		TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), LNG_AUTOSERVIS, g_font24);
		if (m_showname)
			TTDraw(hscreen.m_back, 320, 45, WIN_RGB(255, 255, 255), m_Player.name, g_font18);
		else
        {
            if (selected == 4) blue=0;
            else blue = 255;
			TTDraw(hscreen.m_back, 320, 45, WIN_RGB(255, 255, blue), Cparams[m_Player.car].name, g_font18);
        }
		TTDraw(hscreen.m_back, 560, 100, WIN_RGB(255, 255, 255), LNG_UPGRADE, g_font12B);

		TTDraw(hscreen.m_back, 560, 17, WIN_RGB(255, 255, 255), utext[4*m_selected], g_font18);
		TTDraw(hscreen.m_back, 494, 33, WIN_RGB(255, 255, 255), utext[4*m_selected+1], g_font12, 1);
		TTDraw(hscreen.m_back, 494, 45, WIN_RGB(255, 255, 255), utext[4*m_selected+2], g_font12, 1);
		TTDraw(hscreen.m_back, 494, 57, WIN_RGB(255, 255, 255), utext[4*m_selected+3], g_font12, 1);
		

		if (m_Player.upgrades[m_selected] < Cparams[m_Player.car].upgrade[m_selected].max - 1) {
			sprintf(s, LNG_CENA, cena);
			if (m_Player.money >= cena)
				TTDraw(hscreen.m_back, 560, 158, WIN_RGB(255, 255, 255), s, g_font12);
			else
				TTDraw(hscreen.m_back, 560, 158, WIN_RGB(255, 0, 0), s, g_font12);
		}

		sprintf(s, LNG_PENIZE, m_Player.money);
		TTDraw(hscreen.m_back, 560, 315, WIN_RGB(255, 255, 255), s, g_font12);
/*
		sprintf(s, "%.1f / %.1f s", GRun::GetAccToSto(GRun::GetUpgradedAcc(m_car, &m_Player.upgrades[0], 0)), GRun::GetAccToSto(GRun::GetMaxUpgradedAcc(m_car)));
		TTDraw(hscreen.m_back, 430 , 315, WIN_RGB(255, 255, 255), s, g_font12, 1);
		sprintf(s, "%.0f / %.0f km/h", GRun::GetSpeedKmH(GRun::GetUpgradedSpeed(m_car, &m_Player.upgrades[0], 0)), GRun::GetSpeedKmH(GRun::GetMaxUpgradedSpeed(m_car)));
		TTDraw(hscreen.m_back, 430 , 329, WIN_RGB(255, 255, 255), s, g_font12, 1);
		sprintf(s, "%.1f / %.1f", GRun::GetUpgradedTurning(m_car, &m_Player.upgrades[0], 0), GRun::GetMaxUpgradedTurning(m_car));
		TTDraw(hscreen.m_back, 430 , 343, WIN_RGB(255, 255, 255), s, g_font12, 1);

  */
		sprintf(s, LNG_UPGR_BRZDY, GRun::GetBreaksClass(GRun::GetUpgradedBreaks(m_Player.car, &m_Player.upgrades[0], m_Player.nbreaks)));
        if (selected == 0) blue=0;
        else blue = 255;
		TTDraw(hscreen.m_back, 494, 200, WIN_RGB(255, 255, blue), s, g_font12, 1);
		DrawTuning(494, 214, 1-GRun::m_u_breaks, 1-GRun::GetUpgradedBreaks(m_Player.car, &m_Player.upgrades[0], m_Player.nbreaks), 1-GRun::GetMaxUpgradedBreaks(m_Player.car));

		sprintf(s, LNG_UPGR_ACC, GRun::GetAccToSto(GRun::GetUpgradedAcc(m_Player.car, &m_Player.upgrades[0], m_Player.nacc), GRun::GetUpgradedSpeed(m_Player.car, &m_Player.upgrades[0], m_Player.nspeed)));
        if (selected == 1) blue=1;
        else blue = 255;
        TTDraw(hscreen.m_back, 494, 225, WIN_RGB(255, 255, blue), s, g_font12, 1);
		DrawTuning(494, 239, GRun::m_u_acc, GRun::GetUpgradedAcc(m_Player.car, &m_Player.upgrades[0], m_Player.nacc), GRun::GetMaxUpgradedAcc(m_Player.car));

		sprintf(s, LNG_UPGR_SPEED, GRun::GetSpeedKmH(GRun::GetUpgradedSpeed(m_Player.car, &m_Player.upgrades[0], m_Player.nspeed)));
        if (selected == 2) blue=1;
        else blue = 255;
        TTDraw(hscreen.m_back, 494, 250, WIN_RGB(255, 255, blue), s, g_font12, 1);
		DrawTuning(494, 264, GRun::m_u_speed, GRun::GetUpgradedSpeed(m_Player.car, &m_Player.upgrades[0], m_Player.nspeed), GRun::GetMaxUpgradedSpeed(m_Player.car));

		sprintf(s, LNG_UPGR_TURN, GRun::GetUpgradedTurning(m_Player.car, &m_Player.upgrades[0], m_Player.ntear));
        if (selected == 3) blue=1;
        else blue = 255;
        TTDraw(hscreen.m_back, 494, 275, WIN_RGB(255, 255, blue), s, g_font12, 1);
		DrawTuning(494, 289, GRun::m_u_turning, GRun::GetUpgradedTurning(m_Player.car, &m_Player.upgrades[0], m_Player.ntear), GRun::GetMaxUpgradedTurning(m_Player.car));

		
		OK.Draw();
		zpet.Draw();

		if ((m_Player.upgrades[m_selected] < Cparams[m_Player.car].upgrade[m_selected].max - 1) 
			 && (cena <= m_Player.money))
				koupit.Draw();

		accL.Draw();
		accR.Draw();
		speedL.Draw();
		speedR.Draw();
		breaksL.Draw();
		breaksR.Draw();
		turningL.Draw();
		turningR.Draw();
		carL.Draw();
		carR.Draw();

		for (i = 0; i < MAX_UPGRADES; i++) {
			draw_sprite(hscreen.m_back, m_upgrades[i][m_Player.upgrades[i]], PosServis[i][0], PosServis[i][1]);
		}

		draw_sprite(hscreen.m_back, m_button, PosServis[m_selected][0], PosServis[m_selected][1]);
		for (i = 0; i < MAX_VISIBLE_UPGR; i++)
			if (m_visible[m_selected][i] != NULL)
				draw_sprite(hscreen.m_back, m_visible[m_selected][i], PosServis[m_selected][i*2+2], PosServis[m_selected][i*2+3]);

		// listicka upgrade
		draw_sprite(hscreen.m_back, m_upgr, 507, 112);
		for (i = 0; i < Cparams[m_Player.car].upgrade[m_selected].max; i++)
			if (m_Player.upgrades[m_selected] >= i)
				draw_sprite(hscreen.m_back, m_upZel, 508+21*i, 113);
			else
				draw_sprite(hscreen.m_back, m_upRed, 508+21*i, 113);

		release_bitmap(hscreen.m_back);
	   hscreen.Flip();

/*
        if (keypressed()) {
            int key = readkey() >> 8;
            if (key == KEY_ESC)   return e_zpet;
            if (key == KEY_ENTER) return e_OK;
            if (key == KEY_LEFT)  nextCar(-1);
            if (key == KEY_RIGHT) nextCar(+1);
        }
*/
        int inkey = 0;
        if (keypressed())
        {
            inkey = readkey() >> 8;
            if (inkey == KEY_ESC) return e_zpet;
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
            if (inkey == KEY_DOWN)
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
            if (inkey == KEY_TAB)
            {
                m_selected = (m_selected+1)%MAX_UPGRADES;
            }
        }
        
        
//	  Sleep(10);
		for (i = 0; i < MAX_UPGRADES; i++)
			if (butt[i].Update()) m_selected = i;


		if (koupit.Update(inkey))
			if (m_Player.upgrades[m_selected] < Cparams[m_Player.car].upgrade[m_selected].max - 1) 
				if (cena <= m_Player.money) {
					m_Player.money -= cena;
					m_Player.upgrades[m_selected]++;
				}

		if (zpet.Update(inkey)) return e_zpet;
		if (OK.Update(inkey)) return e_OK;
		if (accL.Update(inkey))
			if ((m_Player.nacc -= TUNING_PERCENT) < 0) m_Player.nacc = 0;
		if (accR.Update(inkey))
			if ((m_Player.nacc += TUNING_PERCENT) > 1) m_Player.nacc = 1;
		if (speedL.Update(inkey))
			if ((m_Player.nspeed -= TUNING_PERCENT) < 0) m_Player.nspeed = 0;
		if (speedR.Update(inkey))
			if ((m_Player.nspeed += TUNING_PERCENT) > 1) m_Player.nspeed = 1;
		if (breaksL.Update(inkey))
			if ((m_Player.nbreaks -= TUNING_PERCENT) < 0) m_Player.nbreaks = 0;
		if (breaksR.Update(inkey))
			if ((m_Player.nbreaks += TUNING_PERCENT) > 1) m_Player.nbreaks = 1;
		if (turningL.Update(inkey))
			if ((m_Player.ntear -= TUNING_PERCENT) < 0) m_Player.ntear = 0;
		if (turningR.Update(inkey))
			if ((m_Player.ntear += TUNING_PERCENT) > 1) m_Player.ntear = 1;
		if (carL.Update(inkey))
			nextCar(-1);
		if (carR.Update(inkey))
			nextCar(+1);


		saveCar();

	}
}

void MAutoservis::DrawTuning(int x, int y, double max, double me, double memax)
{
	draw_sprite(hscreen.m_back, m_tuning[0], x, y);
	stretch_sprite(hscreen.m_back, m_tuning[1], 5+x, 1+y, memax*120/max, 8);
	draw_sprite(hscreen.m_back, m_tuning[2], 4+x+memax*120/max, y+1);
	stretch_sprite(hscreen.m_back, m_tuning[3], 5+x, 1+y, me*120/max, 8);
	draw_sprite(hscreen.m_back, m_tuning[4], 4+x+me*120/max, y+1);
}
void MAutoservis::saveCar()
{
	memcpy(m_Player.cars[m_Player.car].upgrades, m_Player.upgrades, sizeof(int)*MAX_UPGRADES);
}

void MAutoservis::nextCar(int delta)
{
	saveCar();

	if (delta == 0) delta = 1;

	int car = m_Player.car;
	m_Player.cars[car].owned = true; // just to be sure
	do 
	{
		car += delta;
		if (car < 0) car = MAX_CARS_TOTAL-1;
		if (car > MAX_CARS_TOTAL-1) car = 0;
	}
	while (m_Player.cars[car].owned == false);

	memcpy(m_Player.upgrades, m_Player.cars[car].upgrades, sizeof(int)*MAX_UPGRADES);
	m_Player.car = car;
}

