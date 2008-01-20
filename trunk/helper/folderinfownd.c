/*
  (c) Chris Wohlgemuth 1996/2008

  This file is copyrighted.

  For licensing contact cinc-ml@netlabs.org

  No use or copying without prior permission.
*/

#define INCL_WINWORKPLACE
#define INCL_WIN
#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_REXXSAA
#define INCL_GPI

#include <os2.h>

#ifdef __INNOTEK_LIBC__
#include "cw_pmwp.h"
#endif

#include <stdlib.h>
#include <string.h>
#include "folderinfownd.h"
#include "htmlwnd.h"
#include "defines.h"
#include "cell.h"
#include "cwfolder.h"
#include "framectrldata.h"

/* Includes for cairo */
#include <cairo.h>
#include <cairo-os2.h>

#include "cwtheme.h"
#include "nominifile.h"
#include "cwthememgr.h"
#include "cwclasses.h"

PFNWP oldProc=NULLHANDLE;
PFNWP oldPreviewProc=NULLHANDLE;

ULONG  ulQWP_FLDRINFOWNDDATA;

extern ULONG ulQWP_FCTRLDATA;

extern BOOL g_fUseCairoForIconPainting;
extern CWThemeMgr* CWThemeMgrObject;

/*
  This function paints the icon of the folder the info area belongs to at the top ot the
  area. The painting is cairo enabled.
 */
#define SMALL_INFO_ICON
#define DELTA_ICON 5
static void drawIcon(HWND hwnd, HPS hps, RECTL rcl)
{
  WindowCellCtlData *pWCtlData = 0;

  pWCtlData = (WindowCellCtlData *)WinQueryWindowULong(hwnd, QWL_USER);

  if(!pWCtlData)
    return;

  if(somIsObj(pWCtlData->wpObject))
    {
      if(g_fUseCairoForIconPainting)
        {

          for(;;)
            {
              MINIRECORDCORE mrec={0};
              CNRDRAWITEMINFO cdii={0};
              OWNERITEM oi={0};
              ULONG fl=0;
              BOOL rc;

#if 0
              PPAINTCTXT pPaintCtxt_;
              if(!CWThemeMgrObject)
                break;
              
              pPaintCtxt=_wizRequestAPaintCtxt(CWThemeMgrObject, NULLHANDLE);
              
              if(!pPaintCtxt)
                break;
#endif
              mrec.cb=sizeof(MINIRECORDCORE);
              cdii.pRecord=(PRECORDCORE)&mrec;
              
              rcl.xRight=rcl.xLeft+30;

              oi.hwnd=hwnd;
              oi.hps=hps;
              oi.rclItem=rcl;
              oi.idItem=CMA_ICON;
              oi.hItem=(ULONG)&cdii;
     
              /* The draw emphasis call is only to get the paintcontext filled with the
               background color. May go away when cairo 1.4.7 is used. */
              rc=_wizDrawIconEmphasis(pWCtlData->wpObject, &oi, 30, fl);
              rc=_wizDrawIcon(pWCtlData->wpObject, &oi, 30, fl);

              if(rc)
                return;
              break;
            }/* for */
        }

#ifdef SMALL_INFO_ICON
      WinDrawPointer(hps, (rcl.xLeft+DELTA_ICON), rcl.yTop-DELTA_ICON-WinQuerySysValue(HWND_DESKTOP, SV_CXICON)/2,
                     _wpQueryIcon(pWCtlData->wpObject), DP_MINI);
#else
      WinDrawPointer(hps, (rcl.xLeft+DELTA_ICON), rcl.yTop-DELTA_ICON-WinQuerySysValue(HWND_DESKTOP, SV_CXICON),
                     _wpQueryIcon(pWCtlData->wpObject), DP_NORMAL /*DP_MINI*/);
#endif
      
    }/* wpObject */
  return;
}


