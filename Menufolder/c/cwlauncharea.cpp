/*
 * This file is (C) Chris Wohlgemuth 1996/2007
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

#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_WIN
#define INCL_WINWORKPLACE
#define INCL_GPI

#include <os2.h>

#include "newfileDlg.h"
#include "except.h"

/* Include file with definition for WPShadow class */
//#include "wpshadow.hh"

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>//for strcpy
#include <stdio.h>   // for sprintf
#include <stdlib.h>
#include "defines.h"
#include "sys_funcs.h"

#define CW_LAUNCHAREA       "CW_LAUNCHAREA"
#define LAUNCHAREA_DATASIZE 4
/* Vars for launcharea class */
ULONG   g_ulStaticDataOffset=0;
PFNWP   g_pfnwpOrgStaticProc=NULLHANDLE;
ULONG   ulQWP_LAUNCHAREADATA=0;

static BOOL fInitCWLADone=FALSE;

#define LP_USEOBJECTASPARAM  0x00000001
#define WPSWIZARD_TBID "<WPSWIZARD_TOOLBAR>"

#define VIEW_LAUNCHAREA  WPMENUID_USER+0x1000
//#define VIEW_LAUNCHAREA  0x1000

#define  WM_NEWBUBBLE   WM_USER+100 //Use ATOM later
#define xButtonDelta 2
#define xButtonOffset 6

static PFNWP  oldButtonProc;  //place for original button-procedure
static HWND hwndShadow;// The handle of the help window
static PFNWP  oldProc; // For shadow proc

extern char chrTBFlyFontName[CCHMAXPATH];/* Font for toolbar fly over help */
extern RGB rgbTBFlyForeground;
extern RGB rgbTBFlyBackground;
extern BOOL bTBFlyOverEnabled;
extern int iTBFlyOverDelay;

static ULONG ulIconSize=40;
static HAB g_hab;
#include "cwlauncharea.hh"

static WPObject* _wpGetWPObjectFromPath(WPObject *wpObject, char* chrPath)
{
  HOBJECT hObject;

  hObject=WinQueryObject(chrPath);

  if(hObject!=NULLHANDLE) {
    //return ((M_WPObject*)wpObject->somGetClass())->wpclsObjectFromHandle(hObject); <-- only works for filesystem objects
      return ((M_WPObject*)wpObject->somGetClass())->wpclsQueryObject(hObject);
  }
  return NULLHANDLE;
}

static SOMClass* somGetSomClass(char* chrClassName)
{
  somId    mySomId;
  SOMClass *somClass;

  if((mySomId=somIdFromString(chrClassName))==NULLHANDLE)
    return NULLHANDLE;

  //  somClass=_somClassFromId(SOMClassMgrObject, mySomId);
  /* somFindClass() loads the class if not already done */
  somClass=SOMClassMgrObject->somFindClass( mySomId, 1, 1);
  SOMFree(mySomId);

  return somClass;
}

HMODULE queryModuleHandle(void);
cwSaveObjectList(CWLaunchArea* cwla, CWLAObject *cwlaObjectList, ULONG ulNumObjects);
BOOL cwFillPad(CWLaunchArea * somSelf, CWLAObject * cwlaObjectList, ULONG ulNumObjects, HWND hwndLA);
MRESULT EXPENTRY shadowProc(HWND hwnd, ULONG msg,MPARAM mp1,MPARAM mp2 );


extern USHORT id;// Last window the mouse was over 
extern HWND hwndBubbleWindow;// The handle of the help window
void _handleMouseMoveForFlyOver(HWND hwnd);
BOOL _handleTimerForFlyOver(HWND hwnd, USHORT usTimer);
void _createTheFlyOverWindow(HWND hwnd, PSZ chrText);
void _cleanupFlyOverTimers(HWND hwnd);

typedef BOOL (EXPENTRY FNWIZDRAW)(WPObject* somSelf, POWNERITEM pOwnerItem, 
                                   ULONG ulIconSize, 
                                   ULONG flCnr);
typedef FNWIZDRAW *PFNWIZDRAW;
static PFNWIZDRAW wizDrawIconProc;
BOOL fHaveWizDrawIconProc=FALSE;

typedef BOOL (EXPENTRY FNHASCAIROIMG)(WPObject* somSelf, ULONG ulIdx);
typedef FNHASCAIROIMG *PFNHASCAIROIMG;
static  PFNHASCAIROIMG wizQueryHasCairoImage;


static BOOL doDrawUserButton(HWND hwnd, HPS hps, BOOL fHilite)
{
  CWLAObject* lpo;
  
  lpo=(CWLAObject*)WinQueryWindowULong(hwnd,QWL_USER);
  if(somIsObj(lpo)) {
    WPObject *wpObject;
    
    wpObject=lpo->cwQueryWPObject();
    if(somIsObj(wpObject)) {
      RECTL rcl;
      MINIRECORDCORE mrec={0};
      CNRDRAWITEMINFO cdii={0};
      OWNERITEM oi={0};
      ULONG fl=0;
      
      /* Paint */
      WinQueryWindowRect(hwnd, &rcl);

      mrec.cb=sizeof(MINIRECORDCORE);
      cdii.pRecord=(PRECORDCORE)&mrec;
      
      oi.hwnd=hwnd;
      oi.hps=hps;
      oi.rclItem=rcl;
      oi.idItem=CMA_ICON;
      oi.hItem=(ULONG)&cdii;
#if 0                        
      if(!strcmp(wpObject->wpQueryTitle(), "Tutorial")){
        SysWriteToTrapLog("%s %d: %d\n", __FUNCTION__, __LINE__, rcl.xRight-rcl.xLeft);
      }
#endif
      WinFillRect(hps, &rcl, CLR_PALEGRAY /*0x00ff0000*/);
      if(fHilite)
        WinDrawBorder(hps, &rcl, 2, 2, 0, 0, 0x800);
#if 0
      else{
        POINTL ptl;
        WinQueryPointerPos(HWND_DESKTOP, &ptl);
        WinMapWindowPoints(HWND_DESKTOP, hwnd, &ptl, 1);
        if(WinPtInRect(g_hab, &rcl, &ptl)){
          WinDrawBorder(hps, &rcl,1,1,SYSCLR_WINDOWFRAME,SYSCLR_DIALOGBACKGROUND, DB_STANDARD);
        }
      }
#endif
      //  WinDrawBorder(hps, &rcl, 2, 2, 0, 0, 0x400);
      
      //wizDrawIconProc(wpObject, &oi, rcl.xRight-rcl.xLeft, fl);
      wizDrawIconProc(wpObject, &oi, /*ulIconSize */ rcl.xRight-rcl.xLeft-6, fl);
      
      return TRUE;
    }/* wpObject */
  }/* lpo */
  return FALSE;
}


