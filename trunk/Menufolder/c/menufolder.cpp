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
#define INCL_REXXSAA

#include <os2.h>

#include "newfile.h"
#include "newfileDlg.h"
//#include "debug.h"
#include "except.h"
#include <rexxsaa.h>                   /* needed for RexxStart()     */

#include "menufolder.hh"
#include "contextitem.hh"
/* Include file with definition for WPShadow class */
#include "wpshadow.hh"

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>//for strcpy
#include <stdio.h>   // for sprintf
#include <stdlib.h>
#include "defines.h"
#include "sys_funcs.h"

extern BOOL g_fFileSizeEnabled;
extern BOOL bNoFormatMenue;
extern BOOL bGlobalMenuEnabled;

extern BOOL bInit;
extern ULONG ulOptions;
extern char chrInstallPath[CCHMAXPATH];
extern char chrWPSWizIni[CCHMAXPATH];
extern BOOL bMenuFolderRestored;
extern BOOL bMenuFolderReady;

/* Vars for the new button class */
ULONG   g_ulButtonDataOffset=0;
PFNWP   g_pfnwpOrgButtonProc=NULLHANDLE;
ULONG   ulQWP_WIZBUTTONDATA=0;
HWND    g_hwndRexxError=NULLHANDLE; /* Dialog handle of the rexx error console */
BOOL    g_haveFlyOverClient=FALSE;/* Blended flyover window is only available if CWFolder is
                                  installed. */
static BOOL fInitDone=FALSE; /* Flag to make sure global stuff is registered once */

MRESULT EXPENTRY fnwpWizButtonProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
BOOL percentRegisterBarClass(void);
LONG EXPENTRY cwRexxExitHandler(LONG lExitNumber, LONG lSubfunction, PEXIT pexParameter);

extern "C" BOOL cwMoveNotebookButtonsWarp4(HWND hwndDlg, 
                                USHORT usID, 
                                USHORT usDelta);


HMODULE queryModuleHandle(void);
PSZ queryHelpPath(void);
void queryInstallDir(char * installPath, int size);
void queryIniFilePath(char * chrPath, int size);
BOOL checkFileExists(char* chrFileName);
PSZ substitutePlaceHolder(PSZ orgPSZ, PSZ pathPSZ, char* resultString, int iSize);
PSZ deleteTilde(PSZ orgPSZ, char *resultString, int iSize);
ULONG EXPENTRY rxCallWPSFunc(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring);
ULONG EXPENTRY rxCallPMFunc(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring);
ULONG EXPENTRY rxCallCWWPSFunc(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring);
ULONG EXPENTRY rxCallWPSClassFunc(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring);
/* Test function for development to get a hash from the function name */
ULONG EXPENTRY rxGetHash(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring);

void _Optlink rexxThreadFunc (void *arg);

BOOL showMessage(HWND hwnd,PSZ msgText,PSZ titleText="Debug Menu Folder")
{
	return WinMessageBox(HWND_DESKTOP,hwnd ,msgText ,titleText,0,MB_YESNO|MB_MOVEABLE);
}

SOMClass* cwGetSomClass(char* chrClassName)
{
  somId    mySomId;
  SOMClass *somClass;

  if((mySomId=somIdFromString(chrClassName))==NULLHANDLE)
    return NULLHANDLE;

  /* somFindClass() loads the class if not already done */
  somClass=SOMClassMgrObject->somFindClass(mySomId, 1, 1);
  SOMFree(mySomId);

  return somClass;
}

static MRESULT EXPENTRY pfnwpRexxErrorProc(HWND hwnd, ULONG msg,MPARAM mp1,MPARAM mp2 )
{
  switch (msg)
    {
    case WM_COMMAND:
      if(SHORT1FROMMP(mp2)==CMDSRC_PUSHBUTTON) {
        switch(SHORT1FROMMP(mp1))
          {
          case DID_OK:
            WinShowWindow(hwnd, FALSE);
            break;
          default:
            break;
          }
      }
      return MRFALSE;
    case WM_CLOSE:
      WinShowWindow(hwnd, FALSE);
      return MRFALSE;
    default:
      break;
    }

  return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/*
 * Define the class name as an object type
 */
#ifndef M_CWObject
#define M_CWObject SOMObject
#endif
typedef BOOL   SOMLINK somTP_M_CWObject_wizclsSetRexxErrorWindowHWND(M_CWObject *somSelf,
		HWND hwndRexxError);
typedef somTP_M_CWObject_wizclsSetRexxErrorWindowHWND *somTD_M_CWObject_wizclsSetRexxErrorWindowHWND;

void M_CWMenuFolder::wpclsInitData()
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
    M_WPFolder::wpclsInitData();

  if(!fInitDone) {
    /* Get install dir */
    queryInstallDir(chrInstallPath, sizeof(chrInstallPath));
    /* INI file path */
    queryIniFilePath(chrWPSWizIni, sizeof(chrWPSWizIni));

    /* Register new button class */
    if(WinQueryClassInfo(WinQueryAnchorBlock(HWND_DESKTOP),
                         (PSZ) WC_BUTTON,
                         &ci))
      {
        g_ulButtonDataOffset=ci.cbWindowData;
        g_pfnwpOrgButtonProc=ci.pfnWindowProc;
        
        ulQWP_WIZBUTTONDATA=g_ulButtonDataOffset;
        
        if (WinRegisterClass(WinQueryAnchorBlock(HWND_DESKTOP),
                             (PSZ) WC_WIZBUTTON,
                             fnwpWizButtonProc,
                             //  ci.pfnWindowProc, 
                             ci.flClassStyle&~CS_PUBLIC,
                             ci.cbWindowData + WIZBUTTON_DATASIZE))
          {
            /* */

          }
      }

    if(WinQueryClassInfo(WinQueryAnchorBlock(HWND_DESKTOP),
                         WC_STATIC,
                         &ci))
      {
        g_haveFlyOverClient=TRUE; /* Blended flyover window is only available if CWFolder is
                                     installed. */
      }
    
    /* Register percentbar class */
    if(!percentRegisterBarClass())
      SysWriteToTrapLog("Can't register percent bar window class.\n");
    
    /* Register REXX functions */
    RexxRegisterFunctionExe("WPSWizCallWPSFunc", (PFN)rxCallWPSFunc);
    RexxRegisterFunctionExe("WPSWizCallWinFunc", (PFN)rxCallPMFunc);
    RexxRegisterFunctionExe("WPSWizCallFunc", (PFN)rxCallCWWPSFunc);
    RexxRegisterFunctionExe("WPSWizCallWPSClsFunc", (PFN)rxCallWPSClassFunc);
    RexxRegisterFunctionExe("DevGetHash", (PFN)rxGetHash);
    /* Register the gadget REXX function */
    RexxRegisterFunctionExe("WPSWizGadgetFunc", (PFN)rxCallCWWPSFunc);
    RexxRegisterExitExe("WIZREXX", (PFN)cwRexxExitHandler, NULLHANDLE);

    /* Load Rexx error dialog */
    if(!g_hwndRexxError) {
      somTD_M_CWObject_wizclsSetRexxErrorWindowHWND methodPtr;

      g_hwndRexxError=WinLoadDlg(HWND_DESKTOP, HWND_DESKTOP, pfnwpRexxErrorProc, 
                                 queryModuleHandle(), IDDLG_REXXERROR, NULLHANDLE);

      TRY_LOUD(ERROR_HWND) {
        /* Make sure M_CWObject class is installed and replaces the normal M_WPObject
           class. If yes, the M_CWMenuFolder class has a new static method
           "wizclsSetRexxErrorWindowHWND". */
        methodPtr=(somTD_M_CWObject_wizclsSetRexxErrorWindowHWND)
          somResolveByName( this,
                          "wizclsSetRexxErrorWindowHWND");
        //   SysWriteToTrapLog("%s: %x\n", __FUNCTION__, methodPtr);
        if(methodPtr)
          methodPtr(this, g_hwndRexxError);
          }
      CATCH(ERROR_HWND)
        {
          SysWriteToTrapLog("%s: Error while setting REXX error window HWND.\n", __FUNCTION__);
        } END_CATCH;
        
    }
    fInitDone=TRUE;
  }
}

