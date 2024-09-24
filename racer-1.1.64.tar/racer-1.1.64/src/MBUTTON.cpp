// MButton.cpp: implementation of the MButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MButton::MButton()
{
	for(int i = 0 ; i < MAX_BMPS; i++)
		m_bmp[i] = NULL;
}

MButton::~MButton()
{
	Destroy();
}

void MButton::Draw()
{
	if (m_bmp[0] == NULL) return;
	// animace
	if (m_dstat > m_stat) m_stat++;
	if (m_dstat < m_stat) m_stat--;

	if (m_stat != 0)
		draw_sprite(hscreen.m_back, m_bmp[m_stat-1], m_pos.left, m_pos.top);

	if ((m_dstat == m_stat) && (m_stat == m_min)) {
		m_dstat = 0;
		m_stat  = 0;
	}

}

bool MButton::Update()
{
	if (IsMouseIn()) {
		if (m_dstat == m_stat) m_dstat = m_max;
		if (!m_mLeft && (mouse_b & 1)) 
			m_me = true;
		if (m_mLeft && !(mouse_b & 1) && m_me) {
			m_mLeft = false;
			return true;
		}
	}
	else {
		if ((m_dstat == m_stat) && (m_dstat != 0)) m_dstat = m_min;
		if (!m_mLeft && (mouse_b & 1)) 
			m_me = false;
	}

	m_mLeft = mouse_b & 1;
	return false;
}

void MButton::Init(int left, int top, int width, int height, int maxstat, int max, int min, char* file)
{
	Destroy();
	m_max = max;
	m_min = min;
	m_stat  = 0;
	m_dstat = 0;
	m_pos.left   = left;
	m_pos.top    = top;
	m_pos.right  = left + width;
	m_pos.bottom = top  + height;
	m_maxstat = maxstat;

	char buf[80];
	for (int i = 1; i <= maxstat; i++){
		if(maxstat > 9) sprintf(buf, "%s%.2d",file,i);
		else sprintf(buf, "%s%d",file,i);
		ASSERT(m_bmp[i-1] == NULL);
		m_bmp[i-1] = fload_bitmap(buf);
		if (m_bmp[i-1] == NULL)
			allegro_message("Chybi soubor %s",buf);
	}
}

void MButton::Destroy()
{
	for(int i = 0 ; i < MAX_BMPS; i++)
		if (m_bmp[i] != NULL) {
			destroy_bitmap(m_bmp[i]);
			m_bmp[i] = NULL;
		}
}

bool MButton::IsMouseIn()
{
	if (mouse_x > m_pos.left && mouse_x < m_pos.right && 
		 mouse_y > m_pos.top  && mouse_y < m_pos.bottom) return true;
	return false;
}