/******************************************************************************/
/* New button procedure which handles the context menu stuff for the objects  */
/* and some other things I still have to document.                            */
/******************************************************************************/
static MRESULT EXPENTRY newLaunchAreaButtonProc(HWND hwnd, ULONG msg,MPARAM mp1,MPARAM mp2 )
{
  POINTL ptl;
  HWND hwndMenu;
  WPObject * wpObject;
  PDRAGINFO pDragInfo;
  static BOOL bDrawn=FALSE;

  switch (msg)
    {
    case 0x41E: /* WM_MOUSEENTER mp2 holds the other window*/
      {
        if(wizDrawIconProc){
          HPS hps;
          RECTL rcl;
          
          hps=WinGetPS(hwnd);
          WinQueryWindowRect(hwnd, &rcl);
          //WinDrawBorder(hps, &rcl,1,1,SYSCLR_WINDOWFRAME,SYSCLR_DIALOGBACKGROUND, DB_STANDARD);
          WinDrawBorder(hps, &rcl, 1, 1, 0, 0, 0x400);
          WinReleasePS(hps);
        }
        WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd , 2, (ULONG) iTBFlyOverDelay); // New timer for delay	
      break;
      }
    case 0x41F: /* WM_MOUSELEAVE mp2 holds the other window? */
      {
        // Mouse left frame
        WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, 1);  // stop the running timer
        if(hwndBubbleWindow)
          WinDestroyWindow(hwndBubbleWindow);/*  close the bubblewindow  */
        hwndBubbleWindow=NULLHANDLE;
        
        id=0;

        if(wizDrawIconProc){
          HPS hps;
          
          hps=WinGetPS(hwnd);
          doDrawUserButton(hwnd, hps, FALSE);
          WinReleasePS(hps);
        }
      break;
      }
    case WM_DESTROY:
      {
        _cleanupFlyOverTimers(hwnd);
        break;
      }
    case WM_NEWBUBBLE:
      {
        char winText[255];

        /* Get window text (It's the object title) for size calculating */
        WinQueryWindowText(hwnd,sizeof(winText),winText);
        _createTheFlyOverWindow(hwnd, winText);

        //WinStartTimer(WinQueryAnchorBlock(hwnd),hwnd,1,35); //Intervall timer
        return MRFALSE;
      }
#if 0
    case WM_MOUSEMOVE:
      {
        _handleMouseMoveForFlyOver(hwnd);
        break;
      }
#endif
    case WM_TIMER:
      {
        if( _handleTimerForFlyOver(hwnd, SHORT1FROMMP(mp1)))
          return MRFALSE;

        break; /* It's a system timer*/
      }
    case WM_CONTEXTMENU:
      /* User requests a context menu */
      if(!SHORT2FROMMP(mp2)) {
        CWLAObject* lpo;
        lpo=(CWLAObject*)WinQueryWindowULong(hwnd,QWL_USER);
        if(somIsObj(lpo)) {
          CWLaunchArea* cwla;
          /* Get the launch area of the button */
          cwla=lpo->cwQueryLaunchArea();
          if(somIsObj(cwla)) {
            wpObject=lpo->cwQueryWPObject();
            if(somIsObj(wpObject)) {

              if(!cwla->cwQueryLaunchAreaHoldsActionButtons()) {
                /* Full blown context menu */
                ptl.y=0;
                ptl.x=0;
                
                hwndMenu=wpObject->wpDisplayMenu(hwnd, hwnd,&ptl,MENU_OBJECTPOPUP,0);
              }
              else
                /* Action buttons only have a delete menu item */
                {
                  HWND hwndMenu;

                  hwndMenu=WinLoadMenu(HWND_DESKTOP, queryModuleHandle(), IDM_DELETEMENU); /* Load menu */
                  WinQueryPointerPos(HWND_DESKTOP, &ptl);
                  WinMapWindowPoints(HWND_DESKTOP, hwnd, &ptl, 1);
                  WinPopupMenu(hwnd, hwnd, hwndMenu, ptl.x, ptl.y, IDM_DELETEMENU, 
                              PU_MOUSEBUTTON1|/*PU_MOUSEBUTTON1DOWN|*/PU_KEYBOARD|PU_NONE|PU_SELECTITEM);
                }
            }
          }
          return (MRESULT) TRUE;
        }
      }/* end of if(!SHORT2FROMMP(mp2)) */
      break;

    case DM_DRAGOVER:
      {
        BOOL bReturn=FALSE;
        BOOL bBreak=FALSE;
        CWLAObject* lpo;
        
        TRY_LOUD(LP_DRAGOVER) {
          /* Get the button object */
          lpo=(CWLAObject*)WinQueryWindowULong(hwnd,QWL_USER);
          
          if(lpo) {
            CWLaunchArea* cwla;
            /* Get the launch area of the button */
            cwla=lpo->cwQueryLaunchArea();
            if(somIsObj(cwla)) {
              wpObject=cwla->cwQueryOwnerObject(WinQueryWindow(hwnd, QW_PARENT));
              if(somIsObj(wpObject))
                bBreak=TRUE; /* Don't drop on folder action buttons */
            }
            
            wpObject=lpo->cwQueryWPObject();
            if(somIsObj(wpObject) /* && !bBreak */ ) {
              char chrClassName[10];
              LONG lLength;
                            
              pDragInfo=(PDRAGINFO) mp1;
              hwndMenu=pDragInfo->hwndSource;
              
              lLength=sizeof(chrClassName);
              WinQueryClassName(hwndMenu, lLength, chrClassName);
              if(strcmp(chrClassName,"#37"))
                hwndMenu=NULLHANDLE;/* Source isn't a container */

              if(!bDrawn && wizDrawIconProc) {
                HPS hps;
                RECTL rectl;
                SWP swp;

                /* Draw the rect */
                hps=DrgGetPS(hwnd);
                WinQueryWindowPos(hwnd,&swp);
                rectl.xLeft=1;
                rectl.yBottom=1;
                rectl.xRight=swp.cx-1;
                rectl.yTop=swp.cy-1;
                WinDrawBorder(hps,&rectl, 1, 1, 0, 0, DB_STANDARD|DB_PATINVERT);  
                DrgReleasePS(hps);
                bDrawn=TRUE;
              }
              bReturn=TRUE;
            }
          }
          
        }
        CATCH(LP_DRAGOVER)
          {
          } END_CATCH;
        
        if(bReturn && !bBreak)
          return wpObject->wpDragOver(hwndMenu,(PDRAGINFO) mp1);
        break;
      }
    case DM_DRAGLEAVE:
      {
        RECTL rectl;
        SWP swp;
        HPS hps;
        
        /* Remove target emphasis here */
        hps=DrgGetPS(hwnd);
        if(wizDrawIconProc)
          doDrawUserButton(hwnd, hps, FALSE);
#if 0
        WinQueryWindowPos(hwnd,&swp);
        rectl.xLeft=1;
        rectl.yBottom=1;
        rectl.xRight=swp.cx-1;
        rectl.yTop=swp.cy-1;
        // WinDrawBorder(hps,&rectl,3, 3, 0, 0, DB_STANDARD|DB_PATINVERT);  
#endif
        bDrawn=FALSE;
        DrgReleasePS(hps);
        return (MRESULT) FALSE;
      }
    case DM_DROP:
      {
        BOOL bReturn=FALSE;
        char chrClassName[10];
        LONG lLength;
        PDRAGITEM pDragItem;
        CWLAObject* lpo;

        TRY_LOUD(LP_DROP) {
          /* A drop on a toolbar button */
          lpo=(CWLAObject*)WinQueryWindowULong(hwnd,QWL_USER);
          if(lpo) {
            wpObject=lpo->cwQueryWPObject();
            if(somIsObj(wpObject)) { 
              pDragInfo=(PDRAGINFO) mp1;
              pDragItem=DrgQueryDragitemPtr(pDragInfo, 0);
              hwndMenu=pDragInfo->hwndSource;
              lLength=sizeof(chrClassName);
              WinQueryClassName(hwndMenu, lLength, chrClassName);
              if(strcmp(chrClassName,"#37"))
                hwndMenu=NULLHANDLE;/* Source isn't a container */
              bReturn=TRUE;
            }
          }
        }
        CATCH(LP_DROP)
          {
          } END_CATCH;
          if(bReturn)
            return wpObject->wpDrop(hwndMenu,(PDRAGINFO) mp1, pDragItem);
          break;
      }
