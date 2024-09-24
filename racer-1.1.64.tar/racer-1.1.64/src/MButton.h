// MButton.h: interface for the MButton class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _M_BUTTON_INCLUDE
#define _M_BUTTON_INCLUDE


#define MAX_BMPS 30

class MButton  
{
public:
	bool IsMouseIn();
	void Destroy();
	void Init(int left, int top, int width, int height, int maxstat, int max, int min, char* file);
	bool Update();
	void Draw();
	MButton();
	virtual ~MButton();

	RECT    m_pos;
	BITMAP* m_bmp[MAX_BMPS];
	int     m_maxstat;
	int     m_stat;
	int     m_dstat;

	int     m_max;
	int     m_min;

	bool    m_mLeft;
	bool    m_me;
};

#endif // #ifndef _M_BUTTON_INCLUDE
