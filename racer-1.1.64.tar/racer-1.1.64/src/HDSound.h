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
 *  File    : HDSound.h
 *  
 *  Projekt : HDXII
 *  
 *  Autor   : Bernard Lidicky [berny@prog.cz]
 *
 *  Datum   : 29.12.2000
 *
 */      

#ifndef HDSOUND_HEADER
#define HDSOUND_HEADER

/*
#include <mmsystem.h>
#include <dsound.h>

#pragma comment (lib,"dsound.lib")
#pragma comment (lib,"winmm.lib")

*/
class HDSound  
{
public:
	DWORD m_volume;
	bool Inicialized();
/*
	// operatory
	operator LPDIRECTSOUND();


	bool Create(HINSTANCE hinst = NULL, HWND hwnd = NULL);
	*/
	HDSound();
	virtual ~HDSound();
/*
private:
	LPDIRECTSOUND m_lpDS;
	*/
};

extern HDSound dsound;

class HDSoundBuffer
{
public:
	unsigned long m_freqency_original;
	bool m_playing;
	void Release();
	bool IsPlaying();
	void Stop();
	void SetVolume(int volume);
	void SetFreq(int freq);
	void SetPan(int pan);
//	bool LoadFromRes(const char *resname, HMODULE module = NULL);
	HDSoundBuffer();
	~HDSoundBuffer();

	void Play(bool loop);
//	operator LPDIRECTSOUNDBUFFER();

private:
	char                m_resname[50];
//	HMODULE             m_module;
//	LPDIRECTSOUNDBUFFER m_lpDSB;
};

#endif // !defined(AFX_HDSOUND_H__DBAFE921_6279_45C4_ACF4_A071443D7956__INCLUDED_)
