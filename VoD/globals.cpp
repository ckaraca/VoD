//------------------------------------------------------------------------------
// File: Globals.h
//
// Desc: DirectShow sample code - global data for Jukebox application.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#include "stdafx.h"
//-------------------------


#include <strmif.h>     // Generated IDL header file for streams interfaces
#include <control.h>    // generated from control.odl
#include "playvideo.h"


//-------------------------


//
// Global data
//
IGraphBuilder *pGB = NULL;
IMediaSeeking *pMS = NULL;
IMediaControl *pMC = NULL;
IMediaEventEx *pME = NULL;
IBasicVideo   *pBV = NULL;
IVideoWindow  *pVW = NULL;

FILTER_STATE g_psCurrent=State_Stopped;

BOOL g_bLooping      = FALSE,
     g_bAudioOnly    = FALSE,
     g_bDisplayEvents= FALSE,
     g_bGlobalMute   = FALSE,
     g_bPlayThrough  = FALSE;

// Ftp globals
int Numof1=0,Numof2=0,canceled=0;
int SliderRange;
TCHAR ExtFile[MAX_PATH];
TCHAR TempPath[MAX_PATH];
int ProjectActive =0;
TCHAR DBIDext[100];
