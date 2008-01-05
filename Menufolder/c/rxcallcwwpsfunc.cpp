/*
 * This file is (C) Chris Wohlgemuth 2002-2004
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

#include "gadgetdata.h"


/* Achtung!!! auch in rxcallpmfunc.c und cwfolder!! */
typedef struct _DISPATCHDATA
{
  HAB hab;
  HMQ  hmq;
  QMSG qmsg;  
}DISPATCHDATA;

#include "cwlauncharea.hh"
#include "som_wps_funcs.h"

extern char chrInstallPath[CCHMAXPATH];
extern char chrWPSWizIni[CCHMAXPATH];
extern char chrWPSWizHints[CCHMAXPATH];

typedef ULONG (APIENTRY *PFNWPSFUNC)(WPObject* ,...);

void queryIniFilePath(char * chrPath, int size);
void queryHintFilePath(char * chrPath, int size);

ULONG calculateHash(char * theString);
ULONG calculateHash2(char * theString);
void wpObjectError(char *chrMethod);
void numParameterError(char *chrMethod, ULONG ulReq, ULONG ulFound);
void rxerrMethodNameError(char *chrRexxFunction, char *chrMethodName);
void rxerrParameterValueError(char *chrMethodName, ULONG ulParmNum);
void rxerrPrintErrorTextWithMethod(char *chrMethodName, char *chrText1, char *chrText2);
void rxerrPrintErrorText(char *chrText0, char *chrText1, char *chrText2);

SOMClass* cwGetSomClass(char* chrClassName);
LONG rxGetLongFromStem(RXSTRING args, ULONG ulTail, ULONG* ulError);
PSZ rxGetStringFromStem(RXSTRING args, ULONG ulTail, ULONG* ulError);
LONG rxSetLongInRexxVar(char* chrVar, ULONG ulVal);
LONG rxSetStringInRexxVar(char* chrVarName, char* theString);

void _Optlink rexxThreadFunc (void *arg);

extern HWND g_hwndRexxError;

MRESULT EXPENTRY fnwpGadgetObjectProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
    {
    default:
      break;
    }

  return WinDefWindowProc(hwnd,msg,mp1,mp2);
}

/*
  Destroy all windows owned by hwndObj
 */
static void destroyWindows(HWND hwndObj)
{
  HENUM henum;
  HWND hwndNext;

  henum=WinBeginEnumWindows(HWND_DESKTOP);
  while((hwndNext=WinGetNextWindow(henum))!=NULLHANDLE) {
    if(WinQueryWindow(hwndNext, QW_OWNER)==hwndObj) {
      WinDestroyWindow(hwndNext);
    }/*  if(WinQueryWindow(hwndNext, QW_OWNER)==hwndObj) */
  }
  WinEndEnumWindows(henum);
}

