// SCar.cpp: implementation of the SCar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdh.h"
#include "GCar.h"
#include "GObject.h"
#include "GMap.h"
#include "GFile.h"
#include "GRun.h"
#include "drivers.h"

extern st_GFile_Driving driving;
extern HFONT   g_font12;

//const double mod_speed[5] = {-0.1, -0.05, 0, 0.05, 0.10};
const double mod_speed[5] = {-0.15, -0.10, -0.05, 0,   0.05};
const double mod_turn[5]  = {   0,      0,     0, 0.2,  0.4};

//////////////////////////////////////////////////////////////////////////////////
//                          INICIALIZACNI FUNKCE
//////////////////////////////////////////////////////////////////////////////////

/*////////////////////////////////////////////////////////////////////////////////
  konstructor - nuluje promene
*/
GCar::GCar()
{
	m_ID = 0;
	m_x = 0;
	m_y = 0;
	m_angle = 0;
	m_speed = 0;
	m_gspeed = 0;
	m_bSprite = NULL;
	m_bShadow = NULL;
	m_bZvyrazneni = NULL;
	m_adriver = -1;

	for (int i = 0; i < 3; i++) 
		m_bKola[i] = NULL;


	m_TSTdistance = 0;
	m_TSTangle    = 0;
	m_kola_zadni  = 0;
	m_kola_predni = 0;
}

/*////////////////////////////////////////////////////////////////////////////////
  destruktor
*/
GCar::~GCar()
{
	Destroy();
}

/*////////////////////////////////////////////////////////////////////////////////
  pri novem zavodu - vymaze vse a nasatvi pozici auta 
  angle :  uhel auta
  x,y   :  pozice na hraci plose
*/
void GCar::Init(int angle, int x, int y, bool autodriver, GMap *map)
{
	int i,j;

	m_map = map;
	m_angle  = angle;
	m_angler = angle;

	m_spriteAngle = angle + 3; // musi byt ruzne od m_angle
	m_shadowAngle = angle + 3; // musi byt ruzne od m_angle
	m_alphaRad = m_angler*DPI/255;
	m_alpha  = m_angle*360/255;

	m_speed = 0;
	m_gspeed = 0;
	m_x     = x;
	m_y     = y;
	m_otacky = 0;

	m_bonusu = 0;

	m_distance = 0;
	m_time = 0;
	m_distancekm = 0; // ujeta vzdalenost km
	m_times = 0;      // cas jizdy do cile sekund
	m_avs = 0;        // prumerna rychlost km/h
	m_max = 0;

	m_TSTdistance = 0;
	m_TSTdistanceNext = 0; // index n mape, kam se chceme dostat
	m_TSTdistanceOld = -1; 
	m_TSTangle = 0;

	m_autodriver = autodriver;
	if (autodriver && (m_adriver == -1)) 
		m_par.turning += 0.9;

	m_stopped = 0;
	m_turninig = 0;

	m_chp = 0;
	m_kol = 0;
	m_fpos = -1;
	m_show = 0;
	for (i = 0; i < MAX_KOL_RES; i++)
		for (j = 0; j < MAX_CHP; j++) 
			m_pos[i][j] = -1;

	for (i = 0; i < 4; i++)
		for (j = 0;j < MAX_KOURE; j++)
			 m_kour[i][j].cas = RYCHLOST_KOURE_ANIM*MAX_SMOGS_BITMAPS+1;

	m_fwd = false;
	m_hbreak = false;
	m_back = false;
	m_left = false;
	m_right = false;
	m_crasch = false;
}

