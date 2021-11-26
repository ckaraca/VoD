#include "VoD.h"
#include "VoDDoc.h"
#include "VoDView.h"
#include "MainFrm.h"
#include "playvideo.h"
#include "mediatypes.h"
#include "Design.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_CHANGE_TAB	WM_USER+1

void CVoDView::ConfigureSeekbar()
{
    // Disable seekbar for new file and reset tracker/position label
    // If we can't read the file's duration, disable the seek bar
	
	if (pMS && SUCCEEDED(pMS->GetDuration(&g_rtTotalTime)))
	{
		       		
		__int64 nTotalMS =(g_rtTotalTime / 1000); // 100ns -> ms
		//int nSeconds = nTotalMS / 1000;
			
		//int nMinutes = nSeconds / 60;
		//nSeconds %= 60;
		m_Slider.SetRange(0,(int)(nTotalMS/1000),TRUE);
		extern int SliderRange;
		SliderRange =(int) nTotalMS/1000;  // seconds
		m_Slider.SetTicFreq((int)nTotalMS / 1000);
        m_Slider.EnableWindow(TRUE);
		m_Ruler.SetMargin((int)nTotalMS / 1000);
		
		//Warn(mes);
	}
    else
        m_Slider.EnableWindow(FALSE);
}

void CVoDView::SetPlay(int Play)
{
	this->Play = Play;
}

void CVoDView::OnPlay()
{
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	if(Play==1)
	{
       
		RunMedia();
		StartSeekTimer();
		pM->Say(TEXT("Running"));	
		Play=0;
		m_Play.SetIcon(IDI_PAUSE1,IDI_PAUSE);
	} 
	else if(Play==0)
	{
		StopSeekTimer();
        PauseMedia();
        pM->Say(TEXT("PAUSED"));
		Play=1;
		m_Play.SetIcon(IDI_PLAY1,IDI_PLAY);
	}
}

void CVoDView::StartSeekTimer() 
{
    // Cancel any pending timer event
    StopSeekTimer();

    // Create a new timer
    g_wTimerID = SetTimer(TIMERID, TICKLEN, NULL);
}



void CVoDView::OnStop()
{
	for(unsigned i=0;i<PageCount;i++)
	{
		Pages[i].PageSeenBefore=0;
	}

	Play=1;
	m_Play.SetIcon(IDI_PLAY1,IDI_PLAY);
    HRESULT hr;
	CMainFrame * pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;
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
		TCHAR text[1024];
		wsprintf(text,"Failed to seek to beginning of media!  hr=0x%x!",hr);
		AfxMessageBox(text,MB_ICONWARNING|MB_OK);
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

    pM->Say(TEXT("Paused"));

    // Reset slider bar and position label back to zero
    ReadMediaPosition();
}

void CVoDView::ReadMediaPosition()
{
    HRESULT hr;
    REFERENCE_TIME rtNow;

    if (!pMS)
        return;

    // Read the current stream position
    hr = pMS->GetCurrentPosition(&rtNow);
    if (FAILED(hr))
        return;

    // Convert position into a percentage value and update slider position
    if (g_rtTotalTime != 0)
    {
		__int64 nTotalMS = rtNow / 1000; // 100ns -> ms
        //long lTick = (long)((rtNow * 100) / g_rtTotalTime);
        m_Slider.SetPos((int)(nTotalMS/1000));
		
    }
    else
        m_Slider.SetPos(0);
    
    // Update the 'current position' string on the main dialog
    UpdatePosition(rtNow);
}