#if 0
    case WM_BEGINDRAG:
      {
        PDRAGINFO pDrgInfo;
        DRAGITEM drgItem;
        CWLAObject* lpo;
        BOOL bReturn=FALSE;

        pDrgInfo=DrgAllocDraginfo(1);

        TRY_LOUD(LP_BEGINDRAG) {
          /* A drop on a toolbar button */
          lpo=(CWLAObject*)WinQueryWindowULong(hwnd,QWL_USER);
          if(lpo) {
            wpObject=lpo->cwQueryWPObject();
            if(somIsObj(wpObject)) { 
              bReturn=TRUE;
            }
          }/* if(lpo) */
        }
        CATCH(LP_BEGINDRAG)
          {
          } END_CATCH;
          if(bReturn) {
            DRAGIMAGE dimg={0};

HSTR   hstr;             /* Handle to a string.  The handle must */
                         /* have been created with               */
                         /* DrgAddStrHandle.                     */
char    pBuffer[2000];          /* Buffer where the null-terminated     */
                         /* string is returned                   */
ULONG  ulStrlen;         /* String length                        */
ULONG  ulBytesRead;      /* Number of bytes read                 */
ULONG  rc;               /* Return code                          */


            wpObject->wpFormatDragItem(&drgItem);

            //            drgItem.hstrRMF=DrgAddStrHandle("<DRM_OBJECT,DRF_OBJECT>");
            //   SysWriteToTrapLog("%d %s %s %s %s\n", drgItem.ulItemID, drgItem.hstrType, drgItem.hstrRMF, drgItem.hstrSourceName, drgItem.hstrTargetName);
            SysWriteToTrapLog("%d \n", drgItem.ulItemID, drgItem.hstrType, drgItem.hstrRMF, drgItem.hstrSourceName, drgItem.hstrTargetName);
            ulStrlen = DrgQueryStrNameLen(drgItem.hstrType) + 1;
ulBytesRead = DrgQueryStrName(drgItem.hstrType,
                     ulStrlen,      /* Number of bytes to copy   */
                     pBuffer);
            SysWriteToTrapLog(" %s \n", pBuffer);

            ulStrlen = DrgQueryStrNameLen(drgItem.hstrRMF) + 1;
ulBytesRead = DrgQueryStrName(drgItem.hstrRMF,
                     ulStrlen,      /* Number of bytes to copy   */
                     pBuffer);
            SysWriteToTrapLog(" %s \n", pBuffer);

            ulStrlen = DrgQueryStrNameLen(drgItem.hstrContainerName) + 1;
ulBytesRead = DrgQueryStrName(drgItem.hstrContainerName,
                     ulStrlen,      /* Number of bytes to copy   */
                     pBuffer);
            SysWriteToTrapLog(" %s \n", pBuffer);

            ulStrlen = DrgQueryStrNameLen(drgItem.hstrSourceName) + 1;
ulBytesRead = DrgQueryStrName(drgItem.hstrSourceName,
                     ulStrlen,      /* Number of bytes to copy   */
                     pBuffer);
            SysWriteToTrapLog(" %s \n", pBuffer);

            ulStrlen = DrgQueryStrNameLen(drgItem.hstrTargetName) + 1;
ulBytesRead = DrgQueryStrName(drgItem.hstrTargetName,
                     ulStrlen,      /* Number of bytes to copy   */
                     pBuffer);
            SysWriteToTrapLog(" %s \n", pBuffer);

            drgItem.hwndItem=WinQueryWindow(WinQueryWindow(WinQueryWindow(hwnd,QW_PARENT), QW_PARENT),QW_PARENT);
            drgItem.hwndItem=WinQueryWindow(WinQueryWindow(hwnd,QW_PARENT), QW_PARENT);
SysWriteToTrapLog("hwnd: %x\n", drgItem.hwndItem);
pDrgInfo->hwndSource = drgItem.hwndItem;

            if(DrgSetDragitem(pDrgInfo,
                           &drgItem,
                           sizeof(DRAGITEM),
                           0L))
           
            /***********************************************************************/
            /*  Initialize the drag image.                                         */
            /***********************************************************************/
            dimg.cb       = sizeof (DRAGIMAGE);
            dimg.hImage   = WinQuerySysPointer (HWND_DESKTOP, SPTR_FILE, TRUE);
SysWriteToTrapLog("hImage: %x\n", dimg.hImage);
            dimg.fl       = DRG_ICON | DRG_TRANSPARENT;
            dimg.cxOffset = 0;
            dimg.cyOffset = 0; 

pDrgInfo->hwndSource = drgItem.hwndItem;
            if(DrgDrag(pDrgInfo->hwndSource, pDrgInfo, &dimg, 1, VK_ENDDRAG , NULL))


            //            DrgFreeDraginfo(pDrgInfo);
            return (MRESULT)TRUE;
          }
          else {
            DrgFreeDraginfo(pDrgInfo);
          }
        break;
      }
    case WM_ENDDRAG:
      {
        PDRAGINFO pDrgInfo;

        break;
      }
#endif
      /* Menu command on a button */
    case WM_COMMAND:
      {
        CWLAObject* lpo;

        TRY_LOUD(LP_COMMAND) {
          lpo=(CWLAObject*)WinQueryWindowULong(hwnd,QWL_USER);
          if(lpo) {
            wpObject=lpo->cwQueryWPObject();
            if(somIsObj(wpObject)) {
              if((SHORT)0x6d == SHORT1FROMMP(mp1)) {
                CWLaunchArea *cwla;
                /* Delete menu item */       
                if(wpObject->wpDelete(CONFIRM_DELETE|CONFIRM_DELETEFOLDER ) == OK_DELETE)
                  cwla=lpo->cwQueryLaunchArea();
                if(somIsObj(cwla))
                  cwla->cwRemoveObject(lpo, WinQueryWindow(hwnd, QW_PARENT));
              }
              else
                wpObject->wpMenuItemSelected( NULLHANDLE, SHORT1FROMMP(mp1));
            }/* end of if(somIsObj(wpObject)) */
          }
        }  
        CATCH(LP_COMMAND)
          {
          } END_CATCH;
          return (MRESULT) FALSE;
      }
    default:
      break;			
    }
  // call the standard flyover button procedure to handle the rest of the messages
  if(oldButtonProc)
    return (oldButtonProc)(hwnd,msg,mp1,mp2);	
  return WinDefWindowProc(hwnd,msg,mp1,mp2);	//We shouldn't end here...
};