void _setupRxVars()
{
  char theString[20];
  ULONG ulBuf;

  /* Screen dimensions */
  rxSetLongInRexxVar(RXVAR_SCREENWIDTH, WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN));
  rxSetLongInRexxVar(RXVAR_SCREENHEIGHT, WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN));
  /* Icon size */
  rxSetLongInRexxVar(RXVAR_ICONWIDTH, WinQuerySysValue(HWND_DESKTOP, SV_CXICON));
  rxSetLongInRexxVar(RXVAR_ICONHEIGHT, WinQuerySysValue(HWND_DESKTOP, SV_CYICON));

  /* Bootdrive */
  DosQuerySysInfo(QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &ulBuf, sizeof(ulBuf));
  sprintf(theString, "%c:", 'A'+ulBuf-1);
  rxSetStringInRexxVar(RXVAR_BOOTDRIVE, theString);
  /* WPS-wizard install dir */
  rxSetStringInRexxVar(RXVAR_WPSWIZARD, chrInstallPath);
  /* INI file path */
  queryIniFilePath(chrWPSWizIni, sizeof(chrWPSWizIni)); /* Make sure file path exists */
  rxSetStringInRexxVar(RXVAR_WPSWIZARDINI, chrWPSWizIni);
  /* Hint database path */
  queryHintFilePath(chrWPSWizHints, sizeof(chrWPSWizHints)); /* Make sure file path exists */
  rxSetStringInRexxVar(RXVAR_WPSWIZARDHINT, chrWPSWizHints);

  /* Predefined gadget types */
  rxSetLongInRexxVar(RXVAR_GADGETHTML, RXVAR_GADGETHTMLVALUE);
  rxSetLongInRexxVar(RXVAR_BARGADGET, RXVAR_BARGADGETVALUE);
  rxSetLongInRexxVar(RXVAR_GROUPGADGET, RXVAR_GROUPGADGETVALUE);
  rxSetLongInRexxVar(RXVAR_ICONGADGET, RXVAR_ICONGADGETVALUE);
  rxSetLongInRexxVar(RXVAR_IMAGEGADGET, RXVAR_IMAGEGADGETVALUE);

  /* Gadget styles */
  rxSetLongInRexxVar(RXVAR_GSTYLENORESIZE, GSTYLE_NORESIZE);
  rxSetLongInRexxVar(RXVAR_GSTYLENOACTIVATE, GSTYLE_NOACTIVATE);
  rxSetLongInRexxVar(RXVAR_GSTYLEPOPUP, GSTYLE_POPUP);
  rxSetLongInRexxVar(RXVAR_GSTYLEBORDER, GSTYLE_BORDER);
  rxSetLongInRexxVar(RXVAR_GSTYLECLRBORDER, GSTYLE_CLRBORDER);
  rxSetLongInRexxVar(RXVAR_GSTYLESUNKENBORDER, GSTYLE_SUNKENBORDER);
  rxSetLongInRexxVar(RXVAR_GSTYLEFORECLRBORDER, GSTYLE_FORECLRBORDER);

  /* Physical memory size */
  DosQuerySysInfo(QSV_TOTPHYSMEM, QSV_TOTPHYSMEM, &ulBuf, sizeof(ulBuf));
  rxSetLongInRexxVar(RXVAR_PHYSMEM, ulBuf);
}


BOOL splitGadgetAndFunc(RXFNCCAL_PARM *parm)
{
  char * chrPtr;
  char *chrCommand=(char*)parm->rxfnc_name;
  int num=0;
  ULONG ulHash[10];

  while((chrPtr=strchr(chrCommand, '.'))!=NULLHANDLE)
    {
      *chrPtr++=0;
      //SysWriteToTrapLog("Part %d: %s\n", num, chrCommand);
      ulHash[num]=calculateHash(chrCommand)<<8 + calculateHash2(chrCommand);
      *--chrPtr='.';
      chrCommand=++chrPtr;

      num++;
      if(num>10) /* Safety net */
        return FALSE;
    }

  ulHash[num]=(calculateHash(chrCommand)<<8) + calculateHash2(chrCommand);

  if(num>1) {
    /* More than one Gadget in the chain is currently an error */
    parm->rxfnc_flags.rxffnfnd=1;
    //    SysWriteToTrapLog("Last part: %s\n", chrCommand);
    return TRUE;
  }
  else if(num==1) {
    /* Script calls a gadget method */
    return TRUE;
  }

  //  SysWriteToTrapLog("%s Hash: %x %x\n", chrCommand, ulHash[num],
  //                (calculateHash(chrCommand)<<8) + calculateHash2(chrCommand));

  /* Check if it's one of our commands */
  switch(ulHash[num])
    {
    case 0x1e16: /* GETGADGETMESSAGE */
      SysWriteToTrapLog("Called: %s\n", "GETGADGETMESSAGE");

      return TRUE;
    default:
      break;
    }
  return FALSE;
};

