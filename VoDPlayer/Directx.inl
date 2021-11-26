#include "VoDPlayer.h"
#include "VoDPlayerDlg.h"
#include "Video.h"
#include "playvideo.h"
#include "mediatypes.h"
#include "amstream.h"    // DirectShow multimedia stream interfaces

extern TCHAR TempPath[];
extern CSql	Log;
HRESULT CVideo::FreeDirectShow(void)
{
    HRESULT hr=S_OK;

    StopSeekTimer();
    StopMedia();

    // Disable event callbacks
    if (pME)
        hr = pME->SetNotifyWindow((OAHWND)NULL, 0, 0);

    // Hide video window and remove owner.  This is not necessary here,
    // since we are about to destroy the filter graph, but it is included
    // for demonstration purposes.  Remember to hide the video window and
    // clear its owner when destroying a window that plays video.
    if(pVW)
    {
        hr = pVW->put_Visible(OAFALSE);
        hr = pVW->put_Owner(NULL);
    }

    SAFE_RELEASE(pMC);
    SAFE_RELEASE(pMS);
    SAFE_RELEASE(pVW);
    SAFE_RELEASE(pBV);
    SAFE_RELEASE(pME);
    SAFE_RELEASE(pGB);
    return hr;
}

void CVideo::StartSeekTimer() 
{
    // Cancel any pending timer event
    StopSeekTimer();

    // Create a new timer
    g_wTimerID = SetTimer(TIMERID, TICKLEN, NULL);
}

void CVideo::StopSeekTimer() 
{
    // Cancel the timer
    if(g_wTimerID)        
    {                
        KillTimer(g_wTimerID);
        g_wTimerID = 0;
    }
}

void CVideo::ReadMediaPosition()
{
    HRESULT hr;
    REFERENCE_TIME rtNow;

    if (!pMS)
        return;
    // Read the current stream position
    hr = pMS->GetCurrentPosition(&rtNow);
	if (FAILED(hr))
        return;
	if(rtNow==g_rtTotalTime)
	{
		LONGLONG pos=0;
		hr = pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
                           NULL, AM_SEEKING_NoPositioning);
		for(unsigned i=0;i<PageCount;i++)
		{
			Pages[i].PageSeenBefore = 0;
		}
	}

    // Convert position into a percentage value and update slider position
    if (g_rtTotalTime != 0)
    {
		//unsigned long nTotalMS = (unsigned long) rtNow / 10000; // 100ns -> ms
		ULONGLONG nTotalMS = rtNow/10000;
        //long lTick = (long)((rtNow * 100) / g_rtTotalTime);
		//DWORD dwPosition = m_Slider.GetPos();
        m_Slider.SetPos((int)(nTotalMS/100));
	}
    else
        m_Slider.SetPos(0);
    
    // Update the 'current position' string on the main dialog
    UpdatePosition(rtNow);
}

void CVideo::UpdatePosition(REFERENCE_TIME rtNow) 
{
    HRESULT hr;
	CString html;
	int Position = m_Slider.GetPos();
    if (!pMS)
        return;

    // If no reference time was passed in, read the current position
    if (rtNow == 0)
    {
        // Read the current stream position
        hr = pMS->GetCurrentPosition(&rtNow);
        if (FAILED(hr))
            return;
    }

    // Convert the LONGLONG duration into human-readable format
    unsigned long nTotalMS = (unsigned long) rtNow / 10000; // 100ns -> ms
    int nSeconds = nTotalMS / 1000;
    //int nMinutes = nSeconds / 60;
    nSeconds %= 60;
	
	for(unsigned i=0;i<PageCount;i++)
	{
		if((int)(Position/10)==(int)(Pages[i].Frame/10) && Pages[i].PageSeenBefore==FALSE)
		{
			html.Format("%s",TempPr);
			Pages[i].PageSeenBefore = TRUE;
			html.Format("%s\\%s",html,Pages[i].PageName);
			m_explorer.Navigate(html,NULL,NULL,NULL,NULL);
			
	
		}
	}
	for(unsigned i=0;i<uPageCount;i++)
	{
		if((int)(Position/10)==(int)(uPages[i].Frame/10) && uPages[i].PageSeenBefore==FALSE)
		{
			html.Format("%s",uPages[i].PageName);
			uPages[i].PageSeenBefore = TRUE;
			//html.Format("%s\\%s",html,uPages[i].PageName);
			m_explorer.Navigate(html,NULL,NULL,NULL,NULL);
			//AfxMessageBox(html);
	
		}
	}
 }

