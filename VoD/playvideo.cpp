//------------------------------------------------------------------------------
// File: PlayVideo.cpp
//
// Desc: DirectShow sample code - media control functions.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#include "stdafx.h"
#include <dshow.h>

#include "playvideo.h"


HRESULT RunMedia()
{
    HRESULT hr=S_OK;

    if (!pMC)
        return S_OK;

    // Start playback
    hr = pMC->Run();
    if (FAILED(hr)) {
        TCHAR text[1024];
		wsprintf(text,"Failed(%08lx) in Run()!",hr);
		MessageBox(NULL,text,"Warning",MB_ICONWARNING|MB_OK);
        return hr;
    }

    // Remember play state
    g_psCurrent = State_Running;
    return hr;
}


HRESULT StopMedia()
{
    HRESULT hr=S_OK;

    if (!pMC)
        return S_OK;

    // If we're already stopped, don't check again
    if (g_psCurrent == State_Stopped)
        return hr;

    // Stop playback
    hr = pMC->Stop();
    if (FAILED(hr)) {
        TCHAR text[1024];
		wsprintf(text,"Failed(%08lx) in Stop()!",hr);
		MessageBox(NULL,text,"Warning",MB_ICONWARNING|MB_OK);
        return hr;
    }

    // Remember play state
    g_psCurrent = State_Stopped;
    return hr;
}


HRESULT PauseMedia(void)
{
    HRESULT hr=S_OK;

    if (!pMC)
        return S_OK;

    // Play/pause
    if(g_psCurrent != State_Running)
        return S_OK;

    hr = pMC->Pause();
    if (FAILED(hr)) {
        TCHAR text[1024];
		wsprintf(text,"Failed(%08lx) in Pause()!!",hr);
		MessageBox(NULL,text,"Warning",MB_ICONWARNING|MB_OK);
        return hr;
    }

    // Remember play state
    g_psCurrent = State_Paused;
    return hr;
}


HRESULT MuteAudio(void)
{
    HRESULT hr=S_OK;
    IBasicAudio *pBA=NULL;
    long lVolume;

    if (!pGB)
        return S_OK;

    hr = pGB->QueryInterface(IID_IBasicAudio, (void **)&pBA);
    if (FAILED(hr))
        return S_OK;

    // Read current volume
    hr = pBA->get_Volume(&lVolume);
    if (hr == E_NOTIMPL)
    {
        // Fail quietly if this is a video-only media file
        pBA->Release();
        return hr;
    }
    else if (FAILED(hr))
    {
		TCHAR text[1024];
		wsprintf(text,"Failed in pBA->get_Volume!  hr=0x%x",hr);
		MessageBox(NULL,text,"warning",MB_ICONWARNING|MB_OK);
        pBA->Release();
        return hr;
    }

    lVolume = VOLUME_SILENCE;

    // Set new volume
    hr = pBA->put_Volume(lVolume);
    if (FAILED(hr))
    {
        TCHAR text[1024];
		wsprintf(text,"Failed in pBA->put_Volume!  hr=0x%x",hr);
		MessageBox(NULL,text,"Warning",MB_ICONWARNING|MB_OK);
    }

    pBA->Release();
    return hr;
}


HRESULT ResumeAudio(void)
{
    HRESULT hr=S_OK;
    IBasicAudio *pBA=NULL;
    long lVolume;

    if (!pGB)
        return S_OK;

    hr =  pGB->QueryInterface(IID_IBasicAudio, (void **)&pBA);
    if (FAILED(hr))
        return S_OK;

    // Read current volume
    hr = pBA->get_Volume(&lVolume);
    if (hr == E_NOTIMPL)
    {
        // Fail quietly if this is a video-only media file
        pBA->Release();
        return hr;
    }
    else if (FAILED(hr))
    {
		CHAR text[1024];
		wsprintf(text,"Failed in pBA->get_Volume!  hr=0x%x",hr);
		MessageBox(NULL,text,"Warning",MB_ICONWARNING|MB_OK);
        pBA->Release();

        return hr;
    }

    // Set new volume
    lVolume = VOLUME_FULL;

    hr = pBA->put_Volume(lVolume);
    if (FAILED(hr))
    {
        CHAR text[1024];
		wsprintf(text,"Failed in pBA->put_Volume!  hr=0x%x",hr);
		MessageBox(NULL,text,"Warning",MB_ICONWARNING|MB_OK);
    }

    pBA->Release();
    return hr;
}



