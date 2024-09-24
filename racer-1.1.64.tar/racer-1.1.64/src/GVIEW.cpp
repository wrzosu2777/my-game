// GView.cpp: implementation of the GView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdh.h"
#include "cars.h"
#include "GCar.h"
#include "GView.h"
#include "GObject.h"
#include "GMap.h"
#include "GRun.h"

#define MAX_GW_BITMAPS 23
BITMAP *b_gw[MAX_GW_BITMAPS];

#define MAX_T_BITMAPS 12
BITMAP *b_t[MAX_T_BITMAPS];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*////////////////////////////////////////////////////////////////////////////////
  NULL-ovaci konstruktor
*/
GView::GView()
{
	m_rect.left   = 0;
	m_rect.top    = 0;
	m_rect.right  = 640;
	m_rect.bottom = 480;

}

/*////////////////////////////////////////////////////////////////////////////////
  Prazdnu destruktor
*/
GView::~GView()
{
	Destroy();
}

/*////////////////////////////////////////////////////////////////////////////////
  Nakresli uplne vsechno na hscreen.back
*/
void GView::Draw(GRun *run)
{
//	set_clip(hscreen.m_back, m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);

	bool stin = run->m_stin;
	int i;

	if (run->m_noc) stin = false;
	if (hscreen.m_bpp == 8) stin = false;

 // cast pod autem
	DF_X = m_rect.right- m_rect.left; DF_Y = m_rect.bottom-m_rect.top;
	BITMAP* bmp = create_sub_bitmap(hscreen.m_back, m_rect.left, m_rect.top, DF_X, DF_Y);

	run->m_map.Draw(bmp, hscreen.m_aabuffer, m_Cx, m_Cy, stin);

	run->m_map.DrawObj(bmp, m_Cx, m_Cy,  OBJ_HI_GROUNG, stin);
	run->m_map.DrawObj(bmp, m_Cx, m_Cy,  OBJ_HI_LAY, stin);

// stin auta
	if (stin)
		for (i = 0; i < run->m_carsc; i++)
			if (run->m_cars[i].m_active) run->m_cars[i].DrawShadow(bmp, m_Cx, m_Cy);
// karoserie auta
	for (i = 0; i < run->m_carsc; i++)
		if (run->m_cars[i].m_active) run->m_cars[i].DrawCar(bmp, m_Cx, m_Cy, run->m_noc);

	// bonusy
	if (run->m_bonusy_enabled)
		run->m_map.DrawBonusy(bmp, b_gw[21], m_Cx, m_Cy);


// kour auta 
	for (i = 0; i < run->m_carsc; i++)
		if (run->m_cars[i].m_active) run->m_cars[i].DrawSmog(bmp, m_Cx, m_Cy);
	// svetla
	if (run->m_noc) {
		run->m_map.DrawLights(bmp, m_Cx, m_Cy);
		for (i = 0; i < run->m_carsc; i++)
			if (run->m_cars[i].m_active) run->m_cars[i].DrawLight(bmp, m_Cx, m_Cy);
	}
 // cast nad autem
	run->m_map.DrawObj(bmp, m_Cx, m_Cy,  OBJ_HI_NORMAL, stin);
	run->m_map.DrawObj(bmp, m_Cx, m_Cy,  OBJ_HI_TOP, stin);
	run->m_map.DrawObj(bmp, m_Cx, m_Cy,  OBJ_HI_FLY, stin);

	// jmena ridicu
	for (i = 0; i < run->m_carsc; i++)
		if (run->m_cars[i].m_active) run->m_cars[i].DrawName(bmp, m_Cx, m_Cy);


//	for (i = 0; i < run->m_carsc; i++)
		run->m_cars[0].DrawDBG(run, bmp, m_Cx, m_Cy);
	

	// Popisky trate
	GCar *car = &run->m_cars[m_moncar];

	// zvyrazneni
	if (run->m_semafor > SEMAFOR_ZELENA)
		car->DrawZvyrazneni(bmp, m_Cx, m_Cy);

	// Tachometr / otackometr
	if (car->m_active) {
		int speed = abs(car->m_speed*10);
		if (DF_X >= 640) {
			draw_sprite(bmp, b_gw[1], DF_X-150, DF_Y-150);

			if (speed >= 100)
				blit(b_gw[0], bmp, 16*(speed/100), 0, DF_X-58, DF_Y-56, 15, 22);
			speed %= 100;
			if (abs(car->m_speed) >= 1)
				blit(b_gw[0], bmp, 16*(speed/10), 0, DF_X-43, DF_Y-56, 15, 22);
			speed %= 10;
			blit(b_gw[0], bmp, 16*(speed), 0, DF_X-28, DF_Y-56, 15, 22);

			pivot_sprite(bmp, b_gw[2], DF_X-18, DF_Y-14, 92, 4, ftofix(8+car->m_otacky*35));
		}
		else {
			draw_sprite(bmp, b_gw[20], DF_X-b_gw[20]->w, DF_Y-b_gw[20]->h);
			if (speed >= 100)
				blit(b_gw[0], bmp, 16*(speed/100), 0, DF_X-49, DF_Y-26, 15, 22);
			speed %= 100;
			if (abs(car->m_speed) >= 1)
				blit(b_gw[0], bmp, 16*(speed/10), 0, DF_X-34, DF_Y-26, 15, 22);
			speed %= 10;
			blit(b_gw[0], bmp, 16*(speed), 0, DF_X-19, DF_Y-26, 15, 22);
		}
	}

	// semafor
	if (run->m_semafor > SEMAFOR_ORANZOVA)
		draw_sprite(bmp, b_gw[3], DF_X/2-b_gw[3]->w/2, 20);
	else
		if (run->m_semafor > SEMAFOR_ZELENA)
			draw_sprite(bmp, b_gw[4], DF_X/2-b_gw[4]->w/2, 20);
		else
			if (run->m_semafor > SEMAFOR_NENI)
				draw_sprite(bmp, b_gw[5], DF_X/2-b_gw[5]->w/2, 20);


	// poradi
	if (car->m_show > 0) {
		if (DF_GAMEBPP != 8) {
			set_trans_blender(0, 0, 0, (255*car->m_show)/POS_SHOW);
			draw_trans_sprite(bmp, b_gw[6+car->m_pos[car->m_kol][car->m_chp]], 10, 50);
		}
		else
		   draw_sprite(bmp, b_gw[6+car->m_pos[car->m_kol][car->m_chp]], 10, 50);
		car->m_show--;
	}

	if (car->m_show == POS_SHOW_STILL)
	   draw_sprite(bmp, b_gw[6+car->m_pos[car->m_kol][car->m_chp]], 10, 50);

	// pocet kol
	if (car->m_active) {
		if (car->m_kol >= run->m_kol) 
		   draw_sprite(bmp, b_gw[17], 10, 10);
		else {
			int tx = 10, w = b_gw[16]->w/11;
			tx-=w;
			if (++car->m_kol >= 10)
				masked_blit(b_gw[16], bmp, w*(car->m_kol/10), 0, tx+=w, 10, w-1, b_gw[16]->h);
			masked_blit(b_gw[16], bmp, w*(car->m_kol%10), 0, tx+=w, 10, w-1, b_gw[16]->h);
			car->m_kol--;
			masked_blit(b_gw[16], bmp, w*10, 0, tx+=w, 10, w-1, b_gw[16]->h);
			if (run->m_kol >= 10)
				masked_blit(b_gw[16], bmp, w*(run->m_kol/10), 0, tx+=w, 10, w-1, b_gw[16]->h);
			masked_blit(b_gw[16], bmp, w*(run->m_kol%10), 0, tx+=w, 10, w-1, b_gw[16]->h);
		}
	}

		// minimapa
	if (run->m_semafor == SEMAFOR_NENI) {
		int left = DF_X - run->m_map.m_bMini->w - 20;
		int top  = 20;
		int w = run->m_map.m_bMini->w;
		int h = run->m_map.m_bMini->h;
		draw_sprite(bmp, run->m_map.m_bMini, left, top);
		for (i = 0; i < run->m_carsc; i++) {
			if (!run->m_cars[i].m_active) continue;
			if (i == m_moncar) {
				draw_sprite(bmp, b_gw[18], left + (w*run->m_cars[i].m_x)/run->m_map.m_sx-b_gw[18]->w/2, top + (h*run->m_cars[i].m_y)/run->m_map.m_sy-b_gw[18]->h/2);
				continue;
			}
			if (i < run->m_hracu)
				draw_sprite(bmp, b_gw[22], left + (w*run->m_cars[i].m_x)/run->m_map.m_sx-b_gw[19]->w/2, top + (h*run->m_cars[i].m_y)/run->m_map.m_sy-b_gw[19]->h/2);
			else
				draw_sprite(bmp, b_gw[19], left + (w*run->m_cars[i].m_x)/run->m_map.m_sx-b_gw[19]->w/2, top + (h*run->m_cars[i].m_y)/run->m_map.m_sy-b_gw[19]->h/2);
		}
	}

	

	// stopky
	int x = 50;
	int y = DF_Y - 25;

	draw_sprite(bmp, b_t[11], x, y);
	draw_sprite(bmp, b_t[((int)(car->m_times*10)%10)], x+b_t[11]->w+1, y);
	
	int sec = ((int)(car->m_times)%60);
	x -= b_t[sec%10]->w + 1;
	draw_sprite(bmp, b_t[sec%10], x, y);
	x -= b_t[sec/10]->w + 1;
	draw_sprite(bmp, b_t[sec/10], x, y);

	x -= b_t[10]->w + 1;
	draw_sprite(bmp, b_t[10], x, y);

	
	int mins = ((int)car->m_times)/60;
	while (mins != -1) {
		x -= b_t[mins%10]->w + 1;
		draw_sprite(bmp, b_t[mins%10], x, y);
		mins /= 10;
		if (mins == 0) mins = -1;
	}


//	sprintf(s, "%d:%02d.%d s", ((int)car->m_times)/60, ((int)(car->m_times)%60), ((int)(car->m_times*10)%10));
// DEBUG STRINGY


//	char s[50];

//	textout(bmp, font, s, 10, DF_Y-10, makecol(200,200,200));
/*	sprintf(s,"cas %.2f sekund",car->m_times);
	textout(bmp, font, s, 10, DF_Y-25, makecol(200,200,200));
	sprintf(s,"avs %.2f km/h",car->m_avs);
	textout(bmp, font, s, 10, DF_Y-40, makecol(200,200,200));
	*/
	destroy_bitmap(bmp);

}

