/*
 * This file is (C) Chris Wohlgemuth 2002-2003
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
#include "newfile.h"
#include "newfileDlg.h"
#include "except.h"
#include <rexxsaa.h>                   /* needed for RexxStart()     */
#include <stdio.h>
#include <stdlib.h>
#include <sys\stat.h>
#include "defines.h"
#include "sys_funcs.h"

typedef struct _DISPATCHDATA
{
  HAB hab;
  HMQ  hmq;
  QMSG qmsg;  
}DISPATCHDATA;

#include "menufolder.hh"
#include "menu_funcs.h"

#define  INVALID_ROUTINE 40            /* Raise Rexx error           */
#define  VALID_ROUTINE    0            /* Successful completion      */


extern ULONG   ulQWP_WIZBUTTONDATA;

#include "menufolder.hh"

ULONG calculateHash(char * theString);
ULONG calculateHash2(char * theString);
//void parameterError(char *chrMethod);
void numParameterError(char *chrMethod, ULONG ulReq, ULONG ulFound);
void rxerrMethodNameError(char *chrRexxFunction, char *chrMethodName);
void rxerrParameterValueError(char *chrMethodName, ULONG ulParmNum);
void rxerrPrintErrorTextWithMethod(char *chrMethodName, char *chrText1, char *chrText2);
void rxerrPrintErrorText(char *chrText0, char *chrText1, char *chrText2);

LONG rxGetLongFromStem(RXSTRING args, ULONG ulTail, ULONG* ulError);
LONG rxGetLongFromStem2(RXSTRING args, char * chrTail, ULONG* ulError);
PSZ rxGetStringFromStem(RXSTRING args, ULONG ulTail, ULONG* ulError);
LONG rxSetLongInStem(RXSTRING args, ULONG ulTail, LONG lValue);
LONG rxSetStringInStem(RXSTRING args, ULONG ulTail, char* chrTailString, char* theString);

HMODULE queryModuleHandle(void);
SOMClass* cwGetSomClass(char* chrClassName);

/* This function is undocumented */
#pragma import(WinBuildPtrHandle,,"PMMERGE", 5117)
HPOINTER _System WinBuildPtrHandle(void* pIconData);

/*
  Gets objects of an open folder. Returns TRUE if successfull.
*/
BOOL getObjectsFromFolder(HWND hwndFrame, RXSTRING stem, USHORT usEmphasis, char* chrClassName)
{
  HWND hwndContainer;
  PMINIRECORDCORE mrc, mrcTemp;
  MPARAM mp2;
  LONG lNumObjects=0;

  hwndContainer=WinWindowFromID(hwndFrame, FID_CLIENT);
  if(!WinIsWindow(WinQueryAnchorBlock(hwndFrame), hwndContainer))
    return FALSE;

  /* set stem to zero */
  rxSetLongInStem( stem , 0, 0);

  if(usEmphasis) {
    mp2=MPFROMSHORT(CRA_SELECTED);
    /* Get first selected container item of our folder */
    mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORDEMPHASIS, MPFROMSHORT(CMA_FIRST),
                                    MPFROMSHORT(usEmphasis));
  }
  else {
    /* Get first container item of our folder */
    mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD,NULL,
                                    MPFROM2SHORT(CMA_FIRST,CMA_ITEMORDER));
      }

  if(!mrc || (int)mrc==-1) {
    /* No object */
    return FALSE;
  }
  do {
    WPObject * contentObject;

    contentObject=(WPObject*)OBJECT_FROM_PREC(mrc);//Get object
    if(somIsObj(contentObject))
      {
        if(contentObject->somIsA(cwGetSomClass(chrClassName))) {

          /* It's a valid object */
          rxSetLongInStem( stem , ++lNumObjects, (LONG) contentObject);
          /* Set object title */
          rxSetStringInStem(stem , lNumObjects, "_title", contentObject->wpQueryTitle());
          /* Set object class name */
          rxSetStringInStem(stem , lNumObjects, "_class", contentObject->somGetClassName());

          /* Set the object id if any. wpQueryObjectID() may return NULL. rxSetStringInStem()
             handles that by setting the variable value to "" */
          rxSetStringInStem(stem , lNumObjects, "_objectid", contentObject->wpQueryObjectID());

          if(contentObject->somIsA(cwGetSomClass("WPFileSystem"))) {
            char chrName[CCHMAXPATH];
            ULONG ulSize=sizeof(chrName);

            /* Get path to object */
            if(((WPFileSystem*)contentObject)->wpQueryRealName(chrName, &ulSize, TRUE))
              rxSetStringInStem(stem , lNumObjects, "_realname", chrName);
            else
              rxSetStringInStem(stem , lNumObjects, "_realname", "");
            /* Query type */
            rxSetStringInStem(stem , lNumObjects, "_type", ((WPFileSystem*)contentObject)->wpQueryType());

            rxSetStringInStem(stem , lNumObjects, "_filesystem", "1");
          }
          else {
            rxSetStringInStem(stem , lNumObjects, "_realname", "");
            rxSetStringInStem(stem , lNumObjects, "_type", "");
            rxSetStringInStem(stem , lNumObjects, "_filesystem", "0");
          }
          /* Set the object handle */
          //      rxSetStringInStem(stem , lNumObjects, "_hobject", contentObject->wpQueryHandle());

          /* Set number of objects */
          rxSetLongInStem( stem , 0, lNumObjects);
        }
      }
    /* Next object */
    if(usEmphasis)
      mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORDEMPHASIS, mrc,
                                      MPFROMSHORT(usEmphasis));
    else
      mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD, mrc,
                                      MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));  
  }while(mrc && (int)mrc!=-1);

  return TRUE;
}


BOOL fileGetBinFileSize(char *chrFileName, ULONG* ulFileSize)
{
  FILE *file;
  struct stat buf;

  if((file=fopen(chrFileName, "rb"))==NULLHANDLE)
    return FALSE;

  if(fstat(fileno(file), &buf)!=0) {
    fclose(file);
    return FALSE;
  }

  fclose(file);
  *ulFileSize=buf.st_size;

  return TRUE;
}


