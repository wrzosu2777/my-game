// TButton.cpp: implementation of the TButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TButton::TButton()
{
	m_blue = 255;
}

TButton::~TButton()
{

}

void TButton::Init(int x, int y, const char *c, HFONT font, int key_activation)
{
	strncpy(m_c, c, 99);
	m_x = x;
	m_y = y;

	m_mLeft = false;
	m_me = false;

	m_font = font;
    m_key_sensitive =key_activation;
	
//LINUX

	int width, height;
	TTGetDimension(c, m_font, width, height);

	m_rect.left  = x - width/2 - 10;
	m_rect.right = x + width/2 + 10;
	m_rect.top   = y - height/2 - 3;
	m_rect.bottom= y + height/2 + 3;
	

//LINDO: zjisteni velikosti buttonku
	
//	SIZE sz;
//	HDC dc;
//	dc = win_get_dc(screen);
//	HFONT dc_f = SelectObject(dc, font);
//	GetTextExtentPoint(dc, c, strlen(c), &sz);
//	SelectObject(dc, dc_f);
//  win_release_dc(screen, dc);

//	SetRect(&m_rect, x-sz.cx/2-3, y-sz.cy/2-3, x+sz.cx/2+3, y+sz.cy/2+3);
}

void TButton::Draw()
{
	TTDraw(hscreen.m_back, m_x, m_y, WIN_RGB(255, 255, m_blue), m_c, m_font);
}

bool TButton::IsMouseIn()
{
    if (m_keyboard_selected == true)
    {
        return true;
    }
    
	if (mouse_x > m_rect.left && mouse_x < m_rect.right && 
		 mouse_y > m_rect.top  && mouse_y < m_rect.bottom) return true;
	return false;
}

bool TButton::Update(int key_pressed)
{
	if (IsMouseIn()) {
		if ((m_blue -= BLUE_CH) < 0) m_blue = 0;
	}
	else
		if ((m_blue += BLUE_CH) > 255) m_blue = 255;


	if (IsMouseIn()) {
		if (!m_mLeft && (mouse_b & 1)) 
			m_me = true;
		if (m_mLeft && !(mouse_b & 1) && m_me) {
			m_mLeft = false;
			return true;
		}
        if (m_keyboard_selected == true && key_pressed == m_key_sensitive)
        {
            return true;
        }
	}
	else {
		if (!m_mLeft && (mouse_b & 1)) 
			m_me = false;
	}

    
	m_mLeft = mouse_b & 1;
	return false;

}

void TButton::Destroy()
{

}


void unselect_all_buttons(TButton *buttons[], int count)
{
    for (int i = 0; i < count; i++)
    {
        buttons[i]->m_keyboard_selected = false;
    }
}
