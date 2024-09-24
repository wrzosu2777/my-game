// MNastaveni.cpp: implementation of the MNastaveni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MNastaveni.h"
#include "lang.h"
#include "player.h"
#include "MKariera.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define MIN_VOLUME (-2000)
#define AMIN_VOLUME (2000)
#define VOLUME_DS 100

MNastaveni::MNastaveni()
{
	for (int i = 0; i < 3; i++) {
		m_volume[i] = NULL;
		m_tabulka[i] = NULL;
	}
}

MNastaveni::~MNastaveni()
{
	Destroy();
}

void MNastaveni::LoadGFX()
{
	Destroy();

	ASSERT(m_tabulka[0] == NULL);
	m_tabulka[0] = fload_bitmap("TR");
	m_tabulka[1] = fload_bitmap("hrac_1");
	m_tabulka[2] = fload_bitmap("hrac_2");
	ASSERT(m_volume[0] == NULL);
	m_volume[0]  = fload_bitmap("volume");
	ASSERT(m_volume[1] == NULL);
	m_volume[1]  = fload_bitmap("volume1");
	ASSERT(m_volume[2] == NULL);
	m_volume[2]  = fload_bitmap("volume2");
	
}

void MNastaveni::Run()
{
	m_pl = -1;
	if (Select() == e_OK) {
	}
}

void MNastaveni::Destroy()
{
	int i;
	for (i = 0; i < 3; i++)
		if(m_tabulka[i] != NULL) {
			destroy_bitmap(m_tabulka[i]);
			m_tabulka[i] = NULL;
		}

	for (i = 0; i < 3; i++)
		if(m_volume[i] != NULL) {
			destroy_bitmap(m_volume[0]);
			m_volume[0] = NULL;
		}
}

int MNastaveni::Select()
{
	LoadGFX();
	MKariera::UpdateAPlayers();


	hrac1.Init(320,  89, LNG_NAS_1HRAC, g_font12);
	hrac1l.Init(240, 107, "<", g_font12,KEY_LEFT);
	hrac1r.Init(396, 107, ">", g_font12,KEY_RIGHT);
	hrac2.Init(320, 129, LNG_NAS_2HRAC, g_font12);
	hrac2l.Init(240, 147, "<", g_font12,KEY_LEFT);
	hrac2r.Init(396, 147, ">", g_font12,KEY_RIGHT);
	hudbal.Init(196, 184, "<", g_font12,KEY_LEFT);
	hudbar.Init(441, 184, ">", g_font12,KEY_RIGHT);
	zvukl.Init(196, 216, "<", g_font12,KEY_LEFT);
	zvukr.Init(441, 216, ">", g_font12,KEY_RIGHT);
	rozlisenil.Init(257, 249, "<", g_font12,KEY_LEFT);
	rozlisenir.Init(380, 249, ">", g_font12,KEY_RIGHT);
	stinl.Init(257, 282, "<", g_font12,KEY_LEFT);
	stinr.Init(380, 282, ">", g_font12,KEY_RIGHT);
	svetlol.Init(257, 316, "<", g_font12,KEY_LEFT);
	svetlor.Init(380, 316, ">", g_font12,KEY_RIGHT);
	obtiznostl.Init(257, 350, "<", g_font12,KEY_LEFT);
	obtiznostr.Init(380, 350, ">", g_font12,KEY_RIGHT);
	rychlostl.Init(257, 385, "<", g_font12,KEY_LEFT);
	rychlostr.Init(380, 385, ">", g_font12,KEY_RIGHT);

	zpet.Init(30, 463, LNG_ZPET, g_font18);
	OK.Init(610, 463, LNG_OK, g_font18);

//	hscreen.m_mouse = true;

	int ret = Loop();

	hrac1.Destroy();
	hrac1l.Destroy();
	hrac1r.Destroy();
	hrac2.Destroy();
	hrac2l.Destroy();
	hrac2r.Destroy();
	hudbal.Destroy();
	hudbar.Destroy();
	zvukl.Destroy();
	zvukr.Destroy();
	stinl.Destroy();
	stinr.Destroy();
	svetlol.Destroy();
	svetlor.Destroy();
	rozlisenil.Destroy();
	rozlisenir.Destroy();
	obtiznostl.Destroy();
	obtiznostr.Destroy();
	rychlostl.Destroy();
	rychlostr.Destroy();

	zpet.Destroy();
	OK.Destroy();
	
	return ret;
}



