//------------------------------------------------------------------------------
// File: PlayVideo.h
//
// Desc: DirectShow sample code - declarations for media control functions.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef PLAY_VID_H
#define PLAY_VID_H

//
// Constants
//
#define VOLUME_FULL     0L
#define VOLUME_SILENCE  -10000L

// Application-defined messages
#define WM_GRAPHNOTIFY  WM_APP + 1
#define WM_FIRSTFILE    WM_APP + 2
#define WM_PLAYFILE     WM_APP + 3
#define WM_NEXTFILE     WM_APP + 4
#define WM_PREVIOUSFILE WM_APP + 5

//
// Macros
//
#define SAFE_RELEASE(i) {if (i) i->Release(); i = NULL;}
//RetailOutput(TEXT("FAILED(0x%x) ") TEXT(#x) TEXT("\n\0"), hr);
#define JIF(x) if (FAILED(hr=(x))) \
    {goto CLEANUP;}
	
//
// Global data
//
extern IGraphBuilder *pGB;
extern IMediaSeeking *pMS;
extern IMediaControl *pMC;
extern IMediaEventEx *pME;
extern IBasicVideo2   *pBV;
extern IVideoWindow  *pVW;

extern FILTER_STATE g_psCurrent;
extern BOOL g_bLooping, g_bAudioOnly, g_bPlayThrough;
extern BOOL g_bDisplayEvents, g_bGlobalMute;


//-----------------------------------------------------------------------------
// External function-prototypes
//-----------------------------------------------------------------------------
HRESULT RunMedia(void);
HRESULT StopMedia(void);
HRESULT PauseMedia(void);
HRESULT PlayMedia(LPTSTR lpszMovie, HINSTANCE hInstance);
HRESULT CheckMovieState(BOOL *pbComplete);
HRESULT GetInterfaces(void);
HRESULT MuteAudio(void);
HRESULT ResumeAudio(void);
void CleanupInterfaces(void);
void ToggleFullscreen(void);



#endif // !defined(PLAY_VID_H)
