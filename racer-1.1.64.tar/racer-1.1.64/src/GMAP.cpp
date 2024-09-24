// GMap.cpp: implementation of the GMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GObject.h"
#include "GMap.h"
#include "GView.h"
#include "GCar.h"
#include "GRun.h"

#include <iostream>


st_ground g_Ground[MAX_GROUNDS] =
{
	{0, 0,    -1,},  // cerna         - neprujezdna
	{0, 1,    -1,},  // bila          - silnice
	{0, 0.25,  0,},  // zluta         - pole a slama
	{0, 0.5,   1,},  // zelena        - trava
	{0, 0.9,  -1,},  // modra         - prasna cesta
	{0, 0.25, -1,},  // svetle modra  - snih
	{0, 0.25, -1,},  // cervena       - pisek
	{0, 1,    -1,},  // ruzova        - neidetifikovana
};

/*////////////////////////////////////////////////////////////////////////////////
  Konstruktor
*/
GMap::GMap()
{
	m_bGround = NULL;
	m_bSys    = NULL;
	m_bMini   = NULL;

	int i;
	for(i = 0; i < MAX_OBJECTS_ON_MAP; i++)
		m_objects[i] = NULL;
	m_cobjects = 0;

	for (int j = 0; j < MAX_ROADS_TYPES; j++) {
		for (i = 0; i < MAX_ROADS_ON_MAP; i++)
				m_roads[j][i] = NULL;
		m_croads[j] = 0;
	}

	g_Ground[1].color = makecol(255,255, 255);

	g_Ground[5].color = makecol(0,  255, 255);
	g_Ground[7].color = makecol(255,  0, 255);
	g_Ground[2].color = makecol(255,255,   0);

	g_Ground[6].color = makecol(255,  0,   0);
	g_Ground[3].color = makecol(0,  255,   0);
	g_Ground[4].color = makecol(0,    0, 255);

	g_Ground[0].color = makecol(0,    0,   0);

}

/*////////////////////////////////////////////////////////////////////////////////
  Destruktor
*/
GMap::~GMap()
{
	DestroyArrays();
	Destroy();
}

/*////////////////////////////////////////////////////////////////////////////////
  Smaze vsechna data a pripravi se na nove zadavani
  road   :  pocet bodu silnice
  object :  pocet objektu
*/
void GMap::Reset(int X, int Y, int bonusu, POINT cam_start, POINT cam_end)
{
	DestroyArrays();
	m_sx = X;
	m_sy = Y;
	m_cam_start = cam_start;
	m_cam_end = cam_end;
	m_bonusu = bonusu;
	for (int i = 0; i < MAX_BONUSU_ON_MAP; i++) {
		m_bonusy[i].x = -1; // zneplatneni
		m_bonusy[i].y = -1;
	}
}

/*////////////////////////////////////////////////////////////////////////////////
  Oznamuje, ze byla hotova inicializace (pri nacitani ze souboru)
*/
void GMap::InitDone(char *filename)
{
	strcpy(m_file, filename);
	int i;
	for (i = 0; i < MAX_CHP; i++)
		m_chp[i] = -1;

	m_checkpoints = 1;
	m_maxcheckpoints = 0;
	for(i = 0; i < MAX_OBJECTS_ON_MAP; i++)
		if (m_objects[i] != NULL) {
			if (m_objects[i]->m_par.startpos == 2) {
				m_chp[0] = i;
				m_objects[i]->m_turning = 0;
			}
			if (m_objects[i]->m_par.startpos == 3) {
				m_chp[m_checkpoints] = i;
				m_checkpoints++;
				if (m_maxcheckpoints < m_objects[i]->m_turning) 
					m_maxcheckpoints = m_objects[i]->m_turning;
			}
		}
	m_checkpoints--;
}

