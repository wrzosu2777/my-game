// MKariera.cpp: implementation of the MKariera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MKariera.h"
#include "MTrain.h"
#include "lang.h"
#include "player.h"

void SaveDrivers();

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MKariera::MKariera()
{
	m_tabulka = NULL;
}

MKariera::~MKariera()
{
	Destroy();
}

void MKariera::LoadGFX()
{
	Destroy();

	ASSERT(m_tabulka == NULL);
	m_tabulka = fload_bitmap("HM_1");
	
}

void MKariera::Run()
{
	int ret;

	InitPlayer();

	for(;;){

		// vyber z menu
		if((ret = Select()) == e_zpet) break;

		switch(ret){
		case e_zavod :
			m_zavody.Run();
			SavePlayer();
			break;
		case e_koupe :
			m_koupeauta.Run(&a_player[0]);
			break;
		case e_historie :
			m_historie.Run();
			break;
		case e_servis :
			if (a_player[0].car != -1)
				m_autoservis.Run(&a_player[0], false);
			break;
		}
	}

	SavePlayer();
}

void MKariera::Destroy()
{
	if(m_tabulka != NULL) {
		destroy_bitmap(m_tabulka);
		m_tabulka = NULL;
	}
}

int MKariera::Select()
{
	LoadGFX();

	if (a_player[0].car != -1) {
		zavod.Init(320, 146, LNG_VYBERZAVODU, g_font18); 
		servis.Init(320, 239, LNG_SERVIS, g_font18);
		historie.Init(320, 288, LNG_HISTORIE, g_font18);
	}

	koupe.Init(320, 193, LNG_KOUPEVOZU, g_font18);
	zpet.Init(320, 336, LNG_KONEC, g_font18);

	int ret = Loop();

	Destroy();
	zavod.Destroy();
	koupe.Destroy();
	zpet.Destroy();
	historie.Destroy();
	return ret;
}


int MKariera::Loop()
{
    int selected = -1;  // This is a new hack for using keyboard to select things...
    int buttons_cnt  = 5;
    int old_mouse_x = mouse_x;
    int old_mouse_y = mouse_y;
    TButton *buttons[] = {&zavod, &koupe, &servis, &historie, &zpet};
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

		TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), LNG_KARIERAV, g_font24);
		TTDraw(hscreen.m_back, 320, 45, WIN_RGB(255, 255, 255), a_player[0].name, g_font18);
	
		if (a_player[0].car != -1) {
			zavod.Draw();
			servis.Draw();
			historie.Draw();
		}

		koupe.Draw();
		zpet.Draw();

		if (zavod.IsMouseIn() && a_player[0].car != -1) {
			TTDraw(hscreen.m_back, 320, 159, WIN_RGB(255, 255, 255), LNG_VYBERZAVODU_L1, g_font12);
			TTDraw(hscreen.m_back, 320, 171, WIN_RGB(255, 255, 255), LNG_VYBERZAVODU_L2, g_font12);
		}
		if (koupe.IsMouseIn()) {
			TTDraw(hscreen.m_back, 320, 206, WIN_RGB(255, 255, 255), LNG_KOUPEVOZU_L1, g_font12);
			TTDraw(hscreen.m_back, 320, 218, WIN_RGB(255, 255, 255), LNG_KOUPEVOZU_L2, g_font12);
		}
		if (servis.IsMouseIn()&& a_player[0].car != -1) {
			TTDraw(hscreen.m_back, 320, 252, WIN_RGB(255, 255, 255), LNG_SERVIS_L1, g_font12);
			TTDraw(hscreen.m_back, 320, 264, WIN_RGB(255, 255, 255), LNG_SERVIS_L2, g_font12);
		}
		if (historie.IsMouseIn()&& a_player[0].car != -1) {
			TTDraw(hscreen.m_back, 320, 301, WIN_RGB(255, 255, 255), LNG_HISTORIE_L1, g_font12);
			TTDraw(hscreen.m_back, 320, 313, WIN_RGB(255, 255, 255), LNG_HISTORIE_L2, g_font12);
		}
		if (zpet.IsMouseIn()&& a_player[0].car != -1)
			TTDraw(hscreen.m_back, 320, 349, WIN_RGB(255, 255, 255), LNG_KONEC_L1, g_font12);
	
		release_bitmap(hscreen.m_back);
	   hscreen.Flip();

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
            
            //if (a_player[0].car == -1 && inkey == KEY_ENTER)
            {
                //return e_koupe;
            }
            
        }
        
        
        
        
/*		zavod.Init(320, 146, LNG_VYBERZAVODU, g_font18); 
		servis.Init(320, 239, LNG_SERVIS, g_font18);
		historie.Init(320, 288, LNG_HISTORIE, g_font18);
	}

	koupe.Init(320, 193, LNG_KOUPEVOZU, g_font18);
	zpet.Init(320, 336, LNG_KONEC, g_font18);*/

//	  Sleep(10);

		if (a_player[0].car != -1) {
			if (zavod.Update(inkey)) return e_zavod;
			if (servis.Update(inkey)) return e_servis;
			if (historie.Update(inkey)) return e_historie;
		}
		if (zpet.Update(inkey)) return e_zpet;
		if (koupe.Update(inkey)) return e_koupe;
	}
}

void MKariera::InitPlayer()
{
	UpdateAPlayers();

	if (a_player[0].gpID != -1)
		a_player[0] = g_player[a_player[0].gpID];
	else {
		a_player[0].gpID = g_players;
		a_player[0].car = -1;
		a_player[0].money = 600000; // penize dozacatku
		a_player[0].nacc = 1;
		a_player[0].nbreaks = 1;
		a_player[0].nspeed = 1;
		a_player[0].ntear = 1;
		a_player[0].kbodu = 0;
		for (int i = 0; i < CUP_MAX; i++) {
			a_player[0].best[i].vitezstvi = 0;
			a_player[0].best[i].best = -1;
			a_player[0].best[i].obtiznost = -1;
			a_player[0].best[i].poradi = 0;
			a_player[0].best[i].jeto = 0;
		}
	}
}

void MKariera::SavePlayer()
{
	 if (a_player[0].gpID == g_players && a_player[0].car != -1) g_players++;

	 if (a_player[0].car != -1)
		g_player[a_player[0].gpID] = a_player[0];

	 SaveDrivers();

}

void MKariera::UpdateAPlayers()
{
	a_player[0].gpID = -1;
//	a_player[1].gpID = -1;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < g_players; j++)
			if (strcmp(a_player[i].name, g_player[j].name) == 0) {
				a_player[i] = g_player[j];
				break;
			}
}
