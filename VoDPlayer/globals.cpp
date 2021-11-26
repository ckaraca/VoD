#include "stdafx.h"
#include "Sql.h"
#include "Stats.h"
#include "VoDPlayer.h"

//#define SMT
#ifdef SMT
	int smt_start=0;
	CFile smt_genel,smt_ftp,smt_content;
	
#endif



CVoDPlayerApp* Main;
CSql	Log;
CStats *_Stats;
BOOL Stats;
PROJECTDB *Pr;
int Cnt=0, ProRange=4,ProPos=0;
TCHAR TempPath[MAX_PATH];
TCHAR VoDPath[MAX_PATH];
TCHAR VoDPrPath[MAX_PATH];
TCHAR uDatFile[MAX_PATH];

TCHAR TempPr[MAX_PATH];
CString VideoStream;
CString VideoPath;
int ProjectActive =0;
int Return;

int Aspect;
int Size;
int cs;		//Connection speed
CString Connection;
int	BufferFTP,BufferTCP,BufferUDP;
HWND hWnd;
// ftp vars
int Numof1=0,Numof2=0,canceled=0;
int SliderRange;
int nCount = 0,ntime=0;
int tWait= 0;
BOOL TimerisSet = FALSE;
#include <strmif.h>     // Generated IDL header file for streams interfaces
#include <control.h>    // generated from control.odl
#include "playvideo.h"

//
// Global data
//
IGraphBuilder *pGB = NULL;
IMediaSeeking *pMS = NULL;
IMediaControl *pMC = NULL;
IMediaEventEx *pME = NULL;
IBasicVideo2   *pBV = NULL;
IVideoWindow  *pVW = NULL;
LONG	pBitRate;

FILTER_STATE g_psCurrent=State_Stopped;

BOOL g_bLooping      = FALSE,
     g_bAudioOnly    = FALSE,
     g_bDisplayEvents= FALSE,
     g_bGlobalMute   = FALSE,
     g_bPlayThrough  = FALSE;
// bandwidth test vars
//long TotalLength;
//LONGLONG TTime;