/*////////////////////////////////////////////////////////////////////////////////
  Prida dalsi bod silnice
*/
void GMap::AddRoadPoint(int type, st_GFile_Road road)
{
	if (m_croads[type] < MAX_ROADS_ON_MAP) {
		m_roads[type][m_croads[type]] = new st_GFile_Road;
		ZeroMemory(m_roads[type][m_croads[type]], sizeof(st_GFile_Road));
		*m_roads[type][m_croads[type]] = road;
		m_croads[type]++;
	}

}

/*////////////////////////////////////////////////////////////////////////////////
  Prida dalsi objekt
*/
void GMap::AddObject(st_GFile_Object ob)
{
	if (m_cobjects < MAX_OBJECTS_ON_MAP) {
		m_objects[m_cobjects] = new GObject;
		m_objects[m_cobjects]->Init(ob.x, ob.y, ob.ID);
		if (Oparams[ob.ID]->dangle) m_objects[m_cobjects]->m_angle   = ob.alpha *256/360; // pocatecni uhel natoceni
		if (Oparams[ob.ID]->angles) m_objects[m_cobjects]->m_angles  = ob.angles;  // rychlost toceni
		if (Oparams[ob.ID]->speed)  m_objects[m_cobjects]->m_speed   = ob.speed;   // rychlost pohybu
		if (Oparams[ob.ID]->turning)m_objects[m_cobjects]->m_turning = ob.turning; // rychlost zataceni
		fprintf(stdout, "Object added\n");
		m_cobjects++;
	}
}

/*////////////////////////////////////////////////////////////////////////////////
  Znici alokovane bitmapy
*/
void GMap::Destroy()
{
	if(m_bGround != NULL) {
		destroy_bitmap(m_bGround);
		m_bGround = NULL;
	}

	if(m_bSys != NULL) {
		destroy_bitmap(m_bSys);
		m_bSys = NULL;
	}

	if(m_bMini != NULL) {
		destroy_bitmap(m_bMini);
		m_bMini = NULL;
	}
	

	int i;
	// ruseni objektu
	for(i = 0; i < MAX_OBJECTS_ON_MAP; i++)
		if (m_objects[i] != NULL) m_objects[i]->Destroy();

}

/*////////////////////////////////////////////////////////////////////////////////
  Kresli mapu - podklad (travu a silnice)
  CX  :  x-ova souradnice kamery
  CY  :  y-ova souradnice kamery
*/
void GMap::Draw(BITMAP *dest,BITMAP *aabuff, int CX, int CY, bool stin)
{
	blit(m_bGround, dest, (CX - DF_X/2), (CY - DF_Y/2), 0, 0, DF_X, DF_Y);
}

/*////////////////////////////////////////////////////////////////////////////////
  Kresli mapu - objekty
  CX  :  x-ova souradnice kamery
  CY  :  y-ova souradnice kamery
  hi  :  vyskova hladina 
*/
void GMap::DrawObj(BITMAP *dest, int CX, int CY, int hi, bool stin)
{
	for( int i = 0; i < m_cobjects; i++)
		if (m_objects[i] != NULL && !m_objects[i]->m_par.large)
			m_objects[i]->Draw(dest, CX, CY, hi, stin);
}