void M_CWMenuFolder::wpclsUnInitData()
{
  somId mySomId;

  /* Don't deregister REXX funcs here because other classes (CWFolder) need them */
#if 0
  RexxDeregisterFunction("WPSWizCallFunc");
  RexxDeregisterFunction("WPSWizCallWinFunc");
  RexxDeregisterFunction("WPSWizCallWPSFunc");
  RexxDeregisterFunction("WPSWizCallWPSClsFunc");
#endif

  /* Call parent */
  if((mySomId=somIdFromString("wpclsUnInitData"))!=NULLHANDLE) {
    ((somTD_M_WPObject_wpclsUnInitData)
     somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                         1,
                         __ClassObject->
                         somGetMethodToken(mySomId))
     )(this);
    SOMFree(mySomId);
  }
  else
    M_WPFolder::wpclsUnInitData();

}
PSZ M_CWMenuFolder::wpclsQueryTitle()
{
  static char name[50]="";
  
  if(!WinLoadString(WinQueryAnchorBlock(HWND_DESKTOP),queryModuleHandle(),
                    ID_MENUFOLDERCLASSNAME,sizeof(name),name))
    strcpy(name,"Menu-Folder");	
  
  return name;
}

ULONG M_CWMenuFolder::wpclsQueryIconData(PICONINFO pIconInfo)
{
	if (pIconInfo)   {
		pIconInfo->fFormat = ICON_RESOURCE;
		pIconInfo->hmod    = queryModuleHandle();
		pIconInfo->resid   = ID_ICONMENUFOLD1;
	} /* endif */
	return ( sizeof(ICONINFO) );
}

ULONG M_CWMenuFolder::wpclsQueryIconDataN(PICONINFO pIconInfo, ULONG ulIconIndex)
{
	if (pIconInfo)   {
		pIconInfo->fFormat = ICON_RESOURCE;
		pIconInfo->hmod    = queryModuleHandle();
		pIconInfo->resid   = ID_ICONMENUFOLD2;
	} /* endif */
	return ( sizeof(ICONINFO) );
}

ULONG M_CWMenuFolder::wpclsQueryDefaultHelp(PULONG HelpPanelId,PSZ HelpLibrary)
{
  if(HelpPanelId)
    *HelpPanelId=201;

  if(HelpLibrary)
    strcpy(HelpLibrary,queryHelpPath());	
  
  return (TRUE);
}


void CWMenuFolder::wpObjectReady(ULONG ulCode, WPObject* refObject)
{
  somId mySomId;

  if((mySomId=somIdFromString("wpObjectReady"))!=NULLHANDLE) {
    ((somTD_WPObject_wpObjectReady)
     somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                         1,
                         __ClassObject->
                         somGetMethodToken(mySomId))                    
     )(this, ulCode, refObject);    
    SOMFree(mySomId);
  }
  else
    WPFolder::wpObjectReady(ulCode, refObject);
 
  bMenuFolderReady=TRUE;
}

BOOL CWMenuFolder::wpSetupOnce(PSZ pSetupString)
{
 somId mySomId;

 bMenuEnabled=TRUE;
 ulEnable=ID_MENUENABLED;
 wpSaveDeferred();

  if((mySomId=somIdFromString("wpSetupOnce"))!=NULLHANDLE) {
    BOOL rc;

    rc=((somTD_WPObject_wpSetupOnce)
        somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                            1,
                            __ClassObject->    
                            somGetMethodToken(mySomId))                    
        )(this, pSetupString);
    SOMFree(mySomId);
    return rc;
  }
  else 
    return WPFolder::wpSetupOnce(pSetupString);
}

BOOL CWMenuFolder::wpSetup(PSZ pSetupString)
{
  char buffer[CCHMAXPATH];
  ULONG bufferSize;
  somId mySomId;
  
  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString, SETUP_HIDECLASSPAGE, buffer, &bufferSize))
    { 
      if(atol(buffer) == 1)
        ulEnable|=ID_HIDECLASSPAGE;
      else
        ulEnable&=~ID_HIDECLASSPAGE;
    }

  /* Force the folder to reload the menu items */
  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString, SETUP_CLEARMENUCACHE , buffer, &bufferSize))
    { 
      if(atol(buffer) == 1)
        mfFreeMenu();	  
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
    return WPFolder::wpSetup(pSetupString);
}

void CWMenuFolder::wpCopiedFromTemplate()
{
  somId mySomId;

  ulEnable=ID_MENUENABLED;
  wpSaveDeferred();

  /* Call parent */
  if((mySomId=somIdFromString("wpCopiedFromTemplate"))!=NULLHANDLE) {
    ((somTD_WPObject_wpCopiedFromTemplate)
     somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                         1,
                         __ClassObject->    
                         somGetMethodToken(mySomId))                    
     )(this);
    SOMFree(mySomId);
  }
  else 
    WPFolder::wpCopiedFromTemplate();

  if(!strcmp(somGetClassName(),wpQueryFolder()->somGetClassName()) || (ulEnable & ID_HIDECLASSPAGE))// 0 if equal
    return; 

  // Open the settings so User may choose the class. Do it only if parent isn't a menu folder
  wpViewObject(NULLHANDLE, OPEN_SETTINGS, 0);
}