static BOOL bDrawn=FALSE;
static RECTL rclDrawn;
static MRESULT handleDragOver(HWND hwnd, MPARAM mp1, MPARAM mp2)
{
  MRESULT mResult;
  ULONG ulNumberOfObjects;
  PDRAGINFO pDragInfo;
  HPS hps;
  RECTL rectl;
  SWP swp;

  mResult = MRFROM2SHORT( DOR_NODROPOP, DO_LINK);
  
  /* Get access to the dragInfo */
  pDragInfo=(PDRAGINFO)mp1;
  DrgAccessDraginfo(pDragInfo);
  /* Get number of items */
  ulNumberOfObjects = DrgQueryDragitemCount( pDragInfo);

  if(ulNumberOfObjects>1) {
    /* Free the draginfo */
    DrgFreeDraginfo(pDragInfo);
    return MRFROM2SHORT( DOR_NODROPOP, DO_LINK);
  }

  switch(pDragInfo->usOperation)
    {
    case DO_DEFAULT:
    case DO_LINK:
      if(!bDrawn) {
        POINTL ptl;
        CWLaunchArea * lp;
        
        ptl.x=SHORT1FROMMP(mp2);
        ptl.y=SHORT2FROMMP(mp2);
        /* Pos in window coords */
        WinMapWindowPoints(HWND_DESKTOP, hwnd, &ptl, 1);

        hps=DrgGetPS(hwnd);
        WinQueryWindowPos(hwnd,&swp);

        rectl.xLeft=swp.cx;

        lp=(CWLaunchArea*)WinQueryWindowULong(hwnd,QWL_USER);
        if(somIsObj(lp)) {
          if(lp->cwQueryNumObjects()==0) {
            rectl.xLeft=0;
          }
        }

        swp.cy-=xButtonDelta;        
        rectl.yTop=swp.cy-1;    
        rectl.yBottom=1;
        if(ptl.x<rectl.xLeft) {
          int numB;

          numB=(ptl.x-xButtonOffset)/(swp.cy+xButtonDelta);
          rectl.xLeft=((ptl.x-numB*swp.cy) > swp.cy/2 ? numB+1: numB);
          rectl.xLeft*=(swp.cy+xButtonDelta);
          rectl.xLeft-=(xButtonDelta)-xButtonOffset+xButtonDelta;
          rectl.xRight=rectl.xLeft+(xButtonDelta*2);
        }
        else {
          /* No button on the pad, yet */
          rectl.xRight=swp.cx-1;
        }
        rclDrawn=rectl;
        WinDrawBorder(hps,&rectl, 2, 2, 0, 0, DB_STANDARD|DB_PATINVERT);  
        bDrawn=TRUE; 
        
        DrgReleasePS(hps);
      }
      mResult = MRFROM2SHORT( DOR_DROP, DO_LINK);
      break;
    default:
      break;
    }
  /* Free the draginfo */
  DrgFreeDraginfo(pDragInfo);

  return mResult;
}
static MRESULT handleDragLeave(HWND hwnd, MPARAM mp1, MPARAM mp2)
{
  RECTL rectl;
  SWP swp;
  HPS hps;

  /* Remove target emphasis here */
  hps=DrgGetPS(hwnd);
  WinDrawBorder(hps,&rclDrawn, 2, 2, 0, 0, DB_STANDARD|DB_PATINVERT);  
  DrgReleasePS(hps);
  bDrawn=FALSE; 
  return (MRESULT) FALSE;
}


static MRESULT EXPENTRY fnwpLaunchAreaProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
  CWLaunchArea *somSelf;
  static USHORT id=0;//// Last button the mouse was over 

  switch(msg)
    {
    case WM_CONTROL:
      {
        switch(SHORT2FROMMP(mp1))
          {
            /* The buttons are created as ownerdrawn */
            case BN_PAINT:
              {
                /* wizDrawIconProc is queried in cwFillPad() */
                if(wizDrawIconProc)
                  {
                    PUSERBUTTON pub=(PUSERBUTTON)PVOIDFROMMP(mp2);

                    if(doDrawUserButton(pub->hwnd, pub->hps, (pub->fsState & BDS_HILITED)!=NULL))
                      return (MRESULT)FALSE;
                  }/* wizDrawIcon */
                break;
              }
          default:
            break;
          }
        break;
      }
    case WM_NEWBUBBLE:
      {
        CWLaunchArea * lp;

        lp=(CWLaunchArea*)WinQueryWindowULong(hwnd,QWL_USER);
        if(somIsObj(lp))
          _createTheFlyOverWindow(hwnd, lp->cwQueryFlyOverText());
        
        WinStartTimer(WinQueryAnchorBlock(hwnd),hwnd,1,35); //Intervall timer
        return MRTRUE;
      }
    case WM_MOUSEMOVE:
      {
        USHORT  tempID;
        CWLaunchArea * lp;

        lp=(CWLaunchArea*)WinQueryWindowULong(hwnd,QWL_USER);
        if(somIsObj(lp) && !lp->cwQueryNumObjects())
          _handleMouseMoveForFlyOver(hwnd);
     
        break;
      }
    case WM_TIMER:
      {
        if( _handleTimerForFlyOver(hwnd, SHORT1FROMMP(mp1)))
          return MRFALSE;

        break; /* It's a system timer*/
      }
    case DM_DRAGOVER:
      return handleDragOver(hwnd, mp1, mp2);
    case DM_DRAGLEAVE:
      return handleDragLeave(hwnd, mp1, mp2);
      break;
    case DM_DROP:
      {
        ULONG ulCount;
        ULONG ulNumberOfObjects;
        PDRAGITEM pDragItem;
        SOMClass *folderClass;
        WPObject * wpObject;
        PDRAGINFO pDragInfo;
       
        TRY_LOUD(LP_FRAMEDROP) { 
          /* A new object dropped on the launchpad */
          pDragInfo=(PDRAGINFO)mp1;
          if(DrgAccessDraginfo(pDragInfo)) {
            /* Get number of items */
            ulNumberOfObjects = DrgQueryDragitemCount( pDragInfo);           
            if(ulNumberOfObjects>1){
              /* Free the draginfo */
              DrgDeleteDraginfoStrHandles(pDragInfo);
              DrgFreeDraginfo(pDragInfo);
            }
            else { 
              ulCount=0;
              
              pDragItem=DrgQueryDragitemPtr( pDragInfo, ulCount);
              wpObject=(WPObject*)OBJECT_FROM_PREC(DrgQueryDragitemPtr( pDragInfo, ulCount)->ulItemID);
              somSelf=(CWLaunchArea*) WinQueryWindowULong(hwnd,QWL_USER);
              if(somIsObj(somSelf)) {
                if(somIsObj(wpObject)) {
                  POINTL ptl;
                  int numB;
                  SWP swp;
                  
                  WinQueryWindowPos(hwnd,&swp);
                  ptl.x=pDragInfo->xDrop;
                  ptl.y=pDragInfo->yDrop;
                  /* Pos in window coords */
                  WinMapWindowPoints(HWND_DESKTOP, hwnd, &ptl, 1);
                  numB=(ptl.x-xButtonOffset)/(swp.cy+xButtonDelta);
                  numB=((ptl.x-numB*swp.cy) > swp.cy/2 ? numB+1: numB);

                  /* Do a link */
                  somSelf->cwAddObject(wpObject, numB, hwnd);
                  handleDragLeave(hwnd, mp1, mp2);
                }
              }
              DrgDeleteDraginfoStrHandles(pDragInfo);
              DrgFreeDraginfo(pDragInfo);
            }
          }
          handleDragLeave(hwnd, mp1, mp2);
        }
        CATCH(LP_FRAMEDROP)
          {
          } END_CATCH;
          break;
      }
    case WM_PAINT:
      {
        RECTL rcl;
        CWLaunchArea * lp;
        HPS hps=WinBeginPaint(hwnd,NULLHANDLE, &rcl);

        WinFillRect(hps, &rcl, SYSCLR_DIALOGBACKGROUND);
        lp=(CWLaunchArea*)WinQueryWindowULong(hwnd,QWL_USER);
        if(somIsObj(lp)) {
            WinQueryWindowRect(hwnd,&rcl);
            WinDrawBorder(hps,&rcl,1,1,SYSCLR_WINDOWFRAME,SYSCLR_DIALOGBACKGROUND, DB_STANDARD);
        }
        WinEndPaint(hps);
        return (MRESULT) 0;
      }
      /*****************************************************/
      /* Make sure tabbing works.                          */
      /*****************************************************/
    case WM_CHAR:
      if(SHORT1FROMMP(mp1) & KC_ALT) {  // Send keys with ALT modifier to the folder
        HWND hwndTemp;
        // Save control with focus
        hwndTemp=WinQueryFocus(HWND_DESKTOP);
        // Set focus to container so the key will be processed as usual
        WinSetFocus(HWND_DESKTOP, WinWindowFromID(WinQueryWindow(WinQueryWindow(hwnd,QW_PARENT), QW_PARENT), FID_CLIENT));
        // Set saved control as old focus owner so it will get the focus back when necessary, not the container
        WinSetWindowULong(WinQueryWindow(hwnd,QW_PARENT), QWL_HWNDFOCUSSAVE, hwndTemp);
        return WinSendMsg(WinWindowFromID(WinQueryWindow(hwnd, QW_PARENT), FID_CLIENT),msg,mp1,mp2);
      }
      else if((SHORT1FROMMP(mp1) & KC_VIRTUALKEY)== KC_VIRTUALKEY)
        {
          if(SHORT2FROMMP(mp2)==VK_TAB ) {
            //  if(WinQueryFocus(HWND_DESKTOP)==WinWindowFromID(hwnd,IDSL_POSBAR)) {
            // Set focus to container
            WinSetFocus(HWND_DESKTOP, WinWindowFromID(WinQueryWindow(WinQueryWindow(hwnd,QW_PARENT), QW_PARENT),
                                                      FID_CLIENT));
            return (MRESULT)TRUE;
              //}
          }/* if(SHORT2FROMMP(mp2)==VK_TAB ) */
          else if(SHORT2FROMMP(mp2)==VK_BACKTAB )
            {
                // Set focus to container
              WinSetFocus(HWND_DESKTOP, WinWindowFromID(WinQueryWindow(WinQueryWindow(hwnd,QW_PARENT),QW_PARENT),
                                                        FID_CLIENT));
              return (MRESULT)TRUE;
            }
        }/* else if(SHORT1FROMMP(mp1) & KC_VIRTUALKEY) */
      break;
    case WM_COMMAND:
      /* Button clicked */
      if(SHORT1FROMMP(mp2)==CMDSRC_PUSHBUTTON) {
        CWLAObject* lpo;
        /* It's a push button */
        lpo=(CWLAObject*)WinQueryWindowULong(WinWindowFromID(hwnd,SHORT1FROMMP(mp1)),QWL_USER);
        if(somIsObj(lpo)) {
          CWLaunchArea *cwla;
          cwla=lpo->cwQueryLaunchArea();
          if(somIsObj(cwla))
            {
              WPObject *wpObject;
              wpObject=cwla->cwQueryOwnerObject(hwnd);
              if(wpObject) {
                if(somIsObj(wpObject))
                  if(somIsObj(lpo->cwQueryWPObject()))
                    cwla->mfExecuteRexxFile( (WPProgram*)lpo->cwQueryWPObject(), wpObject, hwnd);

                return (MRESULT)FALSE;
              }
            }
          if(somIsObj(lpo->cwQueryWPObject()))
            lpo->cwQueryWPObject()->wpViewObject(NULLHANDLE, OPEN_DEFAULT,0);
        }
      }
      return (MRESULT)FALSE;
    case WM_CLOSE:
      WinDestroyWindow(hwnd);
      //      WinPostMsg(hwnd, WM_DESTROY, 0, 0);
      return MRFALSE;
    case WM_DESTROY:
      {
        somSelf=(CWLaunchArea*)WinQueryWindowULong(hwnd, QWL_USER);
        if(somIsObj(somSelf)) {
          CWLAVIEW* cwlaView;
           cwlaView=(CWLAVIEW*)WinQueryWindowULong(hwnd, ulQWP_LAUNCHAREADATA);
           if(cwlaView) {
             somSelf->wpDeleteFromObjUseList((PUSEITEM)cwlaView);
             somSelf->wpFreeMem((PBYTE)cwlaView);
           }
        }
        break;
      }
    default:
      break;
    }

  if(g_pfnwpOrgStaticProc)
     return  g_pfnwpOrgStaticProc(hwnd, msg, mp1, mp2);
  return WinDefWindowProc(hwnd, msg, mp1, mp2);
}


