// TButton.h: interface for the TButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TBUTTON_H__859D1E67_0D80_4315_950E_296671BDBE64__INCLUDED_)
#define AFX_TBUTTON_H__859D1E67_0D80_4315_950E_296671BDBE64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BLUE_CH  20

class TButton  
{
public:
	void Destroy();
	bool Update(int key_pressed=0);
	bool IsMouseIn();
	void Draw();
	void Init(int x, int y, const char *c, HFONT font, int key_activation=KEY_ENTER);
	TButton();
	virtual ~TButton();

	int   m_blue;
	int   m_x;
	int   m_y;
	RECT  m_rect;
	HFONT m_font;
	char  m_c[100];

	bool    m_mLeft;
	bool    m_me;
    
    bool    m_keyboard_selected;
    int     m_key_sensitive;
};

void unselect_all_buttons(TButton *buttons[], int count);


#endif // !defined(AFX_TBUTTON_H__859D1E67_0D80_4315_950E_296671BDBE64__INCLUDED_)