BOOL CWMenuFolder::wpRestoreState(ULONG ulReserved)
{
  somId mySomId;

  ptrCItem=NULL;
  if(wpRestoreLong(MENUFOLDER_CLASSNAME, IDKEY_ENABLED, &ulEnable))
    bMenuEnabled=(ulEnable & ID_MENUENABLED);
  else
    bMenuEnabled=TRUE;
  
  bMenuFolderRestored=TRUE;

  /* Call parent */
  if((mySomId=somIdFromString("wpRestoreState"))!=NULLHANDLE) {
    BOOL rc;

    rc=((somTD_WPObject_wpRestoreState)
        somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                            1,
                            __ClassObject->    
                            somGetMethodToken(mySomId))                    
        )(this, ulReserved);
    SOMFree(mySomId);
    return rc;
  }
  else 
    return WPFolder::wpRestoreState(ulReserved);

}

BOOL CWMenuFolder::wpSaveState()
{
  somId mySomId;
  
  if(bMenuEnabled)
    ulEnable|=ID_MENUENABLED;
  else
    ulEnable &= ~ID_MENUENABLED;
  
  wpSaveLong(MENUFOLDER_CLASSNAME, IDKEY_ENABLED, ulEnable);

  /* Call parent */
  if((mySomId=somIdFromString("wpSaveState"))!=NULLHANDLE) {
    BOOL rc;
    
    rc=((somTD_WPObject_wpSaveState)
        somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                            1,
                            __ClassObject->    
                            somGetMethodToken(mySomId))                    
        )(this);
    SOMFree(mySomId);
    return rc;
  }
  else 
    return WPFolder::wpSaveState();
}

MRESULT CWMenuFolder::wpDragOver(HWND hwndCnr,PDRAGINFO pDragInfo)
{
	ULONG ulNumberOfObjects;
	ULONG   ulCount;
	MRESULT mResult;
	PDRAGITEM pDragItem;
	SOMClass *folderClass;
	WPObject * wpObject;

    /* Do not create shadows if the source is the current container */
    if(hwndCnr==pDragInfo->hwndSource)
      return MRFROM2SHORT( DOR_DROP, DO_MOVE);

	folderClass=this->somGetClass();

	/********************************************************************/
	/* Don't call the parent. Initialize mResult to allow the drag over */
	/* to proceed.                                                      */
	/********************************************************************/
	mResult = MRFROM2SHORT( DOR_DROP, DO_LINK);
 
	/*******************************************************************/
	/* Determine the number of objects dragged over                    */
	/*******************************************************************/
	ulNumberOfObjects = DrgQueryDragitemCount( pDragInfo);
  if(ulNumberOfObjects>1)return MRFROM2SHORT( DOR_NEVERDROP, SHORT2FROMMR( mResult));
	/*************************/
	/* Check all the objects */
	/*************************/
	for( ulCount=0; ulCount < ulNumberOfObjects &&
				 SHORT1FROMMR( mResult) != DOR_NEVERDROP; ulCount++){
		
		pDragItem=DrgQueryDragitemPtr( pDragInfo, ulCount);
		
		/*****************************************/
		/* It must be an object. */
		/*****************************************/
		if( DrgVerifyRMF( pDragItem,"DRM_OS2FILE", NULL)){
			mResult = MRFROM2SHORT( DOR_NEVERDROP,DO_LINK);
			wpObject=(WPObject*)OBJECT_FROM_PREC(DrgQueryDragitemPtr( pDragInfo, ulCount)->ulItemID);
			if(!wpObject->somIsA(folderClass)) {
				/* The dragged object isn't a menufolder: */
				if(wpObject->wpQueryFolder()==this){
					mResult = MRFROM2SHORT( DOR_DROP, DO_MOVE);
					return mResult;
				}
				if((pDragInfo->usOperation==DO_DEFAULT)||(pDragInfo->usOperation==DO_LINK))
					mResult = MRFROM2SHORT( DOR_DROP, DO_LINK);// only allow link
			}
			else {/* the dragged object is a menufolder*/
				if(!wpObject->wpQueryObjectID()) {
					mResult = MRFROM2SHORT( DOR_DROP, DO_MOVE);
					if(pDragInfo->usOperation==DO_DEFAULT)
						mResult = MRFROM2SHORT( DOR_DROP, DO_MOVE);
				}
			}
		}
		else {// the dragged object isn't a file object
			if(pDragInfo->usOperation==DO_DEFAULT)
				mResult = MRFROM2SHORT( DOR_DROP, SHORT2FROMMR( mResult));
			else
				if(pDragInfo->usOperation==DO_MOVE)
					mResult=MRFROM2SHORT( DOR_DROP,pDragInfo->usOperation );
		}
	}	
	return( mResult);
}

BOOL CWMenuFolder::wpAddSettingsPages(HWND hwndNotebook)
{
	ULONG rc;
	somId mySomId;

    /* Call parent */
    if((mySomId=somIdFromString("wpAddSettingsPages"))!=NULLHANDLE) {
      rc=((somTD_WPObject_wpAddSettingsPages)
          somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                              1,
                              __ClassObject->    
                              somGetMethodToken(mySomId))                    
          )(this, hwndNotebook);
      SOMFree(mySomId);
    }
    else 
      rc= WPFolder::wpAddSettingsPages(hwndNotebook);
    
	if(!strcmp(somGetClassName(), wpQueryFolder()->somGetClassName()) 
       || (ulEnable & ID_HIDECLASSPAGE))// strcmp()==0 if equal
      return rc; // Add ClassChoosePage only if parent is no CWMenuFolder.

	/*We must be the menuroot folder */
	return(rc && mfAddClassChoosePage(hwndNotebook));
}
 

MRESULT CWMenuFolder::wpDrop(HWND hwndCnr,PDRAGINFO pDragInfo,PDRAGITEM pDragItem)
{
  somId mySomId;

  mfFreeMenu();	
  
  /* Call parent */
  if((mySomId=somIdFromString("wpDrop"))!=NULLHANDLE) {
    MRESULT rc;
    
    rc=((somTD_WPObject_wpDrop)
        somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                            1,
                            __ClassObject->    
                            somGetMethodToken(mySomId))                    
        )(this, hwndCnr, pDragInfo, pDragItem);
    SOMFree(mySomId);
    return rc;
  }
  else 
    return WPFolder::wpDrop(hwndCnr, pDragInfo, pDragItem);

}

BOOL CWMenuFolder::wpDeleteFromContent(WPObject *object)
{
  somId mySomId;

  mfFreeMenu();	

  /* Call parent */
  if((mySomId=somIdFromString("wpDeleteFromContent"))!=NULLHANDLE) {
    BOOL rc;
    
    rc=((somTD_WPFolder_wpDeleteFromContent)
        somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                            1,
                            __ClassObject->    
                            somGetMethodToken(mySomId))                    
        )(this, object);
    SOMFree(mySomId);
    return rc;
  }
  else 
    return WPFolder::wpDeleteFromContent(object);
}

