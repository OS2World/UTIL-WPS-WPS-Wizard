/*
 * This file is (C) Chris Wohlgemuth 1996/2004
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
#define INCL_PM

#include <os2.h>

#include "defines.h"
#include "newfile.h"
#include "newfileDlg.h"
#include "debug.h"
#include "except.h"

#include "wpswizzsettings.h"

#include "wpswizzsettings.hh"
#include "contextitem.hh"
#include "menufolder.hh"
#include "wpfolder.hh"

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>//for strcpy
#include <stdio.h>   // for sprintf
#include <stdlib.h>
#include "sys_funcs.h"

BOOL g_fRexxScriptsEnabled=FALSE; /* TRUE if REXX support is enabled */
BOOL g_fHintsEnabled=FALSE;       /* TRUE if hints enabled           */
BOOL g_fInfoWindowEnabled=FALSE;  /* TRUE if info window is enabled  */
BOOL g_fFileSizeEnabled=FALSE;

BOOL bNoFormatMenue=FALSE;
BOOL bGlobalMenuEnabled=FALSE;
BOOL bInit=FALSE;
ULONG ulOptions=0;

char chrInstallPath[CCHMAXPATH]={0};
char chrWPSWizIni[CCHMAXPATH]={0}; /* Path to INI file (...\wpswizz.ini ) */
char chrWPSWizHints[CCHMAXPATH]={0};
static char chrHomeDir[CCHMAXPATH];      /* The users home directory or "". Used as
                                            the target for the INI file  */
BOOL bMenuFolderRestored=FALSE;
BOOL bWizSettingsRestored=FALSE;
BOOL bMenuFolderReady=FALSE;

extern "C" BOOL cwMoveNotebookButtonsWarp4(HWND hwndDlg, 
                                USHORT usID, 
                                USHORT usDelta);

//void _Optlink rexxGuiThreadFunc (void *arg);

HMODULE queryModuleHandle(void)
{
	static HMODULE hmod=0;
	somId mySomId;

    if(hmod)
      return hmod;

	if(!hmod) {
      mySomId=somIdFromString(SETTINGS_CLASSNAME);
      PSZ pathname=SOMClassMgrObject
        ->somLocateClassFile(mySomId,1,2);
      SOMFree(mySomId);
      if(DosQueryModuleHandle(pathname,&hmod)!=NO_ERROR)
        hmod=0;
	}
	if(!hmod) {
      mySomId=somIdFromString(MENUFOLDER_CLASSNAME);
      PSZ pathname=SOMClassMgrObject
        ->somLocateClassFile(mySomId, 1, 2);
      SOMFree(mySomId);
      if(DosQueryModuleHandle(pathname,&hmod)!=NO_ERROR)
        hmod=0;
	}	
    return hmod;
}


PSZ queryHelpPath(void)
{
  static char directory[CCHMAXPATH]={0};
  char dir[_MAX_DIR];
  char drive[_MAX_DRIVE];
  char fname[_MAX_FNAME];
  char ext[_MAX_EXT];
  
  if(!(*directory)) {
    PSZ pathname=SOMClassMgrObject
      ->somLocateClassFile(somIdFromString(SETTINGS_CLASSNAME),1,2);
    _splitpath(pathname,drive,dir,fname,ext);
    sprintf(directory,"%s%sHelp\\%s",drive,dir,HELPFILENAME);
    if(SysCheckFileExists(directory))
      return directory;
  }
  return directory; 
}

void queryInstallDir(char * installPath, int size)
{
  char * ptrChar;

  if(*installPath)
    return;

  strcpy(installPath, queryHelpPath());
  if((ptrChar=strrchr(installPath, '\\'))!=NULLHANDLE) {
    *ptrChar=0;
    if((ptrChar=strrchr(installPath, '\\'))!=NULLHANDLE)
      *ptrChar=0;
  }
}

