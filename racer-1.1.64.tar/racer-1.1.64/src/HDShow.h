// HDShow.h: interface for the HDShow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HDSHOW_H__1D05EA1B_B0A3_43D0_A0B3_C80EF7B4902C__INCLUDED_)
#define AFX_HDSHOW_H__1D05EA1B_B0A3_43D0_A0B3_C80EF7B4902C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include <dshow.h>

// #define WM_GRAPHNOTIFY  (WM_USER+13)


class HDShow  
{
public:
	bool IsPlaying();
	static int /*__stdcall*/ Timer();
	void SetVideoWindow();
	bool HandleEvents(); // true - kdyz stopped
	void Run();
	void ToggleFullScreen();
	void CheckVisibility();
	void OpenClip(const char *filename);
	void CloseInterfaces();
	void CloseClip();
	void Stop();
	void Pause();
	void Play(bool loop);
	void Create();
	HDShow();
	virtual ~HDShow();
/*
	enum PLAYSTATE {Stopped, Paused, Running, Init};

	static HDShow *self;

	int  m_user; // uzivatel muze napsat co chce

	bool      m_loop;
	bool      m_fullscreen;


//protected:
	HWND      mhApp;
	TCHAR     m_szFileName[MAX_PATH];
	DWORD     m_dwGraphRegister;
	PLAYSTATE m_psCurrent;
	LONG      m_lVolume;
	BOOL      m_bAudioOnly;

	IGraphBuilder *pGB;
	IMediaControl *pMC;
	IMediaEventEx *pME;
	IVideoWindow  *pVW;
	IBasicAudio   *pBA;
	IBasicVideo   *pBV;
	IMediaSeeking *pMS;*/
};

#endif // !defined(AFX_HDSHOW_H__1D05EA1B_B0A3_43D0_A0B3_C80EF7B4902C__INCLUDED_)