LONG EXPENTRY cwRexxExitHandler(LONG lExitNumber, LONG lSubfunction, PEXIT pexParameter)
{
  switch(lExitNumber)
    {
#if 0
    case RXSIO:
      if(lSubfunction==RXSIOTRC && pexParameter)
        {
          RXSIOTRC_PARM *parm=(RXSIOTRC_PARM *)pexParameter;
          /* Set error text */
          WinSendMsg(WinWindowFromID(g_hwndRexxError, IDLB_REXXERROR), 
                     LM_INSERTITEM, MPFROMSHORT(LIT_END), parm->rxsio_string.strptr);
          /* Show the error window */
          WinShowWindow(g_hwndRexxError, TRUE);
          WinSetWindowPos(g_hwndRexxError, HWND_TOP, 0, 0, 0, 0,SWP_ZORDER);
          
          return RXEXIT_HANDLED;
        }
      break;
#endif
    case RXFNC:
      if(lSubfunction==RXFNCCAL && pexParameter)
        {
          RXFNCCAL_PARM *parm=(RXFNCCAL_PARM *)pexParameter;
          if(splitGadgetAndFunc(parm))
            return RXEXIT_HANDLED;
          //          SysWriteToTrapLog("%s\n", parm->rxfnc_name);
          break;
        }
    case RXINI:
      if(lSubfunction==RXINIEXT)
        _setupRxVars();
      break;
    default:
      break;
    };
  return RXEXIT_NOT_HANDLED;
}

#if 0
/* Now in cwfolder.c */
void _Optlink rexxGuiThreadFunc (void *arg)
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;
  PREXXTHREADPARMS tPt;
  WPObject *thisPtr;
  DISPATCHDATA dd;

  tPt=(PREXXTHREADPARMS) arg;
  if(!tPt)
    return;
  thisPtr=(WPObject*)tPt->thisPtr;    //Pointer auf CWMediaFolder-Object
  if(!somIsObj(thisPtr))
    return;

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {
      HWND hwndGadgetObj;

      char text[1024];      
      
      RXSTRING arg[3];                    /* argument string for REXX  */
      RXSTRING rexxretval;                /* return value from REXX    */
      APIRET   rc;                        /* return code from REXX     */
      SHORT    rexxrc = 0;                /* return code from function */
      char chrThis[20];
      char chrHwnd[20];
      char chrRexxEnv[20]="/EXECUTE";
      static RXSYSEXIT exit_list[4]={0};

      hwndGadgetObj=WinCreateWindow(HWND_OBJECT,WC_STATIC,"GadgetObj", WS_VISIBLE,
                                    0,200,40,40,NULLHANDLE,HWND_BOTTOM,13345,NULL,NULL);
      if(hwndGadgetObj) {
        WinSubclassWindow(hwndGadgetObj, &fnwpGadgetObjectProc);

        dd.hab=hab;
        dd.hmq=hmq;
        dd.qmsg=qmsg;
        
        WinSetWindowPtr(hwndGadgetObj, QWL_USER, &dd);
        
        //#if 0
        /* ---- Execute script ---- */
        
        if(tPt->chrCommand[0])
          strncpy(chrRexxEnv, tPt->chrCommand, sizeof(chrRexxEnv));
        
        chrRexxEnv[sizeof(chrRexxEnv)-1]=0;
        
        sprintf(chrThis, "%d", thisPtr);
        sprintf(chrHwnd, "%d", hwndGadgetObj);

        exit_list[0].sysexit_name="WIZREXX";
        exit_list[0].sysexit_code=RXINI;
        exit_list[1].sysexit_name="GADGETREXX"; /* Registered in cwfolder */
        exit_list[1].sysexit_code=RXFNC;
        
        //exit_list[1].sysexit_code=RXENDLST;

        exit_list[2].sysexit_code=RXENDLST;
        exit_list[2].sysexit_name="WIZREXX";

        //exit_list[1].sysexit_code=RXTER;
        //exit_list[2].sysexit_name="WIZREXX";
        //exit_list[2].sysexit_code=RXENDLST;

        /* By setting the strlength of the output RXSTRING to zero, we   */
        /* force the interpreter to allocate memory and return it to us. */
        /* We could provide a buffer for the interpreter to use instead. */
        rexxretval.strlength = 0L;          /* initialize return to empty*/
        
        MAKERXSTRING(arg[0], chrRexxEnv, strlen(chrRexxEnv));/* create input argument     */
        MAKERXSTRING(arg[1], chrThis, strlen(chrThis));/* create input argument     */              
        MAKERXSTRING(arg[2], chrHwnd, strlen(chrHwnd));/* create input argument     */
        
        TRY_LOUD(RX_START) {
          /* Here we call the interpreter.  We don't really need to use    */
          /* all the casts in this call; they just help illustrate         */
          /* the data types used.                                          */
          rc=RexxStart((LONG)       3,             /* number of arguments   */
                       (PRXSTRING)  &arg,          /* array of arguments    */
                       (PSZ)        tPt->rexxSkript,/* name of REXX file     */
                       (PRXSTRING)  0,             /* No INSTORE used       */
                       (PSZ)        "WIZREXX",         /* Command env. name     */
                       (LONG)       RXSUBROUTINE,  /* Code for how invoked  */
                       (PRXSYSEXIT) &exit_list,
                       //   (PRXSYSEXIT) 0,             /* No EXITs on this call */
                       (PSHORT)     &rexxrc,       /* Rexx program output   */
                       (PRXSTRING)  &rexxretval ); /* Rexx program output   */
          
          if(rc) {
            sprintf(text,"Error in the Rexx skript %s", 
                    tPt->rexxSkript);
            WinSendMsg(WinWindowFromID(g_hwndRexxError, IDLB_REXXERROR), LM_INSERTITEM, MPFROMSHORT(LIT_END), text);
            
            sprintf(text,"Get more information with 'help REX%04d'.", rc*-1);
            WinSendMsg(WinWindowFromID(g_hwndRexxError, IDLB_REXXERROR), LM_INSERTITEM, MPFROMSHORT(LIT_END), text);
            
            WinShowWindow(g_hwndRexxError, TRUE);
            WinSetWindowPos(g_hwndRexxError, HWND_TOP, 0, 0, 0, 0,SWP_ZORDER);
            //          WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "", 1234, MB_OK|MB_MOVEABLE|MB_ERROR);
          }
          DosFreeMem(rexxretval.strptr);          /* Release storage given to us by REXX.  */      
        }
        CATCH(RX_START)
          {}END_CATCH;
          //#endif

          /* Destroy all windows owned by our object window. If not doing
             it here PM will silently destroy them, when the thread ends.
             But when doing that the WM_DESTROY msg will NOT be sent and thus
             we can't clean up the resources. An explicit call to WinDestroyWindow()
             ensures we have time to clean up. */
          destroyWindows(hwndGadgetObj);
          /* Desxtroy the object window */
          WinDestroyWindow(hwndGadgetObj);
      } /* hwndGadgetObj */
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  thisPtr->wpFreeMem((PBYTE)tPt);
}
#endif