void queryIniFilePath(char * chrPath, int size)
{
  char * ptrChar;

  if(*chrPath)
    return; /* already queried */

  if(SysQueryHomeDir(chrHomeDir, sizeof(chrHomeDir))) {
    strcpy(chrPath, chrHomeDir);
    strncat(chrPath, "\\WPSWIZZ.INI", size-strlen(chrPath)-1);/* Default is INI file in users home dir */
    return;
  }

  strcpy(chrPath, queryHelpPath());
  if((ptrChar=strrchr(chrPath, '\\'))!=NULLHANDLE) {
    *ptrChar=0;
    if((ptrChar=strrchr(chrPath, '\\'))!=NULLHANDLE)
      *ptrChar=0;
  }
  strncat(chrPath, "\\WPSWIZZ.INI", size-strlen(chrPath)-1);
  chrPath[size-1]=0;
}

void queryHintFilePath(char * chrPath, int size)
{
  char * ptrChar;

  if(*chrPath)
    return;

  strcpy(chrPath, queryHelpPath());
  if((ptrChar=strrchr(chrPath, '\\'))!=NULLHANDLE) {
    *ptrChar=0;
    if((ptrChar=strrchr(chrPath, '\\'))!=NULLHANDLE)
      *ptrChar=0;
  }
  strncat(chrPath, "\\help\\OBJHINTS.INI", size-strlen(chrPath)-1);
  chrPath[size-1]=0;
}

static void recreateConfigFolders()
{
  HOBJECT hObject;
  char installPath[CCHMAXPATH];

  /* Check for default WPFolder folder */
  if((hObject=WinQueryObject("<MENU_WPFolder>"))==NULLHANDLE) {//is there a Menufolder?
    /* No folder, recreate it */
    queryInstallDir(installPath, sizeof(installPath));
    strcat(installPath,"\\bin\\createfd.cmd");
    if((hObject=WinQueryObject(installPath))!=NULLHANDLE)
      WinOpenObject(hObject,OPEN_DEFAULT, FALSE);
  }
  /* Check for default WPDataFile folder */
  if((hObject=WinQueryObject("<MENU_WPDataFile>"))==NULLHANDLE) {//is there a Menufolder?
    /* Config folder lost. Recreate it */
    queryInstallDir(installPath, sizeof(installPath));
    strcat(installPath,"\\bin\\createdf.cmd");
    if((hObject=WinQueryObject(installPath))!=NULLHANDLE)
      WinOpenObject(hObject,OPEN_DEFAULT, FALSE);
  }
}


/************* Meta class ******************/

ULONG M_CWWizzSettings::wpclsQueryStyle()
{
  somId id;
  ULONG rc;

  if((id=somIdFromString("wpclsQueryStyle"))!=NULL){
    rc=((somTD_M_WPObject_wpclsQueryStyle)
         somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                             1,
                             __ClassObject->
                             somGetMethodToken(id))
         )(this)|CLSSTYLE_NEVERTEMPLATE;
    SOMFree(id);
  }
  return rc;
}

PSZ M_CWWizzSettings::wpclsQueryTitle()
{
	static char name[50]="";


	if(!WinLoadString(WinQueryAnchorBlock(HWND_DESKTOP),queryModuleHandle(),
                      ID_CONFIGURATIONCLASSNAME,sizeof(name),name))
      strcpy(name,"Menu-Configuration");	
	
	return name;
}

ULONG M_CWWizzSettings::wpclsQueryIconData(PICONINFO pIconInfo)
{
	if (pIconInfo)   {
		pIconInfo->fFormat = ICON_RESOURCE;
		pIconInfo->hmod    = queryModuleHandle();
		pIconInfo->resid   = ID_ICONSETTINGS;
	} /* endif */
	return ( sizeof(ICONINFO) );
}

ULONG M_CWWizzSettings::wpclsQueryDefaultHelp(PULONG HelpPanelId,PSZ HelpLibrary)
{
	static char test[200];
	sprintf(test,"menufold.hlp");
	
	if(HelpPanelId)
      *HelpPanelId=202;
	if(HelpLibrary)
      strcpy(HelpLibrary,queryHelpPath());	
	return (TRUE);
}