HPOINTER createPointerFromFile(char *chrIconFile)
{
  ULONG ulFileSize;
  void * pData;
  FILE *file;
  HPOINTER hPtr=NULLHANDLE;

  if(!fileGetBinFileSize(chrIconFile, &ulFileSize))
    return NULLHANDLE;  

  if((pData=malloc(ulFileSize))==NULLHANDLE)
    return NULLHANDLE;  

  if((file=fopen(chrIconFile, "rb"))==NULLHANDLE)
    {
      free(pData); 
      return NULLHANDLE;
    }

  if(ulFileSize!=fread(pData, sizeof(BYTE), ulFileSize, file))
    {
      free(pData); 
      return NULLHANDLE;
    }

  hPtr=WinBuildPtrHandle(pData);
  
  free(pData);
  fclose(file);
  
  return hPtr;
}

static void setupWizButton(HWND hwndButton, char * chrFlyOverText, char * chrIconFile)
{
  BTNCDATA btCtrl;
  WNDPARAMS wndParams;
  ULONG ulStyle;
  HAB hab;
  HPOINTER hPtr;

  hab=WinQueryAnchorBlock(HWND_DESKTOP);

  if(WinIsWindow(hab,hwndButton)) {
    WIZBUTTONDATA *wbd;

    WinSetWindowULong(hwndButton,ulQWP_WIZBUTTONDATA, (ULONG)0L);

   if((wbd=(WIZBUTTONDATA*)malloc(sizeof(WIZBUTTONDATA)))!=NULLHANDLE)
     {
       memset(wbd, 0, sizeof(WIZBUTTONDATA));
       if(strlen(chrFlyOverText)) {
         wbd->chrFlyOver=(char*)malloc(strlen(chrFlyOverText)+1);
         if(wbd->chrFlyOver)
           strcpy(wbd->chrFlyOver, chrFlyOverText);
       }
       WinSetWindowULong(hwndButton,ulQWP_WIZBUTTONDATA, (ULONG)wbd);
     };
   
    /* Set button icon */
   if(strlen(chrIconFile)) {
     hPtr=createPointerFromFile(chrIconFile);
     
     ulStyle=WinQueryWindowULong(hwndButton,QWL_STYLE);
     ulStyle|=(BS_MINIICON);
     ulStyle&=~BS_TEXT;
     if(WinSetWindowULong(hwndButton,QWL_STYLE,ulStyle)) {
       memset(&btCtrl,0,sizeof(btCtrl));
       btCtrl.cb=sizeof(btCtrl);
       btCtrl.hImage=hPtr;
       memset(&wndParams,0,sizeof(wndParams));
       wndParams.fsStatus=WPM_CTLDATA;
       wndParams.cbCtlData=btCtrl.cb;
       wndParams.pCtlData=&btCtrl;
       WinSendMsg(hwndButton,WM_SETWINDOWPARAMS,
                  MPFROMP(&wndParams),0);
       /* Set flyover help proc */
       //oldButtonProc2=WinSubclassWindow(hwndButton,newButtonProc);
     }
   }
  }
}

MRESULT EXPENTRY fnwpFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

  switch(msg)
    {
    case WM_COMMAND:
      {
#if 0
        FRAMECTRLDATA * fctrlData;
  
        /* Get our control data */
        fctrlData=(FRAMECTRLDATA*)WinQueryWindowPtr(hwnd, QWL_USER);
        if(!fctrlData) 
          return (MRESULT)FALSE;

        if(!somIsObj(fctrlData->cwFolder))
          return (MRESULT)FALSE;


        wpsWizRunRexx("/STATUSBAREVENT",          /* Describes the function from where this is called */
                      fctrlData->cwFolder,         /* Object on which the function is invoked          */
                      SKRIPTID_STATUSBAR,
                      SHORT1FROMMP(mp1),
                      fctrlData->chrCtlBottomScript);

        wpsWizRunWindowScript("/STATUSBAREVENT",          /* Describes the function from where this is called */
                              fctrlData->cwFolder,       /* Object on which the function is invoked          */
                              SKRIPTID_STATUSBAR,
                              fctrlData->chrCtlBottomScript,/* The script */
                              "COMMAND %d %d %d", SHORT1FROMMP(mp1), hwnd, WinQueryWindow(hwnd, QW_PARENT));
#endif
        return (MRESULT)FALSE;
      }
    default:
      break;
    }
  return (*WinDefWindowProc)(hwnd, msg, mp1, mp2);
}


/*
  This is the handling routine for the "WPSWizCallWinFunc" Rexx function.
 */