static void paintPreview(HWND hwnd, HPS hps, RECTL *rcl)
{
  CWObject *cwObject = (CWObject *)WinQueryWindowULong(hwnd, QWL_USER);


  if(somIsObj(cwObject))
    {
      _wizDrawPreview(cwObject, hwnd,  hps, rcl);
    }
}

MRESULT EXPENTRY previewControlProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

  switch(msg)
    {
    case WM_PAINT:
      {
        RECTL rcl;
        HPS hps;
        ULONG  attrFound;
        RGB bg;
        LONG lBg;
        FLDRINFOWNDDATA* wndData;

        hps=WinBeginPaint(hwnd, NULLHANDLE, &rcl);

        WinQueryWindowRect(hwnd, &rcl);

        /* Get folder bg color */
        GpiCreateLogColorTable(hps, LCOL_PURECOLOR, LCOLF_RGB, 0, 0, NULL);
        
        // Query the current background colour (inherited from parent which is the dialog control)
        // The presparam of the dialog control is set when the control is created
        if(WinQueryPresParam(WinQueryWindow(WinQueryWindow(hwnd, QW_PARENT), QW_PARENT),
                             PP_BACKGROUNDCOLOR, PP_BACKGROUNDCOLORINDEX ,&attrFound,sizeof(bg),
                             &bg, QPF_PURERGBCOLOR|QPF_ID2COLORINDEX /*QPF_NOINHERIT*/)) {
          lBg=(bg.bRed<<16)+(bg.bGreen<<8) + bg.bBlue;
          /* Set it */
          //HlpWriteToTrapLog("0x%x %x %x %x %d\n", lBg, bg.bRed, bg.bGreen, bg.bBlue, attrFound);
          WinFillRect(hps, &rcl, (LONG) lBg);
        }
        else
          WinFillRect(hps, &rcl, SYSCLR_WINDOW /* 0x00FFFFFF*/ );
        
        /* Paint preview here */
        paintPreview(hwnd, hps, &rcl);

        WinEndPaint(hps);
        return MRFALSE;
      }
    case WM_APPTERMINATENOTIFY:
      {
        switch(SHORT2FROMMP(mp1))
          {
          case ACTION_PREVIEWHEIGHT:
            {
              WinSendMsg(WinQueryWindow(hwnd, QW_PARENT), WM_APPTERMINATENOTIFY, mp1, mp2);
              break;
            }
          default:
            break;
          }/* SHORT2FROMMP(mp1) */
        return MRFALSE;
        break;
      }
    default:
      break;
  }

  if(oldPreviewProc)
    return oldPreviewProc( hwnd, msg, mp1, mp2);

  return WinDefWindowProc( hwnd, msg, mp1, mp2);
};

/* The minimal height of an html control. When this height is reached the preview control is made smaller. */
#define MIN_TEXTCTRL_HEIGHT 150
/*
  This proc handles the info area of a window. Note that the text control within the info area has it's own
  window proc.
  New information in the info window is set by the classWorkerThread (cwobject.c) . The folder proc posts a
  message to that thread whenever a new object is selected.
 */
