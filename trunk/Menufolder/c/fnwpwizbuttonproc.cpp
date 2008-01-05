/*
 * This file is (C) Chris Wohlgemuth 2002
 * It is part of the WPS-Wizard package
 */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#define INCL_SW
#define INCL_REXXSAA
#define INCL_WIN
#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_GPI

#include <os2.h>
#include "newfileDlg.h"
#include "except.h"
#include <rexxsaa.h>                   /* needed for RexxStart()     */
#include <stdio.h>
#include <stdlib.h>
#include <sys\stat.h>
#include "defines.h"
#include "sys_funcs.h"

static HWND hwndShadow;// The handle of the help window
static PFNWP  oldProc; // For shadow proc


#define  WM_NEWBUBBLE   WM_USER+100 //Use ATOM later
#define xVal  12      //x-distance of Bubble
#define yVal  8      //y-distance of Bubble

char chrTBFlyFontName[CCHMAXPATH]="9.WarpSans";/* Font for toolbar fly over help */
RGB rgbTBFlyForeground={0};
RGB rgbTBFlyBackground={180,255,255};
BOOL bTBFlyOverEnabled=TRUE;
int iTBFlyOverDelay=250;

extern PFNWP   g_pfnwpOrgButtonProc;
extern ULONG   ulQWP_WIZBUTTONDATA;
extern BOOL    g_haveFlyOverClient;
extern BOOL fHaveWizDrawIconProc; /* CWThemeObject is installed */

#include "menufolder.hh"

HWND hwndBubbleWindow=NULLHANDLE;// The handle of the flyover help window
USHORT id=0;//// Last window the mouse was over 
static oldHwnd;
PFNWP g_orgBubbleProc=NULLHANDLE; //Flyover frame proc

