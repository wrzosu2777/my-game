// GView.h: interface for the GView class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _G_VIEW_INCLUDE_
#define _G_VIEW_INCLUDE_

#include "GClasses.h"

#define G_MAX_SPEED          30
#define G_CAMERA_DISTANCE_X 400 // ((DF_X/2+50)*1.5)
#define G_CAMERA_DISTANCE_Y 350 // ((DF_Y/2+50)*1.5)

class GView  
{
public:
	bool LoadGFX();
	void Destroy();
	void UpdateCamera(GRun *run);
	void Init(GMap *map, int moncar);
	void Draw(GRun *run);
	GView();
	virtual ~GView();

	int m_Cx; // X-ova souradnice pohledu (Camera x)
	int m_Cy; // Y-ova souradnice pohledu (Camera y)
	int m_moncar;

	POINT m_cam_end;

	RECT m_rect;
};

#endif // #ifndef _G_VIEW_INCLUDE_