void CWMenuFolder::wpInitData()
{
  ULONG dataSize;
  char profileName[CCHMAXPATH]={0};
  HINI hini;
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
    WPFolder::wpInitData();
  // Durch obige Konstruktion wird die direkte Elternmethode aufgerufen,
  // und nicht die ursprÅngliche von WPLaunchPad, falls WPLaunchPad replaced wurde.
  // Durch diesen Aufruf gehen eventuelle Erweiterungen durch Vererbung verloren:
  //            < return WPLaunchPad::wpMenuItemSelected(hwndFrame, ulMenuId); >


  if(!bInit) {
    // Read values from user.ini //
    dataSize=sizeof(ulOptions);
    queryIniFilePath(profileName, sizeof(profileName));
    //   sprintf(profileName,"%s\\wpswizz.ini",chrInstallPath);
    if((hini=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),profileName))!=NULL) {
      if(!PrfQueryProfileData(hini,"WPSWizard","Options",&ulOptions,&dataSize)) {
        g_fFileSizeEnabled=TRUE;
        bNoFormatMenue=TRUE;
        bGlobalMenuEnabled=TRUE;
        ulOptions=FILESIZEENABLED|NOFORMATMENUE|MENUENABLED;
        PrfWriteProfileData(hini,"WPSWizard","Options",
                            &ulOptions,sizeof(ULONG));
      }
      else {
        if(ulOptions&FILESIZEENABLED)
          g_fFileSizeEnabled=1;//BM_SETCHECK only allows 0, 1, 2
        else
          g_fFileSizeEnabled=0;
        if(ulOptions&NOFORMATMENUE)
          bNoFormatMenue=1;
        else
          bNoFormatMenue=0;
        if(ulOptions&MENUENABLED)
          bGlobalMenuEnabled=1;
        else
          bGlobalMenuEnabled=0;
      }
      PrfCloseProfile(hini);
    }
    bInit=TRUE;
  } 
}


BOOL CWMenuFolder::mfCheckMenuItems(WPObject* wpObject,ULONG ulMenuId)
{
  BOOL bReturn=FALSE;

  TRY_LOUD(CHECK_MENU) {
    bReturn=mfPrivateCheckMenuItems( wpObject, ulMenuId, NULLHANDLE, FALSE);
  }
  CATCH(CHECK_MENU)
    {
    } END_CATCH;

   return bReturn;
}

BOOL CWMenuFolder::mfCheckSkriptMenuItems(WPObject* wpObject, ULONG ulMenuId, HWND hwndFrame)
{
  BOOL bReturn=FALSE;
  
  TRY_LOUD(CHECK_MENU) {
    bReturn=mfPrivateCheckMenuItems( wpObject, ulMenuId, hwndFrame, TRUE);
  }
  CATCH(CHECK_MENU)
    {
    } END_CATCH;

   return bReturn;
}


WPObject* mfResolveShadows(WPObject *wpObject)
{
  if(!somIsObj(wpObject))
    return NULL;//No object given
  
  /* Check if it's a shadow */
  if(somResolveByName(wpObject,"wpQueryShadowedObject")){ 
    /* Yes, it's a shadow. Query the linked object. */
    wpObject=((WPShadow*)wpObject)->wpQueryShadowedObject(FALSE);
  }
  
  if(!somIsObj(wpObject))
    return NULL;//The link is broken

  return wpObject;
}

WPFileSystem* mfResolveShadowsToFileSystem(WPObject *wpObject)
{
  if(!somIsObj(wpObject))
    return NULL;//No object given
  
  /* Check if it's a shadow */
  if(somResolveByName(wpObject,"wpQueryShadowedObject")){ 
    /* Yes, it's a shadow. Query the linked object. */
    wpObject=((WPShadow*)wpObject)->wpQueryShadowedObject(FALSE);
  }
  
  if(!somIsObj(wpObject))
    return NULL;//The link is broken

  /* Check if it's a file system object */
  if(somResolveByName(wpObject, "wpQueryRealName")){
    return (WPFileSystem*)wpObject;/* Yes */
  }
  else
    return NULL;
}

BOOL CWMenuFolder::mfOpenObjectWithParam( WPProgram* wp, WPObject *wpObject )
{
  /* wpObject: the folder or file object on which the menu was invoced */
  /* wp: The program object which represents the menu item */

  PPROGDETAILS pProgDetails;
  ULONG        ulSize;
  PSZ orgParam;
  PSZ orgDirectory;
  PSZ orgTitle;
  char *ptrLastBS;
  char szFileName[CCHMAXPATH+1];
  char szParameters[CCHMAXPATH*2];
  char szTitle[CCHMAXPATH];
  WPFileSystem* wpFS;

  wp=(WPProgram*)mfResolveShadows(wp);
  wpFS=mfResolveShadowsToFileSystem(wp);

  if(!somIsObj(wp))
    return TRUE;/* This means:  do not try to handle the menu any more */

  if(!wpFS){
    // Get information about a program object //
    if ((wp->wpQueryProgDetails ( (PPROGDETAILS)NULL, &ulSize))){
      if ((pProgDetails = (PPROGDETAILS) wp->wpAllocMem(ulSize, NULL)) != NULL){
        /* We have memory */
        if ((wp->wpQueryProgDetails ( pProgDetails, &ulSize))){
          /* Save the information */
          orgParam=pProgDetails->pszParameters;
          orgDirectory=pProgDetails->pszStartupDir;
          orgTitle=pProgDetails->pszTitle;
          
          /* Get full path of object */
          ulSize=sizeof(szFileName);
          ((WPFileSystem*)wpObject)->wpQueryRealName(szFileName,&ulSize,TRUE);
          
          /* Put the path into parameter field */
          if(pProgDetails->pszParameters)
            pProgDetails->pszParameters=substitutePlaceHolder(orgParam, szFileName, szParameters, sizeof(szParameters));
          /* Build working directory */
          /* Check if it's a folder */
          if(somResolveByName(wpObject, "wpPopulate" )){
            if(!pProgDetails->pszStartupDir)
              pProgDetails->pszStartupDir=szFileName;
            /* Yes */
          }
          else {
            /* Remove the file name */
            if((ptrLastBS=strrchr(szFileName, '\\'))!=NULL) {
              *ptrLastBS=0;
              if(!pProgDetails->pszStartupDir)
                pProgDetails->pszStartupDir=szFileName;
            }
          }
          if(pProgDetails->pszTitle)
            pProgDetails->pszTitle=deleteTilde(orgTitle, szTitle, sizeof(szTitle));
          
          wp->wpSetProgDetails (pProgDetails);
          
          /* Start the object */
          wp->wpViewObject(NULL,OPEN_DEFAULT,(ULONG)0);
          
          pProgDetails->pszParameters=orgParam;
          pProgDetails->pszStartupDir=orgDirectory;
          pProgDetails->pszTitle=orgTitle;
          
          wp->wpSetProgDetails (pProgDetails);
          wp->wpFreeMem((PBYTE)pProgDetails);
          return TRUE;
        }
        else { //wpQueryProgDetails - unable to get details
          wp->wpFreeMem((PBYTE)pProgDetails);
          return TRUE;
        }
      }
      else{//wpAllocMem failed
        return TRUE;
      }
    }
    else{//pQueryProgDetails - unable to determine size for buffer
      wp->wpViewObject(NULL,OPEN_DEFAULT,0);
      return TRUE;
    }
  }
  else/* No program object */
    wp->wpViewObject(NULL,OPEN_DEFAULT,0);
  return TRUE;
}



