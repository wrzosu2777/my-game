// MSigle.cpp: implementation of the MSigle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSigle.h"
#include "MTrain.h"
#include "MKariera.h"
#include "lang.h"
#include "player.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MSigle::MSigle()
{
	m_tabulka = NULL;
}

MSigle::~MSigle()
{
	Destroy();
}

void MSigle::LoadGFX()
{
	Destroy();

	ASSERT(m_tabulka == NULL);
	m_tabulka = fload_bitmap("HM_1");
	
}

void MSigle::Run()
{
	int ret;

	for(;;){

		// vyber z menu
		if((ret = Select()) == e_zpet) break;

		switch(ret){
		case e_train :
			m_train.m_vyrazovacka = false;
			m_train.Run(1);
			break;
		case e_pohar :
			m_pohar.Run(1);
			break;
		case e_kariera :
			m_kariera.Run();
			break;
		case e_vyrazovacka :
			m_train.m_vyrazovacka = true;
			m_train.Run(1);
			break;
		}
	}
}

void MSigle::Destroy()
{
	if(m_tabulka != NULL) {
		destroy_bitmap(m_tabulka);
		m_tabulka = NULL;
	}
}

int MSigle::Select()
{
	LoadGFX();

	kariera.Init(320, 146, LNG_KARIERA, g_font18); 
	trenink.Init(320, 193, LNG_TRENINK, g_font18);
	vyrazovacka.Init(320, 243, LNG_VYRAZOVACKA, g_font18);
	pohar.Init(320, 290, LNG_POHAR, g_font18);
	zpet.Init(320, 338, LNG_ZPETV, g_font18);

//	hscreen.m_mouse = true;

	int ret = Loop();

	Destroy();
	pohar.Destroy();
	kariera.Destroy();
	trenink.Destroy();
	vyrazovacka.Destroy();
	zpet.Destroy();
	return ret;
}


int MSigle::Loop()
{
    int selected = -1;  // This is a new hack for using keyboard to select things...
    int buttons_cnt  = 5;
    int old_mouse_x = mouse_x;
    int old_mouse_y = mouse_y;
    TButton *buttons[] = {&kariera, &trenink, &vyrazovacka, &pohar, &zpet};
    unselect_all_buttons(buttons, buttons_cnt);
    
    
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

		TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), LNG_SINGLEPLAY, g_font24);
		TTDraw(hscreen.m_back, 320, 45, WIN_RGB(255, 255, 255), a_player[0].name, g_font18);
	
		pohar.Draw();
		kariera.Draw();
		trenink.Draw();
		vyrazovacka.Draw();
		zpet.Draw();
		release_bitmap(hscreen.m_back);

		if (kariera.IsMouseIn()) {
			TTDraw(hscreen.m_back, 320, 159, WIN_RGB(255, 255, 255), LNG_KARIERA_L1, g_font12);
			TTDraw(hscreen.m_back, 320, 171, WIN_RGB(255, 255, 255), LNG_KARIERA_L2, g_font12);
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
                buttons[selected]->m_keyboard_selected = true;
            }
            if (inkey == KEY_DOWN || inkey == KEY_TAB)
            {
                unselect_all_buttons(buttons, buttons_cnt);
                selected = (selected+1)%buttons_cnt;
                buttons[selected]->m_keyboard_selected = true;
            }
        }
        
		if (zpet.Update(inkey)) return e_zpet;
		if (pohar.Update(inkey))  return e_pohar;
		if (kariera.Update(inkey)) return e_kariera;
		if (trenink.Update(inkey)) return e_train;
		if (vyrazovacka.Update(inkey)) return e_vyrazovacka;
	}

}