/*////////////////////////////////////////////////////////////////////////////////
  Nacte graficka data
*/
bool GMap::LoadGFX(bool noc)
{
	Destroy();

	char s[250];
	sprintf(s, "maps/%s.JPG", m_file);

	BITMAP *btmp = hload_bitmap(locateFile(s).c_str());
	ASSERT(m_bGround == NULL);
	m_bGround = create_bitmap(btmp->w*2, btmp->h*2);
	hanti_alias(m_bGround, btmp);
	destroy_bitmap(btmp);

	if(noc)
		hfade_sprite(m_bGround);
	snprintf(s,250, "maps/%s.PCX", m_file);
	ASSERT(m_bSys == NULL);
	m_bSys = hload_bitmap(locateFile(s).c_str());

	snprintf(s,250, "%sM", m_file);
	ASSERT(m_bMini == NULL);
	m_bMini = fload_bitmap(locateFile(s).c_str());
	


	for( int i = 0; i < m_cobjects; i++)
		if (m_objects[i] != NULL) {
			m_objects[i]->LoadGFX();
			m_objects[i]->UpdateTmp();
		}

	FillLargeGround();


	return true;
}
/*////////////////////////////////////////////////////////////////////////////////
  Pohne vsemi objekty
*/
void GMap::Move()
{
	int i;
	for (i = 0; i < m_cobjects; i++)
		if (m_objects[i] != NULL) m_objects[i]->Move(this);

	for (i = 0; i < m_bonusu; i++)
		if (m_bonusy[i].x == -1) {
			int tid = rand()%m_croads[2];
			m_bonusy[i].x = m_roads[2][tid]->x-40+rand()%80;
			m_bonusy[i].y = m_roads[2][tid]->y-40+rand()%80;
		}

}

/*////////////////////////////////////////////////////////////////////////////////
  Rusi pole m_objects
*/
void GMap::DestroyArrays()
{
	int i;

	// ruseni objektu
	for(i = 0; i < MAX_OBJECTS_ON_MAP; i++)
    {
		if (m_objects[i] != NULL) {
			delete m_objects[i];
			m_objects[i] = NULL;
		}
    }
    m_cobjects = 0;

	// ruseni silnice
	for (int j = 0; j < MAX_ROADS_TYPES; j++) {
		for(i = 0; i < MAX_ROADS_ON_MAP; i++)
			if (m_roads[j][i] != NULL) {
				delete m_roads[j][i];
				m_roads[j][i] = NULL;
			}
		m_croads[j] = 0;
	}
}

/*////////////////////////////////////////////////////////////////////////////////
  Testuje narazy - bere vsechna auticka a zkousi je na vsechny objekty
*/
int GMap::Crashig(GRun *run)
{
	GObject  *ob = m_objects[0];  // pouzivame ukazatele a ne indexovani, jelikoz takhle to je rychlejsi

	int i,j,l = 0;
	
	l = 0; // pocet narazeni
	for(i = 0; i < m_cobjects; i++, ob++)
		if (m_objects[i] != NULL) {
			ob = m_objects[i];
			for (j = 0; j < run->m_carsc; j++)
				if (run->m_cars[j].Crash(ob)) l++;
		}

	if (run->m_bonusy_enabled) 
		for(i = 0; i < m_bonusu; i++)
			for (j = 0; j < run->m_carsc; j++)
				if (run->m_cars[j].PtInRect(m_bonusy[i]) && run->m_cars[j].m_active) {
					run->m_cars[j].m_bonusu++;
					m_bonusy[i].x = -1;
					m_bonusy[i].y = -1;
				}
	
	return l;
}



/*////////////////////////////////////////////////////////////////////////////////
  Hleda index silnice podle ID
*/
int GMap::GetIndex(int type, int ID)
{
	if (ID == -1) return -1;
	for (int i = 0; i < m_croads[type]; i++) 
		if (m_roads[type][i]->ID == ID) return i;
	return -1;
}

void GMap::FillLargeGround()
{
	for (int hi = OBJ_HI_GROUNG; hi <= OBJ_HI_FLY; hi++)
		for (int i = 0; i < m_cobjects; i++) 
			if (m_objects[i] != NULL) {
				if (DF_STINY == S_STATIC && m_run->m_noc == false && m_objects[i]->m_speed == 0) 
					m_objects[i]->DrawShadow(m_bGround, DF_X/2, DF_Y/2, hi);
				if (DF_SVETLA == S_STATIC && m_run->m_noc == true && m_objects[i]->m_speed == 0) 
					m_objects[i]->DrawLight(m_bGround, DF_X/2, DF_Y/2, hi);
				if (m_objects[i]->m_par.large)
					m_objects[i]->Draw(m_bGround, DF_X/2, DF_Y/2, hi, false);
			}


}