bool ReadFromSC(char* s, int max, int sc) {
    char c = sc & 0xff;
    sc >>= 8;
    if (sc == KEY_BACKSPACE) {
        if (strlen(s) > 0) {
            s[strlen(s)-1] = '\0';
        }
        else return false;
    }
    else
        if ((int)strlen(s) < max && isprint(c)) {
            int len = strlen(s);
            s[len] = c;
            s[len+1] = '\0';
        }
        else return false;
    return true;
}


int MNastaveni::Loop()
{
    int selected = -1;  // This is a new hack for using keyboard to select things...
    int selections= 11;
    int buttons_cnt  = 20;
    int old_mouse_x = mouse_x;
    int old_mouse_y = mouse_y;
    TButton *buttons[] = {&hrac1l, &hrac1r, &hrac2l, &hrac2r, &hudbal, &hudbar, &zvukl, &zvukr,  &rozlisenil, &rozlisenir, &stinl, &stinr, &svetlol, &svetlor, &obtiznostl, &obtiznostr, &rychlostl, &rychlostr, &OK, &zpet};
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
		draw_sprite(hscreen.m_back, m_tabulka[0], 133, 76);
		draw_sprite(hscreen.m_back, m_tabulka[1], 250, 98);
		draw_sprite(hscreen.m_back, m_tabulka[2], 250, 138);

		TTDraw(hscreen.m_back, 320, 22, WIN_RGB(255, 255, 255), LNG_NASTAVENI_H, g_font24);

		TTDraw(hscreen.m_back, 320, 172, WIN_RGB(255, 255, 255), LNG_HUDBA, g_font12);
		TTDraw(hscreen.m_back, 320, 202, WIN_RGB(255, 255, 255), LNG_ZVUKY, g_font12);

		TTDraw(hscreen.m_back, 320, 232, WIN_RGB(255, 255, 255), LNG_SCREEN, g_font12);
		TTDraw(hscreen.m_back, 320, 266, WIN_RGB(255, 255, 255), LNG_STINY, g_font12);
		TTDraw(hscreen.m_back, 320, 301, WIN_RGB(255, 255, 255), LNG_SVETLA, g_font12);
		TTDraw(hscreen.m_back, 320, 335, WIN_RGB(255, 255, 255), LNG_OBTIZNOST, g_font12);
		TTDraw(hscreen.m_back, 320, 335, WIN_RGB(255, 255, 255), LNG_OBTIZNOST, g_font12);

		TTDraw(hscreen.m_back, 320, 370, WIN_RGB(255, 255, 255), LNG_GAME_SPEED, g_font12);


		if (m_pl == 0) {
			sprintf(s, "%s_", a_player[0].name);
			TTDraw(hscreen.m_back, 320, 104, WIN_RGB(255, 255, 255), s, g_font12B);
		}
		else
			TTDraw(hscreen.m_back, 320, 104, WIN_RGB(255, 255, 255), a_player[0].name, g_font12B);

		if (m_pl == 1) {
			sprintf(s, "%s_", a_player[1].name);
			TTDraw(hscreen.m_back, 320, 144, WIN_RGB(255, 255, 255), s, g_font12B);
		}
		else
			TTDraw(hscreen.m_back, 320, 144, WIN_RGB(255, 255, 255), a_player[1].name, g_font12B);

		draw_sprite(hscreen.m_back, m_volume[1], 204 - m_volume[1]->w, 180);
		draw_sprite(hscreen.m_back, m_volume[1], 204 - m_volume[1]->w, 212);
		stretch_sprite(hscreen.m_back, m_volume[0], 204, 180, (hmp3.m_volume+AMIN_VOLUME)*230/AMIN_VOLUME, 8);
		stretch_sprite(hscreen.m_back, m_volume[0], 204, 212, (dsound.m_volume+AMIN_VOLUME)*230/AMIN_VOLUME, 8);
		draw_sprite(hscreen.m_back, m_volume[2], 204 + (hmp3.m_volume+AMIN_VOLUME)*230/AMIN_VOLUME, 180);
		draw_sprite(hscreen.m_back, m_volume[2], 204 + (dsound.m_volume+AMIN_VOLUME)*230/AMIN_VOLUME, 212);


		hrac1.Draw();
		hrac1l.Draw();
		hrac1r.Draw();
		hrac2.Draw();
		hrac2l.Draw();
		hrac2r.Draw();
		hudbal.Draw();
		hudbar.Draw();
		zvukl.Draw();
		zvukr.Draw();
		stinl.Draw();
		stinr.Draw();
		svetlol.Draw();
		svetlor.Draw();
		rozlisenil.Draw();
		rozlisenir.Draw();
		obtiznostl.Draw();
		obtiznostr.Draw();
		rychlostl.Draw();
		rychlostr.Draw();
		zpet.Draw();
		OK.Draw();

		if (DF_GAMEX == 640)
			TTDraw(hscreen.m_back, 320, 249, WIN_RGB(255, 255, 255), LNG_SCREEN640, g_font12B);
		else
			TTDraw(hscreen.m_back, 320, 249, WIN_RGB(255, 255, 255), LNG_SCREEN800, g_font12B);

		if (DF_SVETLA == S_STATIC)
			TTDraw(hscreen.m_back, 320, 317, WIN_RGB(255, 255, 255), LNG_SVETLAS, g_font12B);
		else
			TTDraw(hscreen.m_back, 320, 317, WIN_RGB(255, 255, 255), LNG_SVETLAD, g_font12B);

		if (DF_STINY == S_STATIC)
			TTDraw(hscreen.m_back, 320, 277, WIN_RGB(255, 255, 255), LNG_STINIS, g_font12B);
		else
			TTDraw(hscreen.m_back, 320, 277, WIN_RGB(255, 255, 255), LNG_STINID, g_font12B);

		TTDraw(hscreen.m_back, 320, 350, WIN_RGB(255, 255, 255), LNG_DOBTIZNOST[DF_OBTIZNOST], g_font12B);

		if (DF_GAME_SPEED >= 1.01) {
			sprintf(s, "+%.0f%%", (DF_GAME_SPEED-1)*100);
			TTDraw(hscreen.m_back, 320, 385, WIN_RGB(255, 255, 255), s, g_font12B);
		}
		else 
			if (DF_GAME_SPEED <= 0.99) {
				sprintf(s, "-%.0f%%", (1-DF_GAME_SPEED)*100);
				TTDraw(hscreen.m_back, 320, 385, WIN_RGB(255, 255, 255), s, g_font12B);
			}
			else
				TTDraw(hscreen.m_back, 320, 385, WIN_RGB(255, 255, 255), LNG_NORMALNI, g_font12B);

		release_bitmap(hscreen.m_back);

	   hscreen.Flip();

        
//      Sleep(10);
        int inkey = 0;
        int insc = 0;
        if (keypressed())
        {
            insc = readkey();

            inkey = insc >> 8;
            if (inkey == KEY_ESC) return e_zpet;
            if (inkey == KEY_ENTER) return e_OK;
            if (inkey == KEY_UP)
            {
                unselect_all_buttons(buttons, buttons_cnt);
                if (--selected < 0)
                    selected = selections-1;
                if (selected <= 8)
                {
                    buttons[selected*2]->m_keyboard_selected = true;
                    buttons[selected*2+1]->m_keyboard_selected = true;
                }
                else
                {
                    buttons[selected+9]->m_keyboard_selected = true;
                }
            }
            if (inkey == KEY_DOWN || inkey == KEY_TAB)
            {
                unselect_all_buttons(buttons, buttons_cnt);
                selected = (selected+1)%selections;
                if (selected <= 8)
                {
                    buttons[selected*2]->m_keyboard_selected = true;
                    buttons[selected*2+1]->m_keyboard_selected = true;
                }
                else
                {
                    buttons[selected+9]->m_keyboard_selected = true;
                }
            }
        }

        
        
		if (zpet.Update(inkey)) return e_zpet;
		if (OK.Update(inkey)) return e_OK;

		if (rozlisenil.Update(inkey) || rozlisenir.Update(inkey)) {
			if (DF_GAMEX == 640) {DF_GAMEX = 800; DF_GAMEY = 600; }
			else {DF_GAMEX = 640; DF_GAMEY = 480; }
			m_pl = -1;
		}
 
		if (svetlol.Update(inkey) || svetlor.Update(inkey)) {
			if (DF_SVETLA == S_STATIC) DF_SVETLA = S_DINAMIC;
			else DF_SVETLA = S_STATIC;
			m_pl = -1;
		}

		if (stinl.Update(inkey) || stinr.Update(inkey)) {
			if (DF_STINY == S_STATIC) DF_STINY = S_DINAMIC;
			else DF_STINY = S_STATIC;
			m_pl = -1;
		}
/*
		if (hudbal.Update()) 
			if (hmp3.m_volume > MIN_VOLUME) {
				hmp3.m_volume -= VOLUME_DS;
				if (hmp3.m_pBasicAudio != 0)
					hmp3.m_pBasicAudio->put_Volume(hmp3.m_volume);
			}
		if (hudbar.Update()) 
			if (hmp3.m_volume < 0) {
				hmp3.m_volume += VOLUME_DS;
				if (hmp3.m_pBasicAudio != 0)
					hmp3.m_pBasicAudio->put_Volume(hmp3.m_volume);
			}

*/
		if (zvukl.Update(inkey))
			if (dsound.m_volume > MIN_VOLUME)
				dsound.m_volume -= VOLUME_DS;
		if (zvukr.Update(inkey))
			if (dsound.m_volume < 0)
				dsound.m_volume += VOLUME_DS;

		if (obtiznostl.Update(inkey))
			if (DF_OBTIZNOST > 0)
				DF_OBTIZNOST--;
		if (obtiznostr.Update(inkey))
			if (DF_OBTIZNOST < 4)
				DF_OBTIZNOST++;

		if (rychlostl.Update(inkey))
			if (DF_GAME_SPEED > 0.7) DF_GAME_SPEED -= 0.05;
		if (rychlostr.Update(inkey))
			if (DF_GAME_SPEED < 1.3) DF_GAME_SPEED += 0.05;

		if (g_players != 0) {
			if (hrac1l.Update(inkey)) {
				if (--a_player[0].gpID < 0) {
					a_player[0].gpID = g_players-1;
					m_pl = -1;
				}
				a_player[0] = g_player[a_player[0].gpID];
			}
			if (hrac1r.Update(inkey)) {
				if (++a_player[0].gpID >= g_players) {
					a_player[0].gpID = 0;
					m_pl = -1;
				}
				a_player[0] = g_player[a_player[0].gpID];
			}
			if (hrac2l.Update(inkey)) {
				if (--a_player[1].gpID < 0) {
					a_player[1].gpID = g_players-1;
					m_pl = -1;
				}
				a_player[1] = g_player[a_player[1].gpID];
			}
			if (hrac2r.Update(inkey)) {
				if (++a_player[1].gpID >= g_players) {
					a_player[1].gpID = 0;
					m_pl = -1;
				}
				a_player[1] = g_player[a_player[1].gpID];
			}
		}



		if (hrac1.Update(inkey) || selected == 0)
			m_pl = 0;

		if (hrac2.Update(inkey) || selected == 1)
			m_pl = 1;

		if (m_pl != -1)
		{
			if (ReadFromSC(a_player[m_pl].name, 20, insc))
				a_player[m_pl].gpID = -1;
		}
	}
}