/* This function handles new WPS functions introduced by private classes */
ULONG EXPENTRY rxCallCWWPSFunc(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring)
{
  char text[200];
  ULONG ulHash;
  ULONG ulReturn;
  WPObject* wpObject;
  WPFileSystem* wpFSObject;

  /* At least WPObject* and name of method */
  if(argc<2)
    return 40;

  TRY_LOUD(RX) {

    /* Get a hash for the methodname for the switch() statement */
    ulHash=calculateHash(argv[0].strptr)<<8;
    ulHash+=calculateHash2(argv[0].strptr);
    ulReturn=0;

    wpObject=(WPObject*)atol(argv[1].strptr);
    /* Check if it's a valid object */
    if(somIsObj(wpObject)) {
      PFNWPSFUNC procAddr;

      //SysWriteToTrapLog("%s object: %x hash: %x\n", argv[0].strptr, wpObject, ulHash);

      /* Try to find the requested method */
      if((procAddr=(PFNWPSFUNC) somResolveByName(wpObject, argv[0].strptr))==NULL) {
        /* method not found. Check if it's a wrapped one */
        switch(ulHash)
          {
          case 0x1b2a:  /* cwIsA */
            procAddr=(PFNWPSFUNC) somResolveByName(wpObject, "somIsA");
            break;
          case 0x2c21:  /* cwIsInstanceOf */
            procAddr=(PFNWPSFUNC) somResolveByName(wpObject, "somIsInstanceOf");
            break;
          case 0x5226:  /* cwQueryRealName */ 
            procAddr=(PFNWPSFUNC) somResolveByName(wpObject, "wpQueryRealName");
            break;
            /* The following are not methods of the object. Set procAddr!=0
               to make sure no error is raised */
          case 0x1c51:  /* cwDisplayRexxError */
          case 0x0a5e:  /* cwIsObj */
          case 0x6140:  /* cwCreateLaunchArea */
            procAddr=(PFNWPSFUNC) 1; /* Make sure we continue */
            break;
#if 0
          case 0x5530: /* cwShowLaunchArea */
            procAddr=(PFNWPSFUNC) somResolveByName(wpObject, "cwShowLaunchArea");
            break;
          case 0x512c: /* somIsObj                */               
            break;
          case 0x24 : /* cwQueryFilename */
            procAddr=(PFNWPSFUNC) somResolveByName(wpObject, "wpQueryFilename");
            break;
#endif
          default:

            break;
          }
      }

      if(procAddr==NULL) {
        /* method not found and no private one */
        sprintf(retstring->strptr, "ERROR_METHOD:");
        retstring->strlength=strlen(retstring->strptr);
      }
      else {
        // SysWriteToTrapLog("2 %s object: %x hash: %x\n", argv[0].strptr, wpObject, ulHash);
        switch(ulHash)
          {
            /* Functions with one (ULONG) param returning a string: */
          case 0x5226: /* cwQueryRealName */
            {
              ULONG ulParam1;
             
              if(argc<3) {
                numParameterError(argv[0].strptr, 3, argc);
                ulReturn=40;
                break;
              }
              ulParam1=atol(argv[2].strptr);/* BOOL fQualified */

              if(!procAddr(wpObject, retstring->strptr, &retstring->strlength, ulParam1))
                sprintf(retstring->strptr,"ERROR:");
              /*
                if(!procAddr(wpObject, &retstring->strptr, &retstring->strlength, ulParam1))*/
              
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
            /* Function with one param returning ULONG */
          case 0x1b2a:  /* cwIsA */
          case 0x2c21:  /* cwIsInstanceOf */
            {
              SOMClass *somClass;

              if(argc<3) {
                numParameterError(argv[0].strptr, 3, argc);
                ulReturn=40;
                break;
              }

              somClass=cwGetSomClass(argv[2].strptr);
              if(!somIsObj(somClass)) {
                sprintf(retstring->strptr,"ERROR_CLASS:");/* Return error */
                retstring->strlength=strlen(retstring->strptr);
                break;
              }

              sprintf(retstring->strptr,"%d", procAddr(wpObject, somClass));
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
          case 0x0a5e: /* cwIsObj */
            sprintf(retstring->strptr,"%d", somIsObj(wpObject));
            retstring->strlength=strlen(retstring->strptr);
            break;

          case 0x6444:  /* wizCreateGadget */
            {
              GADGETDATA gd={0};
              ULONG ulError;
              char *chrFont;
              /*
                arg[0]: function name
                arg[1]: object
                arg[2]: stem
                arg[3]: gadget text
                */
              
              if(argc!=4) {
                numParameterError(argv[0].strptr, 3, argc);
                ulReturn=40;
                break;
              }
              gd.x=rxGetLongFromStem(argv[2], 1, &ulError);    /* x */
              gd.y=rxGetLongFromStem(argv[2], 2, &ulError);    /* y */
              gd.cx=rxGetLongFromStem(argv[2], 3, &ulError);   /* cx */
              gd.cy=rxGetLongFromStem(argv[2], 4, &ulError);   /* cy */
              gd.ulType=rxGetLongFromStem(argv[2], 5, &ulError); /* Gadget typ */
              gd.hwnd=rxGetLongFromStem(argv[2], 6, &ulError); /* owner */
              /* font */
              if((chrFont=rxGetStringFromStem(argv[2], 7, &ulError))!=NULLHANDLE) {
                strncpy(gd.chrFont, chrFont, sizeof(gd.chrFont));
                gd.chrFont[sizeof(gd.chrFont)-1]=0;
                DosFreeMem(chrFont);
              }
              sprintf(retstring->strptr,"%d", procAddr(wpObject, &gd , argv[3].strptr) );
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
          case 0x3d62:   /* wizDestroyGadget */
            {
              /*
                arg[0]: function name
                arg[1]: folder object
                arg[2]: gadget
                */
              if(argc!=3) {
                numParameterError(argv[0].strptr, 3, argc);
                ulReturn=40;
                break;
              }
              sprintf(retstring->strptr,"%d", procAddr(wpObject, atol(argv[2].strptr)));
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
          case 0x5306:  /* wizUpdateGadget */
            {
              GADGETDATA gd;
              ULONG ulError;
              char* chrText;
              /*
                arg[0]: function name
                arg[1]: object
                arg[2]: stem
                arg[3]: gadget
                */

              if(argc!=4) {
                numParameterError(argv[0].strptr, 4, argc);
                ulReturn=40;
                break;
              }
              gd.x=rxGetLongFromStem(argv[2], 1, &ulError);
              gd.y=rxGetLongFromStem(argv[2], 2, &ulError);
              gd.cx=rxGetLongFromStem(argv[2], 3, &ulError);
              gd.cy=rxGetLongFromStem(argv[2], 4, &ulError);
              chrText=rxGetStringFromStem(argv[2], 5, &ulError);
              sprintf(retstring->strptr,"%d", procAddr(wpObject, atol(argv[3].strptr), chrText) );
              if(chrText)
                DosFreeMem(chrText);
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
          case 0x4401: /* wizAddToolBar */
          case 0x4129: /* wizAddStatusBar */
            if(argc!=5) {
              numParameterError(argv[0].strptr, 5, argc);
              ulReturn=40;
              break;
            }
            
            sprintf(retstring->strptr,"%d",procAddr(wpObject, atol(argv[2].strptr), 
                                                     atol(argv[3].strptr), argv[4].strptr));
            retstring->strlength=strlen(retstring->strptr);
            break;
          case 0x6140: /* cwCreateLaunchArea */
            {
              /*
                argv[0]: function name
                argv[1]: folder object
                argv[2]: stem with window info
                argv[3]: flag
                argv[4]: config folder

                */
              ULONG ulError;
              HWND hwndParent, hwndNew;
              CWLaunchArea *cwla;
              HOBJECT hObject;

              if(argc!=5) {
                numParameterError(argv[0].strptr, 5, argc);
                ulReturn=40;
                break;
              }
              if(!rxGetLongFromStem(argv[2], 0, &ulError)) {
                rxerrParameterValueError(argv[0].strptr, 3); /* Show msg to user */
                ulReturn=40;
                break;
              }

              if((hObject=WinQueryObject(argv[4].strptr))==NULLHANDLE)
                {
                  rxerrParameterValueError(argv[0].strptr, 5); /* Show msg to user */
                  rxerrPrintErrorText("", "  Can't get HOBJECT for ", argv[4].strptr);
                  rxerrPrintErrorText("  Make sure the object exists.", "", "");
                  ulReturn=40;
                  break;
                }

              cwla=(CWLaunchArea*)((M_WPObject*)wpObject->somGetClass())->wpclsObjectFromHandle(hObject);

              if(!somObjectIsA(cwla, "CWLaunchArea")) {
                rxerrParameterValueError(argv[0].strptr, 5); /* Show msg to user */
                rxerrPrintErrorTextWithMethod(argv[0].strptr, argv[4].strptr," is not a CWLaunchArea object.");
                ulReturn=40;
                break;
              }

              /* Parent window handle from stem */
              hwndParent=rxGetLongFromStem(argv[2], 1, &ulError);
              
              hwndNew=cwla->cwCreateLaunchArea(hwndParent,
                                               rxGetLongFromStem(argv[2], 2, &ulError),/* Owner */
                                               rxGetLongFromStem(argv[2], 3, &ulError), /* x */
                                               rxGetLongFromStem(argv[2], 4, &ulError), /* y */
                                               rxGetLongFromStem(argv[2], 5, &ulError), /* cx */
                                               rxGetLongFromStem(argv[2], 6, &ulError), /* cy */
                                               atol(argv[3].strptr));                   /* flag, 1: action buttons */
              /* If the launcharea holds action button it has an owner object (the folder). Normal
                 launchpads don't need that. */
              if(atol(argv[3].strptr) & 0x00000001)
                cwla->cwSetOwnerObject(hwndNew, wpObject);

              //(atol(argv[3].strptr) & 1 ? wpObject : NULL);

              sprintf(retstring->strptr,"%d %d", 
                      hwndNew, cwla);
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
#if 0
          case 0x3943:  /* cwStartRexxThread */
            {
              PREXXTHREADPARMS rtP;
              char szFileName[CCHMAXPATH+1];
              /*
                arg[0]: function name
                arg[1]: object
                arg[2]: hwnd
                arg[3]: script
               */

              if(argc!=4) {
                numParameterError(argv[0].strptr, 4, argc);
                ulReturn=40;
                break;
              }
              /* Check window handle */
              if(!WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), atol(argv[2].strptr)))
                {
                  rxerrParameterValueError(argv[0].strptr, 3); /* Show msg to user */
                  ulReturn=40;
                  break;
                }

              /* Default return */
              sprintf(retstring->strptr,"%d", 1);

              rtP=(PREXXTHREADPARMS)wpObject->wpAllocMem(sizeof(REXXTHREADPARMS), NULL);
              if(rtP) {
                memset(rtP, 0, sizeof(REXXTHREADPARMS));
                sprintf(rtP->rexxSkript, argv[3].strptr);
                rtP->hwndFrame=atol(argv[2].strptr);
                //rtP->hwndFrame=NULLHANDLE;
                rtP->thisPtr=wpObject;
                strncpy(rtP->chrCommand, "/THREADSTARTED", sizeof(rtP->chrCommand));
                rtP->chrCommand[sizeof(rtP->chrCommand)-1]=0;
          
                if(_beginthread(rexxGuiThreadFunc, NULL,8192*16,(void*)rtP)==-1) {
                  wpObject->wpFreeMem((PBYTE)rtP); //Fehlerbehandlung fehlt
                  sprintf(retstring->strptr,"%d", 0);
                }
              }
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
#endif

            /* ---- Gadget funcs ---- */
          case 0x3518:  /* cwQueryGadgetPos */
            {
              SWP swp;
              /*
                arg[0]: function name
                arg[1]: object (gadget)
               */

              if(argc!=2) {
                numParameterError(argv[0].strptr, 2, argc);
                ulReturn=40;
                break;
              }
              if(procAddr(wpObject, &swp))
                sprintf(retstring->strptr,"%d %d %d %d", swp.x, swp.y, swp.cx, swp.cy);
              else
                sprintf(retstring->strptr,"");
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
          case 0x4946: /* cwSetGadgetBarValue */
            {
              /*
                arg[0]: function name
                arg[1]: object (gadget)
                arg[2]: ulCurrentValue
                arg[3]: ulMaxValue
               */

              if(argc!=4) {
                numParameterError(argv[0].strptr, 4, argc);
                ulReturn=40;
                break;
              }
              procAddr(wpObject, atol(argv[2].strptr), atol(argv[3].strptr));

              sprintf(retstring->strptr,"");
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
          case 0x1a19:  /* cwSetGadgetPos */
            {
              SWP swp;
              /*
                arg[0]: function name
                arg[1]: object (gadget)
                arg[2]: x
                arg[3]: y
                arg[4]: cx
                arg[5]: cy
               */

              if(argc!=6) {
                numParameterError(argv[0].strptr, 6, argc);
                ulReturn=40;
                break;
              }
              swp.x=atol(argv[2].strptr);
              swp.y=atol(argv[3].strptr);
              swp.cx=atol(argv[4].strptr);
              swp.cy=atol(argv[5].strptr);
              sprintf(retstring->strptr,"%d",procAddr(wpObject, &swp));
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
          case 0x2430:  /* cwSetGadgetText */
            {
              /*
                arg[0]: function name
                arg[1]: object (gadget)
                arg[2]: which text
                arg[3]: new text
               */

              if(argc!=4) {
                numParameterError(argv[0].strptr, 4, argc);
                ulReturn=40;
                break;
              }
              sprintf(retstring->strptr,"%d",procAddr(wpObject, atol(argv[2].strptr), 
                                                      argv[3].strptr));
              retstring->strlength=strlen(retstring->strptr);
              break;
            }

          default:
            rxerrMethodNameError("WPSWizCallFunc", argv[0].strptr);
            ulReturn=40;        
          }/* switch */
      }/* else if((procAddr=(PFNWPSFUNC) somResolveByName(wpObject, argv[1].strptr))==NULL) */
    }/* if(somIsObj(wpObject)) */
    else {
      switch(ulHash)
        {
          /* This proc doesn't has an object */
          case 0x1c51:  /* cwDisplayRexxError */
            {
              /*
                Displays the error text in the Rexx error window
                */
              /*
                arg[0]: function name
                arg[1]: text
                */
              if(argc!=2) {
                numParameterError(argv[0].strptr, 2, argc);
                ulReturn=40;
                break;
              }

              /* Set error text */
              WinSendMsg(WinWindowFromID(g_hwndRexxError, IDLB_REXXERROR), 
                         LM_INSERTITEM, MPFROMSHORT(LIT_END), argv[1].strptr);
              /* Show the error window */
              WinShowWindow(g_hwndRexxError, TRUE);
              WinSetWindowPos(g_hwndRexxError, HWND_TOP, 0, 0, 0, 0,SWP_ZORDER);

              sprintf(retstring->strptr,"");
              retstring->strlength=strlen(retstring->strptr);

              break;
            }
        default:
          /* No object given or object pointer not valid */
          wpObjectError(argv[0].strptr);
          ulReturn=40;
          break;
        }

    }/* else somIsObj(wpObject) */ 
  }
  CATCH(RX)
    {}END_CATCH;
    
    return ulReturn;
}


/* This function handles new WPS functions introduced by private classes */
ULONG EXPENTRY rxGetHash(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring)
{
  char text[200];
  ULONG ulHash;
  ULONG ulReturn;
  WPObject* wpObject;
  WPFileSystem* wpFSObject;

  ulHash=calculateHash(argv[0].strptr)<<8;

  sprintf(text, "%s Hash: %x, Hash2: %x",argv[0].strptr, ulHash, calculateHash2(argv[0].strptr));
  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "",
                1234, MB_OK|MB_MOVEABLE);

  return 0;  
}

/*

Functions without params returning a string:


Functions with one (ULONG) param returning ULONG, BOOL or WPObject*:

0x1b:  cwIsA                      0x2a
0x0a5e cwIsObj
Functions with one (ULONG) param returning a string:
0x52:  cwQueryRealName            0x26
0x24 : cwQueryFilename 

0x4401: wizAddToolBar
0x4129: wizAddStatusBar

0x2c:  cwIsInstanceOf                 0x21
0x2e: cwGetVersionNumbers       hash2: 0x58

0x6140:   cwCreateLaunchArea

0x5530:   cwShowLaunchArea  !!!! Not documented !!!!

0x3943:   cwStartRexxThread  !!!! Not documented !!!!

0x6444:   wizCreateGadget  !!!! Not documented !!!!
0x5306:   wizUpdateGadget  !!!! Not documented !!!!
0x3d62:   wizDestroyGadget !!!! Not documented !!!!

0x2430:  cwSetGadgetText   !!!! Not documented !!!!
0x3518:  cwQueryGadgetPos  !!!! Not documented !!!!
0x1a19:  cwSetGadgetPos    !!!! Not documented !!!!
0x4946:  cwSetGadgetBarValue     !!!! Not documented !!!!

0x1c51:  cwDisplayRexxError  !!!! Not documented !!!!
 */



