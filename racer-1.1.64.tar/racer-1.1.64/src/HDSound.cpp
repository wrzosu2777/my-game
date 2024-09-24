/*  Hippo Games - 2000 (C) - http://www.prog.cz/hippo/
 *    __________________________________________________
 *   /\     __  __    _____   ______   ______   ______  \
 *   \ \   /\ \|\ \  /\_  _\ /\  __ \ /\  __ \ /\  __ \  \
 *    \ \  \ \ \_\ \ \//\ \/ \ \ \_\ \\ \ \_\ \\ \ \ \ \  \
 *     \ \  \ \  __ \  \ \ \  \ \  __/ \ \  __/ \ \ \ \ \  \
 *      \ \  \ \ \/\ \  \_\ \_ \ \ \/   \ \ \/   \ \ \_\ \  \
 *       \ \  \ \_\ \_\ /\____\ \ \_\    \ \_\    \ \_____\  \
 *        \ \  \/_/\/_/ \/____/  \/_/     \/_/     \/_____/   \
 *         \ \_________________________________________________\
 *          \/_________________________________________________/
 *                           
 *  
 *  Sub     : Funkce pro DirectSound
 *
 *  File    : HDSound.cpp
 *  
 *  Projekt : HDXII
 *  
 *  Autor   : Bernard Lidicky [berny@prog.cz]
 *
 *  Datum   : 29.12.2000
 *
 */      

#include "stdafx.h"
#include "HDSound.h"
//#include "DSutils.h"


HDSound dsound;

//////////////////////////////////////////////////////////////////////////////////
// NULL-ovaci konstruktor
//////////////////////////////////////////////////////////////////////////////////
HDSound::HDSound()
{
//	m_lpDS = NULL;
//	m_volume = 0;
}

//////////////////////////////////////////////////////////////////////////////////
// releasovaci destruktor
//////////////////////////////////////////////////////////////////////////////////
HDSound::~HDSound()
{
/*	if (m_lpDS != NULL) {
		m_lpDS->Release();
		m_lpDS = NULL;
	}*/
}

//////////////////////////////////////////////////////////////////////////////////
// Hlavni tvorici fce. Oba parametry mohou byt NULL - automaticke vyplneni
// parametru.
//////////////////////////////////////////////////////////////////////////////////
/*
bool HDSound::Create(HINSTANCE hinst, HWND hwnd)
{
	if (hwnd == NULL)
		hwnd = win_get_window();

	if (DirectSoundCreate(NULL, &m_lpDS, NULL) != DS_OK) return false;

	if (m_lpDS->SetCooperativeLevel(hwnd, DSSCL_EXCLUSIVE) != DS_OK) return false;

	return true;
}
*/
//////////////////////////////////////////////////////////////////////////////////
// operator, ktery umoznuje pouzivat promenou i jako LPDIRECTSOUND
//////////////////////////////////////////////////////////////////////////////////
/*
HDSound::operator LPDIRECTSOUND() {
	return m_lpDS;
}
*/
bool HDSound::Inicialized()
{
//	if (m_lpDS == NULL) return false;
	return true;
}







HDSoundBuffer::HDSoundBuffer()
{
/*	m_lpDSB = NULL;
	m_module = NULL;
	m_resname[0] = '\0';
	m_playing = false;*/
}

HDSoundBuffer::~HDSoundBuffer()
{
	Release();
}

void HDSoundBuffer::Play(bool loop)
{
/*	if (m_lpDSB == NULL) return;
	if (loop)
		m_lpDSB->Play(0, 0, DSBPLAY_LOOPING); 
	else
		m_lpDSB->Play(0, 0, 0);
	m_playing = true;
	SetVolume(0);*/
}
/*
bool HDSoundBuffer::LoadFromRes(const char *resname, HMODULE module)
{
	if (!dsound.Inicialized()) return true;

	Release();

	m_lpDSB = DSLoadSoundBuffer(dsound, resname, module);
	if (m_lpDSB == NULL) return false;
	m_module  = module;
	strcpy(m_resname, resname);
	m_lpDSB->GetFrequency(&m_freqency_original);
	return true;
}
*/
void HDSoundBuffer::SetPan(int pan)
{
//	if (m_lpDSB == NULL) return;
//	m_lpDSB->SetPan(pan);
}

void HDSoundBuffer::SetFreq(int freq)
{
//	if (m_lpDSB == NULL) return;
//	m_lpDSB->SetFrequency(freq);

}

void HDSoundBuffer::SetVolume(int volume)
{
//	if (m_lpDSB == NULL) return;
//	volume += dsound.m_volume;
//	m_lpDSB->SetVolume(volume);

}
/*
HDSoundBuffer::operator LPDIRECTSOUNDBUFFER()
{
	return m_lpDSB;
}
*/
void HDSoundBuffer::Stop()
{
/*	if (m_lpDSB == NULL) return;
	m_lpDSB->Stop();
	m_playing = false;*/
}

bool HDSoundBuffer::IsPlaying()
{
/*	if (m_lpDSB == NULL) return false;
	unsigned long status;
	if (m_lpDSB->GetStatus(&status) == DS_OK)
		if (DSBSTATUS_PLAYING == status) return true;

	return m_playing;
	*/
	return false;
}

void HDSoundBuffer::Release()
{
/*	if (m_lpDSB != NULL) {
		m_lpDSB->Stop();
		m_lpDSB->Release();
		m_lpDSB = NULL;
	}
	m_playing = false;
*/	
}