BOOL CWMenuFolder::mfExecuteRexxFile( WPProgram* wp, WPObject *wpObject, HWND hwndFrame )
{
  /* wpObject: the folder or file object on which the menu was invoced */
  /* wp: The program object which represents the menu item */
  PPROGDETAILS pProgDetails;
  ULONG        ulSize;
  WPFileSystem* wpFS;

  wp=(WPProgram*)mfResolveShadows(wp);
  wpFS=mfResolveShadowsToFileSystem(wp);


  if(!somIsObj(wp))
    return TRUE;/* This means:  do not try to handle the menu any more */

  // Get information about a program object //
  if(!wpFS) {
    if ((wp->wpQueryProgDetails ( (PPROGDETAILS)NULL, &ulSize))){
      if ((pProgDetails = (PPROGDETAILS) wp->wpAllocMem(ulSize, NULL)) != NULL){
        /* We have memory */
        if ((wp->wpQueryProgDetails ( pProgDetails, &ulSize))){
          PREXXTHREADPARMS rtP;
          char szFileName[CCHMAXPATH+1];
          
          rtP=(PREXXTHREADPARMS)wpObject->wpAllocMem(sizeof(REXXTHREADPARMS), NULL);
          if(rtP) {
            memset(rtP, 0, sizeof(REXXTHREADPARMS));
            sprintf(rtP->rexxSkript, pProgDetails->pszExecutable);
            rtP->hwndFrame=hwndFrame;
            rtP->thisPtr=wpObject;
            strncpy(rtP->chrCommand, "/EXECUTE", sizeof(rtP->chrCommand));
            rtP->chrCommand[sizeof(rtP->chrCommand)-1]=0;
            if(_beginthread(rexxThreadFunc,NULL,8192*16,(void*)rtP)==-1)
              wpObject->wpFreeMem((PBYTE)rtP); //Fehlerbehandlung fehlt
          }
          wp->wpFreeMem((PBYTE)pProgDetails);
          return TRUE;
        }
        else { //wpQueryProgDetails - unable to get details
          wp->wpFreeMem((PBYTE)pProgDetails);
          return TRUE;
        }
      }
      else{//wpAllocMem failed
        return TRUE;
      }
    }
    else{//pQueryProgDetails - unable to determine size for buffer
      wp->wpViewObject(NULL,OPEN_DEFAULT,0);
      return TRUE;
    }
  }
  else
    {
      if(somIsObj(wpFS)) {
        /* Object is not a WPProgram */
        PREXXTHREADPARMS rtP;
        
        rtP=(PREXXTHREADPARMS)wpObject->wpAllocMem(sizeof(REXXTHREADPARMS), NULL);
        if(rtP) {
          ULONG ulSize;

          memset(rtP, 0, sizeof(REXXTHREADPARMS));

          ulSize=sizeof(rtP->rexxSkript);
          wpFS->wpQueryRealName(rtP->rexxSkript, &ulSize, TRUE);
          
          /*ulSize=sizeof(fileName);
            if(wpFS->wpQueryRealName(fileName, &ulSize, TRUE))
            sprintf(rtP->rexxSkript, fileName);
            */
          rtP->hwndFrame=hwndFrame;
          rtP->thisPtr=wpObject;

          strncpy(rtP->chrCommand, "/EXECUTE", sizeof(rtP->chrCommand));
          rtP->chrCommand[sizeof(rtP->chrCommand)-1]=0;
                                        
          if(_beginthread(rexxThreadFunc,NULL,8192*16,(void*)rtP)==-1)
            wpObject->wpFreeMem((PBYTE)rtP); //Fehlerbehandlung fehlt
        }
      }
    }
  return TRUE;
}


BOOL CWMenuFolder::mfPrivateCheckMenuItems(WPObject* wpObject,ULONG ulMenuId, HWND hwndFrame, BOOL bRexx)
{

  /* wpObject: the folder or file object on which the menu was invoced */
  CONTEXTITEM * tempCI;
  
  if(!bMenuEnabled||!bGlobalMenuEnabled)
    return FALSE;
  
  tempCI=ptrCItem;

  while(tempCI) {
    if(tempCI->menuItem.afStyle&MIS_SUBMENU) {
      if(bRexx) {
        if(((CWMenuFolder*)tempCI->wpObject)->mfCheckSkriptMenuItems(wpObject,ulMenuId, hwndFrame))
          return TRUE; /* Menu item handled */
      }
      else
        if(((CWMenuFolder*)tempCI->wpObject)->mfCheckMenuItems(wpObject,ulMenuId))
          return TRUE; /* Menu item handled */
    }
        
    if((ulMenuId==tempCI->menuItem.id)&&(!(tempCI->menuItem.afStyle&MIS_SUBMENU))){
      /* tempCI->wpObject: The object which represents the menu item */
      if(bRexx)
        return  mfExecuteRexxFile((WPProgram*)tempCI->wpObject, wpObject, hwndFrame);
      else
        return mfOpenObjectWithParam((WPProgram*)tempCI->wpObject, wpObject);

    }// end of if(ulMenuId==tempCI->menuItem.id)
    tempCI=tempCI->nextItem;	
  }// end of while(tempCI) //

  return FALSE;
}

void CWMenuFolder::mfInsertMItems( HWND hwndMenu,ULONG iPosition)
{
	CONTEXTITEM * tempCI;
	MENUITEM mi;

	/* Insert the submenu with the data of cItem */
	tempCI=ptrCItem;
	while(tempCI) {
      if(tempCI->menuItem.afStyle&MIS_SUBMENU)// Is it a Submenu-item?
        { //Yes
          // Create Empty Submenu //
          tempCI->menuItem.hwndSubMenu=WinCreateWindow(hwndMenu,WC_MENU,"",0,
                                                       0,0,0,0,
                                                       hwndMenu,//Owner. If set, this window 
                                                       //is destroyed if owner is destroyed
                                                       HWND_TOP,
                                                       tempCI->menuItem.id,//menuid
                                                       NULL,NULL);
        }
      WinSendMsg(hwndMenu,MM_INSERTITEM,(MPARAM)&tempCI->menuItem,
                 (MPARAM)&tempCI->objectName);		
      if(tempCI->subMenu)//this is the List of submenu-Items
        { //Insert Item into submenu
          if(tempCI->wpObject)
            ((CWMenuFolder*)tempCI->wpObject)->mfInsertMItems(tempCI->menuItem.hwndSubMenu,iPosition);
        }	
      tempCI=tempCI->nextItem;
	}
}