HRESULT CVideo::InitDirectShow(void)
{
    HRESULT hr = S_OK;

    g_bAudioOnly = FALSE;

    // Zero interfaces (sanity check)
    pVW = NULL;
    pBV = NULL;

    JIF(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&pGB));
    JIF(pGB->QueryInterface(IID_IMediaControl,  (void **)&pMC));
    JIF(pGB->QueryInterface(IID_IMediaSeeking,  (void **)&pMS));
    JIF(pGB->QueryInterface(IID_IBasicVideo2,    (void **)&pBV));
    JIF(pGB->QueryInterface(IID_IVideoWindow,   (void **)&pVW));
    JIF(pGB->QueryInterface(IID_IMediaEventEx,  (void **)&pME));

	

    return S_OK;

CLEANUP:
    FreeDirectShow();
    return(hr);
}

__inline HRESULT CVideo::PrepareMedia(LPTSTR lpszMovie)
{
	//CMainFrame *pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;

    USES_CONVERSION;
    HRESULT hr = S_OK;
	/*if(_tcsstr(Pr->VideoName, TEXT(".avi")))
	{
		hr = RenderAVIFile(T2W(lpszMovie));
        if (FAILED(hr)) 
		{
            return hr;
        }
	}*/
   // Allow DirectShow to create the FilterGraph for this media file
	//pGB->AddFilter
	hr = pGB->RenderFile(T2W(lpszMovie), NULL);
    if (FAILED(hr)) 
	{
		return hr;
    }
	 
    
#ifdef TSAT
	GetTime("Render file finished");
#endif

    // Set the message drain of the video window to point to our main
    // application window.  If this is an audio-only or MIDI file, 
    // then put_MessageDrain will fail.
    hr = pVW->put_MessageDrain((OAHWND) m_hWnd);
    if (FAILED(hr))
    {
        g_bAudioOnly = TRUE;
    }

    // Have the graph signal event via window callbacks
    hr = pME->SetNotifyWindow((OAHWND)m_hWnd, WM_GRAPHNOTIFY, 0);

    // Configure the video window
    if (!g_bAudioOnly)
    {
		hr = pVW->put_Visible(OAFALSE);
		hr = pVW->put_WindowStyle(WS_CHILD);
        hr = pVW->put_Owner((OAHWND) m_Screen.GetSafeHwnd());
		//hr = pVW->put_Owner((OAHWND) ::GetDesktopWindow());
		//::GetDesktopWindow()->GetDesktopWindow();
        // Place video window within the bounding rectangle
        CenterVideo();

        // Make the video window visible within the screen window.
        // If this is an audio-only file, then there won't be a video interface.
        hr = pVW->put_Visible(OATRUE);
        hr = pVW->SetWindowForeground(-1);
    }
#ifdef TSAT
	GetTime("Returning from prepare media file");
#endif
    return hr;
}


/*HRESULT CVideo::RenderAVIFile(LPCWSTR wFile)
{
	HRESULT hr=S_OK;
    //IFileSourceFilter *pFS=NULL;
    //IBaseFilter *pReader=NULL;
	IAMMultiMediaStream *pAMStream=NULL;

    hr = CoCreateInstance(CLSID_AMMultiMediaStream, NULL, 
        CLSCTX_INPROC_SERVER, IID_IAMMultiMediaStream, 
        (void **)&pAMStream);
    if (FAILED(hr))
    {
        return hr;
    }

	//Initialize stream
    hr = pAMStream->Initialize(STREAMTYPE_READ, 0, NULL);
	if (FAILED(hr))
	{   
		AfxMessageBox(_T("Initialize failed."));
		return E_FAIL;
	}
	return hr;
}*/

extern int Aspect;
extern int Size;
void CVideo::CenterVideo(void)
{
    LONG width, height,w,h;
    HRESULT hr;
	if ((g_bAudioOnly) || (!pVW))
        return;
	pBV->GetVideoSize(&w,&h);
	RECT rc;
	m_Screen.GetClientRect(&rc);
    width  =  rc.right - rc.left;
    height = rc.bottom - rc.top;
	
	switch(Aspect)
	{
	case 0:
		hr = pVW->SetWindowPosition(rc.left, rc.top, width, height);
	    if (FAILED(hr))
		{
			CHAR text[1024];
			wsprintf(text,"Failed to set window position!  hr=0x%x",hr);
			MessageBox(text,"Warning",MB_ICONWARNING|MB_OK);
			return;
		}
		break;
	case 1:
		switch(Size)
		{
		case 0:
			int left,top;
			left = (rc.right - w) /2;
			top = (rc.bottom - h) /2;
			pVW->SetWindowPosition(left,top, w, h);
			break;
		case 1:
			w*=2;h*=2;
			left = (rc.right - w) /2;if(left<0)left=0;
			top = (rc.bottom - h) /2;if(top<0)top=0;
			pVW->SetWindowPosition(left,top, w, h);
			break;
		}
		break;
	}	
}

