#ifndef _HMP3_INCLUDED_
#define _HMP3_INCLUDED_

// #include <dshow.h>


class HMP3 {
public:
	HMP3();
	~HMP3();
	bool Create();
	void Play(char* file, bool loop);
	void Pause();
	void Play();
	void Stop();
	void CleanUp();
/*
	IGraphBuilder *m_pGraphBuilder;
	IMediaControl *m_pMediaControl;
	IMediaSeeking *m_pMediaSeeking;
	IMediaEventEx *m_pMediaEventEx;
	IBaseFilter   *m_pSourceCurrent;
	IBaseFilter   *m_pSourceNext;
 	IBasicAudio   *m_pBasicAudio;
	TCHAR          m_szCurrentFile[128];
*/
	bool           m_loop;
	bool           m_play;
	bool           m_inicialized;

	bool           m_iconicpause;

public:
	void PlayGame();
	void PlayMenu();
	DWORD m_volume;
	void SetFocus();
	void KillFocus();
	static int /* __stdcall */ Timer();
};

extern HMP3 hmp3;



#endif // _HMP3_INCLUDED_