#if 0
MRESULT EXPENTRY shadowProc(HWND hwnd, ULONG msg,MPARAM mp1,MPARAM mp2 )
{
  switch (msg)
    {
    case WM_PAINT:
      {
        HPS hps;
        HPS hpsMem;
        BITMAPINFOHEADER2 bmpIH2;
        PBITMAPINFO2 pbmp2;
        char * chrBuff;
        PBYTE ptr;
        HDC hdcMem;
        HBITMAP hbm;
        ULONG ulCx, ulCy;
        SWP swp;

        hps=WinBeginPaint(hwnd, NULLHANDLE, NULLHANDLE/*&rectl*/);

        //paintBlendedFlyOverWindow(hps, hwnd);

        GpiCreateLogColorTable(hps, 0, LCOLF_RGB, 0, 0, NULL);
        WinQueryWindowPos(hwnd, &swp);

        ulCx=swp.cx;
        ulCy=swp.cy;

        bmpIH2.cbFix=sizeof(BITMAPINFOHEADER2);
        bmpIH2.cx=ulCx;
        bmpIH2.cy=ulCy;
        bmpIH2.cPlanes=1;
        bmpIH2.cBitCount=8;
        bmpIH2.cbImage=(((ulCx*(1<<bmpIH2.cPlanes)*(1<<bmpIH2.cBitCount))+31)/32)*bmpIH2.cy;

        chrBuff=(char*)malloc(bmpIH2.cbImage+sizeof(BITMAPINFO2)+256*sizeof(RGB2));
        pbmp2=(PBITMAPINFO2)chrBuff;
        memset(pbmp2, 0, sizeof(BITMAPINFO2)+256*sizeof(RGB2));
        ptr=chrBuff+sizeof(BITMAPINFO2)+256*sizeof(RGB2);

        pbmp2->cbFix=sizeof(BITMAPINFO2);
        pbmp2->cx=ulCx;
        pbmp2->cy=ulCy;
        pbmp2->cPlanes=1;
        pbmp2->cBitCount=8;
        pbmp2->cbImage=((pbmp2->cx+31)/32)*pbmp2->cy;
        pbmp2->ulCompression=BCA_UNCOMP;
        pbmp2->ulColorEncoding=BCE_RGB;

        hdcMem=DevOpenDC(WinQueryAnchorBlock(hwnd),OD_MEMORY,"*", 0L/*4L*/, (PDEVOPENDATA)NULLHANDLE/*pszData*/, NULLHANDLE);
        if(hdcMem) {
          SIZEL sizel= {0,0};

          hpsMem=GpiCreatePS(WinQueryAnchorBlock(hwnd), hdcMem, &sizel, PU_PELS|GPIT_MICRO|GPIA_ASSOC);
          if(hpsMem)
            {                 
              hbm=GpiCreateBitmap(hpsMem, &bmpIH2, FALSE, NULL, pbmp2);
              if(hbm) {
                HPS hpsDesktop;
                POINTL ptl[3]={0};
                RGB2 *prgb2;
                int a, r,g,b;
 
                hpsDesktop=WinGetScreenPS(HWND_DESKTOP);
                GpiSetBitmap(hpsMem, hbm);

                ptl[0].x=0;
                ptl[0].y=0;
                ptl[1].x=0+ulCx;
                ptl[1].y=0+ulCy;

                ptl[2].x=swp.x;
                ptl[2].y=swp.y;

                if(GpiBitBlt(hpsMem, hpsDesktop, 3, ptl , ROP_SRCCOPY, BBO_IGNORE)==GPI_ERROR)
                  {
                  }

                WinReleasePS(hpsDesktop);

                if(GpiQueryBitmapBits(hpsMem, 0, ulCy, ptr, pbmp2)==GPI_ALTERROR)
                  {
           
                  }

                prgb2=(RGB2*)(++pbmp2);
                for(a=0;a<256; a++, prgb2++) {
                  r=-50;
                  g=-50;
                  b=-50;
                  
                  b+=prgb2->bBlue;
                  g+=prgb2->bGreen;
                  r+=prgb2->bRed;
                  if(r>255)
                    r=255;
                  if(r<0)
                    r=0;
                  prgb2->bRed=r;

                  if(g>255)
                    g=255;
                  if(g<0)
                    g=0;
                  prgb2->bGreen=g;

                  if(b>255)
                    b=255;
                  if(b<0)
                    b=0;
                  prgb2->bBlue=b;        
                }

                if(GpiSetBitmapBits(hpsMem, 0, ulCy, ptr, --pbmp2)!=GPI_ALTERROR)
                  {
                    ptl[0].x=0;
                    ptl[0].y=0;
                    ptl[1].x=ulCx;
                    ptl[1].y=ulCy;
                    ptl[2].x=0;
                    ptl[2].y=0;
                    
                    GpiBitBlt(hps, hpsMem, 3, ptl , ROP_SRCCOPY, BBO_IGNORE);
                  }
                GpiSetBitmap(hpsMem, NULLHANDLE);
                GpiDeleteBitmap(hbm);
              }/* hbm */
              GpiDestroyPS(hpsMem);
            }/* hpsMem */
          DevCloseDC(hdcMem);
        }/* if(hdcMem) */
        WinEndPaint(hps);
        free(chrBuff);
        return MRFALSE;
      }
    default:
      break;
    }
  return (*oldProc)(hwnd,msg,mp1,mp2);	
}
#endif


static MRESULT EXPENTRY fnwpBubbleProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
    {
    case WM_WINDOWPOSCHANGED:
      {
        MRESULT mr;
        PSWP pswp=(PSWP)PVOIDFROMMP(mp1);

        if(pswp->fl & SWP_HIDE)
          {
            RECTL rcl;
            pswp++;

            rcl.xRight=pswp->x+pswp->cx+shadowDeltaX;
            rcl.xLeft=pswp->x;
            rcl.yBottom=pswp->y-shadowDeltaY;
            rcl.yTop=pswp->y+pswp->cy;
            pswp--;

            WinInvalidateRect(HWND_DESKTOP, &rcl, TRUE);
          }
        break;
      }
    default:
      break;
    }  
  return g_orgBubbleProc(hwnd,msg,mp1,mp2);
}