void CVoDView::UpdatePosition(REFERENCE_TIME rtNow) 
{
    HRESULT hr;

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
    __int64 nTotalMS = rtNow / 1000; // 100ns -> ms
    int nSeconds =(int) nTotalMS / 1000;
    //int nMinutes = nSeconds / 60;
    
	for(unsigned i=0;i<PageCount;i++)
	{
		if(Pages[i].Frame==(((int)m_Ruler.GetScrollPos()/3)) && !Pages[i].PageSeenBefore)
		{
            SetPage(Pages[i].PageName);
			Pages[i].PageSeenBefore=TRUE; 
		}
	}

	m_Ruler.SetScrollPos((long)(nTotalMS / 330));
	m_Ruler.Invalidate();
 }

HRESULT CVoDView::InitDirectShow(void)
{
    HRESULT hr = S_OK;

    g_bAudioOnly = FALSE;

    // Zero interfaces (sanity check)
    pVW = NULL;
    pBV = NULL;

    JIF(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&pGB));
    JIF(pGB->QueryInterface(IID_IMediaControl,  (void **)&pMC));
    JIF(pGB->QueryInterface(IID_IMediaSeeking,  (void **)&pMS));
    JIF(pGB->QueryInterface(IID_IBasicVideo,    (void **)&pBV));
    JIF(pGB->QueryInterface(IID_IVideoWindow,   (void **)&pVW));
    JIF(pGB->QueryInterface(IID_IMediaEventEx,  (void **)&pME));

	

    return S_OK;

CLEANUP:
    FreeDirectShow();
    return(hr);
}

HRESULT CVoDView::FreeDirectShow(void)
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

void CVoDView::StopSeekTimer() 
{
    // Cancel the timer
    if(g_wTimerID)        
    {                
        KillTimer(g_wTimerID);
        g_wTimerID = 0;
    }
}

void CVoDView::ResetDirectShow(void)
{
    // Destroy the current filter graph its filters.
    FreeDirectShow();
    // Reinitialize graph builder and query for interfaces
    InitDirectShow();
}

HRESULT CVoDView::PrepareMedia(LPTSTR lpszMovie)
{
	CMainFrame *pM = (CMainFrame *) AfxGetApp()->m_pMainWnd;

    USES_CONVERSION;
    HRESULT hr = S_OK;

    pM->Say(TEXT("Loading..."));

    // Is this a Windows Media file (ASF, WMA, WMV)?  If so, use the new
    // ASF Reader filter, which is faster and much better at seeking than
    // the default ASF Reader filter used by default with RenderFile.
    if (IsWindowsMediaFile(lpszMovie))
    {
        hr = RenderWMFile(T2W(lpszMovie));
        if (FAILED(hr)) {
            TCHAR text[1024];
			wsprintf(text,"Failed(%08lx) to Render WM File(%s)!",hr,lpszMovie);
			AfxMessageBox(text,MB_ICONWARNING|MB_OK);

            return hr;
        }
    }
    else
    {
        // Allow DirectShow to create the FilterGraph for this media file
        hr = pGB->RenderFile(T2W(lpszMovie), NULL);
        if (FAILED(hr)) {
            CHAR text[1024];
			wsprintf(text,"Failed(%08lx) in RenderFile(%s)!",hr, lpszMovie);
			MessageBox(text,"Warning",MB_ICONWARNING|MB_OK);
            return hr;
        }
    }

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
        // We'll manually set the video to be visible
        hr = pVW->put_Visible(OAFALSE);

        hr = pVW->put_WindowStyle(WS_CHILD);
        hr = pVW->put_Owner((OAHWND) m_Screen.GetSafeHwnd());

        // Place video window within the bounding rectangle
        CenterVideo();

        // Make the video window visible within the screen window.
        // If this is an audio-only file, then there won't be a video interface.
        hr = pVW->put_Visible(OATRUE);
        hr = pVW->SetWindowForeground(-1);
    }

    pM->Say(TEXT("Ready"));
	long *rate,rat;
	rate = &rat;
	//TCHAR mes[200];
	//pBV->get_BitRate(rate);
	//wsprintf(mes,"rate = %ld bits/s",rat);
	//Warn(mes);
	//wsprintf(mes,"rate = %lu bits/s",rat);
	//Warn(mes);
	
    return hr;
}

