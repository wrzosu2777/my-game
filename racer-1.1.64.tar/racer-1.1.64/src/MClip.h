// MClip.h: interface for the MClip class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _M_CLIP_INCLUDE
#define _M_CLIP_INCLUDE


extern BITMAP* b_clipL[2];  // ma vzdy jen dve pozice
extern BITMAP* b_clipR[2];  // ma vzdy jen dve pozice


class MClip  
{
public:
	MClip();
	virtual ~MClip();
	bool IsMouseIn();
	void Init(int left, int top, int width, int height, int typ);
	bool Update();
	void Draw();

	RECT    m_pos;
	int     m_stat;

	bool    m_mLeft;
	bool    m_me;
	int     m_typ; // 0 = left; 1 == right;
};

#endif // 