void _createTheFlyOverWindow(HWND hwnd, PSZ chrText)
{
  HWND hwndStore;
  RECTL  rclWork;
  LONG  ulWinTextLen;
  POINTL ptl;

  /*  we have to build a new information window  */
  if(hwndBubbleWindow){// if(){...} new in V1.00a 
    WinDestroyWindow(hwndBubbleWindow);/*  close the bubblewindow  */
    hwndBubbleWindow=NULL;
  }

  // Query the pointer position
  WinQueryPointerPos(HWND_DESKTOP,&ptl);
  WinMapWindowPoints(HWND_DESKTOP,hwnd,&ptl,1);
  WinQueryWindowRect(hwnd,&rclWork);				

  if(!hwndBubbleWindow 
     && WinPtInRect(WinQueryAnchorBlock(hwnd),&rclWork,&ptl)
     && bTBFlyOverEnabled) {
          
    ULONG style=FCF_BORDER|FCF_NOBYTEALIGN;
    HPS  hps;
    POINTL aptlPoints[TXTBOX_COUNT];
    RECTL   rcl;
    LONG   deltaX,deltaY;
    HWND hwndBubbleClient;

    ulWinTextLen=(LONG)strlen(chrText); // Query text length
          
    /* Delete 'Returns' in object title */
    char *pBuchst;
    char *pRest;
    pRest=chrText;

    while((pBuchst=strchr(pRest, 0x0d))!=NULL || (pBuchst=strchr(pRest,0x0a))!=NULL) {
      *pBuchst=' ';
      pBuchst++;
      if(*pBuchst==10)
        *pBuchst=' ';
      pRest=pBuchst;
    }

    /* Create help window */
    if(g_haveFlyOverClient)
      hwndBubbleWindow=WinCreateStdWindow(HWND_DESKTOP,
                                          0,
                                          &style,
                                          WC_FLYOVERCLIENT,
                                          "",
                                          SS_TEXT|DT_CENTER|DT_VCENTER,
                                          NULLHANDLE,
                                          400,
                                          &hwndBubbleClient);
    else
      hwndBubbleWindow=WinCreateStdWindow(HWND_DESKTOP,
                                          0,
                                          &style,
                                          WC_STATIC,
                                          "",
                                          SS_TEXT|DT_CENTER|DT_VCENTER,
                                          NULLHANDLE,
                                          400,
                                          &hwndBubbleClient);

    g_orgBubbleProc=WinSubclassWindow(hwndBubbleWindow, fnwpBubbleProc);
#if 0
    hwndShadow=WinCreateWindow(HWND_DESKTOP,
                               WC_STATIC,
                               "",
                               SS_TEXT|DT_CENTER|DT_VCENTER,
                               0, 0,
                               0, 0,
                               hwndBubbleWindow,
                               hwndBubbleWindow,
                               401,
                               NULLHANDLE,
                               NULLHANDLE);
    oldProc=WinSubclassWindow(hwndShadow, shadowProc);
#endif
     
    // Set the font for the help
    WinSetPresParam(hwndBubbleClient,PP_FONTNAMESIZE,
                    sizeof(chrTBFlyFontName),
                    chrTBFlyFontName);
    /* Calculate text size in pixel */
    hps=WinGetPS(hwndBubbleClient);
    GpiQueryTextBox(hps, ulWinTextLen, chrText,TXTBOX_COUNT,aptlPoints);
    WinReleasePS(hps);
          
    /* Set colors */
    WinSetPresParam(hwndBubbleClient,
                    PP_BACKGROUNDCOLOR,sizeof(rgbTBFlyBackground) ,
                    &rgbTBFlyBackground );
    WinSetPresParam(hwndBubbleClient,
                    PP_FOREGROUNDCOLOR,sizeof(rgbTBFlyForeground) ,
                    &rgbTBFlyForeground );


    /* Calculate bubble positon and show bubble */
    WinQueryPointerPos(HWND_DESKTOP,&ptl);//Query pointer position in the desktop window
    WinQueryWindowRect(HWND_DESKTOP,&rcl);//Query desktop size
    aptlPoints[TXTBOX_BOTTOMRIGHT].x-aptlPoints[TXTBOX_BOTTOMLEFT].x+7+xVal+ptl.x 
      > rcl.xRight 
      ? deltaX=-aptlPoints[TXTBOX_BOTTOMRIGHT].x-aptlPoints[TXTBOX_BOTTOMLEFT].x-xVal-xVal-7 
      : deltaX=0 ;
          
    aptlPoints[TXTBOX_TOPLEFT].y-aptlPoints[TXTBOX_BOTTOMLEFT].y+2+yVal+ptl.y 
      > rcl.yTop 
      ? deltaY=-aptlPoints[TXTBOX_TOPLEFT].y-aptlPoints[TXTBOX_BOTTOMLEFT].y-2*yVal-7
      : deltaY=0 ;		
    /* Set bubble text */
    WinSetWindowText(hwndBubbleClient,chrText);

    WinSetWindowPos(hwndBubbleWindow,
                    HWND_TOP,
                    ptl.x+xVal+deltaX,ptl.y+yVal+deltaY,  
                    aptlPoints[TXTBOX_BOTTOMRIGHT].x-aptlPoints[TXTBOX_BOTTOMLEFT].x+8,
                    aptlPoints[TXTBOX_TOPLEFT].y-aptlPoints[TXTBOX_BOTTOMLEFT].y+2,
                    SWP_ZORDER|SWP_SIZE|SWP_MOVE|SWP_SHOW);
#if 0          
    WinSetWindowPos(hwndShadow,
                    hwndBubbleWindow,
                    ptl.x+xVal+deltaX+5
                    ,ptl.y+yVal+deltaY-5,  
                    aptlPoints[TXTBOX_BOTTOMRIGHT].x-aptlPoints[TXTBOX_BOTTOMLEFT].x+8,
                    aptlPoints[TXTBOX_TOPLEFT].y-aptlPoints[TXTBOX_BOTTOMLEFT].y+2,
                    SWP_ZORDER|SWP_SIZE|SWP_MOVE|SWP_SHOW);
#endif
     
  } // end if(!hwndBubbleWindow)
}
void _handleMouseMoveForFlyOver(HWND hwnd)
{
  USHORT  tempID;
  
  tempID=WinQueryWindowUShort(hwnd,QWS_ID);/*  get the id of the window under the pointer  */  			
  if(id!=tempID) {	// New Button?
    WinStartTimer(WinQueryAnchorBlock(hwnd),hwnd, 2,(ULONG)iTBFlyOverDelay); // New timer for delay

#if 0
    if(oldHwnd)
      WinInvalidateRect(oldHwnd, NULLHANDLE, FALSE);
    WinInvalidateRect(hwnd, NULLHANDLE, FALSE);
    oldHwnd=hwnd; //Not used...
#endif
    id=tempID;  // Save ID 
  }
  else {
    if(!hwndBubbleWindow) /* Make sue the mouse isn't moved */
      WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd , 2, (ULONG) iTBFlyOverDelay); // New timer for delay	
  }			
}
BOOL _handleTimerForFlyOver(HWND hwnd, USHORT usTimer)
{
  RECTL  rclWork;
  POINTL ptl;

  switch (usTimer)
    {
#if 0
    case 1: //Intervall timer not used anymore
      /* Test pointer position */
      WinQueryPointerPos(HWND_DESKTOP, &ptl);
      WinMapWindowPoints(HWND_DESKTOP, hwnd, &ptl,1);
      WinQueryWindowRect(hwnd, &rclWork);/* Window rectl */

      if(!WinPtInRect(WinQueryAnchorBlock(hwnd),&rclWork,&ptl)
         || WinWindowFromPoint(hwnd, &ptl, FALSE)!=hwnd)
        {	// Mouse left frame
          WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, 1);  // stop the running timer
          if(hwndBubbleWindow)
            WinDestroyWindow(hwndBubbleWindow);/*  close the bubblewindow  */
          hwndBubbleWindow=NULLHANDLE;
#if 0
          if(oldHwnd)
            WinInvalidateRect(oldHwnd, NULLHANDLE, FALSE);
#endif
          id=0;
        }
      return TRUE;
#endif
    case 2: //delay over
      WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, usTimer);//Stop the delay timer
      /* Check the pointer position */
      WinQueryPointerPos(HWND_DESKTOP,&ptl);
      WinMapWindowPoints(HWND_DESKTOP,hwnd,&ptl,1);
      WinQueryWindowRect(hwnd,&rclWork);
      if(WinPtInRect(WinQueryAnchorBlock(hwnd),&rclWork,&ptl))
        WinPostMsg(hwnd, WM_NEWBUBBLE,NULL,NULL);//Request a help window