void M_CWWizzSettings::wpclsInitData()
{
  ULONG dataSize;
  char profileName[CCHMAXPATH]={0};
  somId id;  
  HMODULE hmodule;
  ULONG rc;
  HINI hini;
  char error[CCHMAXPATH]; 
  
  /* Call parent */
  if((id=somIdFromString("wpclsInitData"))!=NULL){
    ((somTD_M_WPObject_wpclsInitData)//Damit existiert nur das Einstellungen-Menue, kein ôffnen
     somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                         1,
                         __ClassObject->
                         somGetMethodToken(id))
     )(this);  
    SOMFree(id);
  }
  else
    M_WPAbstract::wpclsInitData();
    
  if(!bInit) {
    queryInstallDir(chrInstallPath, sizeof(chrInstallPath));	
    // Read values from user.ini //
    dataSize=sizeof(ulOptions);

    queryIniFilePath(profileName, sizeof(profileName));
    //    sprintf(profileName,"%s\\wpswizz.ini",chrInstallPath);
    if((hini=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),profileName))!=NULL) {
      if(!PrfQueryProfileData(hini,"WPSWizard","Options",&ulOptions,&dataSize)) {
        g_fFileSizeEnabled=TRUE;
        bNoFormatMenue=TRUE;
        bGlobalMenuEnabled=TRUE;
        g_fHintsEnabled=TRUE;
        ulOptions=FILESIZEENABLED|NOFORMATMENUE|MENUENABLED;
        PrfWriteProfileData(hini,"WPSWizard","Options",
                            &ulOptions,sizeof(ULONG));
      }
      else {
#if 0
        bFileSizeEnabled=ulOptions&FILESIZEENABLED;
        bNoFormatMenue=ulOptions&NOFORMATMENUE;
        bGlobalMenuEnabled=ulOptions&MENUENABLED;
        g_fRexxScriptsEnabled=(ulOptions & REXXSCRIPTSENABLED);
        g_fHintsEnabled=ulOptions & HINTSENABLED;
#endif
          if(ulOptions & FILESIZEENABLED)
            g_fFileSizeEnabled=1;//BM_SETCHECK only allows 0, 1, 2
          else
            g_fFileSizeEnabled=0;
          if(ulOptions & NOFORMATMENUE)
            bNoFormatMenue=1;
          else
            bNoFormatMenue=0;
          if(ulOptions & MENUENABLED)
            bGlobalMenuEnabled=1;
          else
            bGlobalMenuEnabled=0;
          if(ulOptions & REXXSCRIPTSENABLED)
            g_fRexxScriptsEnabled=1;
          else
            g_fRexxScriptsEnabled=0;
          if(ulOptions & HINTSENABLED)
            g_fHintsEnabled=1;
          else
            g_fHintsEnabled=0;
          /* Info area in folders */
          if(ulOptions & INFOWINDOWENABLED)
            g_fInfoWindowEnabled=1;
          else
            g_fInfoWindowEnabled=0;

      }
      PrfCloseProfile(hini);
    }
    bInit=TRUE;
  }
}

void M_CWWizzSettings::wpclsUnInitData()
{
  somId myId;

  if((myId=somIdFromString("wpclsUnInitData"))!=NULLHANDLE) {
    ((somTD_M_WPObject_wpclsUnInitData)//Damit existiert nur das Einstellungen-Menue, kein ôffnen
     somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                         1,
                         __ClassObject->
                         somGetMethodToken(myId))
     )(this);
    SOMFree(myId);
  }
  else
    M_WPAbstract::wpclsUnInitData();

  bInit=FALSE;
}


/******* Class methods ***************/

