// GObject.cpp: implementation of the GObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdh.h"
#include "GObject.h"
#include "GCar.h"
#include "objects.h"
#include "GMap.h"

/*////////////////////////////////////////////////////////////////////////////////
  Nulovaci konstruktor
*/
GObject::GObject()
{
	m_par.bindex = -1;
	m_par.h2  = -1;
	m_par.w2  = -1;
	m_smashed = false;
	m_x = 0;
	m_y = 0;
	m_btmp = NULL;
	m_angle  = 0;
	m_angles = 0;
	m_speed  = 0;
	m_turning = 0;
	m_ralpha = 0;
}

/*////////////////////////////////////////////////////////////////////////////////
  Prazdny destruktor
*/
GObject::~GObject()
{
	Destroy();
}

/*////////////////////////////////////////////////////////////////////////////////
  Kresli auta na danou bitmapu ( zavodni plochu )
  dest - pointer na bitmapu, kam se ma auto nakreslit (velikosti DF_X, DF_X )
  x,y  - souradnice stredu dest vuci trati ( jinak jsou ve stredu obrazovky )
  hi   - kreslena haldina
*/
void GObject::Draw(BITMAP *dest, int x, int y, int hi, bool shadow)
{
	if (DF_STINY == S_STATIC) shadow = false;
  // test na platnost parametru
	if((dest == NULL) || (m_par.bindex == -1) || (m_par.anims == 0)) return; // error

 // test na vysku
	if (m_hi != hi) return;

 // stin
	if ((!m_smashed) && (shadow == true)) 
		DrawShadow(dest, x, y, hi);
	
  // uprava parametru (ukacuji na stred objektu)
	x = m_x - x + (DF_X/2);
	y = m_y - y + (DF_Y/2);


	if(m_angle == 0) {
		if(!m_smashed)
			draw_sprite(dest, b_objects[m_par.bindex+2+m_anim/3], x - m_par.wb2, y - m_par.hb2);
		else 
			draw_sprite(dest, b_objects[m_par.bindex+2+m_anim/3+m_par.anims], x - m_par.wb2, y - m_par.hb2);
	}
	else {
		if(!m_smashed)
			rotate_sprite(dest, b_objects[m_par.bindex+2+m_anim/3], x - m_par.wb2, y - m_par.hb2, itofix(m_angle));
		else
			rotate_sprite(dest, b_objects[m_par.bindex+2+m_anim/3+m_par.anims], x - m_par.wb2, y - m_par.hb2, itofix(-m_angle+64));
	}
}

/*////////////////////////////////////////////////////////////////////////////////
  Inicializuje objekt
  x,y  : souradnice seho umisteni v bitmape
  ID   : index v poli Oparams
*/
void GObject::Init(int x, int y, int ID)
{
	m_x = x;
	m_y = y;

	m_dx = x;
	m_dy = y;

	m_krev = 0;
	
	m_smashed = false;
	m_par = *Oparams[ID];
	m_ID  = ID;

	m_angle = 0;
	m_ralpha = 0;
	m_speed = 0;
	m_turning = 0;

	m_anim = 0;

	LoadGFX();
	m_tmpa = m_angle + 1;

	ComputeRect();
}

/*////////////////////////////////////////////////////////////////////////////////
  Hybne objektem (je-li type letajici)
*/
void GObject::Move(GMap *map)
{

	ComputeRect();

	// pocitani animaci
	if (m_smashed) 
		if (m_anim < (m_par.canims*3-1)) m_anim++;
		else m_anim = (m_par.canims*3-1);
	else
		if (m_anim < (m_par.anims*3-1)) m_anim++;
		else m_anim = 0;


	if (m_smashed)  return;

	double dalpha; // uhel, kterym chceme letet

	if (m_speed != 0) { 
		if ((abs(m_dx-m_x) < 10) && (abs(m_dy-m_y)) < 10) {
			m_dx = rand()%map->m_sx;
			m_dy = rand()%map->m_sy;
		}
			dalpha = atan2(m_y-m_dy, m_dx-m_x);
			if(dalpha == 0) { // delame novy vyber
				m_dy = m_y;
				m_dx = m_x;
			}
		
		if (dalpha > m_ralpha) m_ralpha += m_turning;
		if (dalpha < m_ralpha) m_ralpha -= m_turning;

		if (map->CarOK(m_x + cos(m_ralpha) * m_speed, m_y - sin(m_ralpha) * m_par.speed) ||
			 m_par.type == OBJECT_FLYING) {
			m_x += cos(m_ralpha) * m_speed;		
			m_y -= sin(m_ralpha) * m_speed;		
		}
		else 
			if (abs(dalpha - m_ralpha) < 10) {
				m_dy = m_y;
				m_dx = m_x;
			}

		// pokud se objekt sam neotaci, tak ho nasmerujeme, podletoho, kam leti
		if (m_par.angles == 0) m_angle = -m_ralpha * 256 / DPI;

	}

	if (m_par.angles != 0) m_angle += m_angles;

}

/*////////////////////////////////////////////////////////////////////////////////
  Oznamuje objektu, ze ho nekdo rozjel
  car  :  ukazatel na aute, ktere si dovolilo rozjet tento objekt
*/
void GObject::Smashed(GCar *car)
{
	if (m_par.type != OBJECT_SOFT) return; // pro jistotu
	m_smashed = true;
	m_anim = 0;
	m_angle   = (car->m_angler); // vec spadne pod uhlem, pod kterym do ni ten drzoun najel	
	if(car->m_speed < 0) m_angle -= 128;
	m_sound.Stop();
	m_soundSmash.Play(false);
}

