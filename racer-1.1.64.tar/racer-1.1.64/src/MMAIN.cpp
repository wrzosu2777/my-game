// MMain.cpp: implementation of the SMMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdh.h"
#include "MMain.h"
#include "MButton.h"
#include "lang.h"
#include "GRun.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MMain::MMain()
{
	m_tabulka = NULL;
	printf("main menu \n");
}

MMain::~MMain()
{
	Destroy();
}

void MMain::LoadGFX()
{
	Destroy();
	ASSERT(m_tabulka == NULL);
	
	printf("loading BMP \n");
	m_tabulka = fload_bitmap("HM_1");
}

void MMain::Run()
{
	int ret;

	for(;;){

		// vyber z menu

		if((ret = Select()) == e_exit) break;

		// odskok z menu (e_exit je zpracovan vyse)
		switch(ret){
		case e_train :
			m_doubleplay.Run();
			break;
		case e_about :
			m_autori.Run();
			break;
		case e_nastaveni :
			m_nastaveni.Run();
			break;
		case e_demo :
			Demo();
			break;
		case e_single :
			m_single.Run();
			break;
		}
	}
}

void MMain::Destroy()
{
	if(m_tabulka != NULL) {
		destroy_bitmap(m_tabulka);
		m_tabulka = NULL;
	}
}

int MMain::Select()
{
	m_carindex = rand()%5+1;
	LoadGFX();

	kariera.Init(320, 146, LNG_1HRAC, g_font18); 
	trenink.Init(320, 193, LNG_2HRACI, g_font18);
	nastaveni.Init(320, 243, LNG_NASTAVENI, g_font18);
	autori.Init(320, 290, LNG_AUTORI, g_font18);
	konec.Init(320, 338, LNG_KONEC, g_font18);

//	hscreen.m_mouse = true;

	int ret = Loop();

	Destroy();
	autori.Destroy();
	kariera.Destroy();
	trenink.Destroy();
	nastaveni.Destroy();
	konec.Destroy();
	return ret;
}




int MMain::Loop()
{
    int selected = -1;  // This is a new hack for using keyboard to select things...
    int buttons_cnt  = 5;
    int old_mouse_x = mouse_x;
    int old_mouse_y = mouse_y;
    TButton *buttons[] = {&kariera, &trenink, &nastaveni, &autori, &konec};
    unselect_all_buttons(buttons, buttons_cnt);
    
	tmr = 3000;
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
		
		autori.Draw();
		kariera.Draw();
		trenink.Draw();
		nastaveni.Draw();
		konec.Draw();
		release_bitmap(hscreen.m_back);

		if (kariera.IsMouseIn()) {
			TTDraw(hscreen.m_back, 320, 159, WIN_RGB(255, 255, 255), LNG_SINGLE_L1, g_font12);
			TTDraw(hscreen.m_back, 320, 171, WIN_RGB(255, 255, 255), LNG_SINGLE_L2, g_font12);
		}
		if (trenink.IsMouseIn()) {
			TTDraw(hscreen.m_back, 320, 205, WIN_RGB(255, 255, 255), LNG_DOUBLE_L1, g_font12);
			TTDraw(hscreen.m_back, 320, 217, WIN_RGB(255, 255, 255), LNG_DOUBLE_L2, g_font12);
		}
		if (nastaveni.IsMouseIn()) {
			TTDraw(hscreen.m_back, 320, 255, WIN_RGB(255, 255, 255), LNG_NASTAVENI_L1, g_font12);
			TTDraw(hscreen.m_back, 320, 267, WIN_RGB(255, 255, 255), LNG_NASTAVENI_L2, g_font12);
		}
		if (autori.IsMouseIn()) {
			TTDraw(hscreen.m_back, 320, 302, WIN_RGB(255, 255, 255), LNG_AUTORI_L1, g_font12);
			TTDraw(hscreen.m_back, 320, 314, WIN_RGB(255, 255, 255), LNG_AUTORI_L2, g_font12);
		}
        
		TTDraw(hscreen.m_back, 560, 460, WIN_RGB(255, 255, 255), "v 1.1 64bit", g_font12);

	
	   hscreen.Flip();

//	  Sleep(10);
        int inkey = 0;
		if (keypressed())
        {
            inkey = readkey() >> 8;
			if (inkey == KEY_ESC) return e_exit;
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

        if (kariera.Update(inkey)) return e_single;
        if (trenink.Update(inkey)) return e_train;
        if (nastaveni.Update(inkey)) return e_nastaveni;
        if (autori.Update(inkey))  return e_about;
		if (konec.Update(inkey)) return e_exit;
		if (tmr == 0) return e_demo;
	}
}



void MMain::Demo()
{
//	GRun run;
//	run.Demo();
//	GRun run; // promena typu GRun musi byt jen jedna !!!!!!


	run.PrepareScreen();

	// trat
	if(!run.NewRace(Tparams[rand()%MAX_TRATI].file)) {
//		run.Destroy();
		allegro_message("Nepodarilo se nacist trat !!"); // SMULA !!!!
		return;
	}



	for(int i = 0; i < 10; i++)
			run.m_cars[i].SetCar(rand()%MAX_CARS, "Demo", -1);



	run.PrepareRace((rand()%2)==1, true, 10, 0, 2+rand()%5);

	hmp3.PlayGame();
	run.Demo();
	hmp3.PlayMenu();

	// rusni bitmap
	run.Destroy();

	run.RestoreScreen();

}