BOOL CWWizzSettings::wpSetup(PSZ pSetupString)
{
  char buffer[CCHMAXPATH+20];
  ULONG bufferSize;
  BOOL rc, rc2;
  TID tid;
  somId mySomId;

  PSZ pMyString="DEFAULTVIEW=SETTINGS;NODELETE=YES";

#if 0
  if(bShowMenuItems) {
    bufferSize=sizeof(buffer);
    if(wpScanSetupString(pSetupString,"LOADDLLS",buffer,&bufferSize)) {
      DosCreateThread(&tid,initDLLsThreadFunc,(ULONG)this,CREATE_READY|STACK_SPARSE,8192*2);
    }
    
    bufferSize=sizeof(buffer);
    if(wpScanSetupString(pSetupString,"UNLOADDLLS",buffer,&bufferSize)) {
      DosCreateThread(&tid,freeDLLsThreadFunc,0,CREATE_READY|STACK_SPARSE,8192*2);
    }
  }
#endif
  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString,"RECREATECONFIG",buffer,&bufferSize))
    recreateConfigFolders();

#if 0
  /* Now in cwfolder.c */
    /* Start gadget */
    bufferSize=sizeof(buffer);
    if(wpScanSetupString( pSetupString, SETUP_LAUNCHGADGET, buffer, &bufferSize)) {
      PREXXTHREADPARMS rtP;
      char *chrPtr;
      if((chrPtr=strrchr(buffer, ','))!=NULLHANDLE) {
        WPObject *wpObject;
        *chrPtr++=0;
        wpObject=(WPObject*)atol(chrPtr);
        rtP=(PREXXTHREADPARMS)wpObject->wpAllocMem(sizeof(REXXTHREADPARMS), NULL);
        if(rtP) {
          memset(rtP, 0, sizeof(REXXTHREADPARMS));
          sprintf(rtP->rexxSkript, buffer);
          rtP->hwndFrame=NULLHANDLE;
          rtP->thisPtr=wpObject;
          strncpy(rtP->chrCommand, "/GADGETSTARTED", sizeof(rtP->chrCommand));
          rtP->chrCommand[sizeof(rtP->chrCommand)-1]=0;
          
          if(_beginthread(rexxGuiThreadFunc, NULL,8192*16,(void*)rtP)==-1) {
            wpFreeMem((PBYTE)rtP); //Fehlerbehandlung fehlt
          }
        }
      }
    }
#endif

  if((mySomId=somIdFromString("wpSetup"))!=NULLHANDLE) {
    rc=((somTD_WPObject_wpSetup)
        somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                            1,
                            __ClassObject->
                            somGetMethodToken(mySomId))                    
        )(this,pSetupString);
    SOMFree(mySomId);
  }
  else
    rc=WPAbstract::wpSetup(pSetupString);
  

  bufferSize=sizeof(buffer);
  rc2=wpScanSetupString(pSetupString,"NODELETE",buffer,&bufferSize);

  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString,"DEFAULTVIEW",buffer,&bufferSize) || rc2)
    { 
      somId mySomId;
      
      if((mySomId=somIdFromString("wpSetup"))!=NULLHANDLE) {
        rc=((somTD_WPObject_wpSetup)//Damit existiert nur das Einstellungen-Menue, kein ôffnen
            somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                                1,
                                __ClassObject->
                                somGetMethodToken(mySomId))
            )(this, pMyString);
        SOMFree(mySomId);
      }
      else
        rc=WPAbstract::wpSetup(pMyString);
    }
  
  return rc;
}


ULONG CWWizzSettings::wpAddObjectWindowPage(HWND hwndNotebook)
{
	return SETTINGS_PAGE_REMOVED;
}