/*////////////////////////////////////////////////////////////////////////////////
	Vypocita hranicni obdelniky
*/
void GObject::ComputeRect()
{
	m_rect[0].x = m_x + m_par.w2;
	m_rect[0].y = m_y - m_par.h2;

	m_rect[1].x = m_x - m_par.w2;
	m_rect[1].y = m_y - m_par.h2;

	m_rect[2].x = m_x - m_par.w2;
	m_rect[2].y = m_y + m_par.h2;

	m_rect[3].x = m_x + m_par.w2;
	m_rect[3].y = m_y + m_par.h2;

	if (m_par.startpos > 1 ) {
		m_srect.bottom = m_y + m_par.hb2;
		m_srect.top    = m_y - m_par.hb2;
		m_srect.left   = m_x - m_par.wb2;
		m_srect.right  = m_x + m_par.wb2;
	}
	else {
		m_srect.bottom = m_y + m_par.h2;
		m_srect.top    = m_y - m_par.h2;
		m_srect.left   = m_x - m_par.w2;
		m_srect.right  = m_x + m_par.w2;
	}


	// uprava vysky
	// test na spravnou hloubku
	m_hi = m_par.hi;
	if (m_par.type == OBJECT_SOFT && m_smashed == true && m_anim == (m_par.canims*3-1))
		m_hi = OBJ_HI_LAY;
}

/*////////////////////////////////////////////////////////////////////////////////
  vraci, zda auto narazilo do hrany objectu
  car  :  ukazatel na auto, ktere si dovolilo narazit
*/
bool GObject::Hit(GCar *car)
{
	if (PtInRect(&m_srect, car->m_rect[0]) ||
		 PtInRect(&m_srect, car->m_rect[1]) ||
		 PtInRect(&m_srect, car->m_rect[2]) ||
		 PtInRect(&m_srect, car->m_rect[3]) ||
		 PtInRect(&m_srect, car->m_rect2[0]) ||
		 PtInRect(&m_srect, car->m_rect2[1]) ||
		 PtInRect(&m_srect, car->m_rect2[2]) ||
		 PtInRect(&m_srect, car->m_rect2[3])) return true;
	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
  Zaktualizuje stin
*/
void GObject::UpdateTmp()
{
	if (m_btmp == NULL) return;
	if (m_angle == m_tmpa) return;
	if (b_objects[m_par.bindex] == NULL) return;

	clear_to_color(m_btmp, makecol(255, 0, 255)); // ruzova je barva nase :)
	rotate_sprite(m_btmp, b_objects[m_par.bindex], m_tmpx/2-m_par.wb2, m_tmpx/2-m_par.hb2, itofix(m_angle));
	m_tmpa = m_angle;
}

/*////////////////////////////////////////////////////////////////////////////////
  Znici vsechny bitmapy
*/
void GObject::Destroy()
{
	if (m_btmp != NULL) 
		destroy_bitmap(m_btmp);
	m_btmp = NULL;

	m_sound.Stop();
	m_sound.Release();
	m_soundSmash.Stop();
	m_soundSmash.Release();
}

/*////////////////////////////////////////////////////////////////////////////////
  Vytvori bitmapu stinu
*/
void GObject::LoadGFX()
{
	char c[100];

	Destroy();

	ASSERT(m_btmp == NULL);
	m_tmpx = 2*pow(pow(m_par.hb2,2) + pow( m_par.wb2,2), 0.5);
	if (m_par.type != OBJECT_LOW && m_par.type != OBJECT_NONE && b_objects[m_par.bindex] != NULL) 
		m_btmp = create_bitmap(m_tmpx, m_tmpx);

	if (m_par.sound) {
		rload_snd(m_par.name, &m_sound);
		m_toplay = rand()%(m_par.reptime+1);
		sprintf(c, "%s_S", m_par.name);
		rload_snd(c, &m_soundSmash);
	}

}

void GObject::DrawLight(BITMAP *lmap, int x, int y, int hi)
{
	if (m_par.light == 0 || m_smashed) return;
	if (m_hi != hi && hi != -1) return;

  // uprava parametru (ukacuji na stred objektu)
	x = m_x - x + (DF_X/2);
	y = m_y - y + (DF_Y/2);
	
	hdraw_light_spritetcm(lmap, b_objects[m_par.bindex+1], x - b_objects[m_par.bindex+1]->w/2, y - b_objects[m_par.bindex+1]->h/2);
}

void GObject::DrawShadow(BITMAP *dest, int x, int y, int hi)
{
	if (m_hi != hi || (m_btmp == NULL)) return;
	x = m_x - x + (DF_X/2);
	y = m_y - y + (DF_Y/2);
	UpdateTmp();
	set_trans_blender(0, 0, 0, 125);
	draw_trans_sprite(dest, m_btmp, x - m_tmpx/2-m_par.stin, y - m_tmpx/2+m_par.stin);
}

void GObject::UpdateSound(int x, int y, int dst)
{
	if (m_toplay-- == 0) {
		if (m_par.reptime == 0) {
			m_sound.Play(true);
			m_toplay = -1;
		}
		else {
			if (m_smashed == false) {
				m_sound.Play(false);
				m_toplay = rand()%(m_par.reptime/2) + m_par.reptime/2;
			}
		}
	}

	m_sound.SetVolume(0-dst*3);
	m_sound.SetPan((x-m_x)*3);

}

void GObject::StopSound()
{
	m_sound.Stop();
	m_soundSmash.Stop();
	m_toplay = rand()%(m_par.reptime+1);
}