ULONG CWMenuFolder::mfInsertMenuItems(HWND hwndMenu,ULONG iPosition, ULONG ulLastMenuId)
{
  /* ulLastMenuId contains the last used free menu id */
  ULONG ulID;

  /* Check, if we should have a custom menu */
  if(!bMenuEnabled||!bGlobalMenuEnabled)
    return ulLastMenuId;

  ulID=ulLastMenuId;
  TRY_LOUD(INSERT_MENU) { 
    /* Build our custom items */
    mfPrivateBuildMenu(this, &ulID);
    /* Put the menu items into the popup menu */
    mfInsertMItems(hwndMenu,iPosition);
  }
  CATCH(INSERT_MENU)
    {
    } END_CATCH;


  return ulID;

}


CONTEXTITEM * CWMenuFolder::mfPrivateBuildMenu(CWMenuFolder * wpFolder, ULONG * ulLastMenuId)
{
	CONTEXTITEM *	ptrContItem;//SpÑter in Head oder Ñhnliches umbenennen
	CONTEXTITEM * tempCI;
	WPObject * contentObject;
	SOMClass *folderClass;
	WPObject *shadow;
	char *pBuchst;
    ULONG ulId;

    // showMessage(HWND_DESKTOP,this->wpQueryTitle(),"mfPrivateBuildMenu(), start");

    /* We already have built the data for the menu */
  if(ptrCItem) {
    *ulLastMenuId=ulLastMenuIDPrivate;
    return ptrCItem;
  }
  //showMessage(HWND_DESKTOP,"In mfPrivateBuildMenu()","Debug");

    ulId=*ulLastMenuId;
 
	ptrContItem=NULL;
	if(somIsObj(wpFolder)){
		folderClass=wpFolder->somGetClass();
		/* Awake the folder contents */
		wpFolder->wpPopulate(NULLHANDLE,NULL,FALSE);
		/* Query first item */
		contentObject=wpFolder->wpQueryContent(NULL,QC_FIRST);	
		if(somIsObj(contentObject)) {
			/* Allocate the Head of the Menuitem list */				
			ptrContItem=new(CONTEXTITEM);//Head
			tempCI=ptrContItem;

			while(somIsObj(contentObject)) {
              ulId++;
				/* Get itemtitle as menutext */
				strcpy(tempCI->objectName,contentObject->wpQueryTitle());
				/* Delete 'Returns' */
				//	pBuchst=strchr(tempCI->objectName,10);
				pBuchst=strchr(tempCI->objectName,10);
				if(pBuchst)*pBuchst=' ';
				//		pBuchst=strchr(tempCI->objectName,13);
				pBuchst=strchr(tempCI->objectName,13);
				if(pBuchst)*pBuchst=' ';

				tempCI->menuItem.iPosition=MIT_END;
				tempCI->menuItem.afStyle=MIS_TEXT;
				tempCI->menuItem.id=ulId;
				tempCI->menuItem.afAttribute=NULL;				
				tempCI->menuItem.hwndSubMenu=0;
				tempCI->menuItem.hItem=NULL;
				tempCI->nextItem=NULL;
				tempCI->wpObject=contentObject;
				//tempCI->ulSubMenuID=menuID;//Root-Menu has menuID=0
				tempCI->ulSubMenuID=0;
				tempCI->subMenu=NULL;
				/* Menu Folders are treated as Submenus */
				if(contentObject->somIsA(folderClass)) {
					tempCI->menuItem.afStyle|=MIS_SUBMENU;
					//subMenu enthÑlt die UntermenÅ-EintrÑge
					tempCI->subMenu=((CWMenuFolder*)contentObject)->mfPrivateBuildMenu((CWMenuFolder*)contentObject,
                                                                              &ulId);
                    //                                         tempCI->menuItem.id);
					((CWMenuFolder*)tempCI->wpObject)->mfSetMenuEnabled(TRUE);
					
				}
				contentObject->wpUnlockObject();//Object is Locked through wpPopulate()
				contentObject=wpFolder->wpQueryContent(contentObject,QC_NEXT);
				if(contentObject) {
                  tempCI->nextItem=new(CONTEXTITEM);
                  tempCI=tempCI->nextItem;
				}
			}
		}		
	}
    *ulLastMenuId=ulId;
    ulLastMenuIDPrivate=ulId;
    ptrCItem=ptrContItem;
	return ptrContItem;	
}

CWMenuFolder* mfGetTopMostParent(CWMenuFolder* pMenuFolder)
{
  SOMClass* folderClass;
  WPObject * tempFolder;

  folderClass=pMenuFolder->somGetClass();

  tempFolder=pMenuFolder;

  pMenuFolder=(CWMenuFolder*)pMenuFolder->wpQueryFolder();

  if(!somIsObj(pMenuFolder))
    return NULL;/* Error! */
  
  while(pMenuFolder->somIsA(folderClass)){
    tempFolder=pMenuFolder;
    pMenuFolder=(CWMenuFolder*)pMenuFolder->wpQueryFolder();
    if(!somIsObj(pMenuFolder))
      return NULL;/* Error! */
  };

  return (CWMenuFolder*)tempFolder;
}

void CWMenuFolder::mfPrivateFreeMenu(void)
{
  CONTEXTITEM * tempCI;
  SOMClass* folderClass;
  char text[100];
  
  folderClass=this->somGetClass();

  while(ptrCItem) {
    tempCI=ptrCItem;
    ptrCItem=ptrCItem->nextItem;
    if(somIsObj(tempCI->wpObject))
      if(tempCI->wpObject->somIsA(folderClass))
        ((CWMenuFolder*)tempCI->wpObject)->mfPrivateFreeMenu();
    //if(somIsObj(tempCI->wpObject))
    //showMessage(HWND_DESKTOP,tempCI->wpObject->wpQueryTitle(),"Deleting...");
    delete	tempCI;
  }
  ulLastMenuIDPrivate=0;
  //  sprintf(text, "Deleted contents. %x", ptrCItem);
  // showMessage(HWND_DESKTOP,this->wpQueryTitle(),text);

}

void CWMenuFolder::mfFreeMenu()
{
	CONTEXTITEM * tempCI;
	WPObject * parentFolder;
	SOMClass* folderClass;
    CWMenuFolder* topFolder;

    TRY_LOUD(FREE_MENU) {
      topFolder=mfGetTopMostParent(this);
      //if(somIsObj(topFolder))
      //  showMessage(HWND_DESKTOP,topFolder->wpQueryTitle(),"Topmost Foldername");
      
      if(somIsObj(topFolder))
        topFolder->mfPrivateFreeMenu();
    }
    CATCH(FREE_MENU)
      {
      } END_CATCH;

    return;
} 