static MRESULT EXPENTRY FileOptionDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
	PAGEINFO pageInfo;
	HINI hini;
	HOBJECT  hObject;
	ULONG  attrFound;
	ULONG  len;
    CWWizzSettings* thisPtr;
	

	
	switch(msg) {
	case WM_INITDLG :
      WinSetWindowPtr(WinWindowFromID(hwnd, ID_CONTEXTMENUE),QWL_USER,PVOIDFROMMP(mp2));//this-ptr sichern
      WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwnd, FODLG_SHOWSIZE));
      WinSendMsg(WinWindowFromID(hwnd, FODLG_SHOWSIZE),
                 BM_SETCHECK,MPFROMSHORT(g_fFileSizeEnabled),(MPARAM)NULL);
      WinSendMsg(WinWindowFromID(hwnd, ID_NOFORMATMENUE),
                 BM_SETCHECK,MPFROMSHORT(bNoFormatMenue),(MPARAM)NULL);
      WinSendMsg(WinWindowFromID(hwnd, ID_CONTEXTMENUE),
                 BM_SETCHECK,MPFROMSHORT(bGlobalMenuEnabled),(MPARAM)NULL);
      WinSendMsg(WinWindowFromID(hwnd, ID_ENABLEREXX),
                 BM_SETCHECK,MPFROMSHORT(g_fRexxScriptsEnabled),(MPARAM)NULL);
      WinSendMsg(WinWindowFromID(hwnd, ID_ENABLEHINTS),
                 BM_SETCHECK,MPFROMSHORT(g_fHintsEnabled),(MPARAM)NULL);
      WinSendMsg(WinWindowFromID(hwnd, IDCB_FLDRINFOWINDOW),
                 BM_SETCHECK,MPFROMSHORT(g_fInfoWindowEnabled),(MPARAM)NULL);
      if(!g_fRexxScriptsEnabled)
        {
          /* Rexx extension disabled */
          WinEnableWindow(WinWindowFromID(hwnd, ID_ENABLEHINTS), FALSE);
          WinEnableWindow(WinWindowFromID(hwnd, IDCB_FLDRINFOWINDOW), FALSE);
        }
      /* Move default buttons on Warp 4 */
      cwMoveNotebookButtonsWarp4(hwnd,
                                 ID_OPTIONHELP,    // in: ID threshold
                                 20);              // in: dialog units or 0
      
      return (MRESULT) TRUE;
	case WM_COMMAND :
      switch (SHORT1FROMMP(mp1)) {
        /* Process commands here */
        break;
      }
      /* Don't call WinDefDlgProc here, or the dialog gets closed */
      return (MRESULT) TRUE;
      break;
	case WM_CONTROL:
      switch (SHORT2FROMMP(mp1)) {
      case BN_CLICKED:
        {
          char chrPath[CCHMAXPATH];

          DosQuerySysInfo(QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &attrFound, sizeof(attrFound));
          sprintf(chrPath,"%c:\\config.sys", 'A'+attrFound-1);

          switch(SHORT1FROMMP(mp1)) {
          case IDCB_FLDRINFOWINDOW:
            if(g_fInfoWindowEnabled){
              g_fInfoWindowEnabled=FALSE;
              if((hObject=WinQueryObject(chrPath))!=NULLHANDLE) {
                WinSetObjectData(hObject, SETUP_ENABLEINFOWINDOW"=0");/* This is catched in wpSetup() of CWObject */
              }
              ulOptions&=~INFOWINDOWENABLED;
            }// end if
            else {
              if((hObject=WinQueryObject(chrPath))!=NULLHANDLE) {
                WinSetObjectData(hObject, SETUP_ENABLEINFOWINDOW"=1");/* This is catched in wpSetup() of CWObject */
              }
              g_fInfoWindowEnabled=TRUE;
              ulOptions|=INFOWINDOWENABLED;
            }
            break;          
          case ID_ENABLEHINTS:
            if(g_fHintsEnabled){
              g_fHintsEnabled=FALSE;
              if((hObject=WinQueryObject(chrPath))!=NULLHANDLE) {
                WinSetObjectData(hObject,SETUP_ENABLEHINTS"=0");/* This is catched in wpSetup() of CWObject */
              }
              ulOptions&=~HINTSENABLED;
            }// end if
            else {
              if((hObject=WinQueryObject(chrPath))!=NULLHANDLE) {
                WinSetObjectData(hObject,SETUP_ENABLEHINTS"=1");/* This is catched in wpSetup() of CWObject */
              }
              g_fHintsEnabled=TRUE;
              ulOptions|=HINTSENABLED;
            }
            break;          
          case ID_ENABLEREXX:
            if(g_fRexxScriptsEnabled){
              g_fRexxScriptsEnabled=FALSE;
              if((hObject=WinQueryObject(chrPath))!=NULLHANDLE) {
                WinSetObjectData(hObject,SETUP_ENABLESCRIPTS"=0");/* This is catched in wpSetup() of CWObject */
              }
              ulOptions&=~REXXSCRIPTSENABLED;
              WinEnableWindow(WinWindowFromID(hwnd, ID_ENABLEHINTS), FALSE);
              WinEnableWindow(WinWindowFromID(hwnd, IDCB_FLDRINFOWINDOW), FALSE);
            }// end if
            else {
              if((hObject=WinQueryObject(chrPath))!=NULLHANDLE) {
                WinSetObjectData(hObject,SETUP_ENABLESCRIPTS"=1");/* This is catched in wpSetup() of CWObject */
              }
              g_fRexxScriptsEnabled=TRUE;
              ulOptions|=REXXSCRIPTSENABLED;
              WinEnableWindow(WinWindowFromID(hwnd, ID_ENABLEHINTS), TRUE);
              WinEnableWindow(WinWindowFromID(hwnd, IDCB_FLDRINFOWINDOW), TRUE);
            }
          
            break;
          case FODLG_SHOWSIZE:
            if(g_fFileSizeEnabled){
              g_fFileSizeEnabled=FALSE;
              if((hObject=WinQueryObject(chrPath))!=NULLHANDLE) {
                WinSetObjectData(hObject,"ENABLESIZEITEM=0");
              }
              ulOptions&=~FILESIZEENABLED;
            }// end if
            else {
              if((hObject=WinQueryObject(chrPath))!=NULLHANDLE) {
                WinSetObjectData(hObject,"ENABLESIZEITEM=1");
              }
              g_fFileSizeEnabled=TRUE;
              ulOptions|=FILESIZEENABLED;
            }
            /*				WinSendMsg(WinWindowFromID(hwnd, FODLG_SHOWSIZE),
                            BM_SETCHECK,MPFROMSHORT(bFileSizeEnabled),(MPARAM)NULL);*/
            break;
          case ID_NOFORMATMENUE:
            if(bNoFormatMenue){
              if((hObject=WinQueryObject("<WP_DRIVE_C>"))!=NULLHANDLE) {
                WinSetObjectData(hObject,"DISABLEFORMATMENU=0");
              }
              bNoFormatMenue=FALSE;
              ulOptions&=~NOFORMATMENUE;
            }// end if
            else{
              if((hObject=WinQueryObject("<WP_DRIVE_C>"))!=NULLHANDLE) {
                WinSetObjectData(hObject,"DISABLEFORMATMENU=1");
              }
              bNoFormatMenue=TRUE;
              ulOptions|=NOFORMATMENUE;
            }
            break;
          case ID_CONTEXTMENUE:
            if(bGlobalMenuEnabled){
              bGlobalMenuEnabled=FALSE;
              ulOptions&=~MENUENABLED;
            }// end if
            else{
              bGlobalMenuEnabled=TRUE;
              ulOptions|=MENUENABLED;
            }
            WinSendMsg(WinWindowFromID(hwnd, ID_CONTEXTMENUE),
                       BM_SETCHECK,MPFROMSHORT(bGlobalMenuEnabled),(MPARAM)NULL);
            break;
          default:
            break;
          } // end switch(SHORT1FROMMP(mp1)) 
        }
        return (MRESULT) TRUE;
      }
    case WM_DESTROY:
      thisPtr=(CWWizzSettings*)WinQueryWindowPtr(WinWindowFromID(hwnd, ID_CONTEXTMENUE),QWL_USER);
      if(somIsObj(thisPtr))
        thisPtr->wpSaveDeferred();
      break;
    default:
      break;
	}
	return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