/*////////////////////////////////////////////////////////////////////////////////
  Zda je siouradnice prujezdna
*/
bool GMap::CarOK(int x, int y)
{
	if (GetGround(x, y) == 0)
		return false;

	return true;
}

#define NARAZ_ODHODVLEVO  1
#define NARAZ_ODHODVPRAVO 2
#define NARAZ_OTOCVPRAVO  4
#define NARAZ_OTOCVLEVO   8

int GMap::TestMove(GCar *car, int dx, int dy)
{
	// zasah natvrdo (predni, zadni a uplny prostredek)
	if (!CarOK(car->m_x + dx, car->m_y + dy) ||
		 !CarOK(car->m_rect2[1].x + dx, car->m_rect2[1].y + dy) ||
		 !CarOK(car->m_rect2[3].x + dx, car->m_rect2[3].y + dy)
		 ) return 16;

	// leva predni
	if (!CarOK(car->m_rect[0].x + dx, car->m_rect[0].y + dy)) return NARAZ_ODHODVPRAVO | NARAZ_OTOCVPRAVO;

	// prava predni
	if (!CarOK(car->m_rect[3].x + dx, car->m_rect[3].y + dy)) return NARAZ_ODHODVLEVO | NARAZ_OTOCVLEVO;

	// leva zadni
	if (!CarOK(car->m_rect[1].x + dx, car->m_rect[1].y + dy)) return NARAZ_ODHODVPRAVO | NARAZ_OTOCVLEVO;

	// parav zadni
	if (!CarOK(car->m_rect[2].x + dx, car->m_rect[2].y + dy)) return NARAZ_ODHODVLEVO | NARAZ_OTOCVPRAVO;

	// levy bok
	if (!CarOK(car->m_rect2[0].x + dx, car->m_rect2[0].y + dy)) return NARAZ_ODHODVPRAVO;

	// pravy bok
	if (!CarOK(car->m_rect2[0].x + dx, car->m_rect2[0].y + dy)) return NARAZ_ODHODVLEVO;


	return 0;
}

void GMap::RestoreCar(GCar *car)
{

	LogMessage("\nRestoreCar\n");
	if (car->m_autodriver && (car->m_TSTdistanceOld != -1)) {
		LogMessage("m_autodriver\n");
		int a = car->m_TSTdistanceOld;
		int b = car->m_TSTdistance;

		int ux = m_roads[car->m_par.rt][b]->x - m_roads[car->m_par.rt][a]->x;
		int uy = m_roads[car->m_par.rt][b]->y - m_roads[car->m_par.rt][a]->y;

		int c = -(ux*car->m_x + uy*car->m_y);
		int t = -(c + ux*m_roads[car->m_par.rt][a]->x + uy*m_roads[car->m_par.rt][a]->y)/(ux*ux+uy*uy);

		car->m_x = m_roads[car->m_par.rt][a]->x + t*ux;
		car->m_y = m_roads[car->m_par.rt][a]->y + t*uy;

		car->m_alphaRad = -atan2(uy, ux);

	}
	else {

		int nrst = 0;
		int dist = 1000;
		int ds;
		LogMessage("A");

		for (int i = 0; i < m_croads[car->m_par.rt]; i++)
			if (m_roads[car->m_par.rt][i] != NULL) {
				if (m_roads[car->m_par.rt][nrst]->IDA == -1 && m_roads[car->m_par.rt][nrst]->IDB == -1) {
					LogMessage("Road Error\n");
				//	continue;
				}

				if ((ds = pow(pow(m_roads[car->m_par.rt][i]->x-car->m_x,2)+pow(m_roads[car->m_par.rt][i]->y-car->m_y,2), 0.5)) < dist) {
					nrst = i;
					dist = ds;
				}
			}
		int nxt;

		LogMessage("B");
		if (m_roads[car->m_par.rt][nrst]->IDA != -1) nxt = GetIndex(car->m_par.rt, m_roads[car->m_par.rt][nrst]->IDA);
		else nxt = GetIndex(car->m_par.rt, m_roads[car->m_par.rt][nrst]->IDB);

		car->m_x = m_roads[car->m_par.rt][nrst]->x;
		car->m_y = m_roads[car->m_par.rt][nrst]->y;


		LogMessage("C");
		if (nxt != -1)
			car->m_alphaRad = -atan2(m_roads[car->m_par.rt][nxt]->y - m_roads[car->m_par.rt][nrst]->y,
			                         m_roads[car->m_par.rt][nxt]->x - m_roads[car->m_par.rt][nrst]->x);

	}

	// dopocitani ostatnich uhlu
	car->m_angle = car->m_alphaRad*256/DPI;
	car->m_angler = car->m_angle;
	car->m_alpha = car->m_angle*360/256;
	LogMessage("Restore done\n");

}