BOOL CWLaunchArea::cwBuildObjectList()
{

  ULONG ul;
  SOMClass *folderClass;
  char chrPath[CCHMAXPATH];
  ULONG ulBufferSize;
  HINI hIni;
  char * memPtr;
  HOBJECT *hObjectArray;
  int a;
  CWLAObject *lpoTemp;
  WPObject * wpTempObject;

  if(cwlaObjectList) /* List is already built */
    return TRUE;

  folderClass=somGetClass();

  /* Build ini name */
  ulBufferSize=sizeof(chrPath);
  wpQueryRealName( chrPath, &ulBufferSize, TRUE);
  strcat(chrPath,"\\objects.ini");/* Ini-File containing the hobjects */
  
  do{
    /* Open the ini-file */
    if((hIni=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),chrPath))==NULLHANDLE)
      break; 
 
    if(!PrfQueryProfileSize(hIni,"objects","handles", &ulBufferSize))
      break;

    if(ulBufferSize==0)
      break;/* No entries yet */

    if((memPtr=(char*)malloc(ulBufferSize))==NULL)
      break;
    
    ulNumObjects=ulBufferSize/sizeof(HOBJECT);    

    if(!PrfQueryProfileData(hIni,"objects","handles", memPtr, &ulBufferSize)){
      free(memPtr);
      break;
    }

    hObjectArray=(HOBJECT*)memPtr;
    ulBufferSize=ulNumObjects;

    for(a=0;a<ulBufferSize /*&& lpoTemp*/; a++)
      {
        wpTempObject=((M_WPObject*)folderClass)->wpclsQueryObject( hObjectArray[a]);
        if(somIsObj(wpTempObject)) {
          if((lpoTemp=new CWLAObject)!=NULL) {
            wpTempObject->wpLockObject();          /* Make sure object doesn't go dormant */
            lpoTemp->cwSetWPObject( wpTempObject);
            lpoTemp->cwSetObjectHandle( hObjectArray[a]);

            /* Title of the object */
            lpoTemp->cwSetLaunchArea(this);
            lpoTemp->cwSetNextObject(cwlaObjectList);
            cwlaObjectList=lpoTemp;
          }
        }
        else {
          ulNumObjects--;
        }
      }/* for */

    free(hObjectArray);
    
    PrfCloseProfile(hIni);
    return TRUE;
  }while(TRUE);

  if(hIni)
    PrfCloseProfile(hIni);
  return FALSE;
}

BOOL CWLaunchArea::cwRemoveObject( CWLAObject* cwlaObject, HWND hwndLA)
{
  int a;
  CWLAObject* lpoTemp, *lpoTemp2;

  TRY_LOUD(REMOVEOBJECT) {
    /* Find object in list */
    lpoTemp=cwlaObjectList;
    
    if(lpoTemp==cwlaObject) {
      cwlaObjectList=lpoTemp->cwQueryNextObject();
      if(somIsObj(cwlaObject->cwQueryWPObject()))
        ((WPObject*)cwlaObject->cwQueryWPObject())->wpUnlockObject();
      delete(cwlaObject);
      ulNumObjects--;
    }
    else {
      while(lpoTemp->cwQueryNextObject())
        {
          if(lpoTemp->cwQueryNextObject()==cwlaObject) {
            lpoTemp->cwSetNextObject(cwlaObject->cwQueryNextObject());
            if(somIsObj(cwlaObject->cwQueryWPObject()))
              ((WPObject*)cwlaObject->cwQueryWPObject())->wpUnlockObject();
            delete(cwlaObject);
            ulNumObjects--;
            break;
          }
          lpoTemp=lpoTemp->cwQueryNextObject();
        }
    }
    
    cwSaveObjectList(this, cwlaObjectList, ulNumObjects);   
    cwRefreshLaunchArea(hwndLA);
  }
  CATCH(REMOVEOBJECT)
    {
      SysWriteToTrapLog("Trap in %s. hwndLA is: %x, cwlaObject is: %x\n", __FUNCTION__, hwndLA, cwlaObject);
    } END_CATCH;


  return TRUE;
}