/*////////////////////////////////////////////////////////////////////////////////
  Nastavi pozadavky ridice
  fwd    : zda chce jet vpred
  back   : pro jizdu vzad(brzdu)
  left   : zatoceni wlevo
  right  : zatoceni wpravo
  hbreak : rucni brzda
*/
void GCar::SetDriver(bool fwd, bool back, bool left, bool right, bool hbreak, bool crash)
{
	m_fwd    = fwd;
	m_back   = back;
	m_left   = left;
	m_right  = right;
	m_hbreak = hbreak;
	m_crasch = crash;
}
/*////////////////////////////////////////////////////////////////////////////////
  automaticke rizeni auta
*/
void GCar::AutoDriver(GMap *map)
{
//	driving.circle_size = 70;

	if (m_autodriver == false || m_active == false) return;
	m_gspeed = DF_GAME_SPEED * m_speed;

	char s[50];
	if (map == NULL) return;

	m_left  = false;
	m_right = false;

	int  i = m_TSTdistance;

	LogMessage("-- Autodriver --\n");
	// zda jsme dijeli do kolecka
	if( abs(m_y - map->m_roads[m_par.rt][i]->y) < driving.circle_size && abs(m_x - map->m_roads[m_par.rt][i]->x) < driving.circle_size) {
		m_TSTdistanceOld = m_TSTdistance;
		m_TSTdistance = m_TSTdistanceNext;
	}

	LogMessage("predikce test\n");
	// test na hledani predikce
	if (m_TSTdistance == m_TSTdistanceNext) {
		if (map->m_roads[m_par.rt][m_TSTdistanceNext]->IDA == -1 &&
			map->m_roads[m_par.rt][m_TSTdistanceNext]->IDB == -1) {
			LogMessage("road failed IDA,IDB \n");
			sprintf(s, "ID = %d , kategorie = %d", m_TSTdistance, m_par.kategorie);
			LogMessage(s);
		}

		if (map->GetIndex(m_par.rt, map->m_roads[m_par.rt][m_TSTdistanceNext]->IDA) == -1 &&
			map->GetIndex(m_par.rt, map->m_roads[m_par.rt][m_TSTdistanceNext]->IDB) == -1) {
			LogMessage("road failed GetIndex()\n");
			sprintf(s, "ID = %d , kategorie = %d", m_TSTdistance, m_par.kategorie);
			LogMessage(s);
			exit(1);

		}

		i = -1;
		while (i == -1) {
			if(rand()%2 == 0)
				i = map->GetIndex(m_par.rt, map->m_roads[m_par.rt][m_TSTdistanceNext]->IDA);
			else
				i = map->GetIndex(m_par.rt, map->m_roads[m_par.rt][m_TSTdistanceNext]->IDB);
		}
		m_TSTdistanceNext = i;
		i = m_TSTdistance;
	}


	LogMessage("vzdalenost\n");
	// vzdalenost
	int  dist = pow((pow(abs(m_y - map->m_roads[m_par.rt][i]->y)+0.1,2) + pow(abs(map->m_roads[m_par.rt][i]->x - m_x)+0.1,2)),0.5); // uhel, pod kterym chceme jet
	// potrebny uhel jizdy
	int  TSTangle = atan2(m_y- map->m_roads[m_par.rt][i]->y, map->m_roads[m_par.rt][i]->x - m_x)*256/DPI; // uhel, pod kterym chceme jet
	if (TSTangle < 0) TSTangle += 256;
	// skutecny uhle jizdy
	int angle = (int)(m_angler+m_angle)/2;
	while (angle < 0) angle += 256;
	angle %= 256;

	m_fwd  = true;
	m_back = false;   // jizda vzad (brzda)

	LogMessage("drzeni na silnici\n");
	// aby se auto drzelo na silnici 
	if ((dist > 100) && (m_TSTdistanceOld != -1)) {
		int  Oangle = atan2(map->m_roads[m_par.rt][m_TSTdistanceOld]->y - map->m_roads[m_par.rt][i]->y, map->m_roads[m_par.rt][i]->x - map->m_roads[m_par.rt][m_TSTdistanceOld]->x)*256/DPI; // uhel, pod kterym by mela vest trajektorie
		if (Oangle < 0) Oangle += 256;

		if (abs(TSTangle - Oangle) > 126)
		{
			if (TSTangle > Oangle) Oangle += 256;
			else Oangle -= 256;
		}


		if (abs(TSTangle - Oangle) > 1)
		{
			if (TSTangle > Oangle) TSTangle += 1;
			else TSTangle -= 1;
		}
	}



	LogMessage("zataceni\n");
	double turning = (m_speed)/(2*m_par.max_speed)*(3*m_par.turning);  // aby se auticko netocilo na miste jako tank
	if (turning > m_par.turning) turning = m_par.turning;            // ^
	turning *= DF_GAME_SPEED * 1.1;

	if (abs(TSTangle - angle) > turning) {

		int dl;
		int dr;
		if (TSTangle > angle) {
			dl = TSTangle - angle;
			dr = 256 - dl;
		}
		else {
			dr = angle - TSTangle;
			dl = 256 - dr;
		}

		if (dl < dr) m_left  = true;   // zataceni vlevo
		else m_left = false;

		if (dr < dl) m_right = true;
		else m_right = false;

		// test, zda je treba pustit plyn
		if (m_speed > 1) {
			int n = 256/turning;

			double a = (((n-2)*PI)/n - DPI/n)/2;

			double radius = abs(tan(a)*m_gspeed/2);

			double mangle;
			if (m_left)
				mangle = (m_angler + 64)*DPI/256;
			else
				mangle = (m_angler - 64) *DPI/256;

			double sy = m_y - sin(mangle) * radius;
			double sx = m_x + cos(mangle) * radius;

			double rx = map->m_roads[m_par.rt][i]->x, ry = map->m_roads[m_par.rt][i]->y;

			double dr = pow(pow((sx-rx), 2) + pow((sy-ry), 2), 0.5);
			if (dr+10 < radius) m_fwd = false;

		}
	}
	else {
		m_left = false;
		m_right = false;
		m_hbreak = false;
		m_angler = TSTangle;
		m_angle  = TSTangle;
	}

	
	m_crasch = true;
	LogMessage("hotovo\n\n");
}

/*////////////////////////////////////////////////////////////////////////////////
  Nastavi parametry auta podle ID
*/
void GCar::SetCar(int ID, const char* name, int adriver)
{
	m_ID = ID;
	m_par = Cparams[ID];
	m_poharove = false;
	m_kola_predni = 0;
	m_kola_zadni = 0;

	m_adriver = adriver;
	if (m_adriver >= 0) {
//		m_par.turning = g_driver[m_adriver].turning + mod_turn[DF_OBTIZNOST];
		m_par.max_speed *= g_driver[m_adriver].speed + mod_speed[DF_OBTIZNOST];
	}

	strcpy(m_dname, name);
}

// nastaveni auta podle player - zapocitani upgradu
void GCar::SetCar(st_player *player)
{
	m_ID = player->car;
	m_par = Cparams[m_ID];
	strcpy(m_dname, player->name);
	m_poharove = false;


	m_kola_predni = player->upgrades[2];
	m_kola_zadni = player->upgrades[3];


	// Zlepseni auta podle naupgradovani
	m_par.acc       = GRun::GetUpgradedAcc(m_ID, &player->upgrades[0], player->nacc);
	m_par.breakes   = GRun::GetUpgradedBreaks(m_ID, &player->upgrades[0], player->nbreaks);
	m_par.max_speed = GRun::GetUpgradedSpeed(m_ID, &player->upgrades[0], player->nspeed);
	m_par.turning   = GRun::GetUpgradedTurning(m_ID, &player->upgrades[0], player->nspeed);

	if (player->gpID < -1)
		m_adriver = abs(player->gpID) - 2;

	if (m_adriver >= 0) {
//		m_par.turning += g_driver[m_adriver].turning;
		m_par.max_speed *= g_driver[m_adriver].speed;
	}
}


//////////////////////////////////////////////////////////////////////////////////
//                              GRAFICKE FUNKCE
//////////////////////////////////////////////////////////////////////////////////

/*////////////////////////////////////////////////////////////////////////////////
  Kresli stin auta na danou bitmapu ( zavodni plochu )
  dest - pointer na bitmapu, kam se ma auto nakreslit (velikosti DF_X, DF_X )
  x,y  - souradnice stredu dest vuci trati
*/
void GCar::DrawShadow(BITMAP *dest, int x, int y)
{
	// uprava parametru
	x = m_x - x + (DF_X - DF_CAR_BMPS) / 2;
	y = m_y - y + (DF_Y - DF_CAR_BMPS) / 2;

  // test na platnost parametru
	if (dest == NULL || m_bShadow == NULL) return;
	
	UpdateShadow();
   set_trans_blender(0, 0, 0, 125);
   draw_trans_sprite(dest, m_bShadow, x-SH_X, y+SH_Y);
}