MRESULT EXPENTRY folderInfoProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
    {
    case WM_PAINT:
      {
        RECTL rcl;
        HPS hps;
        ULONG  attrFound;
        RGB bg;
        LONG lBg;
        FLDRINFOWNDDATA* wndData;

        hps=WinBeginPaint(hwnd, NULLHANDLE, &rcl);

        WinQueryWindowRect(hwnd, &rcl);

        /* Only paint visible area */
        wndData=(FLDRINFOWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_FLDRINFOWNDDATA);
        if(wndData)
          rcl.yBottom=rcl.yTop-wndData->lCyIcon-2*DELTA_ICON;

        /* Get folder bg color */
        GpiCreateLogColorTable(hps, LCOL_PURECOLOR, LCOLF_RGB, 0, 0, NULL);
        
        // Query the current background colour (inherited from parent which is the dialog control)
        // The presparam of the dialog control is set when the control is created
        if(WinQueryPresParam(WinQueryWindow(hwnd, QW_PARENT),
                             PP_BACKGROUNDCOLOR, PP_BACKGROUNDCOLORINDEX ,&attrFound,sizeof(bg),
                             &bg, QPF_PURERGBCOLOR|QPF_ID2COLORINDEX /*QPF_NOINHERIT*/)) {
          lBg=(bg.bRed<<16)+(bg.bGreen<<8) + bg.bBlue;
          /* Set it */
          WinFillRect(hps, &rcl, (LONG) lBg);
        }
        else
          WinFillRect(hps, &rcl, SYSCLR_WINDOW /* 0x00FFFFFF*/ );

        drawIcon(hwnd, hps, rcl);

        WinEndPaint(hps);
        return MRFALSE;
      }
    case WM_PRESPARAMCHANGED:
      {
        char fontName[255];
        ULONG  len;
        ULONG  attrFound;

        len=WinQueryPresParam(hwnd,
                              //len=WinQueryPresParam(WinQueryWindow(hwnd, QW_PARENT),
                              PP_FONTNAMESIZE,0,&attrFound,sizeof(fontName),
                              fontName, 0 /*QPF_NOINHERIT*/);


        WinSetPresParam(WinWindowFromID(hwnd, FLDRINFO_WINDOWID_TEXTCTRL),PP_FONTNAMESIZE,
                   len, fontName);

        break;
      }
    case WM_SETWINDOWPARAMS:
      {
      PWNDPARAMS pwp=(PWNDPARAMS)PVOIDFROMMP(mp1);
      RECTL rcl;
      MRESULT mr;
      FLDRINFOWNDDATA* wndData=(FLDRINFOWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_FLDRINFOWNDDATA);

      /* Forward text change to the html control */
      if(pwp && (pwp->fsStatus & WPM_TEXT))
        {
          mr=WinSendMsg(WinWindowFromID(hwnd, FLDRINFO_WINDOWID_TEXTCTRL),
                        msg, mp1, mp2);
         return mr;
        }
      break;     
      }
    case WM_WINDOWPOSCHANGED:
      {
        PSWP pswp;
        PSWP pswpNew=(PSWP)PVOIDFROMMP(mp1);
        pswp=pswpNew++;

        if(!pswp)
          break;

        if((pswp->fl & SWP_SIZE)/*&& pswp->cx!=pswpNew->cx*/) {
          FLDRINFOWNDDATA* wndData=(FLDRINFOWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_FLDRINFOWNDDATA);

          if(wndData) {
            ULONG ulY;
            ULONG ulYPreview=0;
            WindowCellCtlData *pWCtlData = 0;
            CWObject* cwObject=(CWObject*)WinQueryWindowULong( WinWindowFromID(hwnd, FLDRINFO_WINDOWID_PREVIEWCTRL), QWL_USER);


            pWCtlData = (WindowCellCtlData *)WinQueryWindowULong(hwnd, QWL_USER);

            if(somIsObj(cwObject) && pWCtlData && somIsObj(pWCtlData->wpObject))
              ulYPreview=_wizQueryPreviewHeight(cwObject, pWCtlData->wpObject, WinWindowFromID(hwnd, FLDRINFO_WINDOWID_PREVIEWCTRL), pswp->cx);

            /* Size of info area to be covered by text and preview control */
            ulY=pswp->cy-wndData->lCyIcon-DELTA_ICON*2;
            if(0==ulYPreview)
              {
                /* Position textcontrol */        
                WinSetWindowPos(WinWindowFromID(hwnd, FLDRINFO_WINDOWID_TEXTCTRL),
                                //NULLHANDLE, 0, 0, pswp->cx, pswp->cy-wndData->lCyIcon, SWP_MOVE|SWP_SIZE);
                                NULLHANDLE, 0, 0, pswp->cx, ulY, SWP_MOVE|SWP_SIZE);
                /* Position preview control */        
                WinSetWindowPos(WinWindowFromID(hwnd, FLDRINFO_WINDOWID_PREVIEWCTRL),
                                //NULLHANDLE, 0, 0, pswp->cx, pswp->cy-wndData->lCyIcon, SWP_MOVE|SWP_SIZE);
                                NULLHANDLE, 0, 0, pswp->cx, 0, SWP_MOVE|SWP_SIZE);

              }
            else
              {
                WinEnableWindowUpdate(WinWindowFromID(hwnd, FLDRINFO_WINDOWID_PREVIEWCTRL), FALSE);
                /* Preview window requested by setting a height */

                if(ulY-ulYPreview > MIN_TEXTCTRL_HEIGHT)
                  {
                    /* Position textcontrol */        
                    WinSetWindowPos(WinWindowFromID(hwnd, FLDRINFO_WINDOWID_TEXTCTRL),
                                    //NULLHANDLE, 0, 0, pswp->cx, pswp->cy-wndData->lCyIcon, SWP_MOVE|SWP_SIZE);
                                    NULLHANDLE, 0, ulYPreview, pswp->cx, ulY-ulYPreview, SWP_MOVE|SWP_SIZE);
                    /* Position preview control */        
                    WinSetWindowPos(WinWindowFromID(hwnd, FLDRINFO_WINDOWID_PREVIEWCTRL),
                                    //NULLHANDLE, 0, 0, pswp->cx, pswp->cy-wndData->lCyIcon, SWP_MOVE|SWP_SIZE);
                                    NULLHANDLE, 0, 0, pswp->cx, ulYPreview, SWP_MOVE|SWP_SIZE);
                  }
                else
                  {

                    /* Preview control must be made smaller now */
                    if(ulY > MIN_TEXTCTRL_HEIGHT)
                      {
                        ulYPreview=ulY-MIN_TEXTCTRL_HEIGHT;
                        ulY=MIN_TEXTCTRL_HEIGHT;
                      }
                    else
                      {
                        ulYPreview=0;
                      }
                    /* Position textcontrol */        
                    WinSetWindowPos(WinWindowFromID(hwnd, FLDRINFO_WINDOWID_TEXTCTRL),
                                    //NULLHANDLE, 0, 0, pswp->cx, pswp->cy-wndData->lCyIcon, SWP_MOVE|SWP_SIZE);
                                    NULLHANDLE, 0, ulYPreview, pswp->cx, ulY, SWP_MOVE|SWP_SIZE);

                    /* Position preview control */        
                    WinSetWindowPos(WinWindowFromID(hwnd, FLDRINFO_WINDOWID_PREVIEWCTRL),
                                    //NULLHANDLE, 0, 0, pswp->cx, pswp->cy-wndData->lCyIcon, SWP_MOVE|SWP_SIZE);
                                    NULLHANDLE, 0, 0, pswp->cx, ulYPreview, SWP_MOVE|SWP_SIZE);
                  }
                WinEnableWindowUpdate(WinWindowFromID(hwnd, FLDRINFO_WINDOWID_PREVIEWCTRL), TRUE);
              }
            //#endif

          }
        }/* SWP_SIZE */
        break;
      }
    case WM_CREATE:
      {
        RECTL rcl;
        MRESULT mr;
        PCREATESTRUCT pCreate=(PCREATESTRUCT)PVOIDFROMMP(mp2);
#ifdef SMALL_INFO_ICON
        LONG lCyIcon=WinQuerySysValue(HWND_DESKTOP, SV_CYICON)/2;
#else
        LONG lCyIcon=WinQuerySysValue(HWND_DESKTOP, SV_CYICON);
#endif
        FLDRINFOWNDDATA *wndData;
        HWND hwndCtrl;


        mr=oldProc( hwnd, msg, mp1, mp2);

        wndData=(FLDRINFOWNDDATA*)malloc(sizeof(FLDRINFOWNDDATA));
        if(wndData) {
          // FRAMECTRLDATA *pfcd;

          memset(wndData, 0, sizeof(FLDRINFOWNDDATA));          

#if 0
          /* Folder control data isn't initialized at this point in time...*/
          /* Get our folder control data which contains the folder object pointer */
          pfcd=(FRAMECTRLDATA*)WinQueryWindowPtr(WinQueryWindow(hwnd, QW_PARENT), ulQWP_FCTRLDATA);
          if(pfcd!=NULLHANDLE)
            wndData->wpFolder=pfcd->cwFolder;
#endif
          wndData->lCyIcon=lCyIcon;
          WinSetWindowULong(hwnd, ulQWP_FLDRINFOWNDDATA, (ULONG)wndData);

          /* Create textcontrol */
          hwndCtrl=WinCreateWindow(hwnd, HTML_WND , "", WS_VISIBLE, 0, 0, 0, 0, hwnd,
                                   HWND_TOP, FLDRINFO_WINDOWID_TEXTCTRL, NULLHANDLE, NULLHANDLE);
          /* Create preview control */
          hwndCtrl=WinCreateWindow(hwnd, WC_STATIC , "", WS_VISIBLE, 0, 0, 0, 0, hwnd,
                                   HWND_TOP, FLDRINFO_WINDOWID_PREVIEWCTRL, NULLHANDLE, NULLHANDLE);
          if(hwndCtrl)
            oldPreviewProc=WinSubclassWindow(hwndCtrl, previewControlProc);
        }
        return mr;
      }
    case WM_DESTROY:
      {
        FLDRINFOWNDDATA* wndData=(FLDRINFOWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_FLDRINFOWNDDATA);

        if(wndData)
          free(wndData);
        
        break;
      }
    case WM_APPTERMINATENOTIFY:
      {
        switch(SHORT2FROMMP(mp1))
          {
          case ACTION_PREVIEWHEIGHT:
            {
              SWP swp;

              WinQueryWindowPos(hwnd, &swp);
              /* Force resizing of controls. This will also cause a call to wizQueryPreviewHeight() */
              swp.fl=SWP_SIZE;
              WinSendMsg(hwnd, WM_WINDOWPOSCHANGED, &swp, 0);
              break;
            }
          case ACTION_SETPREVIEWOBJECT:
            {
              SWP swp;

              WinSetWindowULong(WinWindowFromID(hwnd, FLDRINFO_WINDOWID_PREVIEWCTRL), QWL_USER, LONGFROMMP(mp2));

              WinQueryWindowPos(hwnd, &swp);
              /* Force resizing of controls. This will also cause a call to wizQueryPreviewHeight() */
              swp.fl=SWP_SIZE;
              WinSendMsg(hwnd, WM_WINDOWPOSCHANGED, &swp, 0);
              break;
            }
          case HTML_SET_HANDPTR:
            {
              return (MRESULT) WinSendMsg(WinWindowFromID(hwnd, FLDRINFO_WINDOWID_TEXTCTRL),
                                          msg, mp1, mp2);
            }     
          default:
            break;
          }
        return MRFALSE;
      }
    case WM_COMMAND:
      return (MRESULT) WinSendMsg(WinQueryWindow(hwnd, QW_OWNER),
                                  msg, mp1, mp2);
      break;
    default:
      break;
  }

  if(oldProc)
    return oldProc( hwnd, msg, mp1, mp2);

  return WinDefWindowProc( hwnd, msg, mp1, mp2);
}

BOOL registerFldrInfoWnd(void)
{
  CLASSINFO ci;

  /* Register the new text control */
  if(WinQueryClassInfo(WinQueryAnchorBlock(HWND_DESKTOP),
                       WC_STATIC,
                       &ci))
    {
      ulQWP_FLDRINFOWNDDATA=ci.cbWindowData;
      oldProc=ci.pfnWindowProc;

      if(WinRegisterClass(WinQueryAnchorBlock(HWND_DESKTOP),
                          (PSZ)WC_FOLDERINFO,
                          folderInfoProc,
                          (ci.flClassStyle) &~(CS_PUBLIC|CS_PARENTCLIP),
                          ci.cbWindowData+sizeof(void*)))/* Pointer to private data added */
        {
          /* */
          return TRUE;
        }
    }
  return FALSE;
}