BOOL CWLaunchArea::cwAddObject( WPObject* wpObject, int iPos, HWND hwndLA)
{
  HOBJECT hObject;
  SOMClass *folderClass;
  char chrSetup[CCHMAXPATH];
  WPObject *wpTempObject;
  CWLAObject *lpoTemp, *lpoTemp2, *lpoTempNew;
  int a;


  TRY_LOUD(ADDOBJECT) {
    if(somIsObj(wpObject)) {
      folderClass=somGetClass();
      if(somIsObj(folderClass)) {
        /* Create a shadow in our toolbar folder */
        wpTempObject=wpObject->wpCreateShadowObject(this, FALSE);
        if(somIsObj(wpTempObject)) {
          /* Get the persistent object handle */
          hObject=wpTempObject->wpQueryHandle();


          /* Find the insertion point */
          /* iPos is the button after which we insert the new one. The list of objects is in reverse order. */
          lpoTemp2=cwlaObjectList;

          if(iPos<ulNumObjects) {
            lpoTemp=lpoTemp2;
            for(a=ulNumObjects; a>iPos && lpoTemp; a--)
              {
                lpoTemp=lpoTemp2;
                lpoTemp2=lpoTemp2->cwQueryNextObject();
              }
          }
          else
            lpoTemp=NULLHANDLE;

          /* lpoTemp contains object */
          /* Alloc a struct for this object to insert it on the toolbar */
          if((lpoTempNew=new CWLAObject)!=NULL) {
            ULONG ulRC;
            wpTempObject->wpLockObject();          /* Make sure object doesn't go dormant */

            lpoTempNew->cwSetWPObject(wpTempObject);
            lpoTempNew->cwSetObjectHandle(hObject);
            
            /* Set the pointer to the launch area */
            lpoTempNew->cwSetLaunchArea(this);

            if(lpoTemp) {
              lpoTempNew->cwSetNextObject(lpoTemp->cwQueryNextObject());
              lpoTemp->cwSetNextObject(lpoTempNew);
            }
            else {
              lpoTempNew->cwSetNextObject(cwlaObjectList);
              cwlaObjectList=lpoTempNew;
            }
            ulNumObjects++;
          }
        }
      }
    }    
    cwSaveObjectList(this, cwlaObjectList, ulNumObjects);   
    cwRefreshLaunchArea(hwndLA);
  }
  CATCH(ADDOBJECT)
    {
      SysWriteToTrapLog("Trap in %s. hwndLA is: %x, wpObject is: %x\n", __FUNCTION__, hwndLA, wpObject);
    } END_CATCH;

  return TRUE;
}

cwSaveObjectList(CWLaunchArea* cwla, CWLAObject *cwlaObjectList, ULONG ulNumObjects)
{
  SOMClass *folderClass;
  char chrPath[CCHMAXPATH];
  ULONG ulBufferSize;
  HINI hIni;
  char * memPtr;
  HOBJECT *hObject;
  int a;
  CWLAObject *lpoTemp;

  ulBufferSize=sizeof(chrPath);
  cwla->wpQueryRealName( chrPath, &ulBufferSize, TRUE);
  strcat(chrPath,"\\objects.ini");/* Ini-File containing the hobjects */

  //  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, chrPath, "launchPad", 123, MB_OK| MB_MOVEABLE);  
  do{
    /* Open the ini-file */
    if((hIni=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),chrPath))==NULLHANDLE)
      break; 

    if((memPtr=(char*)malloc(ulNumObjects*sizeof(HOBJECT)))==NULL)
      break;
    
    hObject=(HOBJECT*)memPtr;

    lpoTemp=cwlaObjectList;
    for(a=ulNumObjects;a>0 && somIsObj(lpoTemp); a--)
      {
        hObject[a-1]=lpoTemp->cwQueryObjectHandle();
        lpoTemp=lpoTemp->cwQueryNextObject();
      }

    if(!PrfWriteProfileData(hIni,"objects","handles", hObject, ulNumObjects*sizeof(HOBJECT))) {
      free(hObject);
      break;
    }
    free(hObject);
    
    PrfCloseProfile(hIni);
    return TRUE;
  }while(TRUE);

  if(hIni)
    PrfCloseProfile(hIni);

  return FALSE;
}

void CWLaunchArea::cwRefreshLaunchArea(HWND hwndLA)
{
  /* Refresh all launchpads */
  CWLAVIEW* cwlaView;

  TRY_LOUD(REFRESH) {
  
    cwFreeObjectList();
    cwBuildObjectList();
    
    for(cwlaView=(CWLAVIEW*)wpFindUseItem(USAGE_OPENVIEW, NULLHANDLE) ; cwlaView ; 
        cwlaView=(CWLAVIEW*)wpFindUseItem(USAGE_OPENVIEW, (PUSEITEM)cwlaView)) {
      if(cwlaView->viewItem.view==VIEW_LAUNCHAREA)
        cwFillPad(this, cwlaObjectList, ulNumObjects, cwlaView->viewItem.handle);
    }
  }
  CATCH(REFRESH)
    {
      SysWriteToTrapLog("Trap in %s. HWND is: %x\n", __FUNCTION__, hwndLA);
    } END_CATCH;
    
  return;
}


void CWLaunchArea::cwFreeObjectList()
{
    CWLAObject *lpoTemp, *lpoTemp2;

    /* Free our object list */
    lpoTemp=cwlaObjectList;

    while(somIsObj(lpoTemp)) {
      WPObject * wpObject;
      lpoTemp2=lpoTemp->cwQueryNextObject();

      wpObject=lpoTemp->cwQueryWPObject();
      if(somIsObj(wpObject))
         wpObject->wpUnlockObject();
      delete(lpoTemp);
      lpoTemp=lpoTemp2;
    }
    cwlaObjectList=NULL;
}

void CWLaunchArea::cwSetFlyOverText(PSZ pText)
{
  
  if(pszFlyOver)
    wpFreeMem((PBYTE)pszFlyOver);
  pszFlyOver=NULLHANDLE;

  if(!pText || !strlen(pText))
      return;

  if((pszFlyOver=wpAllocMem(strlen(pText)+1, NULL))!=NULLHANDLE)
    strcpy(pszFlyOver, pText);

}
 
PSZ CWLaunchArea::cwQueryFlyOverText()
{
  return pszFlyOver;
}

/*
  Remove all buttons from a launch area. Doesn't free the
  associated objects. Object freeing is done in ::cwFreeObjectList().
 */
static BOOL  cwClearPad(HWND hwndLA)
{
  HENUM hEnum;
  HWND hwnd;

  /* Destroy all Buttons execept those at the top */
  hEnum=WinBeginEnumWindows(hwndLA);
  while((hwnd=WinGetNextWindow(hEnum))!=NULLHANDLE) {
    SWP swp;
    WinQueryWindowPos(hwnd, &swp);
    if(swp.y < 20) /* Not titlebar and other frame controls */
      WinDestroyWindow(hwnd);
  }
  WinEndEnumWindows(hEnum);

  return TRUE;
}

/*
  Fill a launch area with buttons. Each button represents an
  object of type CWLAObject. The objects are already allocated
  in ::cwBuildList().   
 */