static MRESULT EXPENTRY ClassChooseContainerProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
	RECTL rect;
	HPS hps;
	HPOINTER iconPtr;
	SWP swp;				
	MENUFOLDERINFO * pMenuInfo=NULL;

	pMenuInfo=(MENUFOLDERINFO*)WinQueryWindowPtr(WinQueryWindow(hwnd,QW_PARENT),QWL_USER);
	switch(msg) {
	case WM_PAINT:
		hps=WinBeginPaint(hwnd,NULLHANDLE,&rect);
		WinFillRect(hps,&rect,SYSCLR_DIALOGBACKGROUND);
		if(pMenuInfo) {
			WinQueryWindowPos(hwnd,&swp);		
			if(pMenuInfo->tempClass) {
              if(somIsObj((M_WPObject*)pMenuInfo->tempClass))
				WinDrawPointer(hps,(swp.cx-WinQuerySysValue(HWND_DESKTOP,SV_CXICON))/2,
                               (swp.cy-WinQuerySysValue(HWND_DESKTOP,SV_CYICON))/2,
                               ((M_WPObject*)pMenuInfo->tempClass)->wpclsQueryIcon(),
                               DP_NORMAL);				
              
			}
			else {
				if(pMenuInfo->somClass) {
                  if(somIsObj((M_WPObject*)pMenuInfo->somClass))
					WinDrawPointer(hps,(swp.cx-WinQuerySysValue(HWND_DESKTOP,SV_CXICON))/2,
                                   (swp.cy-WinQuerySysValue(HWND_DESKTOP,SV_CYICON))/2,
                                   ((M_WPObject*)pMenuInfo->somClass)->wpclsQueryIcon(),
                                   DP_NORMAL);				
				}
			}
		}
		WinEndPaint(hps);
		return (MRESULT) FALSE;
	default:
		break;
	}	
	if(pMenuInfo)pMenuInfo->oldContainerProc(hwnd, msg, mp1, mp2);
	return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