/*HRESULT CVideo::CreateFilter(REFCLSID clsid, IBaseFilter **ppFilter)
{
    HRESULT hr;

    hr = CoCreateInstance(clsid, NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void **) ppFilter);
    if(FAILED(hr))
    {
        CHAR text[1024];
		wsprintf(text,"CreateFilter: Failed to create filter!  hr=0x%x",hr);
		MessageBox(text,"Warning",MB_ICONWARNING|MB_OK);
        *ppFilter = NULL;
        return hr;
    }

    return S_OK;
}*/
/*
HRESULT CVideo::RenderOutputPins(IGraphBuilder *pGB, IBaseFilter *pFilter)
{
    HRESULT         hr = S_OK;
    IEnumPins *     pEnumPin = NULL;
    IPin *          pConnectedPin = NULL, * pPin;
    PIN_DIRECTION   PinDirection;
    ULONG           ulFetched;
    // Enumerate all pins on the filter
    hr = pFilter->EnumPins( &pEnumPin );
    if(SUCCEEDED(hr))
    {
        // Step through every pin, looking for the output pins
        while (S_OK == (hr = pEnumPin->Next( 1L, &pPin, &ulFetched)))
        {
            // Is this pin connected?  We're not interested in connected pins.
            hr = pPin->ConnectedTo(&pConnectedPin);
            if (pConnectedPin)
            {
                pConnectedPin->Release();
                pConnectedPin = NULL;
            }
            // If this pin is not connected, render it.
            if (VFW_E_NOT_CONNECTED == hr)
            {
                hr = pPin->QueryDirection( &PinDirection );
                if ( ( S_OK == hr ) && ( PinDirection == PINDIR_OUTPUT ) )
                {
                    hr = pGB->Render(pPin);
                }
            }
            pPin->Release();
            // If there was an error, stop enumerating
            if (FAILED(hr))                      
                break;
        }
    }
    // Release pin enumerator
    pEnumPin->Release();
    return hr;
}*/

void CVideo::OnPlay()
{
	CVoDPlayerDlg *dlg = (CVoDPlayerDlg*)AfxGetMainWnd();
	if(Play==1)
	{
      	RunMedia();
		StartSeekTimer();
		//pM->Say(TEXT("Running"));	
		Play=0;
		dlg->m_Play.SetIcon(IDI_PAUSE1,IDI_PAUSE);
	} 
	else if(Play==0)
	{
		StopSeekTimer();
        PauseMedia();
        //pM->Say(TEXT("PAUSED"));
		Play=1;
		dlg->m_Play.SetIcon(IDI_PLAY1,IDI_PLAY);
	}
}

void CVideo::OnStop()
{
	for(unsigned i=0;i<PageCount;i++)
	{
		Pages[i].PageSeenBefore=0;
		uPages[i].PageSeenBefore = 0;
	}
	CVoDPlayerDlg *dlg = (CVoDPlayerDlg*)AfxGetMainWnd();
	dlg->m_Play.SetIcon(IDI_PLAY1,IDI_PLAY);
	Play=1;
	//m_Play.SetIcon(IDI_PLAY1,IDI_PLAY);
    HRESULT hr;
	//CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
    if (!pMC || !pMS)
        return;

    // Stop playback immediately with IMediaControl::Stop().
    StopSeekTimer();
    StopMedia();

    // Wait for the stop to propagate to all filters
    OAFilterState fs;
    hr = pMC->GetState(500, &fs);
    if (FAILED(hr))
    {
        TCHAR text[1024];
		wsprintf(text,"Failed to read graph state!  hr=0x%x!",hr);
		AfxMessageBox(text,MB_ICONWARNING|MB_OK);
    }

    // Reset to beginning of media clip
    LONGLONG pos=0;
    hr = pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
                           NULL, AM_SEEKING_NoPositioning);
    if (FAILED(hr))
    {
		//TCHAR text[1024];
		//wsprintf(text,"Failed to seek to beginning of media!  hr=0x%x!",hr);
		//AfxMessageBox(text,MB_ICONWARNING|MB_OK);
	}
  
    // Display the first frame of the media clip, if it contains video.
    // StopWhenReady() pauses all filters internally (which allows the video
    // renderer to queue and display the first video frame), after which
    // it sets the filters to the stopped state.  This enables easy preview
    // of the video's poster frame.
    hr = pMC->StopWhenReady();
    if (FAILED(hr))
    {
        TCHAR text[1024];
		wsprintf(text,"Failed in StopWhenReady!  hr=0x%x",hr);
		AfxMessageBox(text,MB_ICONWARNING|MB_OK);
    }
    ReadMediaPosition();
}