BOOL cwFillPad(CWLaunchArea * somSelf, CWLAObject * cwlaObjectList, ULONG ulNumObjects, HWND hwndLA)
{
  HOBJECT hObject;
  char chrSetup[CCHMAXPATH];
  WPFolder* wpFolder;
  CWLAObject *lpoTemp;
  SWP swp;
  HWND hwndTemp;
  ULONG ulStyle;
  int a;
  LONG lXSize;
  LONG lCy;
  HWND hwndTitlebar;
  static fCheckedIsCWThemeObject=FALSE;
  
  hwndTitlebar=WinWindowFromID(hwndLA, FID_TITLEBAR);

  if(!WinQueryWindowPos(hwndTitlebar, &swp))
    lCy=0;
  else
    lCy=swp.cy; /* Titlebar size */

  /* Remove Buttons from pad */
  cwClearPad(hwndLA);

  /* Resize the pad for all objects */
  if(!WinQueryWindowPos(hwndLA, &swp))
    return FALSE;

  swp.cy-=xButtonDelta;
  lXSize=(ulNumObjects == 0 ? swp.cy : (ulNumObjects) * (swp.cy-lCy+xButtonDelta) + xButtonOffset);
  somSelf->cwSetLaunchAreaPos(hwndLA, HWND_TOP, 0, 0, lXSize , swp.cy+xButtonDelta, SWP_SIZE|SWP_ZORDER);
  swp.cy-=lCy;
  lpoTemp=cwlaObjectList;

  /* We use one of the objects from the launchpad here because if using somSelf we would get the
     proc address for folders (CWLaunchArea is a folder class) while objects on the launchpad are
     shadows. Only inside the shadowclass there is some special code to handle launchpad
     object painting.
  */
  if(!fCheckedIsCWThemeObject && lpoTemp)
    {  
      WPObject* wpObject;

      wpObject=lpoTemp->cwQueryWPObject();

      if(somIsObj(wpObject))
        {
          if(somSelf->somIsA(somGetSomClass("CWObject")))
            {
              //SysWriteToTrapLog("%s %d %x\n", __FUNCTION__, __LINE__, wizDrawIconProc);
              /* Get method for icon painting */
              wizDrawIconProc=(PFNWIZDRAW)somResolveByName(wpObject, "wizDrawIcon");
              if(wizDrawIconProc)
                fHaveWizDrawIconProc=TRUE;
              wizQueryHasCairoImage=(PFNHASCAIROIMG)somResolveByName(wpObject, "wizQueryHasCairoImage");
            }
          fCheckedIsCWThemeObject=TRUE;
        }/* wpObject */
    }

  for(a=ulNumObjects;a>0 && somIsObj(lpoTemp); a--)
    {
      ULONG ulCreateFlags;

      if(wizDrawIconProc)
        ulCreateFlags=WS_VISIBLE | BS_USERBUTTON;
      else
        ulCreateFlags=WS_VISIBLE;

      /* Create the Buttons */ 
      if((hwndTemp=WinCreateWindow(hwndLA, WC_BUTTON , (lpoTemp->cwQueryWPObject())->wpQueryTitle(), 
                                   ulCreateFlags, xButtonOffset+(a-1)*(swp.cy+xButtonDelta),
                                   2, swp.cy-2, swp.cy-2, hwndLA, HWND_TOP, 200+a,
                                   NULLHANDLE, NULLHANDLE))!=NULLHANDLE)
        {
          BTNCDATA btCtrl;
          WNDPARAMS wndParams;
          /* Change Style to mini icon */
          ulStyle=WinQueryWindowULong(hwndTemp,QWL_STYLE);
          ulStyle|=(BS_MINIICON);
          ulStyle&=~BS_TEXT;

          if(WinSetWindowULong(hwndTemp, QWL_STYLE, ulStyle)) {
            memset(&btCtrl, 0, sizeof(btCtrl));
            btCtrl.cb=sizeof(btCtrl);
            btCtrl.hImage=lpoTemp->cwQueryObjectIcon();
            memset(&wndParams, 0, sizeof(wndParams));
            wndParams.fsStatus=WPM_CTLDATA;
            wndParams.cbCtlData=btCtrl.cb;
            wndParams.pCtlData=&btCtrl;
            WinSendMsg(hwndTemp,WM_SETWINDOWPARAMS,
                       MPFROMP(&wndParams),0);
            /* Subclass button for flyover help */
            oldButtonProc=WinSubclassWindow(hwndTemp, newLaunchAreaButtonProc);
            /* Set the class pointer into the window words of the button */
            WinSetWindowULong(hwndTemp,QWL_USER,(ULONG)lpoTemp);//Save object ptr.
          }
        }
      lpoTemp=lpoTemp->cwQueryNextObject();
    }
  WinInvalidateRect(hwndLA, NULLHANDLE, TRUE);
  return TRUE;  
}

void CWLaunchArea::cwShowLaunchArea(HWND hwndLA, BOOL bShow)
{
  WinShowWindow(hwndLA, bShow);
}

/*
  Create a launcharea window. This call doesn't create the object but only the window holding the buttons and
  the buttons itself.

  if fl==1: the launcharea holds action buttons.
  if fl==0: the launcharea is a normal launchpad.

*/

HWND CWLaunchArea::cwCreateLaunchArea(HWND hwndParent, HWND hwndOwner, LONG x, LONG y, LONG cx, LONG cy, ULONG fl_)
{
  int a;
  HWND hwndLaunchArea; /* This is the frame window handle of the launchpad */

  /* Try to create the frame window */
  if((hwndLaunchArea=WinCreateWindow( hwndParent , CW_LAUNCHAREA, "Toolbar", FS_BORDER | FS_NOBYTEALIGN/*|WS_VISIBLE*/,
                                    x, y, cx, cy, hwndOwner, HWND_TOP, 1234, NULLHANDLE, NULLHANDLE))!=NULLHANDLE) {
    FRAMECDATA fctl={0};
    ULONG ulNum=0;
    WPObject *wpObj;
    CWLAVIEW* cwlaView;
    PVIEWITEM pv;

    if(fl_ & 0x00000001)
      fl|=CWLA_HOLDS_ACTIONBUTTONS;

    WinSetWindowULong(hwndLaunchArea, QWL_USER,(ULONG) this);//Save object ptr.	

    /* Create the struct for the open view list */
    if((cwlaView=(CWLAVIEW*)wpAllocMem(sizeof(CWLAVIEW), NULL))!=NULLHANDLE) {
      char title[100]="4321";

      memset(cwlaView, 0, sizeof(CWLAVIEW));
      cwlaView->useItem.type=USAGE_OPENVIEW;
      cwlaView->viewItem.handle=hwndLaunchArea;
      cwlaView->viewItem.view=VIEW_LAUNCHAREA;
      wpAddToObjUseList((PUSEITEM)cwlaView);
      WinSetWindowULong(hwndLaunchArea, ulQWP_LAUNCHAREADATA,(ULONG) cwlaView);//Save usage item  ptr.	
      
      WinSetWindowPos(hwndLaunchArea, HWND_TOP, 0, 0, 0, 0, SWP_ZORDER);
    }
    
    cwBuildObjectList();
    cwRefreshLaunchArea(hwndLaunchArea);
  }

  return hwndLaunchArea;
}


ULONG CWLaunchArea::cwQueryNumObjects()
{
  return ulNumObjects;
}

BOOL CWLaunchArea::cwSetLaunchAreaPos(HWND hwndLA, HWND hwndInsertBehind, LONG x, LONG y, LONG cx, LONG cy, ULONG fl)
{
      return WinSetWindowPos(hwndLA, hwndInsertBehind, x, y, cx, cy, fl);
}

void CWLaunchArea::cwSetOwnerObject(HWND hwndLA, WPObject *wpObject)
{
  CWLAVIEW* cwlaView;

  for(cwlaView=(CWLAVIEW*)wpFindUseItem(USAGE_OPENVIEW, NULLHANDLE) ; cwlaView ; 
      cwlaView=(CWLAVIEW*)wpFindUseItem(USAGE_OPENVIEW, (PUSEITEM)cwlaView)) {
    if(cwlaView->viewItem.view==VIEW_LAUNCHAREA && cwlaView->viewItem.handle==hwndLA) {
      cwlaView->wpOwnerObject=wpObject;
      break;
    }
  }
}

