// GMap.h: interface for the GMap class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _G_MAP_INCLUDE_
#define _G_MAP_INCLUDE_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GClasses.h"
#include "file_type.h"

enum {
	TYPE_SMALL = 0,
	TYPE_LARGE
};

#define MAX_CHP  10

#define MAX_BONUSU_ON_MAP 100


#define MAX_GROUNDS 8
typedef struct {
	int    color; // barva pokladu
	double slow;  // zpomaleni
	int    bmp;   // bitmapa -1 == invalid
} st_ground;
extern st_ground g_Ground[MAX_GROUNDS];



class GMap  // obasahuje kompletni mapu cele hry !!
{
public:
	int GetMaxStartPos();
	void DrawBonusy(BITMAP *dst, BITMAP *bns, int cx, int cy);
	int GetGround(int x, int y);
	void TestChpoint(GCar *car);
	bool GetStartPos(int id, int *x, int *y, int *angle);
	void DrawLights(BITMAP *lmap, int Cx, int Cy);
	bool IsObjIndexInMap(int INDEX);
	void RestoreCar(GCar *car);
	int TestMove(GCar *car, int dx, int dy);
	bool CarOK(int x, int y);
	int GetIndex(int type, int ID);
	int  Crashig(GRun *run);
	void DestroyArrays();
	void Move();
	bool LoadGFX(bool noc);
	// kreslici fce
	void DrawObj(BITMAP *dest, int CX, int CY, int hi, bool stin);
	void Draw(BITMAP *dest, BITMAP *aabuff, int CX, int CY, bool stin);

	void Destroy();

	void AddObject(st_GFile_Object ob);
	void AddRoadPoint(int type, st_GFile_Road road);
	void InitDone(char *filename);
	void Reset(int X, int Y, int bonusu, POINT cam_start, POINT cam_end);

	GRun *m_run;

	GMap();
	virtual ~GMap();

	BITMAP *m_bGround; // povrch mapy
	BITMAP *m_bSys;    // popis povrchu
	BITMAP *m_bMini;   // obrazek minimapy

	int     m_sx; // velikost x pro hraci plochu
	int     m_sy; // velikost y pro hraci plochu

	char    m_file[50]; // jmeno souboru


	GObject *m_objects[MAX_OBJECTS_ON_MAP]; // pole ukazatelu na GObject
	int      m_cobjects;       // pocet platnych ukazatelu v poli m_objects (aby se nemuselo prochazet uplne cele)

	st_GFile_Road  *m_roads[MAX_ROADS_TYPES][MAX_ROADS_ON_MAP]; // pole ukazatelu na GObject
	int             m_croads[MAX_ROADS_TYPES];       // pocet platnych ukazatelu v poli m_objects (aby se nemuselo prochazet uplne cele)

	int   m_chp[MAX_CHP];
	int   m_checkpoints;
	int   m_maxcheckpoints;

	POINT m_bonusy[MAX_BONUSU_ON_MAP];
	int   m_bonusu;

	POINT m_cam_start;
	POINT m_cam_end;
private:
	void FillLargeGround();
};

#endif // #ifndef _G_MAP_INCLUDE_
