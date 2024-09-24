// MClip.cpp: implementation of the MClip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MClip.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BITMAP* b_clipL[2];  // ma vzdy jen dve pozice
BITMAP* b_clipR[2];  // ma vzdy jen dve pozice


MClip::MClip()
{

}

MClip::~MClip()
{
}


void MClip::Draw()
{
	if (m_typ == 0)
		draw_sprite(hscreen.m_back, b_clipL[m_stat], m_pos.left, m_pos.top);
	else
		draw_sprite(hscreen.m_back, b_clipR[m_stat], m_pos.left, m_pos.top);
}

bool MClip::Update()
{
	if (IsMouseIn()) {
		if (!m_mLeft && (mouse_b & 1)) m_me = true;

		if (m_mLeft && !(mouse_b & 1) && m_me) {
			m_mLeft = false;
			return true;
		}
		if (m_me && (mouse_b & 1)) m_stat = 1;
		else m_stat = 0;
	}
	else {
		m_stat = 0;
		if (!m_mLeft && (mouse_b & 1)) 
			m_me = false;
	}

	m_mLeft = mouse_b & 1;
	return false;
}

void MClip::Init(int left, int top, int width, int height, int typ)
{
	m_stat  = 0;
	m_pos.left   = left;
	m_pos.top    = top;
	m_pos.right  = left + width;
	m_pos.bottom = top  + height;

	m_typ = typ;	
}

bool MClip::IsMouseIn()
{
	if (mouse_x > m_pos.left && mouse_x < m_pos.right && 
		 mouse_y > m_pos.top  && mouse_y < m_pos.bottom) return true;
	return false;
}