static MRESULT EXPENTRY ClassChooseDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
	char newID[CCHMAXPATH];
	char textString[CCHMAXPATH];
	PCNRDRAGINFO pCnrDragInfo;
	PDRAGINFO pDragInfo;
	DRAGITEM DragItem;
	static WPObject * wpObject;
	MENUFOLDERINFO * pMenuInfo=NULL;
	HPS hps;			
	HOBJECT hObject;
	char *pBuchst;
	RECTL rect;
	PMINIRECORDCORE miniRecord;
	somId mySomId;

	switch(msg) {
	case WM_INITDLG :		
		WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwnd, ID_ENABLECONTEXTMENU));
		pMenuInfo=new(MENUFOLDERINFO);
		if(!pMenuInfo)break;
		pMenuInfo->thisPtr=(CWMenuFolder*)PVOIDFROMMP(mp2);		
		pMenuInfo->tempClass=NULL;
		pMenuInfo->somClass=NULL;
		WinSetWindowPtr(hwnd,QWL_USER,(void*)pMenuInfo);//this-ptr sichern
		pMenuInfo->oldContainerProc=WinSubclassWindow(WinWindowFromID(hwnd, IDCN_CLASSDROP),
                                                      (PFNWP)ClassChooseContainerProc);
				
		/* If we have an ID (<MENU_*>), the user has already chosen a class for which
			 this menufolder provides a context menu. In this case change the title for the groupbox,
			 extract the name of the class from the ID, print it on the page, and find the classobject
			 of the class. During processing of WM_Paint of the pushbutton, we need this classobject
			 for finding the right icon. */
		if(pMenuInfo->thisPtr->wpQueryObjectID()) {
          WinLoadString(WinQueryAnchorBlock(hwnd), queryModuleHandle(),
								ID_GROUPBOXTEXT, sizeof(textString), textString);
          WinSetWindowText(WinWindowFromID(hwnd, IDGB_CLASSDROP),textString);
          /* get classname from ID */
          sprintf(newID,"%s",pMenuInfo->thisPtr->wpQueryObjectID());
          pBuchst=strchr(newID,'_');
          if(pBuchst)strcpy(textString,++pBuchst);
          pBuchst=strchr(textString,'>');
          if(pBuchst)*pBuchst=0;
          WinSetWindowText(WinWindowFromID(hwnd,ID_CHOOSENCLASSNAME),textString);
          mySomId=somIdFromString(textString);
          pMenuInfo->somClass=SOMClassMgrObject->somClassFromId(mySomId);
          SOMFree(mySomId);
		}
		WinSendMsg(WinWindowFromID(hwnd,ID_ENABLECONTEXTMENU),BM_SETCHECK,
                   MPFROMSHORT(((CWMenuFolder*)PVOIDFROMMP(mp2))->mfQueryMenuEnabled()),
                   (MPARAM)NULL);	
		pMenuInfo->tempMenuEnabled=((CWMenuFolder*)PVOIDFROMMP(mp2))->mfQueryMenuEnabled();
        /* Move default buttons on Warp 4 */
        cwMoveNotebookButtonsWarp4(hwnd,
                                   ID_CLASSCHOOSEHELP,    // in: ID threshold
                                   20);                   // in: dialog units or 0
        
		return (MRESULT) TRUE;
	case WM_DESTROY:
		pMenuInfo=(MENUFOLDERINFO*)WinQueryWindowPtr(hwnd,QWL_USER);
		if(pMenuInfo&&((M_WPObject*)pMenuInfo->tempClass)&&!((M_WPObject*)pMenuInfo->somClass)){
			/* Build Title for this MenuFolder */
			WinLoadString(WinQueryAnchorBlock(hwnd),queryModuleHandle(),
								ID_FOLDERTITLE,sizeof(newID),newID);
			/* Fill in 'Returns' */
			pBuchst=strchr(newID,0);
			if(pBuchst){
				*(pBuchst++)=13;
				*(pBuchst++)=10;
				*(pBuchst++)=0;
			}
			
			strcat(newID,((M_WPObject*)pMenuInfo->tempClass)->wpclsQueryTitle());
			pBuchst=strchr(newID,0);
			if(pBuchst){
				*(pBuchst++)=13;
				*(pBuchst++)=10;
				*(pBuchst++)='(';
				*(pBuchst++)=0;
			}
			strcat(newID,wpObject->somGetClassName());
			pBuchst=strchr(newID,0);
			if(pBuchst){
				*(pBuchst++)=')';
				*(pBuchst++)=0;
			}
			pMenuInfo->thisPtr->wpSetTitle(newID);

			/* Set ID */
			if(wpObject) {
				sprintf(newID,"OBJECTID=<MENU_%s>",wpObject->somGetClassName());
				pMenuInfo->thisPtr->wpSetup(newID);			
			}
		}
		if(pMenuInfo) {
          //			pMenuInfo->thisPtr->mfSetMenuEnabled(pMenuInfo->tempMenuEnabled);
          pMenuInfo->thisPtr->wpSaveDeferred();
          delete pMenuInfo;		
          pMenuInfo=NULL;
          wpObject=NULL;
		}
		break;
	case WM_COMMAND :
		switch (SHORT1FROMMP(mp1)) {
			/* Process commands here */
		case ID_CONTEXTCANCEL:
			pMenuInfo=(MENUFOLDERINFO*)WinQueryWindowPtr(hwnd,QWL_USER);
			if(pMenuInfo) {
              if(!(M_WPObject*)pMenuInfo->somClass){
				pMenuInfo->tempClass=NULL;
				wpObject=NULL;
				WinSetWindowText(WinWindowFromID(hwnd,ID_CHOOSENCLASSNAME),"");
                // Container invalidieren, damit er neu gezeichnet wird //
				WinQueryWindowRect(WinWindowFromID(hwnd, IDCN_CLASSDROP),&rect);
				WinInvalidateRect(WinWindowFromID(hwnd, IDCN_CLASSDROP),&rect,FALSE);
              }
                WinSendMsg(WinWindowFromID(hwnd,ID_ENABLECONTEXTMENU),
                           BM_SETCHECK,MPFROMSHORT(pMenuInfo->tempMenuEnabled),(MPARAM)NULL);
                pMenuInfo->thisPtr->mfSetMenuEnabled(pMenuInfo->tempMenuEnabled);
			}		
			break;
		}
		/* Don't call WinDefDlgProc here, or the dialog gets closed */
		return (MRESULT) TRUE;
		break;
	case WM_CONTROL:
		switch (SHORT2FROMMP(mp1)) {
		case CN_DRAGOVER:
			pMenuInfo=(MENUFOLDERINFO*)WinQueryWindowPtr(hwnd,QWL_USER);
			if(!pMenuInfo)break;
			if(!pMenuInfo->thisPtr)break;
			/* There is already an ID <MENU_*> for this Folder so we can't choose a class */
			/* It is not possible to change the class for a Menufolder */
			if((pMenuInfo->thisPtr->wpQueryObjectID()))break;
			pCnrDragInfo=(CNRDRAGINFO*)PVOIDFROMMP(mp2);
			pDragInfo=pCnrDragInfo->pDragInfo;
			/* Get class of the dropped object */
			wpObject=(WPObject*)OBJECT_FROM_PREC(DrgQueryDragitemPtr( pDragInfo,0)->ulItemID);
			if(!wpObject)break;
			/* The object has got the same class, we've chosen before */	
			//	if(pMenuInfo->tempClass==wpObject->somGetClass())break;

			pMenuInfo->tempClass=wpObject->somGetClass();
			if(!pMenuInfo->tempClass)break;
			sprintf(newID,"<MENU_%s>",wpObject->somGetClassName());
			if((hObject=WinQueryObject(newID))!=NULL)break;//is there another Menufolder?
			// ACHTUNG: Informationsmeldung vorsehen!!!!!!
				
			/* We only accept one item! */
			if(DrgQueryDragitemCount( pDragInfo)>1)return(MPFROM2SHORT(DOR_NEVERDROP,DO_MOVE));
			/*****************************************/
			/*     It must be an object.             */
			/*****************************************/
			DrgQueryDragitem(pCnrDragInfo->pDragInfo,sizeof(DRAGITEM),&DragItem,0);
			if(!DrgVerifyRMF( &DragItem,"DRM_OBJECT", NULL))break;
			
				return(MPFROM2SHORT(DOR_DROP,DO_MOVE));
			break;/* The default is neverdrop */

		case CN_DROP:
			pCnrDragInfo=(CNRDRAGINFO*)PVOIDFROMMP(mp2);
			pDragInfo=pCnrDragInfo->pDragInfo;
			/* Get the WPObject-ptr. */
			wpObject=(WPObject*)OBJECT_FROM_PREC(DrgQueryDragitemPtr( pDragInfo,0)->ulItemID);
			if(!wpObject)break;
			
			pMenuInfo=(MENUFOLDERINFO*)WinQueryWindowPtr(hwnd,QWL_USER);
			if(!pMenuInfo)break;
			if(!pMenuInfo->thisPtr)break;
			
			/* Get class of the dropped object */
			pMenuInfo->tempClass=wpObject->somGetClass();
			if(!pMenuInfo->tempClass)break;
			
			sprintf(newID,"<MENU_%s>",wpObject->somGetClassName());
			if((hObject=WinQueryObject(newID))!=NULL){ //is there another Menufolder?
				// ACHTUNG: Fehlermeldung vorsehen!!!!!!
				return (MRESULT) TRUE;//Yes
			}
			WinSetWindowText(WinWindowFromID(hwnd,ID_CHOOSENCLASSNAME),wpObject->somGetClassName());
			// Container invalidieren, damit er neu gezeichnet wird //
			WinQueryWindowRect(WinWindowFromID(hwnd, IDCN_CLASSDROP),&rect);
			WinInvalidateRect(WinWindowFromID(hwnd, IDCN_CLASSDROP),&rect,FALSE);
			break;
		case BN_CLICKED:
          pMenuInfo=(MENUFOLDERINFO*)WinQueryWindowPtr(hwnd,QWL_USER);
          if(!pMenuInfo) break;
          switch(SHORT1FROMMP(mp1)) {
          case ID_ENABLECONTEXTMENU:
            if(WinQueryButtonCheckstate(hwnd, ID_ENABLECONTEXTMENU) & 1)
              pMenuInfo->thisPtr->mfSetMenuEnabled(TRUE);
            else
              pMenuInfo->thisPtr->mfSetMenuEnabled(FALSE);
            break;
          default:
            break;      
          } // end switch(SHORT1FROMMP(mp1))
          break;
        default:
          break;
		}
		return (MRESULT) TRUE;
    default:
      break;
	}
	return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

ULONG CWMenuFolder::mfAddClassChoosePage(HWND hwndNotebook)
{
	PAGEINFO pageinfo;
	char pageName[50]="~Klassenauswahl";

	memset((PCH)&pageinfo, 0, sizeof(PAGEINFO));
	pageinfo.cb = sizeof(PAGEINFO);
	pageinfo.hwndPage = NULLHANDLE;
	pageinfo.usPageStyleFlags = BKA_MAJOR|BKA_STATUSTEXTON;
	pageinfo.usPageInsertFlags = BKA_FIRST;
	pageinfo.usSettingsFlags = SETTINGS_PAGE_NUMBERS;
	pageinfo.pszName = pageName;
	pageinfo.pfnwp = ClassChooseDlgProc;
	pageinfo.resid = queryModuleHandle();
	pageinfo.dlgid = ID_CLASSCHOOSEPAGE;
	pageinfo.pCreateParams = this;
	pageinfo.idDefaultHelpPanel = 2;
	pageinfo.pszHelpLibraryName = queryHelpPath();
	//Name fÅr Notebookpage laden
	WinLoadString(WinQueryAnchorBlock(hwndNotebook),queryModuleHandle(),
								ID_CLASSPAGENAME,sizeof(pageName),pageName);
	return wpInsertSettingsPage(hwndNotebook,&pageinfo);
}