ULONG EXPENTRY rxCallPMFunc(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring)
{
  char text[200];
  ULONG ulHash;
  ULONG ulReturn;
  WPObject* wpObject;
  WPFileSystem* wpFSObject;

  /* At least the name of the function */
  if(argc<1)
    return 40;

  TRY_LOUD(RX) {

    /* Get a hash for the methodname for the switch() statement */
    ulHash=calculateHash(argv[0].strptr)<<8;
    ulHash+=calculateHash2(argv[0].strptr);
    ulReturn=0;

    switch(ulHash)
      {
        // case 0x2f46: /* winAddPushButton */

      case 0x3302: /* winCreateFrameWindow */
        {
          /*
            argv[0]: function name
            argv[1]: stem with window info
            argv[2]: Text
            */
          ULONG ulError;
          HWND hwnd, hwndNew;
          ULONG flCreateFlags;
          
          if(argc<3 || !rxGetLongFromStem(argv[1], 0, &ulError)) {
            numParameterError(argv[0].strptr, 3, argc);
            ulReturn=40;
            break;
          }
          
          hwnd=rxGetLongFromStem(argv[1], 1, &ulError);
          if(!hwnd)
            hwnd=HWND_DESKTOP;
          
          flCreateFlags=rxGetLongFromStem(argv[1], 2, &ulError);//Frame creation flags

          hwndNew=WinCreateStdWindow(hwnd, 
                                     rxGetLongFromStem(argv[1], 3, &ulError),//window style
                                     &flCreateFlags,
                                     NULL,
                                     argv[2].strptr,//Text 
                                     0,//client style
                                     NULLHANDLE,
                                     rxGetLongFromStem(argv[1], 8, &ulError),//ID
                                     NULLHANDLE);
          if(hwndNew)
            {
              WinSetWindowPos(hwndNew, HWND_TOP,
                              rxGetLongFromStem(argv[1], 4, &ulError),
                              rxGetLongFromStem(argv[1], 5, &ulError),
                              rxGetLongFromStem(argv[1], 6, &ulError),
                              rxGetLongFromStem(argv[1], 7, &ulError),
                              SWP_MOVE|SWP_SIZE|SWP_ZORDER);
              //  fnwpFrameProc
            }
          
          sprintf(retstring->strptr,"%d", 
                  hwndNew);
          retstring->strlength=strlen(retstring->strptr);
          break;
        }

      case 0x5c13: /* winCreateWindow */
        {
          /*
            argv[0]: function name
            argv[1]: stem with window info
            argv[2]: Text
           */
          ULONG ulError;
          HWND hwnd, hwndNew;
          ULONG ulWindowClass, ulStyle;

          if(argc!=3 || !rxGetLongFromStem(argv[1], 0, &ulError)) {
            numParameterError(argv[0].strptr, 3, argc);
            ulReturn=40;
            break;
          }

          hwnd=rxGetLongFromStem(argv[1], 1, &ulError);
          ulWindowClass=rxGetLongFromStem(argv[1], 2, &ulError);

          ulStyle=rxGetLongFromStem(argv[1], 3, &ulError);
          if(ulWindowClass==0xffff0003) {
            char * chrIcon=rxGetStringFromStem(argv[1], 9, &ulError);
            char * chrText=rxGetStringFromStem(argv[1], 10, &ulError);

            /* Create wizButton instead of WC_BUTTON */
            hwndNew=WinCreateWindow(hwnd, 
                                    (PSZ)WC_WIZBUTTON,//WC_WIZBUTTON window class 
                                    argv[2].strptr,//Text 
                                    ulStyle,//Style flags
                                    rxGetLongFromStem(argv[1], 4, &ulError),//x
                                    rxGetLongFromStem(argv[1], 5, &ulError),//y
                                    rxGetLongFromStem(argv[1], 6, &ulError),//cx
                                    rxGetLongFromStem(argv[1], 7, &ulError),//cy
                                    hwnd,                                  
                                    HWND_TOP, 
                                    rxGetLongFromStem(argv[1], 8, &ulError),//ID
                                    NULLHANDLE, NULLHANDLE);
            setupWizButton(hwndNew, chrText, chrIcon);
            /* Free the string memory */
            if(chrIcon)
              free(chrIcon);
            if(chrText)
              free(chrText);
          }
          else
            {
            hwndNew=WinCreateWindow(hwnd, 
                                    (PSZ)ulWindowClass,//Window class e.g. WC_BUTTON, 
                                    argv[2].strptr,//Text 
                                    ulStyle,//Style flags
                                    rxGetLongFromStem(argv[1], 4, &ulError),//x
                                    rxGetLongFromStem(argv[1], 5, &ulError),//y
                                    rxGetLongFromStem(argv[1], 6, &ulError),//cx
                                    rxGetLongFromStem(argv[1], 7, &ulError),//cy
                                    hwnd,                                  
                                    HWND_TOP, 
                                    rxGetLongFromStem(argv[1], 8, &ulError),//ID
                                    NULLHANDLE, NULLHANDLE);
            }

          sprintf(retstring->strptr,"%d", 
                  hwndNew);
          retstring->strlength=strlen(retstring->strptr);
          break;
        }
      case 0x4d04: /* winFileDlg */
        {
          FILEDLG fd = { 0 };

          if(argc<4) {
            numParameterError(argv[0].strptr, 4, argc);
            ulReturn=40;
            break;
          }

          fd.cbSize = sizeof( fd );
          if(atol(argv[1].strptr)==0)
            /* It's an centered 'Open'-dialog */
            fd.fl = FDS_OPEN_DIALOG|FDS_CENTER;
          else
            /* It's an centered 'Save as'-dialog */
            fd.fl = FDS_SAVEAS_DIALOG|FDS_CENTER;
          /* Set the title of the file dialog */
          fd.pszTitle = argv[2].strptr;
          /* File mask */
          sprintf(fd.szFullFile, argv[3].strptr);

          if( WinFileDlg( HWND_DESKTOP, HWND_DESKTOP, &fd ) == NULLHANDLE )
            {
              /* WinFileDlg failed */
              sprintf(retstring->strptr,"ERROR:");
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
          if( fd.lReturn == DID_OK )
            {
              sprintf(retstring->strptr,fd.szFullFile);
            }
          else
            sprintf(retstring->strptr, "");

          retstring->strlength=strlen(retstring->strptr);
          break;
        }
      case 0x3c56: /* winGetMsg */
        {
          DISPATCHDATA *pdd;
          BOOL bNotQuit;
          /*
            argv[0]: function name
            argv[1]: HWND
            */
          
          if(argc!=2) {
            numParameterError(argv[0].strptr, 2, argc);
            ulReturn=40;
            break;
          }
          /* Check window handle */
          if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[1].strptr)))
            {
              rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
              ulReturn=40;
              break;
            }

          pdd=(DISPATCHDATA*)WinQueryWindowPtr(atol(argv[1].strptr), QWL_USER);
          if(pdd) {
            /* Get pending message */
            bNotQuit=WinGetMsg(pdd->hab, &pdd->qmsg, NULL, 0, 0);
            //bNotQuit=WinGetMsg(WinQueryAnchorBlock(HWND_DESKTOP), &pdd->qmsg, NULLHANDLE, 0, 0);
#if 0
            if(pdd->qmsg.msg==WM_CLOSE)
              DosBeep(5000, 700);
            if(pdd->qmsg.msg==WM_QUIT)
              DosBeep(500, 700);
            if(!bNotQuit)
              DosBeep(1500, 700);
#endif
            
            /* Dispatch to window */
            WinDispatchMsg(pdd->hab,&pdd->qmsg);
            //WinDispatchMsg(WinQueryAnchorBlock(HWND_DESKTOP), &pdd->qmsg);
#if 0
            if(pdd->qmsg.msg!=WM_MOUSEMOVE) {
              SysWriteToTrapLog("hwnd: %x, time: %x, x: %x, y: %x\n", 
                                pdd->qmsg.hwnd, pdd->qmsg.time, pdd->qmsg.ptl.x, pdd->qmsg.ptl.y);
              DosBeep(500, 10);
            }
#endif
            
            if(!bNotQuit) {
              sprintf(retstring->strptr,"QUIT");
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
    
            switch(pdd->qmsg.msg)
              {
              case WM_CONTEXTMENU:
                sprintf(retstring->strptr,"onPopup %d , %d , %d ",
                        pdd->qmsg.hwnd, SHORT1FROMMP(pdd->qmsg.mp1), SHORT2FROMMP(pdd->qmsg.mp1));
                break;
              case WM_MOUSEMOVE:
                sprintf(retstring->strptr,"onMouseMove");
                break;
#if 0
              case WM_BUTTON1CLICK:
                sprintf(retstring->strptr,"onClick");
                break;
#endif
              case WM_BUTTON1DBLCLK:
                sprintf(retstring->strptr,"onDoubleClick");
                break;
              case WM_COMMAND:
                sprintf(retstring->strptr,"onCommand %d , %d, %d", pdd->qmsg.hwnd, SHORT1FROMMP(pdd->qmsg.mp1), SHORT1FROMMP(pdd->qmsg.mp2));
                break;
              case WM_TIMER:
                if(SHORT1FROMMP(pdd->qmsg.mp1)<TID_USERMAX)
                  sprintf(retstring->strptr,"onTimer");
                else
                  sprintf(retstring->strptr,"");
                break;
              default:
                sprintf(retstring->strptr,"");
              }

            /* Return to script with message */
            //  sprintf(retstring->strptr,"%d", bNotQuit);
            retstring->strlength=strlen(retstring->strptr);
            break;
          }
          else {
            rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
            ulReturn=40;
            break;
          }
        }
      case 0x2533: /* winSetText */
          /*
            argv[0]: function name
            argv[1]: HWND
            argv[2]: Text
           */

        if(argc!=3) {
          numParameterError(argv[0].strptr, 3, argc);
          ulReturn=40;
          break;
        }
        /* Check window handle */
        if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[1].strptr)))
          {
            rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
            ulReturn=40;
            break;
          }

        sprintf(retstring->strptr,"%d", WinSetWindowText(atol(argv[1].strptr), argv[2].strptr));
        retstring->strlength=strlen(retstring->strptr);
        break;
      case 0x441e: /* winQuerytext */
          /*
            argv[0]: function name
            argv[1]: HWND

           */
        LONG lTextLen;
        HWND hwnd;

        if(argc!=2) {
          numParameterError(argv[0].strptr, 2, argc);
          ulReturn=40;
          break;
        }
        /* Check window handle */
        if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[1].strptr)))
          {
            rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
            ulReturn=40;
            break;
          }

        hwnd=atol(argv[1].strptr);
        lTextLen=WinQueryWindowTextLength(hwnd);

        if(lTextLen>256) {
          if(DosAllocMem((PPVOID)&retstring->strptr, lTextLen, PAG_COMMIT|PAG_READ|PAG_WRITE)==NO_ERROR)
            {
              WinQueryWindowText(hwnd, lTextLen, retstring->strptr);
              retstring->strlength=strlen(retstring->strptr);
            }
        }
        else {
          WinQueryWindowText(hwnd, 256, retstring->strptr);
          retstring->strlength=strlen(retstring->strptr);
        }
        break;
      case 0x2d55: /* winWindowFromID */
        {
          /*
            argv[0]: function name
            argv[1]: HWND
            argv[2]: ID

           */
          if(argc!=3) {
            numParameterError(argv[0].strptr, 3, argc);
            ulReturn=40;
            break;
          }
          /* Check window handle */
          if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[1].strptr)))
            {
              rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
              ulReturn=40;
              break;
            }

          sprintf(retstring->strptr,"%d", WinWindowFromID(atol(argv[1].strptr), atol(argv[2].strptr)));
          retstring->strlength=strlen(retstring->strptr);
          break;       
        }
      case 0x5c10: /* winQueryContainerHWND */
        {
          ULONG ulParam1;
          
          if(argc<2) {
            numParameterError(argv[0].strptr, 2, argc);
            ulReturn=40;
            break;
          }
          ulParam1=atol(argv[1].strptr);

          /* Check window handle */
          if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[1].strptr)))
            {
              rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
              ulReturn=40;
              break;
            }

          sprintf(retstring->strptr,"%d", WinWindowFromID(ulParam1, FID_CLIENT));
          retstring->strlength=strlen(retstring->strptr);
          break;
        }
      case 0x0706: /* winQueryContainerRecord */
        {
          if(argc<5) {
            numParameterError(argv[0].strptr, 5, argc);
            ulReturn=40;
            break;
          }
          sprintf(retstring->strptr,"%d", PVOIDFROMMR(WinSendMsg(atol(argv[1].strptr),
                                                                 CM_QUERYRECORD, MPFROMP(atol(argv[2].strptr)),
                                                                 MPFROM2SHORT((SHORT)atol(argv[3].strptr), 
                                                                              (SHORT)atol(argv[4].strptr)))));
          retstring->strlength=strlen(retstring->strptr);
          break;
        }
      case 0x225d: /* winObjectFromPRec */
        {

          if(argc<2) {
            numParameterError(argv[0].strptr, 2, argc);
            ulReturn=40;
            break;
          }
          sprintf(retstring->strptr,"%d", OBJECT_FROM_PREC(atol(argv[1].strptr)));
          retstring->strlength=strlen(retstring->strptr);
          break;
        }
      case 0x5d08: /* winQueryContRecEmphasis */
        {
          if(argc<4) {
            numParameterError(argv[0].strptr, 4, argc);
            ulReturn=40;
            break;
          }
          SysWriteToTrapLog("%x %x %x\n", atol(argv[1].strptr), MPFROMLONG(atol(argv[2].strptr)),
                                                                           MPFROMSHORT((SHORT)atol(argv[3].strptr))  );
          sprintf(retstring->strptr,"%d", PVOIDFROMMR(WinSendMsg(atol(argv[1].strptr) ,
                                                                 CM_QUERYRECORDEMPHASIS, MPFROMLONG(atol(argv[2].strptr)),
                                                                 MPFROMSHORT((SHORT)atol(argv[3].strptr)))));
          retstring->strlength=strlen(retstring->strptr);
          break;
        }
      case 0x0b19: /* winSetContRecEmphasis */
        {
          /*
            Gets all objects in an open folder of a given class.
            
            arg[0]:  function name
            arg[1]:  HWND of container
            arg[2]:  pRec, pointer to record
            arg[3]:  USHORT, Command value specifying the emphasis
            arg[4]:  set flag
            */          
          if(argc<5) {
            numParameterError(argv[0].strptr, 5, argc);
            ulReturn=40;
            break;
          }

          sprintf(retstring->strptr,"%d", PVOIDFROMMR(WinSendMsg(atol(argv[1].strptr),
                                                                 CM_SETRECORDEMPHASIS, MPFROMP(atol(argv[2].strptr)),
                                                                 MPFROM2SHORT(((SHORT)atol(argv[4].strptr)!=0), 
                                                                              (SHORT)atol(argv[3].strptr)))));
          retstring->strlength=strlen(retstring->strptr);
          break;
        }
      case 0x4c60: /*winQueryFolderObjects */
        {
          /*
            Gets all objects in an open folder of a given class.
            
            arg[0]:  function name
            arg[1]:  HWND of folder
            arg[2]:  stem to place objects in
            arg[3]:  USHORT, emphasis mask or 0 
            arg[4]:  object class
            */
          
          if(argc<4) {
            /* object class may be omitted */
            numParameterError(argv[0].strptr, 4, argc);
            ulReturn=40;
            break;
          }
          /* Check window handle */
          if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[1].strptr)))
            {
              rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
              ulReturn=40;
              break;
            }

          if(argc<5)
            sprintf(retstring->strptr,"%d", getObjectsFromFolder(atol(argv[1].strptr), argv[2], atol(argv[3].strptr), "WPObject"));
          else
            sprintf(retstring->strptr,"%d", getObjectsFromFolder(atol(argv[1].strptr), argv[2], 
                                                                 atol(argv[3].strptr), argv[4].strptr));
          retstring->strlength=strlen(retstring->strptr);
          break;
        }
      case 0x122b: /* winQueryObject */
        {
          if(argc<2) {
            numParameterError(argv[0].strptr, 2, argc);
            ulReturn=40;
            break;
          }

          sprintf(retstring->strptr,"%d", WinQueryObject(argv[1].strptr));
          retstring->strlength=strlen(retstring->strptr);
          break;
        }
      case 0x3a2b:   /* winQuerySysValue */
        {
          /*
            arg[0]:  function name
            arg[1]:  index
            */
          if(argc!=2) {
            numParameterError(argv[0].strptr, 2, argc);
            ulReturn=40;
            break;
          }
          sprintf(retstring->strptr,"%d", WinQuerySysValue(HWND_DESKTOP, atol(argv[1].strptr)));
          retstring->strlength=strlen(retstring->strptr);
          break;
        }
      case 0x359:  /* winQueryWindowPos */
        {
          SWP swp;
          
          /*
            arg[0]:  function name
            arg[1]:  HWND
            
            */
          if(argc!=2) {
            numParameterError(argv[0].strptr, 2, argc);
            ulReturn=40;
            break;
          }
          /* Check window handle */
          if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[1].strptr)))
            {
              rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
              ulReturn=40;
              break;
            }
          WinQueryWindowPos(atol(argv[1].strptr), &swp);
          sprintf(retstring->strptr,"%d %d %d %d %d", swp.x, swp.y, swp.cx, swp.cy,
                  swp.hwndInsertBehind);
          retstring->strlength=strlen(retstring->strptr);
          
          break;
        }
      case 0x3b3a: /*winQueryWindowID */
        {
          /*
            arg[0]:  function name
            arg[1]:  HWND
            */
          if(argc!=2) {
            numParameterError(argv[0].strptr, 2, argc);
            ulReturn=40;
            break;
          }
          /* Check window handle */
          if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[1].strptr)))
            {
              rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
              ulReturn=40;
              break;
            }

          sprintf(retstring->strptr,"%d", WinQueryWindowUShort(atol(argv[1].strptr), QWS_ID));
          retstring->strlength=strlen(retstring->strptr);

          break;
        }
      case 0x111F: /* winSetWindowPos */
        {
          /*
            arg[0]:  function name
            arg[1]:  HWND
            */
          if(argc!=8) {
            numParameterError(argv[0].strptr, 8, argc);
            ulReturn=40;
            break;
          }
          /* Check window handle */
          if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[1].strptr)))
            {
              rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
              ulReturn=40;
              break;
            }

          sprintf(retstring->strptr,"%d",
                  WinSetWindowPos(atol(argv[1].strptr), atol(argv[2].strptr),
                                  atol(argv[3].strptr), atol(argv[4].strptr),
                                  atol(argv[5].strptr), atol(argv[6].strptr),
                                  atol(argv[7].strptr)));
          retstring->strlength=strlen(retstring->strptr);
          break;
        }
      case 0x0812: /* winSendMsg */
      case 0x1822: /* winPostMsg */

        {
          /*
            arg0:  function name
            arg1:  HWND
            arg2:  msg
            arg3:  parm1
            arg4:  parm2
            */
          if(argc!=5) {
            numParameterError(argv[0].strptr, 5, argc);
            ulReturn=40;
            break;
          }
          /* Check window handle */
          if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[1].strptr)))
            {
              rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
              ulReturn=40;
              break;
            }

          if(ulHash==0x0812)
            sprintf(retstring->strptr,"%d",
                    WinSendMsg(atol(argv[1].strptr), atol(argv[2].strptr),
                               MPFROMLONG(atol(argv[3].strptr)),MPFROMLONG( atol(argv[4].strptr))));
                                  
          else
            sprintf(retstring->strptr,"%d",
                    WinPostMsg(atol(argv[1].strptr), atol(argv[2].strptr),
                               MPFROMLONG(atol(argv[3].strptr)),MPFROMLONG( atol(argv[4].strptr))));

          retstring->strlength=strlen(retstring->strptr);
          break;
        }
      case 0x355d: /* winShowWindow */
        {
          /*
            arg[0]:  function name
            arg[1]:  HWND
            arg[2]:  bShow
            */
          if(argc!=3) {
            numParameterError(argv[0].strptr, 3, argc);
            ulReturn=40;
            break;
          }
          if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[1].strptr)))
            {
              rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
              ulReturn=40;
              break;
            }

          sprintf(retstring->strptr,"%d", WinShowWindow(atol(argv[1].strptr), atol(argv[2].strptr)));
          retstring->strlength=strlen(retstring->strptr);
         
          break;
        }

      case 0x5127: /* winStartTimer */
        {
          /*
            arg[0]:  function name
            arg[1]:  HWND
            arg[2]:  timer id
            arg[3]:  timeout
            */
          if(argc!=4) {
            numParameterError(argv[0].strptr, 3, argc);
            ulReturn=40;
            break;
          }
          if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[1].strptr)))
            {
              rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
              ulReturn=40;
              break;
            }
          /*
            CW: 12.04.06 added check for timer ID value.
           */
          if(atol(argv[2].strptr) >= TID_USERMAX)
            {
              rxerrParameterValueError(argv[0].strptr, 3); /* Show msg to user */
              ulReturn=40;
              break;
            }

          sprintf(retstring->strptr,"%d", WinStartTimer(WinQueryAnchorBlock(atol(argv[1].strptr)), atol(argv[1].strptr), 
                                                        atol(argv[2].strptr), atol(argv[3].strptr)));
          retstring->strlength=strlen(retstring->strptr);
          break;
        }
      case 0x1546: /* winStopTimer  */
        {
          /*
            arg[0]:  function name
            arg[1]:  HWND
            arg[2]:  timer id
            */
          if(argc!=3) {
            numParameterError(argv[0].strptr, 3, argc);
            ulReturn=40;
            break;
          }
          if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[1].strptr))
             || atol(argv[2].strptr) >= TID_USERMAX)
            {
              rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
              ulReturn=40;
              break;
            }
          /*
            CW: 12.04.06 added check for timer ID value.
           */
          if(atol(argv[2].strptr) >= TID_USERMAX)
            {
              rxerrParameterValueError(argv[0].strptr, 3); /* Show msg to user */
              ulReturn=40;
              break;
            }
          sprintf(retstring->strptr,"%d", WinStopTimer(WinQueryAnchorBlock(atol(argv[1].strptr)), atol(argv[1].strptr), 
                                                        atol(argv[2].strptr)));
          retstring->strlength=strlen(retstring->strptr);
          break;
        }
        /***** Menu functions *****/
     case 0x141d: /* menuCheckForItem */
          if(argc!=4) {
            numParameterError(argv[0].strptr, 4, argc);
            ulReturn=40;
            break;
          }
          sprintf(retstring->strptr,"%d", menuCheckForItem(atol(argv[1].strptr),//hwndMenu
                                                         (SHORT)atol(argv[2].strptr),//usID
                                                         atol(argv[3].strptr)));//include submenus
          retstring->strlength=strlen(retstring->strptr);
          break;
      case 0x4908: /* menuCheckItem */
        if(argc!=5) {
          numParameterError(argv[0].strptr, 5, argc);
          ulReturn=40;
          break;
        }
        sprintf(retstring->strptr,"%d", menuCheckItem(atol(argv[1].strptr),//hwndMenu
                                                      atol(argv[2].strptr),//itemID
                                                      atol(argv[3].strptr),//include submenus
                                                      atol(argv[4].strptr)));//Check
        retstring->strlength=strlen(retstring->strptr);
        break;
      case 0x3c51: /* menuEnableItem */
        {
          /*
            argv[0]: function
            argv[1]: hwndMenu
            argv[2]: ItemID
            argv[3]: Enable/Disable
           */
        if(argc!=4) {
          numParameterError(argv[0].strptr, 5, argc);
          ulReturn=40;
          break;
        }
        sprintf(retstring->strptr,"%d", WinEnableMenuItem(atol(argv[1].strptr),//hwndMenu
                                                      atol(argv[2].strptr),//itemID
                                                      atol(argv[4].strptr)));//Enable
        retstring->strlength=strlen(retstring->strptr);
        break;
        }
      case 0x355e: /* menuInsertItem */
        if(argc!=6) {
          numParameterError(argv[0].strptr, 6, argc);
          ulReturn=40;
          break;
        }
        sprintf(retstring->strptr,"%d", menuInsertMenuItem(atol(argv[1].strptr), //hwndMenu 
                                                           atol(argv[2].strptr), //hwndSubMenu,
                                                           atol(argv[3].strptr), //sPosition
                                                           atol(argv[4].strptr), //usID 
                                                           argv[5].strptr));     //chrMenuText
        retstring->strlength=strlen(retstring->strptr);
        break;
      case 0x512:  /* menuInsertSeparator */
        if(argc!=4) {
          numParameterError(argv[0].strptr, 4, argc);
          ulReturn=40;
          break;
        }
        //    HlpWriteToTrapLog("%x %x %x", atol(argv[1].strptr),atol(argv[2].strptr), atol(argv[3].strptr));
        sprintf(retstring->strptr,"%d", menuInsertMenuSeparator(atol(argv[1].strptr),//hwndMenu
                                                                atol(argv[2].strptr),//hwndSubmenu
                                                                atol(argv[3].strptr)));//Position
        
        retstring->strlength=strlen(retstring->strptr);
        break;
      case 0x433a: /* menuPopupMenu */
        {
          HWND hwndMenu;
          ULONG ulError;
          /*
            arg[0]:  function name
            arg[1]:  HWND
            arg[2]:  stem with menu data
           */
          if(argc!=3) {
            numParameterError(argv[0].strptr, 3, argc);
            ulReturn=40;
            break;
          }
          if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[1].strptr)))
            {
              rxerrParameterValueError(argv[0].strptr, 2); /* Show msg to user */
              ulReturn=40;
              break;
            }

          hwndMenu=WinLoadMenu(HWND_DESKTOP, queryModuleHandle(), IDM_EMPTYMENU); /* Load empty menu */
          sprintf(retstring->strptr,"%d", 0);
          if(hwndMenu)
            {
              char text[20];
              int numMenus=0;
              ULONG ulError;
              int a;

              numMenus=rxGetLongFromStem(argv[2], 0, &ulError);/* */

              for(a=1; a<=numMenus; a++)
                {
                  char *chrMenuText;
                  if((chrMenuText=rxGetStringFromStem(argv[2], a, &ulError))!=NULLHANDLE) {
                    if(!strcmp(chrMenuText,"-"))
                      menuInsertMenuSeparator(hwndMenu,//hwndMenu
                                              NULLHANDLE,//hwndSubmenu
                                              MIT_END);//Position
                    else
                      menuInsertMenuItem(hwndMenu, //hwndMenu 
                                         NULLHANDLE, //hwndSubMenu,
                                         MIT_END, //sPosition
                                         a, //usID 
                                         chrMenuText);     //chrMenuText
                    DosFreeMem(chrMenuText);
                  }
                }
#if 0
              menuInsertMenuSeparator(hwndMenu,//hwndMenu
                                      NULLHANDLE,//hwndSubmenu
                                      MIT_END);//Position
              menuInsertMenuItem(hwndMenu, //hwndMenu 
                                 NULLHANDLE, //hwndSubMenu,
                                 MIT_END, //sPosition
                                 1, //usID 
                                 "MenuItem 1");     //chrMenuText
              sprintf(text,"%x", atol(argv[1].strptr));
              menuInsertMenuItem(hwndMenu, //hwndMenu 
                                 NULLHANDLE, //hwndSubMenu,
                                 MIT_END, //sPosition
                                 2, //usID 
                                 text);     //chrMenuText
#endif
              if(WinPopupMenu(atol(argv[1].strptr), atol(argv[1].strptr), hwndMenu, rxGetLongFromStem2(argv[2], "_x", &ulError),
                              rxGetLongFromStem2(argv[2], "_y", &ulError), 1, 
                              PU_MOUSEBUTTON1|/*PU_MOUSEBUTTON1DOWN|*/PU_KEYBOARD|PU_NONE|PU_SELECTITEM))
                sprintf(retstring->strptr,"%d", hwndMenu);
              else
                sprintf(retstring->strptr,"%d", 0);
            }

          retstring->strlength=strlen(retstring->strptr);
          break;  
        }
      case 0x3348: /* menuQueryItemCount */
        if(argc!=2) {
          numParameterError(argv[0].strptr, 2, argc);
          ulReturn=40;
          break;
        }
        sprintf(retstring->strptr,"%d", menuQueryItemCount(atol(argv[1].strptr)));//hwndMenu
        retstring->strlength=strlen(retstring->strptr);
        break;
      case 0x212c: /* menuSelectItem */
