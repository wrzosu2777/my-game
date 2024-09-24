// MDouble.cpp: implementation of the MDouble class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MDouble.h"
#include "MTrain.h"
#include "lang.h"
#include "MKariera.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MDouble::MDouble()
{
	m_tabulka = NULL;
}

MDouble::~MDouble()
{
	Destroy();
}

void MDouble::LoadGFX()
{
	Destroy();

	ASSERT(m_tabulka == NULL);
	m_tabulka = fload_bitmap("HM_1");
	
}

void MDouble::Run()
{
	int ret;

	for(;;){

		// vyber z menu
		if((ret = Select()) == e_zpet) break;

		switch(ret){
		case e_train :
			m_train.m_vyrazovacka = false;
			m_train.Run(2);
			break;
		case e_duel :
			if (a_player[0].gpID != -1 && a_player[1].gpID != -1) {
				MKariera::UpdateAPlayers();
				m_train.m_vyrazovacka = false;
				m_train.RunDuel();
			}
			break;
		case e_pohar :
			m_pohar.Run(2);
			break;
		case e_vyrazovacka :
			m_train.m_vyrazovacka = true;
			m_train.Run(2);
			break;
		}
	}
}

void MDouble::Destroy()
{
	if(m_tabulka != NULL) {
		destroy_bitmap(m_tabulka);
		m_tabulka = NULL;
	}
}

int MDouble::Select()
{
	MKariera::UpdateAPlayers();

	LoadGFX();

	if (a_player[0].gpID != -1 && a_player[1].gpID != -1)
		duel.Init(320, 146, LNG_DUEL, g_font18); 

	trenink.Init(320, 193, LNG_TRENINK, g_font18);
	vyrazovacka.Init(320, 243, LNG_VYRAZOVACKA, g_font18);
	pohar.Init(320, 290, LNG_POHAR, g_font18);
	zpet.Init(320, 338, LNG_ZPETV, g_font18);

//	hscreen.m_mouse = true;

	int ret = Loop();

	Destroy();
	pohar.Destroy();
	duel.Destroy();
	trenink.Destroy();
	vyrazovacka.Destroy();
	zpet.Destroy();
	return ret;
}


int MDouble::Loop()
{
    int selected = -1;  // This is a new hack for using keyboard to select things...
    int buttons_cnt  = 5;
    int old_mouse_x = mouse_x;
    int old_mouse_y = mouse_y;
    TButton *buttons[] = {&duel, &trenink, &vyrazovacka, &pohar, &zpet};
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
		draw_sprite(hscreen.m_back, m_tabulka, 225, 113);

		TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), LNG_DOUBLEPLAY, g_font24);
		snprintf(s,100, "%s vs. %s", a_player[0].name, a_player[1].name);
		TTDraw(hscreen.m_back, 320, 45, WIN_RGB(255, 255, 255), s, g_font18);
	
		pohar.Draw();
		if (a_player[0].gpID != -1 && a_player[1].gpID != -1)
			duel.Draw();
		trenink.Draw();
		vyrazovacka.Draw();
		zpet.Draw();
		release_bitmap(hscreen.m_back);

		if (duel.IsMouseIn()) {
			TTDraw(hscreen.m_back, 320, 159, WIN_RGB(255, 255, 255), LNG_DUEL_L1, g_font12);
			TTDraw(hscreen.m_back, 320, 171, WIN_RGB(255, 255, 255), LNG_DUEL_L2, g_font12);
		}
		if (trenink.IsMouseIn()) {
			TTDraw(hscreen.m_back, 320, 205, WIN_RGB(255, 255, 255), LNG_TRENINK_L1, g_font12);
			TTDraw(hscreen.m_back, 320, 217, WIN_RGB(255, 255, 255), LNG_TRENINK_L2, g_font12);
		}
		if (vyrazovacka.IsMouseIn()) {
			TTDraw(hscreen.m_back, 320, 255, WIN_RGB(255, 255, 255), LNG_VYRAZOVACKA_L1, g_font12);
			TTDraw(hscreen.m_back, 320, 267, WIN_RGB(255, 255, 255), LNG_VYRAZOVACKA_L2, g_font12);
		}
		if (pohar.IsMouseIn()) {
			TTDraw(hscreen.m_back, 320, 302, WIN_RGB(255, 255, 255), LNG_POHAR_L1, g_font12);
			TTDraw(hscreen.m_back, 320, 314, WIN_RGB(255, 255, 255), LNG_POHAR_L2, g_font12);
		}

	
	   hscreen.Flip();

//	  Sleep(10);
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
                if (!  (a_player[0].gpID != -1 && a_player[1].gpID != -1) && selected == 0 )
                    selected++;
                buttons[selected]->m_keyboard_selected = true;
            }
            if (inkey == KEY_DOWN || inkey == KEY_TAB)
            {
                unselect_all_buttons(buttons, buttons_cnt);
                selected = (selected+1)%buttons_cnt;
                if (!  (a_player[0].gpID != -1 && a_player[1].gpID != -1) && selected == 0 )
                    selected++;
                buttons[selected]->m_keyboard_selected = true;
            }
        }
        
        
		if (zpet.Update(inkey)) return e_zpet;
		if (pohar.Update(inkey))  return e_pohar;
		if (a_player[0].gpID != -1 && a_player[1].gpID != -1)
			if (duel.Update(inkey)) return e_duel;
		if (trenink.Update(inkey)) return e_train;
		if (vyrazovacka.Update(inkey)) return e_vyrazovacka;

	}
}