/*////////////////////////////////////////////////////////////////////////////////
  Kresli auto na danou bitmapu ( zavodni plochu )
  dest - pointer na bitmapu, kam se ma auto nakreslit (velikosti DF_X, DF_X )
  x,y  - souradnice stredu dest vuci trati
*/
void GCar::DrawCar(BITMAP *dest, int x, int y, int svetlo)
{

	// uprava parametru
	int dx = m_x - x + (DF_X - DF_CAR_X - 10) / 2;
	int dy = m_y - y + (DF_Y - DF_CAR_Y - 10) / 2;

	// kresleni kol
		if (m_left)
			rotate_sprite(dest, m_bKola[1], dx, dy, itofix(-m_angle+64));
		else 
			if (m_right)
				rotate_sprite(dest, m_bKola[2], dx, dy, itofix(-m_angle+64));
			else
				rotate_sprite(dest, m_bKola[0], dx, dy, itofix(-m_angle+64));


	draw_sprite(dest, m_bSprite, m_x - x + (DF_X - DF_CAR_BMPS) / 2, m_y - y + (DF_Y - DF_CAR_BMPS) / 2);
}

/*////////////////////////////////////////////////////////////////////////////////
  Kresli smog auta na danou bitmapu ( zavodni plochu )
  dest - pointer na bitmapu, kam se ma auto nakreslit (velikosti DF_X, DF_X )
  x,y  - souradnice stredu dest vuci trati
*/
void GCar::DrawSmog(BITMAP *dest, int x, int y)
{
	if (DF_SMOGDISABLE) return;

	for (int i = 0; i < 4; i++)
		for (int j = 0;j < MAX_KOURE; j++) {
			if (m_kour[i][j].cas/RYCHLOST_KOURE_ANIM >= MAX_SMOGS_BITMAPS) continue;

			// uprava parametru
			int dx = m_kour[i][j].x - x + (DF_X)/2 - 16 + rand()%4 - 2;
			int dy = m_kour[i][j].y - y + (DF_Y)/2 - 16 + rand()%4 - 2;

//			draw_sprite(dest, b_smogs[0], dx, dy);
			hmasked_spritetcm(dest, b_smogsC[m_kour[i][j].typ], b_smogs[m_kour[i][j].cas/RYCHLOST_KOURE_ANIM], dx, dy);
//			hmasked_spritetcm(dest, b_smogsC[0], b_smogs[0], dx, dy);
			m_kour[i][j].cas++;
		}
}

/*////////////////////////////////////////////////////////////////////////////////
  Refreshne m_bSprite
*/
void GCar::UpdateSprite()
{
	if (m_spriteAngle !=  m_angle) {
		m_spriteAngle = m_angle;
		clear_to_color(m_bSprite, makecol(255, 0, 255)); // ruzova je barva nase :)

	// kresleni karoserie
		if (m_poharove == false)
			if (m_svetlo == 1) 
				rotate_sprite(m_bSprite, b_cars[m_ID*3+1], 30, 10, itofix(-m_angle+64));
			else
				rotate_sprite(m_bSprite, b_cars[m_ID*3], 30, 10, itofix(-m_angle+64));
		else
			if (m_svetlo == 1) 
				rotate_sprite(m_bSprite, b_carsPohar[m_par.poharb+1], 30, 10, itofix(-m_angle+64));
			else
				rotate_sprite(m_bSprite, b_carsPohar[m_par.poharb], 30, 10, itofix(-m_angle+64));				
	}

}

/*////////////////////////////////////////////////////////////////////////////////
  Pripravi grafika data
*/
bool GCar::LoadGFX(bool svetlo)
{
	Destroy();

	ASSERT(m_bSprite == NULL);
	m_bSprite = create_bitmap(DF_CAR_BMPS, DF_CAR_BMPS);
	if (m_bSprite == NULL) return false;

	ASSERT(m_bShadow == NULL);
	m_bShadow = create_bitmap(DF_CAR_BMPS, DF_CAR_BMPS);
	if (m_bShadow == NULL) return false;

	if (m_autodriver == false)
		CreateObrys();
	 

	m_svetlo = svetlo;

	int k = m_kola_zadni;
	for (int i = 0; i < 3; i++) {
		ASSERT(m_bKola[i] == NULL);
		m_bKola[i] = create_bitmap(DF_CAR_X + 10, DF_CAR_Y + 10);
		clear_to_color(m_bKola[i], makecol(255, 0, 255)); // ruzova je barva nase :)
		draw_sprite(m_bKola[i], b_kola[k], 5+m_par.kola[2].x - b_kola[k]->w/2, m_par.kola[2].y - b_kola[k]->h/2);
		draw_sprite(m_bKola[i], b_kola[k], 5+m_par.kola[3].x - b_kola[k]->w/2, m_par.kola[3].y - b_kola[k]->h/2);
	}

	k = m_kola_predni;
	draw_sprite(m_bKola[0], b_kola[k], 5+m_par.kola[0].x-b_kola[k]->w/2, 5+m_par.kola[0].y-b_kola[k]->h/2);
	draw_sprite(m_bKola[0], b_kola[k], 5+m_par.kola[1].x-b_kola[k]->w/2, 5+m_par.kola[1].y-b_kola[k]->h/2);

	rotate_sprite(m_bKola[2], b_kola[k], 5+m_par.kola[0].x-b_kola[k]->w/2, 5+m_par.kola[0].y-b_kola[k]->h/2, itofix(16));
	rotate_sprite(m_bKola[2], b_kola[k], 5+m_par.kola[1].x-b_kola[k]->w/2, 5+m_par.kola[1].y-b_kola[k]->h/2, itofix(16));

	rotate_sprite(m_bKola[1], b_kola[k], 5+m_par.kola[0].x-b_kola[k]->w/2, 5+m_par.kola[0].y-b_kola[k]->h/2, itofix(-16));
	rotate_sprite(m_bKola[1], b_kola[k], 5+m_par.kola[1].x-b_kola[k]->w/2, 5+m_par.kola[1].y-b_kola[k]->h/2, itofix(-16));

	CountRect();
	UpdateShadow();
	m_rold[0] = m_rectkol[0]; m_rold[1] = m_rectkol[1];
	m_rold[2] = m_rectkol[2]; m_rold[3] = m_rectkol[3];

	char c[100];
	sprintf(c, "MOTOR%d", m_par.zvuk);
	rload_snd(c, &m_smotor);
	rload_snd("SMYK", &m_ssmyk);

	return true;
}
/*////////////////////////////////////////////////////////////////////////////////
  Znici grafika data
*/
void GCar::Destroy()
{
	if(m_bSprite != NULL) {
		destroy_bitmap(m_bSprite);
		m_bSprite = NULL;
	}
	if(m_bShadow != NULL) {
		destroy_bitmap(m_bShadow);
		m_bShadow = NULL;
	}
	if(m_bZvyrazneni != NULL) {
		destroy_bitmap(m_bZvyrazneni);
		m_bZvyrazneni = NULL;
	}
	for (int i = 0; i < 3; i++) 
		if (m_bKola[i] != NULL) {
			destroy_bitmap(m_bKola[i]);
			m_bKola[i] = NULL;
		}

	m_smotor.Release();
	m_ssmyk.Release();
}