#if 0
      else
        if(oldHwnd)
          WinInvalidateRect(oldHwnd, NULLHANDLE, FALSE);
#endif

      return TRUE;
    default://System timer
      break;
    }/* switch */
  return FALSE;
}

void _cleanupFlyOverTimers(HWND hwnd)
{
  WinStopTimer(WinQueryAnchorBlock(hwnd),hwnd, 1);//Stop timer if running
  if(hwndBubbleWindow)
    WinDestroyWindow(hwndBubbleWindow);/*  close the bubblewindow  */
  hwndBubbleWindow=NULLHANDLE;
  /* Stop delay timer if running */
  WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, 2);			
}
/*****************************************************************************/
/*  New button procedure with fly over help	                                 */
/*                                                                           */
/* Registered in M_CWMenuFolder::wpclsInitData() as WC_WIZBUTTON             */
/* (wizButton)                                                               */
/*****************************************************************************/
MRESULT EXPENTRY fnwpWizButtonProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  POINTL ptl;
  HPS  hps;
  FONTMETRICS   fm;
  LONG  ulWinTextLen;
  POINTL aptlPoints[TXTBOX_COUNT];
  RECTL   rcl;
  LONG   deltaX,deltaY;
  RGB    rgb= {200,200,0};

  RECTL  rclWork;
  WPObject * wpObject;

  switch (msg)
    {
    case WM_DESTROY:
      {
        BTNCDATA btCtrl;
        WNDPARAMS wndParams;

        memset(&btCtrl,0,sizeof(btCtrl));
        btCtrl.cb=sizeof(btCtrl);
        memset(&wndParams,0,sizeof(wndParams));
        wndParams.fsStatus=WPM_CTLDATA;
        wndParams.cbCtlData=btCtrl.cb;
        wndParams.pCtlData=&btCtrl;
        if(WinSendMsg(hwnd,WM_QUERYWINDOWPARAMS,
                   MPFROMP(&wndParams),0))
          {
            /* Delete the icon ptr. I'm not sure the system will do it
               so to be safe... */
            if(btCtrl.hImage)
              {
                WinDestroyPointer(btCtrl.hImage);
                btCtrl.hImage=NULLHANDLE;
                wndParams.fsStatus=WPM_CTLDATA;
                wndParams.cbCtlData=btCtrl.cb;
                wndParams.pCtlData=&btCtrl;
                WinSendMsg(hwnd,WM_SETWINDOWPARAMS,
                           MPFROMP(&wndParams),0);
              }
          }
        _cleanupFlyOverTimers(hwnd);
#if 0
        WinStopTimer(WinQueryAnchorBlock(hwnd),hwnd,1);//Stop timer if running
        if(hwndBubbleWindow)
          WinDestroyWindow(hwndBubbleWindow);/*  close the bubblewindow  */
        hwndBubbleWindow=NULLHANDLE;
        /* Stop delay timer if running */
        WinStopTimer(WinQueryAnchorBlock(hwnd),hwnd,WinQueryWindowUShort(hwnd,QWS_ID));			
#endif
        break;
      }
    case WM_NEWBUBBLE:
      {
        WIZBUTTONDATA * pWbd;
        char winText[255];

        /* Check if we have button data */
        pWbd=(WIZBUTTONDATA*)WinQueryWindowULong(hwnd,ulQWP_WIZBUTTONDATA);
        if(!pWbd)
          return MRFALSE;
        
        /* Any text?? */
        if(!pWbd->chrFlyOver || pWbd->chrFlyOver[0]==0)
          return MRFALSE;

        _createTheFlyOverWindow(hwnd, pWbd->chrFlyOver);
        WinStartTimer(WinQueryAnchorBlock(hwnd),hwnd,1,35); //Intervall timer
        return MRFALSE;
      }

#if 0
      WIZBUTTONDATA * pWbd;
      /*  we have to build a new information window  */
      if(hwndBubbleWindow){// if(){...} new in V1.00a 
        WinDestroyWindow(hwndBubbleWindow);/*  close the bubblewindow  */
        hwndBubbleWindow=NULL;
      }

      /* Check if we have button data */
      pWbd=(WIZBUTTONDATA*)WinQueryWindowULong(hwnd,ulQWP_WIZBUTTONDATA);
      if(!pWbd)
        break;

      if(!pWbd->chrFlyOver || pWbd->chrFlyOver[0]==0)
        break;

      // Query the pointer position
      WinQueryPointerPos(HWND_DESKTOP,&ptl);
      WinMapWindowPoints(HWND_DESKTOP,hwnd,&ptl,1);
      WinQueryWindowRect(hwnd,&rclWork);				
      if(!hwndBubbleWindow 
         && WinPtInRect(WinQueryAnchorBlock(hwnd),&rclWork,&ptl)
         && bTBFlyOverEnabled) {
        
        static HWND hwndBubbleClient;
        ULONG style=FCF_BORDER|FCF_NOBYTEALIGN;
        char * winText;

        /* Get window text (It's the object title) for size calculating */
        //WinQueryWindowText(hwnd,sizeof(winText),winText);
        winText=pWbd->chrFlyOver;        
        ulWinTextLen=(LONG)strlen(winText); // Query text length
        
        /* Delete 'Returns' in object title */
        char *pBuchst;
        char *pRest;
        pRest=winText;
        while((pBuchst=strchr(pRest,13))!=NULL) {
          *pBuchst=' ';
          pBuchst++;
          if(*pBuchst==10)
            *pBuchst=' ';
          pRest=pBuchst;
        }
        
        /* Create help window */
        hwndBubbleWindow=WinCreateStdWindow(HWND_DESKTOP,
					    0,
					    &style,
					    WC_STATIC,
					    "",
                        SS_TEXT|DT_CENTER|DT_VCENTER,
					    NULLHANDLE,
					    400,
					    &hwndBubbleClient);
#if 0
        hwndShadow=WinCreateWindow(HWND_DESKTOP,
                                   WC_STATIC,
                                   "",
                                   SS_TEXT|DT_CENTER|DT_VCENTER,
                                   0, 0,
                                   0, 0,
                                   hwndBubbleWindow,
                                   hwndBubbleWindow,
                                   401,
                                   NULLHANDLE,
                                   NULLHANDLE);
        oldProc=WinSubclassWindow(hwndShadow, shadowProc);
#endif
   
        // Set the font for the help
        WinSetPresParam(hwndBubbleClient,PP_FONTNAMESIZE,
                        sizeof(chrTBFlyFontName),
                        chrTBFlyFontName);
        /* Calculate text size in pixel */
        hps=WinGetPS(hwndBubbleClient);
        GpiQueryTextBox(hps,ulWinTextLen,winText,TXTBOX_COUNT,aptlPoints);
        WinReleasePS(hps);
        
        /* Set colors */
        WinSetPresParam(hwndBubbleClient,
                        PP_BACKGROUNDCOLOR,sizeof(rgbTBFlyBackground) ,
                        &rgbTBFlyBackground );
        WinSetPresParam(hwndBubbleClient,
                        PP_FOREGROUNDCOLOR,sizeof(rgbTBFlyForeground) ,
                        &rgbTBFlyForeground );
        
        /* Calculate bubble positon and show bubble */
        WinQueryPointerPos(HWND_DESKTOP,&ptl);//Query pointer position in the desktop window
        WinQueryWindowRect(HWND_DESKTOP,&rcl);//Query desktop size
        aptlPoints[TXTBOX_BOTTOMRIGHT].x-aptlPoints[TXTBOX_BOTTOMLEFT].x+7+xVal+ptl.x 
          > rcl.xRight 
          ? deltaX=-aptlPoints[TXTBOX_BOTTOMRIGHT].x-aptlPoints[TXTBOX_BOTTOMLEFT].x-xVal-xVal-7 
	      : deltaX=0 ;
        
        aptlPoints[TXTBOX_TOPLEFT].y-aptlPoints[TXTBOX_BOTTOMLEFT].y+2+yVal+ptl.y 
          > rcl.yTop 
          ? deltaY=-aptlPoints[TXTBOX_TOPLEFT].y-aptlPoints[TXTBOX_BOTTOMLEFT].y-2*yVal-7
	      : deltaY=0 ;		
        WinSetWindowPos(hwndBubbleWindow,
                        HWND_TOP,
                        ptl.x+xVal+deltaX,ptl.y+yVal+deltaY,  
                        aptlPoints[TXTBOX_BOTTOMRIGHT].x-aptlPoints[TXTBOX_BOTTOMLEFT].x+8,
                        aptlPoints[TXTBOX_TOPLEFT].y-aptlPoints[TXTBOX_BOTTOMLEFT].y+2,
                        SWP_ZORDER|SWP_SIZE|SWP_MOVE|SWP_SHOW);
#if 0        
        WinSetWindowPos(hwndShadow,
                        hwndBubbleWindow,
                        ptl.x+xVal+deltaX+5
                        ,ptl.y+yVal+deltaY-5,  
                        aptlPoints[TXTBOX_BOTTOMRIGHT].x-aptlPoints[TXTBOX_BOTTOMLEFT].x+8,
                        aptlPoints[TXTBOX_TOPLEFT].y-aptlPoints[TXTBOX_BOTTOMLEFT].y+2,
                        SWP_ZORDER|SWP_SIZE|SWP_MOVE|SWP_SHOW);
#endif   
        /* Set bubble text */
        WinSetWindowText(hwndBubbleClient,winText);
        WinStartTimer(WinQueryAnchorBlock(hwnd),hwnd,1,35); //Intervall timer
      } // end if(!hwndBubbleWindow)
      break;
#endif
    case WM_MOUSEMOVE:
      {
        _handleMouseMoveForFlyOver(hwnd);
        break;
      }
    case WM_TIMER:			
      {
        if( _handleTimerForFlyOver(hwnd, SHORT1FROMMP(mp1)))
          return MRFALSE;

        break; /* It's a system timer*/
      }
    }
  // call the original button procedure to handle the rest of the messages
  if(g_pfnwpOrgButtonProc)
    return g_pfnwpOrgButtonProc( hwnd, msg, mp1, mp2);
  return WinDefWindowProc( hwnd, msg, mp1, mp2);
  //  return (*oldButtonProc2)(hwnd,msg,mp1,mp2);	
};