ULONG CWWizzSettings::wsAddFileOptionPage(HWND hwndNotebook)
{
	PAGEINFO pageinfo;
	char pageName[50]={0};
	
	memset((PCH)&pageinfo, 0, sizeof(PAGEINFO));
	pageinfo.cb = sizeof(PAGEINFO);
	pageinfo.hwndPage = NULLHANDLE;
	pageinfo.usPageStyleFlags = BKA_MAJOR|BKA_STATUSTEXTON;
	pageinfo.usPageInsertFlags = BKA_FIRST;
	pageinfo.usSettingsFlags = SETTINGS_PAGE_NUMBERS;
	pageinfo.pszName = pageName;
	pageinfo.pfnwp = FileOptionDlgProc;
	pageinfo.resid = queryModuleHandle();
	pageinfo.dlgid = ID_FILEOPTIONPAGE;
	pageinfo.pCreateParams = this;
	pageinfo.idDefaultHelpPanel = 100;
	pageinfo.pszHelpLibraryName = queryHelpPath();
	
    WinLoadString(WinQueryAnchorBlock(hwndNotebook),queryModuleHandle(),
                  ID_OPTIONPAGENAME,sizeof(pageName),pageName);	

	return wpInsertSettingsPage(hwndNotebook,&pageinfo);

}