#if 0
        if(argc!=4) {
          parameterError(argv[0].strptr);
          ulReturn=40;
          break;
        }
        
        sprintf(retstring->strptr,"%d", menuInsertMenuSeparator(atol(argv[1].strptr),//Position
                                                                atol(argv[2].strptr),//hwndMenu
                                                                atol(argv[3].strptr)));//hwndSubmenu
        retstring->strlength=strlen(retstring->strptr);
#endif
        ulReturn=40;
        break;
      case 0x2033: /* menuRemoveItem */
        if(argc!=4) {
          numParameterError(argv[0].strptr, 4, argc);
          ulReturn=40;
          break;
        }
        sprintf(retstring->strptr,"%d", menuRemoveItem(atol(argv[1].strptr),//hwndMenu
                                                         (SHORT)atol(argv[2].strptr),//usID
                                                         atol(argv[3].strptr)));//include submenus
          retstring->strlength=strlen(retstring->strptr);
          break;
          case 0x5510: /* drgCheckIfFileSysOnly */
            {
              ULONG a;
              ULONG ulNum;
              PDRAGINFO pDragInfo;
              BOOL rc=TRUE;

              if(argc!=2) {
                /* Don't show a message box or PM will block!! */
                /*
                  parameterError(argv[0].strptr);
                  ulReturn=40;
                  */
                break;
              }
              /* Num objects */
              pDragInfo=(PDRAGINFO)atol(argv[1].strptr);
              ulNum=DrgQueryDragitemCount(pDragInfo);

              for(a=0;a<ulNum && rc; a++)
                {
                  if(DrgVerifyRMF(DrgQueryDragitemPtr( pDragInfo, a), "DRM_OS2FILE", NULL))
                    rc=TRUE;
                  else
                    rc=FALSE;
                }
              sprintf(retstring->strptr,"%d", rc);
              retstring->strlength=strlen(retstring->strptr);

              break;
            }
          case 0x2b12: /* drgQueryNumObjects */
            { 
              if(argc!=2) {
                /* Don't show a message box or PM will block!! */
                /*
                  parameterError(argv[0].strptr);
                  ulReturn=40;
                  */
                break;
              }
              sprintf(retstring->strptr,"%d", DrgQueryDragitemCount((PDRAGINFO)atol(argv[1].strptr)));
              retstring->strlength=strlen(retstring->strptr);              
              break;
            }
      case 0x138:  /* drgCheckClass */
        {
          ULONG a;
          ULONG ulNum;
          PDRAGINFO pDragInfo;
          SOMClass *somClass;
          BOOL rc=FALSE;

          if(argc!=3) {
            /* Don't show a message box or PM will block!! */
            /*
              parameterError(argv[0].strptr);
              ulReturn=40;
              */
            break;
          }
         
          /* Num objects */
          pDragInfo=(PDRAGINFO)atol(argv[1].strptr);
          ulNum=DrgQueryDragitemCount(pDragInfo);

          somClass=cwGetSomClass(argv[2].strptr);

          if(somIsObj(somClass))
            {
              WPObject *wpObject;
              rc=TRUE;
              for(a=0;a<ulNum && rc; a++)
                {
                  /* Check every object dragged */
                  wpObject=(WPObject*)OBJECT_FROM_PREC(DrgQueryDragitemPtr( pDragInfo, a)->ulItemID);
                  if(!wpObject->somIsA(somClass))
                    {
                      rc=FALSE;
                    }
                  else
                    rc=TRUE;
                }
            }
          else {
            /* The class name is wrong */
            /* Don't show a message box or PM will block!! */
            /*
              parameterError(argv[0].strptr);
              ulReturn=40;
              */
            break;
          }
          sprintf(retstring->strptr,"%d", rc);
          retstring->strlength=strlen(retstring->strptr);
          break;
        }
      case 0x5906: /* drgCheckSpecClass */
        {
          ULONG a;
          ULONG ulNum;
          PDRAGINFO pDragInfo;
          SOMClass *somClass;
          BOOL rc=FALSE;

          if(argc!=3) {
            /* Don't show a message box or PM will block!! */
            /*
              parameterError(argv[0].strptr);
              ulReturn=40;
              */
            break;
          }
         
          /* Num objects */
          pDragInfo=(PDRAGINFO)atol(argv[1].strptr);
          ulNum=DrgQueryDragitemCount(pDragInfo);

          somClass=cwGetSomClass(argv[2].strptr);

          if(somIsObj(somClass))
            {
              WPObject *wpObject;
              rc=TRUE;
              for(a=0;a<ulNum && rc; a++)
                {
                  /* Check every object dragged */
                  wpObject=(WPObject*)OBJECT_FROM_PREC(DrgQueryDragitemPtr( pDragInfo, a)->ulItemID);
                  if(!wpObject->somIsInstanceOf(somClass))
                    {
                      rc=FALSE;
                    }
                  else
                    rc=TRUE;
                }
            }
          else {
            /* The class name is wrong */
            /* Don't show a message box or PM will block!! */
            /*
              parameterError(argv[0].strptr);
              ulReturn=40;
              */
            break;
          }
          sprintf(retstring->strptr,"%d", rc);
          retstring->strlength=strlen(retstring->strptr);
          break;
        }        
      default:
        rxerrMethodNameError("WPSWizCallWinFunc", argv[0].strptr);
        ulReturn=40;        
      }/* switch */
    
    
    /*
    sprintf(text, "%s HAsh: %x, Hash2: %x",argv[1].strptr, ulHash, calculateHash2(argv[1].strptr));
       WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "",
       1234, MB_OK|MB_MOVEABLE);
       */
  }
  CATCH(RX)
    {}END_CATCH;

  return ulReturn;
}