void CVoDView::CenterVideo(void)
{
    LONG width, height;
    HRESULT hr;

    if ((g_bAudioOnly) || (!pVW))
        return;

    // Read coordinates of video container window
    RECT rc;
    m_Screen.GetClientRect(&rc);
    width  =  rc.right - rc.left;
    height = rc.bottom - rc.top;

    // Ignore the video's original size and stretch to fit bounding rectangle
    hr = pVW->SetWindowPosition(rc.left, rc.top, width, height);
    if (FAILED(hr))
    {
        CHAR text[1024];
		wsprintf(text,"Failed to set window position!  hr=0x%x",hr);
		MessageBox(text,"Warning",MB_ICONWARNING|MB_OK);
        return;
    }
}

BOOL CVoDView::IsWindowsMediaFile(LPTSTR lpszFile)
{
    TCHAR szFilename[MAX_PATH];

    // Copy the file name to a local string and convert to lowercase
    _tcsncpy(szFilename, lpszFile, NUMELMS(szFilename));
    szFilename[MAX_PATH-1] = 0;
    _tcslwr(szFilename);

    if (_tcsstr(szFilename, TEXT(".asf")) ||
        _tcsstr(szFilename, TEXT(".wma")) ||
        _tcsstr(szFilename, TEXT(".wmv")))
        return TRUE;
    else
        return FALSE;
}

HRESULT CVoDView::RenderWMFile(LPCWSTR wFile)
{
    HRESULT hr=S_OK;
    IFileSourceFilter *pFS=NULL;
    IBaseFilter *pReader=NULL;

    // Load the improved ASF reader filter by CLSID
    hr = CreateFilter(CLSID_WMAsfReader, &pReader);
    if(FAILED(hr))
    {
        CHAR text[1024];
		wsprintf(text,"Failed to create WMAsfWriter filter!  hr=0x%x",hr);
		MessageBox(text,"Warning",MB_ICONWARNING|MB_OK);
        return hr;
    }

    // Add the ASF reader filter to the graph.  For ASF/WMV/WMA content,
    // this filter is NOT the default and must be added explicitly.
    hr = pGB->AddFilter(pReader, L"ASF Reader");
    if(FAILED(hr))
    {
        CHAR text[1024];
		wsprintf(text,"Failed to add ASF reader filter to graph!  hr=0x%x",hr);
		MessageBox(text,"Warning",MB_ICONWARNING|MB_OK);
        pReader->Release();
        return hr;
    }



    // Set its source filename
    JIF(pReader->QueryInterface(IID_IFileSourceFilter, (void **) &pFS));
    JIF(pFS->Load(wFile, NULL));
    pFS->Release();

    // Render the output pins of the ASF reader to build the
    // remainder of the graph automatically
    JIF(RenderOutputPins(pGB, pReader));

CLEANUP:
    // If there was a rendering error, make sure that the reader is released
    //
    // Otherwise, wince the graph is built and the filters are added to 
    // the graph, the WM ASF reader interface can be released.
    SAFE_RELEASE(pReader);

    return hr;
}

HRESULT CVoDView::CreateFilter(REFCLSID clsid, IBaseFilter **ppFilter)
{
    HRESULT hr;

    hr = CoCreateInstance(clsid,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IBaseFilter,
        (void **) ppFilter);

    if(FAILED(hr))
    {
        CHAR text[1024];
		wsprintf(text,"CreateFilter: Failed to create filter!  hr=0x%x",hr);
		MessageBox(text,"Warning",MB_ICONWARNING|MB_OK);
        *ppFilter = NULL;
        return hr;
    }

    return S_OK;
}

HRESULT CVoDView::RenderOutputPins(IGraphBuilder *pGB, IBaseFilter *pFilter)
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
}

void CVoDView::OnSlide(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	*pResult = 0;
}