/*////////////////////////////////////////////////////////////////////////////////
  Inicializuje GView tridu
  car  : ukazatel na moniotrovane auto
*/
void GView::Init(GMap *map, int moncar)
{
	m_cam_end = map->m_cam_end;
	m_Cx = map->m_cam_start.x;
	m_Cy = map->m_cam_start.y;
	m_moncar = moncar;
}

/*////////////////////////////////////////////////////////////////////////////////
  Posune kameru smerem k jejimu pozadovanemumistu ( to zavisi na smeru a rychlosti auta )
*/
void GView::UpdateCamera(GRun *run)
{
	GCar *car = &(run->m_cars[m_moncar]);
	GMap *map = &(run->m_map);

	if (run->m_semafor > SEMAFOR_ORANZOVA) {
		int ds = (run->m_semafor - SEMAFOR_ORANZOVA);
		m_Cx += (m_cam_end.x - m_Cx)/ds;
		m_Cy += (m_cam_end.y - m_Cy)/ds;
	} 
	else
		if (run->m_semafor > SEMAFOR_ZELENA) {
			int ds = (run->m_semafor - SEMAFOR_ZELENA);
			m_Cx += (car->m_x - m_Cx)/ds;
			m_Cy += (car->m_y - m_Cy)/ds;
		}
		else {
			m_Cx = m_Cx * 0.9 + 0.1 * (car->m_x +  cos(car->m_angle*DPI/256) * car->m_speed / G_MAX_SPEED * G_CAMERA_DISTANCE_X * DF_GAME_SPEED);
			m_Cy = m_Cy * 0.9 + 0.1 * (car->m_y -  sin(car->m_angle*DPI/256) * car->m_speed / G_MAX_SPEED * G_CAMERA_DISTANCE_Y * DF_GAME_SPEED);
		}


	DF_X = m_rect.right - m_rect.left; DF_Y = m_rect.bottom - m_rect.top;
	if(m_Cx < DF_X/2) m_Cx = DF_X/2;
	if(m_Cy < DF_Y/2) m_Cy = DF_Y/2;
	if(m_Cx > map->m_sx - DF_X/2) m_Cx = map->m_sx - DF_X/2;
	if(m_Cy > map->m_sy  - DF_Y/2) m_Cy = map->m_sy  - DF_Y/2;
}

/*////////////////////////////////////////////////////////////////////////////////
  Pred ukoncenim programu znici ( uvolni ) vsechny alokovane bitmapy
*/
void GView::Destroy()
{
	GRun::DestroyBitmaps(&b_gw[0], MAX_GW_BITMAPS);
	GRun::DestroyBitmaps(&b_t[0], MAX_T_BITMAPS);
}

/*////////////////////////////////////////////////////////////////////////////////
  Nacte grafiku
*/
bool GView::LoadGFX()
{
	Destroy();

	GRun::LoadBitmaps(&b_t[0], MAX_T_BITMAPS, 0, "t%d");
	return GRun::LoadBitmaps(&b_gw[0], MAX_GW_BITMAPS, 1, "gw%d");
}