//////////////////////////////////////////////////////////////////////////////////
//                              POHYBOVE FUNKCE
//////////////////////////////////////////////////////////////////////////////////

/*////////////////////////////////////////////////////////////////////////////////
   Udela Pohyb 
*/
void GCar::Move(GMap *map)
{
	LogMessage("\nPohyb auta\n");

	if (m_active == false) {
		LogMessage("Neaktivni auto\n");
		return;
	}


	int i,j;

	if (m_crasch && m_stopped > STOPPED_LIMIT) {
		LogMessage("Restore auta\n");

		map->RestoreCar(this);
//		m_stopped = 0;
	}

	LogMessage("\nTestChpointa\n");
	map->TestChpoint(this);

	if (m_hbreak)  	
		m_back = m_fwd = false;

	LogMessage("\nZataceni test\n");
	// zataceni ve smer vpred
	double turning = (m_speed)/(2*m_par.max_speed)*(3*m_par.turning);  // aby se auticko netocilo na miste jako tank
	if (turning > m_par.turning) turning = m_par.turning;            // ^
	if(m_hbreak) turning *= 2; // s rucni brdou se to toci lip :)
	if (!m_fwd) turning *= 1.5; // kdyz jedes jen tak, brzdis nebo pozpatku lip se ti zataci
	turning *= DF_GAME_SPEED * 1.1; // upravime zataceni podle rychosti hry
	if (m_speed > 0.2) {
		if (m_left)  m_angle += turning;
		if (m_right) m_angle -= turning;
	}

	// zataceni ve smer vzad
	if (m_speed < -0.2) {
		if (m_left)  m_angle += turning;
		if (m_right) m_angle -= turning;
	}


  // zmena realneho uhlu
  //TODO: pridat vlastnosti povrch ( klouzani )
	double tearing = 1; //m_par.tearing; // * klouzavost_podkladu;  = (0..1> 0 - totalni klouzavost, 1 - totalni prilnavost
	if (m_hbreak) tearing = tearing / 10;
	m_angler = (m_angler * (1-tearing)) + (m_angle * tearing);

  // omezeni, aby m_angler nebylo od m_angle prilis rozdilne (cca 110 stupnu)
	if (m_angle > m_angler + 80) { m_angler = m_angle - 80; m_speed = 0; }
	if (m_angle < m_angler - 80) { m_angler = m_angle + 80; m_speed = 0; }

  // rucni brzda
	if (m_hbreak) { 
		if (m_left || m_right) 
			// m_speed *= m_par.hbreaks;
			m_fwd = false;
		else m_speed *= m_par.hbreaks;
		if(abs(m_speed) < 10) m_speed = 0;
	}


	double speed_koef = 0.25 * (g_Ground[m_ground[0]].slow + g_Ground[m_ground[1]].slow +
		                         g_Ground[m_ground[2]].slow + g_Ground[m_ground[3]].slow);


/*	// omezovac rych
	if (m_speed > m_par.max_speed) m_speed = m_par.max_speed;
	if (m_speed < m_par.max_speedB) m_speed = m_par.max_speedB;
*/
	LogMessage("\nZmena rychlosti\n");
	// zmena rychlosti - vpred
   if (m_fwd) 
	{
		if (m_speed >= 0) {
			double max_speed = m_par.max_speed*speed_koef;
			if (m_speed < max_speed) 
			{
				// m_speed += m_par.acc * (1-pow(m_speed,0.2)/pow(m_par.max_speed,0.2));
				double speed_delta = m_par.acc * (1-pow(m_speed,0.2)/pow(0.94*m_par.max_speed,0.2));
				if (speed_delta + m_speed < max_speed) m_speed += speed_delta;
				else m_speed = max_speed;
			}
			else m_speed *= m_par.autoslow*m_par.autoslow; //m_par.breakes*m_par.breakes;
		}
		else {
/*			if (m_speed > -6) m_speed *= m_par.breakes;
			if (m_speed > -4) m_speed *= m_par.breakes;
			if (m_speed > -3) m_speed *= m_par.breakes * m_par.breakes;
			if (m_speed > -2) m_speed *= m_par.breakes * m_par.breakes * m_par.breakes;
			if (m_speed > -1) m_speed *= m_par.breakes * m_par.breakes * m_par.breakes;*/
			if ((m_speed *= m_par.breakes) > -6) m_speed = 0;
		}
	 }
	// zmena rychlosti - vzad
   if (m_back) 
   {
		if (m_speed <= 0) {
			if (m_speed > m_par.max_speedB*speed_koef) 
				m_speed -= m_par.acc * (1-(m_speed / m_par.max_speedB));
			else m_speed *= m_par.autoslow*m_par.autoslow;
		}
		else {
/*			if (m_speed < 6) m_speed *= m_par.breakes;
			if (m_speed < 3) m_speed *= m_par.breakes * m_par.breakes;
			if (m_speed < 2) m_speed *= m_par.breakes * m_par.breakes * m_par.breakes;
			if (m_speed < 1) m_speed *= m_par.breakes * m_par.breakes * m_par.breakes;*/
			if ((m_speed *= m_par.breakes*m_par.breakes*m_par.breakes) < 6) m_speed = 0;
		}
    }

  // kdyz neni plyn, tak auto mlinko samo zpomaluje
	if (!m_back && !m_fwd){
		m_speed *= m_par.autoslow;
	}
	if ((m_left || m_right) && !m_fwd) m_speed *= m_par.autoslow;

	m_gspeed = DF_GAME_SPEED * m_speed;


	// dopocitani m_alpha
	if (m_angle > 256) m_angle -= 256;
	if (m_angler > 256) m_angler -= 256;
	if (m_angle < 0) m_angle += 256;
	if (m_angler < 0) m_angler += 256;
	m_alpha    = m_angle*360/255;
	m_alphaRad = m_angler*DPI/255;

	if (m_left) if (m_turninig < 0) m_turninig = 0;
	            else m_turninig++;
	else if (m_right) if (m_turninig > 0) m_turninig = 0;
	                  else m_turninig--;
		  else m_turninig = 0;
	// vypocet hranicniho obdelniku
	CountRect();


	// vlastni posum souradnic
	double angleRad = m_angler*DPI/256;
	double cang = cos(angleRad);
	double sang = sin(angleRad);


	for(i = 0; abs(i) < abs(m_gspeed)+2; (m_speed > 0)?i++:i--) {
		int tm;
		if ((tm = map->TestMove(this, cang*i, -sang*i)) != 0) {
			ZkusOdrazit(tm, map);
//			m_speed = 0;
			break;
		}
	}

	if (m_speed != 0) {
		m_x += cang * m_gspeed;
		m_y -= sang * m_gspeed;
		m_stopped = 0;
	}
	else {
		m_stopped++;
	}

	if (m_x > map->m_sx-50) m_x = map->m_sx-50;
	if (m_x < 50) m_x = 50;
	if (m_y > map->m_sy-50) m_y = map->m_sy-50;
	if (m_y < 50) m_y = 50;

	LogMessage("\nDopocitani smyku,...\n");
	// vypocet otacek
	if (m_speed > 0 && m_fwd) {
		double dt = m_par.max_speed/m_par.razeni;
		while (m_speed > dt) dt += dt;
		m_otacky = 0.8 * m_otacky + 0.2*m_speed/dt;
	}
	else if (m_speed < 0 && m_back) {
				m_otacky = 0.9*m_otacky + 0.1*m_speed/m_par.max_speedB;
			}
			else m_otacky = 0.95 * m_otacky;

	// kresleni smyku
	CountRect();
	if (m_stopped == 0 && !DF_SMYKDISABLE && m_speed != 0) {

		int sid;
		int dm = 3;
		for (i = 1; i < 3; i++) {
			double dcang = dm*(m_rold[i].x - m_rectkol[i].x)/m_speed;
			double dsang = dm*(m_rold[i].y - m_rectkol[i].y)/m_speed;

			sid = 1;
			if (abs(m_turninig) > 20 && (m_fwd || m_speed > m_par.max_speed/2)) sid = 10;
			if (m_hbreak && (i == 1 || i == 2)) sid = 10;
			if (abs(m_speed) < m_par.max_speed/3 && m_fwd && abs(m_speed) > 0) sid = 10;
//			if (!m_fwd) sid = 10;
			double sx = m_rold[i].x;
			double sy = m_rold[i].y;

			for (int ii = 0; ii <= abs(m_gspeed/3)+1; ii++) {
				int gr = map->GetGround(sx, sy) + sid;
				if (b_smyk[gr] != NULL) {
				//	sid = 0;
					set_trans_blender(0, 0, 0, 50);
					draw_trans_sprite(map->m_bGround, b_smyk[gr], sx-b_smyk[gr]->w/2, sy-b_smyk[gr]->h/2);
				}
				sx += dcang;
				sy += dsang;
			}
		}
	}
	m_rold[0] = m_rectkol[0]; m_rold[1] = m_rectkol[1];
	m_rold[2] = m_rectkol[2]; m_rold[3] = m_rectkol[3];

	// kour
	if (!DF_SMOGDISABLE) {
		for (i = 0; i < 4; i++)  {
			int typ = -1;
			if (abs(m_speed) > 3 && m_ground[i] == 4) typ = 0; // prasna cesta
			if (abs(m_speed) > 3 && m_ground[i] == 6) typ = 1; // pisek
			if (abs(m_speed) > 3 && m_ground[i] == 5) typ = 3; // snih
			if (m_speed < m_par.max_speed/3 && m_fwd && m_speed > 0 &&
				( (m_par.nahon_predni && (i == 0 || i==3)) || 
				  (m_par.nahon_zadni &&  (i == 2 || i==1)) )) typ = 2; // cerny kour

			if (typ == -1) continue;

			for (j = 0; j < MAX_KOURE; j++) 
				if (m_kour[i][j].cas/RYCHLOST_KOURE_ANIM >= MAX_SMOGS_BITMAPS) break;
			if (j != MAX_KOURE) {
				m_kour[i][j].cas = 0;
				m_kour[i][j].x   = m_rectkol[i].x;
				m_kour[i][j].y   = m_rectkol[i].y;
				m_kour[i][j].typ = typ;
			}
		}
	}


	// vypocet casu a drahy
	if (m_show != POS_SHOW_STILL) {
		m_distance += m_gspeed;
		m_time += 1;

		if (m_speed > m_max) 
			m_max = m_speed;

		m_times = m_time * 0.04 * DF_GAME_SPEED;
		m_avs = (m_distance*10)/(m_time*DF_GAME_SPEED);        // prumerna rychlost km/h
		m_distancekm = m_distance*(0.04/360); //m_distance * 0.00001;
	}
}


