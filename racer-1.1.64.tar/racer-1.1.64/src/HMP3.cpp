#include "stdafx.h"
//#include <tchar.h>
#include <malloc.h>
//#include <winuser.h>
#include "HMP3.H"
/*

#pragma comment(lib,"uuid.lib")
#pragma comment(lib,"strmiids.lib")
*/
HMP3 hmp3;

  
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define MEDIA_TIMEOUT   (5)  // 0.01 second


HMP3::HMP3()
{
/*	m_pGraphBuilder = NULL;
	m_pMediaControl = NULL;
	m_pMediaSeeking = NULL;
	m_pMediaEventEx = NULL;
	m_pSourceCurrent = NULL;
	m_pSourceNext = NULL;
	m_szCurrentFile[128];

	m_loop = false;
	m_play = false;
	m_inicialized = false;
	m_iconicpause = false;

	m_volume = 0;

	CoInitialize(NULL);*/
}

HMP3::~HMP3()
{
//	CleanUp();
//	CoUninitialize();
}

bool HMP3::Create()
{
/*    HRESULT hr;
    // Create DirectShow Graph
    if (FAILED (hr = CoCreateInstance(CLSID_FilterGraph, NULL,
                                      CLSCTX_INPROC, IID_IGraphBuilder,
                                      reinterpret_cast<void **>(&m_pGraphBuilder))) ) {
   	  OutputDebugString("HMP3 - CoCreateInstance failed\n");
        return false;
	 }


    // Get the IMediaControl Interface
    if (FAILED (m_pGraphBuilder->QueryInterface(IID_IMediaControl,
																reinterpret_cast<void **>(&m_pMediaControl)))) {
   	  OutputDebugString("HMP3 - IID_IMediaControl failed\n");
        return false;
	 }

    if (FAILED (m_pGraphBuilder->QueryInterface(IID_IMediaEventEx, (LPVOID *)&m_pMediaEventEx))) {
   	  OutputDebugString("HMP3 - IID_IMediaEventEx failed\n");
        return false;
	 }

	 if (FAILED (m_pGraphBuilder->QueryInterface(IID_IBasicAudio, (void **)&m_pBasicAudio))) {
   	  OutputDebugString("HMP3 - IID_IBasicAudio failed\n");
        return false;
	 }

    // Get the IMediaControl Interface
    if (FAILED (m_pGraphBuilder->QueryInterface(IID_IMediaSeeking,
																reinterpret_cast<void **>(&m_pMediaSeeking)))) {
   	  OutputDebugString("HMP3 - IID_IMediaSeeking failed\n");
 	     return false;
	 }


    // Create the intial graph
	 m_play = false;

    // Set a timer for switching the sources
    SetTimer(win_get_window(), 0, MEDIA_TIMEOUT, &Timer);

	 m_inicialized = true;
    return true;;*/
	return true;
}


