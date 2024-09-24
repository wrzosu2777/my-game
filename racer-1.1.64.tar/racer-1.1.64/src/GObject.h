// GObject.h: interface for the GObject class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _G_OBJECT_INCLUDE_
#define _G_OBJECT_INCLUDE_

#include "GClasses.h"
#include "objects.h"

class GObject  
{
public:
	void StopSound();
	void UpdateSound(int x, int y, int dst);
	void DrawShadow(BITMAP *dest, int x, int y, int hi);
	void DrawLight(BITMAP *lmap, int CX, int CY, int hi);
	void LoadGFX();
	void Destroy();
	void UpdateTmp();
	bool Hit(GCar *car);
	void ComputeRect();
	void Smashed(GCar *car);
	void Move(GMap *map);
	void Init(int x, int y, int ID);
	void Draw(BITMAP *dest, int x, int y, int hi, bool shadow);
	GObject();
	virtual ~GObject();

	MYRECT m_rect;  // obdelnik, ktery je diky tomuto objektu neprujezdny;
	RECT   m_srect; // standartni obdelnik
	bool   m_smashed;  // je-li objekt typu OBJECT_SOFT, tak m_smashed urcuje, zda je objekt rozjety 
	int    m_krev;     // index krve pri animaci 

	double  m_x;     // x-ova souradnice stredu 
	double  m_y;     // y-ova souradni ce stredu

	double  m_dx;      // je-li objekt flying, tak tohle urcuje, kam poleti
	double  m_dy;      // je-li objekt flying, tak tohle urcuje, kam poleti
	double  m_ralpha;  // uhel smeru pohybu v radianech

	double  m_angle; // uhel otoceni ( pouziva se u OBJECT_SOFT a OBJECT_FLYING )
	double  m_angles;
	double  m_turning;
	double  m_speed;

	int     m_anim; // animace

	BITMAP *m_btmp;
	int     m_tmpx;
	int     m_tmpa;

	int     m_hi; // pro kresleni

	HDSoundBuffer m_sound;  // zvuk objektu
	HDSoundBuffer m_soundSmash;  // zvuk objektu
	int           m_toplay; // kolik zbyva k dalsimu prehrani


	st_GObjects_params   m_par;
	int                  m_ID;
private:
};

#endif // #ifndef _G_OBJECT_INCLUDE_