bool GMap::IsObjIndexInMap(int INDEX)
{
	for(int i = 0; i < MAX_OBJECTS_ON_MAP; i++)
		if (m_objects[i] != NULL)
			if (m_objects[i]->m_par.INDEX == INDEX) return true;

	return false;
}

void GMap::DrawLights(BITMAP *lmap, int Cx, int Cy)
{
	if (DF_SVETLA == S_STATIC) return;
	for( int i = 0; i < m_cobjects; i++)
		if (m_objects[i] != NULL) m_objects[i]->DrawLight(lmap, Cx, Cy, -1);
}



bool GMap::GetStartPos(int id, int *x, int *y, int *angle)
{
	for( int i = 0; i < m_cobjects; i++) {
		if (m_objects[i]->m_par.startpos == 1) id--;
		if (id == -1) {
			*x = m_objects[i]->m_x;
			*y = m_objects[i]->m_y;
			*angle = m_objects[i]->m_angle;
			return true;
		}
	}
	return false;
}

void GMap::TestChpoint(GCar *car)
{
	if (car->m_kol >= m_run->m_kol) return;


	int m = car->m_chp + 1;
	if (m > m_maxcheckpoints) m = 0;
	for (int i = 0; i <= m_checkpoints; i++ ) 
		if (m_objects[m_chp[i]]->m_turning == m) {
			POINT pt;
		   pt.x = car->m_x;
			pt.y = car->m_y;
			if (PtInRect(&m_objects[m_chp[i]]->m_srect, pt)) {
				if (++car->m_chp > m_maxcheckpoints) {
					car->m_kol++;
					car->m_chp = 0;
					if (car->m_kol >= m_run->m_kol) 
						car->m_show = POS_SHOW_STILL;
					else 
						car->m_show = POS_SHOW;
				}
				car->m_pos[car->m_kol][car->m_chp] = m_run->m_pos[car->m_kol][car->m_chp]++;
			}

		}

}

int GMap::GetGround(int x, int y)
{
	if (m_bSys == NULL) return 1;
	int col = getpixel(m_bSys, x/2, y/2);
	int i;
	for (i = 0; i < MAX_GROUNDS; i++)
		if (g_Ground[i].color == col) break;
	if (i == MAX_GROUNDS) i = 1;
	return i;
}

void GMap::DrawBonusy(BITMAP *dst, BITMAP *bns, int cx, int cy)
{
	int x, y;

	for (int i = 0; i < m_bonusu; i++) {
		x = m_bonusy[i].x - cx + (DF_X/2);
		y = m_bonusy[i].y - cy + (DF_Y/2);
		draw_sprite(dst, bns, x-bns->w/2, y-bns->h/2);
	}
}

int GMap::GetMaxStartPos()
{
	int max = 0;
	for (int i = 0; i < m_cobjects; i++)
		if (m_objects[i]->m_par.startpos == 1) max++;
	return max;
}