//////////////////////////////////////////////////////////////////////////////////
//                              NARAZOVE FUNKCE
//////////////////////////////////////////////////////////////////////////////////

/*////////////////////////////////////////////////////////////////////////////////
  Prepocita strukturu m_rect, ktera obsahuje vrcholy obdelniku auta
*/
void GCar::CountRect()
{
	UpdateSprite();

  // vypleni m_rect
	m_rect[0].x = m_x + cos((m_alpha + BETA)*DPI/360) * D_CON;
	m_rect[0].y = m_y - sin((m_alpha + BETA)*DPI/360) * D_CON;

	m_rect[1].x = m_x + cos((m_alpha - BETA + 180)*DPI/360) * D_CON;
	m_rect[1].y = m_y - sin((m_alpha - BETA + 180)*DPI/360) * D_CON;

	m_rect[2].x = m_x + cos((m_alpha + BETA + 180)*DPI/360) * D_CON;
	m_rect[2].y = m_y - sin((m_alpha + BETA + 180)*DPI/360) * D_CON;

	m_rect[3].x = m_x + cos((m_alpha - BETA)*DPI/360) * D_CON;
	m_rect[3].y = m_y - sin((m_alpha - BETA)*DPI/360) * D_CON;

	m_rectkol[0].x = m_x + cos((m_alpha + BETA)*DPI/360) * D_KOLA;
	m_rectkol[0].y = m_y - sin((m_alpha + BETA)*DPI/360) * D_KOLA;

	m_rectkol[1].x = m_x + cos((m_alpha - BETA + 180)*DPI/360) * D_KOLA;
	m_rectkol[1].y = m_y - sin((m_alpha - BETA + 180)*DPI/360) * D_KOLA;

	m_rectkol[2].x = m_x + cos((m_alpha + BETA + 180)*DPI/360) * D_KOLA;
	m_rectkol[2].y = m_y - sin((m_alpha + BETA + 180)*DPI/360) * D_KOLA;

	m_rectkol[3].x = m_x + cos((m_alpha - BETA)*DPI/360) * D_KOLA;
	m_rectkol[3].y = m_y - sin((m_alpha - BETA)*DPI/360) * D_KOLA;
	

	m_rect2[0].x = (m_rect[0].x + m_rect[1].x)/2;
	m_rect2[0].y = (m_rect[0].y + m_rect[1].y)/2;

	m_rect2[1].x = (m_rect[2].x + m_rect[1].x)/2;
	m_rect2[1].y = (m_rect[2].y + m_rect[1].y)/2;

	m_rect2[2].x = (m_rect[2].x + m_rect[3].x)/2;
	m_rect2[2].y = (m_rect[2].y + m_rect[3].y)/2;

	m_rect2[3].x = (m_rect[0].x + m_rect[3].x)/2;
	m_rect2[3].y = (m_rect[0].y + m_rect[3].y)/2;

// vyplneni m_recth
	m_recth.right = m_recth.left = m_x;
	m_recth.top = m_recth.bottom = m_y;

	for (int i = 0; i < 4; i++) {
		if (m_rect[i].x > m_recth.right) 
			m_recth.right = m_rect[i].x;

		if (m_rect[i].y < m_recth.top)
			m_recth.top = m_rect[i].y;

		if (m_rect[i].x < m_recth.left) 
			m_recth.left = m_rect[i].x;

		if (m_rect[i].y > m_recth.bottom)
			m_recth.bottom = m_rect[i].y;

		m_ground[i] = m_map->GetGround(m_rect[i].x, m_rect[i].y);
	}
}