BOOL CWWizzSettings::wpAddSettingsPages(HWND hwndNotebook)
{
	ULONG rc, rcProc;
    int i;
    PAGEINFO pageinfo;
	char pageName[50]={0};
    somId mySomId;

    if((mySomId=somIdFromString("wpAddSettingsPages"))!=NULLHANDLE) {
      rc=((somTD_WPObject_wpAddSettingsPages)
          somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                              1,
                              __ClassObject->
                              somGetMethodToken(mySomId))
          )(this,hwndNotebook);
      SOMFree(mySomId);
    }
    else
      rc=WPAbstract::wpAddSettingsPages(hwndNotebook);

    return(rc && wsAddFileOptionPage(hwndNotebook));
}


void CWWizzSettings::wpInitData()
{
    somId mySomId;

	// Init parent //
    if((mySomId=somIdFromString("wpInitData"))!=NULLHANDLE) {
      ((somTD_WPObject_wpInitData)
       somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                           1,
                           __ClassObject->
                           somGetMethodToken(mySomId))
       )(this);
      SOMFree(mySomId);
    }
    else
      WPAbstract::wpInitData();

	// Durch obige Konstruktion wird die direkte Elternmethode aufgerufen,
	// und nicht die ursprÅngliche von WPLaunchPad, falls WPLaunchPad replaced wurde.
	// Durch diesen Aufruf gehen eventuelle Erweiterungen durch Vererbung verloren:
	//            < return WPLaunchPad::wpMenuItemSelected(hwndFrame, ulMenuId); >
}

BOOL CWWizzSettings::wpSaveState()
{
	char profileName[CCHMAXPATH]={0};
    HINI hini;
    BOOL rc;
    somId mySomId;

    if((mySomId=somIdFromString("wpSaveState"))!=NULLHANDLE) {
      rc=((somTD_WPObject_wpSaveState)
          somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                              1,
                              __ClassObject->
                              somGetMethodToken(mySomId))
          )(this);
      SOMFree(mySomId);
    }
    else
      rc=WPAbstract::wpSaveState();

    queryIniFilePath(profileName, sizeof(profileName));
    if((hini=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),profileName))!=NULL) {
      PrfWriteProfileData(hini,"WPSWizard","Options",
                          &ulOptions,sizeof(ULONG));
      PrfCloseProfile(hini);
    }
    return rc;
}