void HMP3::Play(char* file, bool loop)
{
/*	if (!m_inicialized) return;

	if (strcmp(file, m_szCurrentFile) == 0) return;

	 Stop();

    HRESULT hr = S_OK;
    IPin *pPin = NULL;
    TCHAR szFilename[MAX_PATH];
    WCHAR wFileName[MAX_PATH];

    strcpy(m_szCurrentFile, file);
    strcpy(szFilename, file);

    // Make sure that this file exists
    DWORD dwAttr = GetFileAttributes(szFilename);
    if (dwAttr == (DWORD) -1)
        return;

    #ifndef UNICODE
        MultiByteToWideChar(CP_ACP, 0, szFilename, -1, wFileName, MAX_PATH);
    #else
        lstrcpy(wFileName, szFilename);
    #endif

    // OPTIMIZATION OPPORTUNITY
	// This will open the file, which is expensive. To optimize, this
    // should be done earlier, ideally as soon as we knew this was the
    // next file to ensure that the file load doesn't add to the
    // filter swapping time & cause a hiccup.
    //
    // Add the new source filter to the graph. (Graph can still be running)
    hr = m_pGraphBuilder->AddSourceFilter(wFileName, wFileName, &m_pSourceNext);

    // Get the first output pin of the new source filter. Audio sources 
    // typically have only one output pin, so for most audio cases finding 
    // any output pin is sufficient.
    if (SUCCEEDED(hr)) {
        hr = m_pSourceNext->FindPin(L"Output", &pPin);  
    }

    // Stop the graph
    if (SUCCEEDED(hr)) {
        hr = m_pMediaControl->Stop();
    }

    // Break all connections on the filters. You can do this by adding 
    // and removing each filter in the graph
    if (SUCCEEDED(hr)) {
        IEnumFilters *pFilterEnum = NULL;

        if (SUCCEEDED(hr = m_pGraphBuilder->EnumFilters(&pFilterEnum))) {
            int iFiltCount = 0;
            int iPos = 0;

            // Need to know how many filters. If we add/remove filters during the
            // enumeration we'll invalidate the enumerator
            while (S_OK == pFilterEnum->Skip(1)) {
                iFiltCount++;
            }

            // Allocate space, then pull out all of the 
            IBaseFilter **ppFilters = reinterpret_cast<IBaseFilter **>
                                      (_alloca(sizeof(IBaseFilter *) * iFiltCount));
            pFilterEnum->Reset();

            while (S_OK == pFilterEnum->Next(1, &(ppFilters[iPos++]), NULL));
            SAFE_RELEASE(pFilterEnum);

            for (iPos = 0; iPos < iFiltCount; iPos++) {
                m_pGraphBuilder->RemoveFilter(ppFilters[iPos]);
				// Put the filter back, unless it is the old source
				if (ppFilters[iPos] != m_pSourceCurrent) {
					m_pGraphBuilder->AddFilter(ppFilters[iPos], NULL);
                }
				SAFE_RELEASE(ppFilters[iPos]);
            }
        }
    }

    // We have the new ouput pin. Render it
    if (SUCCEEDED(hr)) {
        hr = m_pGraphBuilder->Render(pPin);
        m_pSourceCurrent = m_pSourceNext;
        m_pSourceNext = NULL;
    }

    SAFE_RELEASE(pPin);
    SAFE_RELEASE(m_pSourceNext); // In case of errors

    // Re-seek the graph to the beginning
    if (SUCCEEDED(hr)) {
        LONGLONG llPos = 0;
        hr = m_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,
                                           &llPos, AM_SEEKING_NoPositioning);
    } 

    // Start the graph
    if (SUCCEEDED(hr)) {
        hr = m_pMediaControl->Run();
		  m_play = true;
    }

	 m_loop = loop;

    // Release the old source filter.
    SAFE_RELEASE(m_pSourceCurrent)

	m_pBasicAudio->put_Volume(m_volume);*/
}


void HMP3::Pause()
{
/*	if (!m_inicialized) return;
	if (SUCCEEDED(m_pMediaControl->Pause()))
		m_play = false;*/
}


void HMP3::Play()
{
/*	if (!m_inicialized) return;
	if (SUCCEEDED(m_pMediaControl->Run()))
		m_play = true;
	m_pBasicAudio->put_Volume(m_volume);*/
}

void HMP3::Stop()
{
/*	if (!m_inicialized) return;
	if (SUCCEEDED(m_pMediaControl->Stop()))
		m_play = false;*/
}
void HMP3::CleanUp()
{
 /*   // Stop playback
    if (m_pMediaControl)
        m_pMediaControl->Stop();

    // Release all remaining pointers
    SAFE_RELEASE( m_pSourceNext);
    SAFE_RELEASE( m_pSourceCurrent);
	 SAFE_RELEASE( m_pMediaEventEx);
    SAFE_RELEASE( m_pMediaSeeking);
    SAFE_RELEASE( m_pMediaControl);
    SAFE_RELEASE( m_pGraphBuilder);
    SAFE_RELEASE( m_pBasicAudio);*/
}


int HMP3::Timer()
{
/*	 if (hmp3.m_pMediaEventEx == NULL) return 0;

    LONG evCode, evParam1, evParam2;
    HRESULT hr=S_OK;

    while(SUCCEEDED(hmp3.m_pMediaEventEx->GetEvent(&evCode, &evParam1, &evParam2, 0)))
    {
        hr = hmp3.m_pMediaEventEx->FreeEventParams(evCode, evParam1, evParam2);

        if(EC_COMPLETE == evCode)
			  if (hmp3.m_loop) {
					LONGLONG pos=0;
				   if(FAILED(hmp3.m_pMediaSeeking->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
							NULL, AM_SEEKING_NoPositioning))) {
			         hmp3.Stop();
			         hmp3.Play();
					}

			  }
			  else
				  hmp3.PlayMenu();

	 }
	 return 0;*/
	return 0;
}
END_OF_FUNCTION(Timer);


void HMP3::KillFocus()
{
/*	if (hmp3.m_play) {
		hmp3.Pause();
		hmp3.m_iconicpause = true;
	}*/
}

void HMP3::SetFocus()
{
/*	 if (hmp3.m_iconicpause) {
		 hmp3.Play();
		 hmp3.m_iconicpause = false;
	 } */
}

void HMP3::PlayMenu()
{
	char s[100];
	sprintf(s, "sfx\\s%d.mp3", rand()%4);
	Play(s, false);

}

void HMP3::PlayGame()
{
	char s[100];
	sprintf(s, "sfx\\M%d.wav", rand()%6);
	Play(s, true);
}