/* Only action buttons have an owner object */
WPObject *CWLaunchArea::cwQueryOwnerObject(HWND hwndLA)
{
  CWLAVIEW* cwlaView;

  for(cwlaView=(CWLAVIEW*)wpFindUseItem(USAGE_OPENVIEW, NULLHANDLE) ; cwlaView ; 
      cwlaView=(CWLAVIEW*)wpFindUseItem(USAGE_OPENVIEW, (PUSEITEM)cwlaView)) {
    if(cwlaView->viewItem.view==VIEW_LAUNCHAREA && cwlaView->viewItem.handle==hwndLA) {
      return cwlaView->wpOwnerObject;
    }
  }
  return NULLHANDLE;
}

BOOL CWLaunchArea::cwQueryLaunchAreaHoldsActionButtons()
{
  return (fl & CWLA_HOLDS_ACTIONBUTTONS);
}

void CWLaunchArea::wpInitData()
{
  // Init parent 
  CWMenuFolder::wpInitData();

  fl=0;

  cwSetFlyOverText("Drop objects here");

  /* Create the list of objects for this launch area in memory */
  cwBuildObjectList();
}

void CWLaunchArea::wpUnInitData()
{
  /**/
  cwFreeObjectList();

  if(pszFlyOver)
    wpFreeMem((PBYTE)pszFlyOver);

  CWMenuFolder::wpUnInitData();
}

HWND CWLaunchArea::wpOpen(HWND hwndCnr, ULONG ulView, ULONG ulParam)
{
  HWND hwndLaunchArea; /* This is the frame window handle of the launchpad */
  HWND hwndParent=HWND_DESKTOP;
  HWND hwndOwner=hwndCnr;

  if(ulView==VIEW_LAUNCHAREA)
    return NULLHANDLE;
#if 0
  BOOL bVisible=TRUE;
  int a;
  if((hwndLaunchArea=WinCreateWindow(hwndParent , CW_LAUNCHAREA, "Toolbar", FS_BORDER | (bVisible ? WS_VISIBLE: 0),
                                    0, 0, 30, 60, hwndOwner, HWND_TOP, 1234, NULLHANDLE, NULLHANDLE))!=NULLHANDLE) {    
    cwBuildObjectList();
    cwRefreshLaunchArea(hwndLaunchArea);
  }
  hwndLaunchArea=cwCreateLaunchArea(hwndParent, hwndOwner, 0, 0, 30, 60, 0);
  cwShowLaunchArea(hwndLaunchArea, TRUE);
#endif

  return CWMenuFolder::wpOpen( hwndCnr, ulView, ulParam);
}


BOOL CWLaunchArea::wpSetup(PSZ pSetupString)
{
  ULONG bufferSize;
  somId mySomId;

  /* First query size of setup string */  
  if(wpScanSetupString(pSetupString, "FPOBJECTS", NULL, &bufferSize))
    { 
      char* chrBuffer;
      if((chrBuffer=(char*)malloc(bufferSize))!=NULLHANDLE) /* Using malloc() here because wpAllocMem() may block if
                                                      called while any other thread is accessing the objects
                                                      inuse list. This way a possible deadlock is prevented. */
        {
          if(wpScanSetupString(pSetupString, "FPOBJECTS", chrBuffer, &bufferSize))
            {
              char * token;
              /* Now parse the objects */
              token=strtok(chrBuffer, ",");
              do {
               WPObject*  wpObject=_wpGetWPObjectFromPath(this, token);
                /* Found a path or ID */
               if(somIsObj(wpObject)) {
                  cwAddObject( wpObject, 1000, NULLHANDLE);
                  wpObject->wpUnlockObject(); //Object is locked because of wpclsQueryObject()
               }
              }while((token=strtok(NULL, ","))!=NULL);
            }
          free(chrBuffer);
        }
    }

  if((mySomId=somIdFromString("wpSetup"))!=NULLHANDLE) {
    BOOL rc;

    rc=((somTD_WPObject_wpSetup)
        somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                            1,
                            __ClassObject->    
                            somGetMethodToken(mySomId))                    
        )(this, pSetupString);
    SOMFree(mySomId);
    return rc;
  }
  else 
    return CWMenuFolder::wpSetup(pSetupString);

}

/*
We don't wnat the class choose page of the menu folder.
 */
ULONG CWLaunchArea::mfAddClassChoosePage(HWND hwndNotebook)
{
  return SETTINGS_PAGE_REMOVED;
}

PSZ M_CWLaunchArea::wpclsQueryTitle()
{
#if 0
  static char name[50]="";
  
  if(!WinLoadString(WinQueryAnchorBlock(HWND_DESKTOP),queryModuleHandle(),
                    ID_MENUFOLDERCLASSNAME,sizeof(name),name))
    strcpy(name,"Menu-Folder");	
#endif

  return "CWLaunchArea";
}

void M_CWLaunchArea::wpclsInitData()
{
  somId mySomId;
  CLASSINFO ci;

  /* call parent */
  if((mySomId=somIdFromString("wpclsInitData"))!=NULLHANDLE) {
    ((somTD_M_WPObject_wpclsInitData)
     somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                         1,
                         __ClassObject->
                         somGetMethodToken(mySomId))
     )(this);
    SOMFree(mySomId);
  }
  else
    M_CWMenuFolder::wpclsInitData();

  if(!fInitCWLADone) {
    ulIconSize=WinQuerySysValue(HWND_DESKTOP, SV_CXICON);

    g_hab=WinQueryAnchorBlock(HWND_DESKTOP);

    /* Register launch area class */
    if(WinQueryClassInfo(WinQueryAnchorBlock(HWND_DESKTOP),
                         (PSZ) WC_FRAME,
                         &ci))
      {
        g_ulStaticDataOffset=ci.cbWindowData;
        g_pfnwpOrgStaticProc=ci.pfnWindowProc;
        
        ulQWP_LAUNCHAREADATA=g_ulStaticDataOffset;
        
        if (WinRegisterClass(WinQueryAnchorBlock(HWND_DESKTOP),
                             (PSZ) CW_LAUNCHAREA,
                             fnwpLaunchAreaProc,
                             //                                     ci.pfnWindowProc, 
                             ci.flClassStyle&~CS_PUBLIC,
                             ci.cbWindowData + LAUNCHAREA_DATASIZE))
          {
            /* */
          }
      }
    fInitCWLADone=TRUE;
  }
}


CWLAObject* CWLAObject::cwQueryNextObject()
{
    return cwLAONext;
}

CWLAObject* CWLAObject::cwSetNextObject(CWLAObject* cwlaObject)
{
  CWLAObject* cwlaoTemp;

  cwlaoTemp=cwLAONext;
  cwLAONext=cwlaObject;
  
  return cwlaoTemp;
}

HPOINTER CWLAObject::cwQueryObjectIcon()
{
  return wpObject->wpQueryIcon();
}

BOOL CWLAObject::cwSetWPObject(WPObject* wpObject_)
{
  wpObject=wpObject_;
  
  return TRUE;
}

WPObject* CWLAObject::cwQueryWPObject()
{
     return wpObject;
}

/* Every button (CWLAObject) holds a copy of the HOBJECT so it must
   not be searched for every invocation. */
BOOL CWLAObject::cwSetObjectHandle(HOBJECT hObject_)
{
  hObject=hObject_;

  return TRUE;
}

HOBJECT CWLAObject::cwQueryObjectHandle()
{
  return hObject;
}

/* Set the launch area owning this object */
BOOL CWLAObject::cwSetLaunchArea(CWLaunchArea* cwLA)
{
  cwlaParent=cwLA;
  return TRUE;
}

CWLaunchArea* CWLAObject::cwQueryLaunchArea()
{
  return cwlaParent;
}