/*
0x5c10: winQueryContainerHWND
0x0706: winQueryContainerRecord
0x5d08: winQueryContRecEmphasis
0x225d: winObjectFromPRec
0x4d04: winFileDlg
0x122b: winQueryObject
0x359:  winQueryWindowPos
0x111F: winSetWindowPos
0x5c13: winCreateWindow
0x355d: winShowWindow     !!!! Not documented !!!!
0x2533: winSetText
0x441e: winQuerytext
0x2d55: winWindowFromID
0x4c60: winQueryFolderObjects

0x0812: winSendMsg      !!!! Not documented !!!!
0x1822: winPostMsg      !!!! Not documented !!!!
0x3302: winCreateFrameWindow      !!!! Not documented !!!!

0x2f46: winAddPushButton !!!! Not used at the moment !!!!!!!

0x2033: menuRemoveItem
0x2913: menuAddItem
0x4908: menuCheckItem
0x355e: menuInsertItem
0x512:  menuInsertSeparator
0x212c: menuSelectItem
0x3348: menuQueryItemCount
0x3a04: menuQueryItem
0x141d: menuCheckForItem

0x5510: drgCheckIfFileSysOnly
0x2b12: drgQueryNumObjects
0x138:  drgCheckClass
0x5906: drgCheckSpecClass

0x3a2b: winQuerySysValue !!!! Not documented !!!!
0x3c56: winGetMsg        !!!! Not documented !!!!
0x5127: winStartTimer    !!!! Not documented !!!!
0x1546: winStopTimer     !!!! Not documented !!!!
0x6206: menuCreateMenu   !!!! Not documented not implemented !!!!
0x433a: menuPopupMenu    !!!! Not documented !!!!
*/

