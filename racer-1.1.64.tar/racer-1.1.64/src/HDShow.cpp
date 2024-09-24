// HDShow.cpp: implementation of the HDShow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HDShow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
#pragma comment(lib,"uuid.lib")
#pragma comment(lib,"strmiids.lib")


#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }

#define JIF(x) if (FAILED((x))) \
    {CloseClip(); return;}

#define LIF(x) if (FAILED((x))) \
    {return;}


*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HDShow::HDShow()
{
/*	mhApp             = 0;
	m_szFileName[0]   = '\0';
	m_dwGraphRegister = 0;
	m_psCurrent       = Init;
	m_lVolume         = 0; // max hlasitost
	m_bAudioOnly      = FALSE;
	pGB = NULL;
	pMC = NULL;
	pME = NULL;
	pVW = NULL;
	pBA = NULL;
	pBV = NULL;
	pMS = NULL;

	CoInitialize(NULL);
	*/
}

HDShow::~HDShow()
{
//	CloseClip();
//	CoUninitialize();
}

void HDShow::Create()
{
/*	mhApp = win_get_window();

	self = this;

   SetTimer(mhApp, 0, 100, &Timer);
*/	
}
/*
void HDShow::Play(bool loop)
{
    if((m_psCurrent == Paused) || (m_psCurrent == Stopped))
    {
        pMC->Run();
        m_psCurrent = Running;
		  m_loop = loop;
	 }
}

void HDShow::Pause()
{
    if (!pMC)
        return;

    // Toggle play/pause behavior
    if((m_psCurrent == Paused) || (m_psCurrent == Stopped))
    {
 //       pMC->Run();
  //      m_psCurrent = Running;
    }
    else
    {
        pMC->Pause();
        m_psCurrent = Paused;
    }
}

void HDShow::Stop()
{
    HRESULT hr;
    
    if ((!pMC) || (!pMS))
        return;

    // Stop and reset postion to beginning
    if((m_psCurrent == Paused) || (m_psCurrent == Running))
    {
        LONGLONG pos = 0;
        hr = pMC->Stop();
        m_psCurrent = Stopped;

        hr = pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
            NULL, AM_SEEKING_NoPositioning);

        // Display the first frame to indicate the reset condition
        hr = pMC->Pause();
    }
}

void HDShow::CloseClip()
{
    HRESULT hr;

    if(pMC)
        hr = pMC->Stop();

    m_psCurrent = Stopped;
    m_bAudioOnly = TRUE;

    CloseInterfaces();

    // Clear file name to allow selection of new file with open dialog
    m_szFileName[0] = L'\0';

    // No current media state
    m_psCurrent = Init;
}

void HDShow::CloseInterfaces()
{
    HRESULT hr;
    
    // Relinquish ownership (IMPORTANT!) after hiding video window
    if(pVW)
    {
        hr = pVW->put_Visible(OAFALSE);
        hr = pVW->put_Owner(NULL);
    }

    SAFE_RELEASE(pME);
    SAFE_RELEASE(pMS);
    SAFE_RELEASE(pMC);
    SAFE_RELEASE(pBA);
    SAFE_RELEASE(pBV);
    SAFE_RELEASE(pVW);
    SAFE_RELEASE(pGB);
}

void HDShow::OpenClip(const char *filename)
{
//	if (strcmp(m_szFileName, filename)) return;

	strcpy(m_szFileName, filename);

    WCHAR wFile[MAX_PATH];
//    HRESULT hr;

    // Clear open dialog remnants before calling RenderFile()
    UpdateWindow(mhApp);

#ifndef UNICODE
    MultiByteToWideChar(CP_ACP, 0, filename, -1, wFile, MAX_PATH);
#else
    lstrcpy(wFile, szFile);
#endif

    // Get the interface for DirectShow's GraphBuilder
    JIF(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pGB));

    // Have the graph construct its the appropriate graph automatically
        JIF(pGB->RenderFile(wFile, NULL));

        // QueryInterface for DirectShow interfaces
        JIF(pGB->QueryInterface(IID_IMediaControl, (void **)&pMC));
        JIF(pGB->QueryInterface(IID_IMediaEventEx, (void **)&pME));
        JIF(pGB->QueryInterface(IID_IMediaSeeking, (void **)&pMS));

        // Query for video interfaces, which may not be relevant for audio files
        JIF(pGB->QueryInterface(IID_IVideoWindow, (void **)&pVW));
        JIF(pGB->QueryInterface(IID_IBasicVideo, (void **)&pBV));

        // Query for audio interfaces, which may not be relevant for video-only files
        JIF(pGB->QueryInterface(IID_IBasicAudio, (void **)&pBA));

        // Is this an audio-only file (no video component)?
        CheckVisibility();

        if (!m_bAudioOnly)
        {
            JIF(pVW->put_Owner((OAHWND)mhApp));
            JIF(pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN));
        }

        // Have the graph signal event via window callbacks for performance
 //       JIF(pME->SetNotifyWindow((OAHWND)mhApp, WM_GRAPHNOTIFY, (long)this));

        if (!m_bAudioOnly)
        {
           SetVideoWindow();
        }

        // Let's get ready to rumble!
        ShowWindow(mhApp, SW_SHOWNORMAL);
        UpdateWindow(mhApp);
        SetForegroundWindow(mhApp);
        SetFocus(mhApp);

       // Run the graph to play the media file
//        JIF(pMC->Run());
//        m_psCurrent=Running;

//       SetFocus(mhApp);
	m_psCurrent = Stopped;
}

void HDShow::CheckVisibility()
{
    long lVisible;
    HRESULT hr;

    m_bAudioOnly = FALSE;

    if ((!pVW) || (!pBV))
    {
        m_bAudioOnly = TRUE;
        //Msg(TEXT("No video interface.  Assuming audio/MIDI file or unsupported video codec.\r\n"));
        return;
    }

    hr = pVW->get_Visible(&lVisible);
    if (FAILED(hr))
    {
        // If this is an audio-only clip, get_Visible() won't work.
        //
        // Also, if this video is encoded with an unsupported codec,
        // we won't see any video, although the audio will work if it is
        // of a supported format.
        //
        if (hr == E_NOINTERFACE)
        {
            m_bAudioOnly = TRUE;
        }
        else
			  TRACE("Failed(%08lx) in pVW->get_Visible()!\r\n");
    }
}

void HDShow::ToggleFullScreen()
{
    LONG lMode;
    static HWND hDrain=0;

    // Don't bother with full-screen for audio-only files
    if ((m_bAudioOnly) || (!pVW))
        return;

    // Read current state
    JIF(pVW->get_FullScreenMode(&lMode));

    if (lMode == OAFALSE)
    {
        // Save current message drain
        LIF(pVW->get_MessageDrain((OAHWND *) &hDrain));

        // Set message drain to application main window
        LIF(pVW->put_MessageDrain((OAHWND) mhApp));

        // Switch to full-screen mode
        lMode = OATRUE;
        JIF(pVW->put_FullScreenMode(lMode));
    }
    else
    {
        // Switch back to windowed mode
        lMode = OAFALSE;
        JIF(pVW->put_FullScreenMode(lMode));

        // Undo change of message drain
        LIF(pVW->put_MessageDrain((OAHWND) hDrain));

        // Reset video window
        LIF(pVW->SetWindowForeground(-1));

        // Reclaim keyboard focus for player application
        UpdateWindow(mhApp);
        SetForegroundWindow(mhApp);
        SetFocus(mhApp);
    }
}

void HDShow::Run()
{
    if((m_psCurrent == Paused) || (m_psCurrent == Stopped))
    {
        pMC->Run();
        m_psCurrent = Running;
    }
}

bool HDShow::HandleEvents()
{

	 if (!pME) return false;

    LONG evCode, evParam1, evParam2;
    HRESULT hr=S_OK;

    while(SUCCEEDED(pME->GetEvent(&evCode, &evParam1, &evParam2, 0)))
    {
        // Spin through the events
        hr = pME->FreeEventParams(evCode, evParam1, evParam2);

        if(EC_COMPLETE == evCode)
        {
            LONGLONG pos=0;

				if (m_loop) {
				// Reset to first frame of movie
	            hr = pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
		                                NULL, AM_SEEKING_NoPositioning);
					if (FAILED(hr))
					{
						// Some custom filters (like the Windows CE MIDI filter) 
						// may not implement seeking interfaces (IMediaSeeking)
						// to allow seeking to the start.  In that case, just stop 
						// and restart for the same effect.  This should not be
						// necessary in most cases.
						if (FAILED(hr = pMC->Stop()))
							break;
						if (FAILED(hr = pMC->Run()))
							break;
					}
				}
				else {
					CloseClip();
					return true;
				}

        }
    }
	 return false;
}

void HDShow::SetVideoWindow()
{
    RECT rect;

    if (!pBV)
        return;

    GetClientRect(mhApp, &rect);
    JIF(pVW->SetWindowPosition(rect.left, rect.top, rect.right, rect.bottom));
}

int HDShow::Timer()
{
	 HDShow *ds = HDShow::self;
	 if (ds->pME) return 0;

    LONG evCode, evParam1, evParam2;
    HRESULT hr=S_OK;

    while(SUCCEEDED(ds->pME->GetEvent(&evCode, &evParam1, &evParam2, 0)))
    {
        // Spin through the events
        hr = ds->pME->FreeEventParams(evCode, evParam1, evParam2);

        if(EC_COMPLETE == evCode)
        {
            LONGLONG pos=0;

				if (ds->m_loop) {
				// Reset to first frame of movie
	            hr = ds->pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
		                                NULL, AM_SEEKING_NoPositioning);
					if (FAILED(hr))
					{
						// Some custom filters (like the Windows CE MIDI filter) 
						// may not implement seeking interfaces (IMediaSeeking)
						// to allow seeking to the start.  In that case, just stop 
						// and restart for the same effect.  This should not be
						// necessary in most cases.
						if (FAILED(hr = ds->pMC->Stop()))
							break;
						if (FAILED(hr = ds->pMC->Run()))
							break;
					}
				}
				else {
					ds->CloseClip();
					break;
				}

        }
    }

	 return 0;
}

bool HDShow::IsPlaying()
{
	if (m_psCurrent == Running) return true;
	return false;
}
*/
//HDShow* HDShow::self;