/*////////////////////////////////////////////////////////////////////////////////
  Zda je bod v aute ( naraz )
  pt : souradnice testovaneo bodu
*/
bool GCar::PtInRect(POINT pt)
{
	return PtInRect(pt.x, pt.y);
}

/*////////////////////////////////////////////////////////////////////////////////
  Zda je bod v aute ( naraz )
  x,y : souradnice testovaneo bodu
*/
bool GCar::PtInRect(int x, int y)
{
  // test na m_recth ( test nahrubo )
	if (x > m_recth.right || x < m_recth.left ||
		 y > m_recth.bottom || y < m_recth.top) return false;

  // test na naraz
	x -= m_x - DF_CAR_BMPS/2;
	y -= m_y - DF_CAR_BMPS/2;

	int pix = getpixel(m_bSprite,x,y);

	if (pix == -1) return false;
	if (pix != makecol(255, 0, 255)) return true;

	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
  Nastavuje promenou m_eagle a m_alpha.
  alpha : uhel VE STUPNICH (0-360) !!!
*/
void GCar::SetEagle(double alpha)
{
	m_alpha = alpha;
	m_angle = 256 * alpha / 360;
}

/*////////////////////////////////////////////////////////////////////////////////
  Testuje naraz do daneho objeku a upravuje parametry auta
  ob  :  ukazatel na testovany objekt
*/
bool GCar::Crash(GObject *ob)
{
	if ((ob->m_par.type == OBJECT_FLYING) || (ob->m_par.type == OBJECT_NONE) || (ob->m_par.type == OBJ_HI_LAY) || (ob->m_par.type == OBJ_HI_GROUNG)) return false; // do litaciho se auto netrefi

	if (!Hit(ob) && !ob->Hit(this)) return false; // dopadlo to dobre
	if (m_speed == 0) return false;

	if (ob->m_par.type == OBJECT_SOFT) {  // mozna neco rozjelo
		if (!ob->m_smashed) 
			ob->Smashed(this);
		return false;  // tohle se nepocita za naraz :)
	}


	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
  Testuje naraz do jineho auta a meni parametry
  car  :  ukazatel na druhe auto
*/
bool GCar::Hit(GObject *ob)
{
	if (PtInRect(ob->m_rect[0]) || PtInRect(ob->m_rect[1]) ||
		 PtInRect(ob->m_rect[2]) || PtInRect(ob->m_rect[3])) return true;
//	if (ob->Hit(this)) return true;
	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
  testuje, zda je auto narazilo do jineho
  car  :  ukazatel na testovany objekt
*/
bool GCar::Crash(GMap *map, GCar *car)
{
	if (m_active == false || car->m_active == false) return false;
	if (!Hit(car) && !car->Hit(this)) return false;

	if (car->m_speed == 0) car->m_speed = 0.1;
	if (m_speed == 0) m_speed = 0.1;
	int dx,dy;

	int pok = 20;
	while (Hit(car) || car->Hit(this)) {

		if(--pok < 0) break;

		if (m_x > car->m_x) dx = 1;
		else dx = -1;

		if (m_y > car->m_y) dy = 1;
		else dy = -1;

		if (map->TestMove(this, dx, dy) == 0) {
			m_y += dy;
			m_x += dx;
		}

		if (map->TestMove(car, -dx, -dy) == 0) {
			car->m_y -= dy;
			car->m_x -= dx;
		}

		CountRect();
		car->CountRect();
	}

	//TODO: dodelat zmeny rychlosti pri narazeni aut do sebe


	return true;
}

/*////////////////////////////////////////////////////////////////////////////////
  testuje, zda je auta srazla
  car  :  ukazatel na druhe auto
*/
bool GCar::Hit(GCar *car)
{
	if(PtInRect(car->m_rect[0]) ||
		PtInRect(car->m_rect[1]) ||
		PtInRect(car->m_rect[2]) ||
		PtInRect(car->m_rect[3]) ||
		PtInRect(car->m_rect2[0]) ||
		PtInRect(car->m_rect2[1]) ||
		PtInRect(car->m_rect2[2]) ||
		PtInRect(car->m_rect2[3])) return true;
	return false;
}

void GCar::UpdateShadow()
{
	if (DF_SHADOWDISABLE) return;
	if (m_bShadow == NULL || b_cars[m_ID*3+2] == NULL) return;

	if (m_shadowAngle != m_angle) {
		clear_to_color(m_bShadow, makecol(255,0,255)); 
		rotate_sprite(m_bShadow, b_cars[m_ID*3+2], 30, 10, itofix(-m_angle+64));
		m_shadowAngle = m_angle; 
	}

}



void GCar::DrawLight(BITMAP *lmap, int x, int y)
{
	double angleRad = (m_angle)*DPI/256; //m_angler*DPI/256;

	x = m_x - x + (DF_X) / 2;
	y = m_y - y + (DF_Y) / 2;
	clear(b_carlights[0]);
	rotate_sprite(b_carlights[0], b_carlights[m_par.light], 80-b_carlights[m_par.light]->w/2, 80-b_carlights[m_par.light]->h/2, itofix(-m_angle+64));

	hdraw_light_spritetcm(lmap, b_carlights[0], x-80+cos(angleRad)*100, y-80-sin(angleRad)*100);
}


void GCar::UpdateSound(int x, int y, int dst)
{
	if (!m_smotor.IsPlaying()) m_smotor.Play(true);



	m_smotor.SetVolume(0-dst*3);
	int freq = m_smotor.m_freqency_original;
	m_smotor.SetFreq(freq/2 + (int)(m_otacky*(double)freq));
	m_smotor.SetPan((x-m_x)*3);
	
	if ((abs(m_turninig) > 20 && (m_fwd || m_speed > m_par.max_speed/2)) ||
	 	 (m_hbreak && m_speed != 0) ||
		 (m_speed < m_par.max_speed/3 && m_fwd && m_speed > 0)) {
		if (!m_ssmyk.IsPlaying()) m_ssmyk.Play(true);
	}
	else m_ssmyk.Stop();


	m_ssmyk.SetVolume(0-dst*3);
	m_ssmyk.SetPan((x-m_x)*3);

}

void GCar::StopSound()
{
	m_smotor.Stop();
	m_ssmyk.Stop();
}


void GCar::CreateObrys()
{
	ASSERT(m_bZvyrazneni == NULL);
	m_bZvyrazneni = create_bitmap(DF_CAR_BMPS, DF_CAR_BMPS);

	int col  = makecol(255, 0, 255);
	int col2 = makecol(255, 255, 0);
	int col3 = makecol(0, 0, 0);
	int i,j;

	clear_to_color(m_bZvyrazneni, col);

	for (i = -1; i < DF_CAR_X+1; i++)
		for (j = -1; j < DF_CAR_Y+1; j++) 
			if (!IsDiffColl(i, j) && (
				 IsDiffColl(i+1,j+1) ||
				 IsDiffColl(i+1,j  ) ||
				 IsDiffColl(i+1,j-1) ||
				 IsDiffColl(i  ,j+1) ||
				 IsDiffColl(i  ,j-1) ||
				 IsDiffColl(i-1,j-1) ||
				 IsDiffColl(i-1,j+1) ||
				 IsDiffColl(i-1,j  ) 
				)) 
				
				putpixel(m_bZvyrazneni, i+30, j+10, col2);

	for (i = 0; i < DF_CAR_BMPS; i++)
		for (j = 0; j < DF_CAR_BMPS; j++) 
			if (getpixel(m_bZvyrazneni, i, j) == col && (
				 getpixel(m_bZvyrazneni, i+1, j+1) == col2 ||
				 getpixel(m_bZvyrazneni, i+1, j  ) == col2 ||
				 getpixel(m_bZvyrazneni, i+1, j-1) == col2 ||
				 getpixel(m_bZvyrazneni, i,   j+1) == col2 ||
				 getpixel(m_bZvyrazneni, i,   j  ) == col2 ||
				 getpixel(m_bZvyrazneni, i-1, j-1) == col2 ||
				 getpixel(m_bZvyrazneni, i-1, j  ) == col2 ||
				 getpixel(m_bZvyrazneni, i-1, j+1) == col2 
				)) 
				
				putpixel(m_bZvyrazneni, i, j, col3);

}

void GCar::DrawZvyrazneni(BITMAP *dest, int x, int y)
{
	// uprava parametru
	if (m_bZvyrazneni != NULL) {
		rotate_sprite(dest, m_bZvyrazneni, m_x - x + (DF_X - DF_CAR_BMPS) / 2, m_y - y + (DF_Y - DF_CAR_BMPS) / 2, itofix(-m_angle+64));
		ChangeZvyrazneni();
	}
	else
		CreateObrys();
}

bool GCar::IsDiffColl(int x, int y)
{
	int col  = makecol(255, 0, 255);
	int col2 = getpixel(b_cars[m_ID*3], x, y);
	if (col2 == -1 || col2 == col) return false;
	return true;
}

void GCar::ChangeZvyrazneni()
{
	int i,j;
	int col   = makecol(255, 0, 255);
	int col2  = makecol(0, 0, 0);
	int col3  = makecol(rand()%256, rand()%256, rand()%256);

	while (col3 == col2 || col3 == col) col3  = makecol(rand()%256, rand()%256, rand()%256);

	for (i = 0; i < DF_CAR_BMPS; i++)
		for (j = 0; j < DF_CAR_BMPS; j++) {
			int gp = getpixel(m_bZvyrazneni, i, j);
			if (gp != col && gp != col2)
				putpixel(m_bZvyrazneni, i, j, col3);
		}

}

void GCar::ZkusOdrazit(int tm, GMap *map)
{
	double mangle = 0;

	// snizeni rychlosti

	m_speed *= 0.7;
	// odhozeni doleva
	if (tm & NARAZ_ODHODVLEVO) {
		mangle = m_angler + 64;
	}

	// odrazeni doprava
	if (tm & NARAZ_ODHODVPRAVO) {
		mangle = m_angler - 64;

	}

	// otoceni vpravo
	if (tm & NARAZ_OTOCVPRAVO) {
		m_angler -= 5;

	}

	// otoceni vlevo
	if (tm & NARAZ_OTOCVLEVO) {
		m_angler += 5;

	}

	double angleRad = mangle*DPI/256;

	m_y -= sin(angleRad) * 5;
	m_x += cos(angleRad) * 5;


	// dopocitani m_alpha
	if (m_angler > 256) m_angler -= 256;
	if (m_angler < 0) m_angler += 256;
	m_alphaRad = m_angler*DPI/255;
	m_angle = m_angler;

	// vypocet hranicniho obdelniku
	CountRect();

	if (map->TestMove(this, 0, 0) != 0) {
		// nepodarilo se -> undo pohybu
	   if (tm & NARAZ_OTOCVPRAVO) m_angler += 5;
		if (tm & NARAZ_OTOCVLEVO)  m_angler -= 5;

		m_y += sin(angleRad) * 5;
		m_x -= cos(angleRad) * 5;

		if (m_angler > 256) m_angler -= 256;
		if (m_angler < 0) m_angler += 256;
		m_alphaRad = m_angler*DPI/255;
		m_angle = m_angler;

		// vypocet hranicniho obdelniku
		CountRect();

		m_speed  = 0;
		m_gspeed = 0;
		return;
	}


	//zkusiu se novy posun
	angleRad = m_angler*DPI/256;
	double cang = cos(angleRad);
	double sang = sin(angleRad);


	for(int i = 0; abs(i) < abs(m_speed*0.9)+2; (m_speed > 0)?i++:i--) {
		int tm;
		if ((tm = map->TestMove(this, cang*i, -sang*i)) != 0) {
			m_speed = 0;
			break;
		}
	}
	m_gspeed = DF_GAME_SPEED * m_speed;
}

void GCar::DrawDBG(GRun *run, BITMAP *dest, int x, int y)
{
	return;

	x -= DF_X/2;
	y -= DF_Y/2;

	GMap *map = &run->m_map;

	if (m_autodriver == false || m_active == false) return;

	if (map == NULL) return;

	int  i = m_TSTdistance;


	LogMessage("zataceni\n");
	double turning = (m_speed)/(2*m_par.max_speed)*(3*m_par.turning);  // aby se auticko netocilo na miste jako tank
	if (turning > m_par.turning) turning = m_par.turning;            // ^

			int n = 256/turning;

			double a = (((n-2)*PI)/n - DPI/n)/2;

			double radius = abs(tan(a)*m_speed/2);

			double mangle;
			if (m_left)
				mangle = (m_angler + 64)*DPI/256;
			else
				mangle = (m_angler - 64) *DPI/256;

			
			double sy = m_y - sin(mangle) * radius;
			double sx = m_x + cos(mangle) * radius;

			double rx = map->m_roads[m_par.rt][i]->x, ry = map->m_roads[m_par.rt][i]->y;

			double dr = pow(pow((sx-rx), 2) + pow((sy-ry), 2), 0.5);
			if (dr < radius) m_fwd = false;

			int col;
			if (m_fwd) col = makecol(  0, 255, 255);
			else col = makecol( 255, 0, 0);

			line(dest, sx-x, sy-y, m_x-x, m_y-y, col);
			line(dest, rx-x, ry-y, m_x-x, m_y-y, col);
			line(dest, rx-x, ry-y, sx-x,  sy-y, col);
			circle(dest, sx-x, sy-y, radius, col);
			circle(dest, rx-x, ry-y, driving.circle_size, col);

			textprintf_ex(dest, font, 50, 400, makecol(  0, 255, 255), makecol(0, 0, 0), "radius = %.2f", radius);
			textprintf_ex(dest, font, 50, 415, makecol(  0, 255, 255), makecol(0, 0, 0), "turning = %.2f", turning);
			textprintf_ex(dest, font, 50, 430, makecol(  0, 255, 255), makecol(0, 0, 0), "n = %d", n);
			textprintf_ex(dest, font, 50, 445, makecol(  0, 255, 255), makecol(0, 0, 0), "a = %.2f", a);
/*		}
	}
	else {
		m_left = false;
		m_right = false;
		m_hbreak = false;
		m_angler = TSTangle;
		m_angle  = TSTangle;
	}
*/
	
	m_crasch = true;
	LogMessage("hotovo\n\n");
}

void GCar::DrawName(BITMAP *dest, int x, int y)
{
	if (m_autodriver)
		TTDraw(dest, m_x - x + DF_X/ 2, m_y - y + (DF_Y-DF_CAR_Y)/2 - 5, WIN_RGB(255, 255, 255), m_dname, g_font12, 0); 
	else
		TTDraw(dest, m_x - x + DF_X/ 2, m_y - y + (DF_Y-DF_CAR_Y)/2 - 5, WIN_RGB(255, 255, 0), m_dname, g_font12, 0); 
}
