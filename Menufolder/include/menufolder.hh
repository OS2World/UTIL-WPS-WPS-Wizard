#ifndef _MENUFOLDER_HH
#define _MENUFOLDER_HH


#pragma SOMAsDefault(on)
#include <som.hh>
#include <somcls.hh>
#pragma SOMNoMangling(on)
#include <wpfolder.hh>
#include <wppgm.hh>

#pragma SOMAsDefault (off)
class CONTEXTITEM;

class CWMenuFolder;
	
class MENUFOLDERINFO
{
public:
 	SOMClass * somClass;
 	SOMClass * tempClass;
 	CWMenuFolder *thisPtr;
	PFNWP oldContainerProc;
	BOOL tempMenuEnabled;
};

typedef struct wizButtonData {
  char* chrFlyOver;
 } WIZBUTTONDATA;

typedef struct _REXXTHREADPARMS
{
  WPObject *thisPtr;
  HWND hwndFrame;
  char rexxSkript[CCHMAXPATH];
  char chrCommand[40];
}REXXTHREADPARMS;
typedef REXXTHREADPARMS* PREXXTHREADPARMS;
 
#pragma SOMAsDefault(pop)


#define WC_WIZBUTTON "wizButton" /*"WIZ_Button"*/

class M_CWMenuFolder:public M_WPFolder
{
#pragma SOMClassName(*,"M_CWMenuFolder")
#pragma SOMClassVersion(*,1,2)
#pragma SOMCallstyle(oidl)
public:
	virtual PSZ  wpclsQueryTitle();
	virtual	ULONG wpclsQueryIconData(PICONINFO pIconInfo);
  virtual	ULONG wpclsQueryIconDataN(PICONINFO pIconInfo, ULONG ulIconIndex);
	virtual ULONG wpclsQueryDefaultHelp(PULONG HelpPanelId,PSZ HelpLibrary);	
  virtual void wpclsInitData();
  virtual void wpclsUnInitData();
#pragma SOMReleaseOrder()
};

class CWMenuFolder:public WPFolder
{
#pragma SOMClassName(*,"CWMenuFolder")
#pragma SOMMetaClass(CWMenuFolder,"M_CWMenuFolder")
#pragma SOMClassVersion(CWMenuFolder,1,2)
#pragma SOMCallstyle(oidl)
private:
  CONTEXTITEM * ptrCItem;
  BOOL bMenuEnabled;
  ULONG ulEnable;
  ULONG ulLastMenuIDPrivate;
  virtual void mfPrivateFreeMenu(void);
  virtual CONTEXTITEM * mfPrivateBuildMenu(CWMenuFolder * wpFolder, ULONG * ulLastMenuId);
  virtual void mfInsertMItems( HWND hwndMenu,ULONG iPosition);
  virtual BOOL mfPrivateCheckMenuItems(WPObject *wpObject,ULONG ulMenuId, HWND hwndFrame, BOOL bRexx);
public:
  virtual void wpInitData();
  virtual MRESULT wpDragOver(HWND hwndCnr,PDRAGINFO pDragInfo);
  virtual MRESULT wpDrop(HWND hwndCnr,PDRAGINFO pDragInfo,PDRAGITEM pDragItem);
  virtual BOOL wpAddSettingsPages(HWND hwndNotebook);
  virtual BOOL wpRestoreState(ULONG ulReserved);
  virtual BOOL wpSaveState();	
  virtual BOOL wpSetupOnce(PSZ pSetupString);
  virtual BOOL wpSetup(PSZ pSetupString);
  virtual void wpCopiedFromTemplate();
  virtual BOOL wpDeleteFromContent(WPObject *object);
  virtual void wpObjectReady(ULONG ulCode, WPObject* refObject);
  void mfFreeMenu();
  virtual BOOL mfQueryMenuEnabled()	{return bMenuEnabled;};
  void mfSetMenuEnabled(BOOL bEnabled){bMenuEnabled=bEnabled;};
  virtual ULONG mfInsertMenuItems(HWND hwndMenu,ULONG iPosition, ULONG ulLastMenuId);
  virtual BOOL mfCheckMenuItems(WPObject *wpObject,ULONG ulMenuId);
  virtual ULONG mfAddClassChoosePage(HWND hwndNotebook);
  virtual BOOL mfOpenObjectWithParam( WPProgram* wp, WPObject *wpObject );
  virtual BOOL mfCheckSkriptMenuItems(WPObject *wpObject,ULONG ulMenuId, HWND hwndFrame);
  virtual BOOL mfExecuteRexxFile( WPProgram* wp, WPObject *wpObject , HWND hwndFrame);
#pragma SOMReleaseOrder(\
"mfPrivateBuildMenu",\
"mfInsertMItems",\
"mfFreeMenu",\
"mfQueryMenuEnabled",\
"mfSetMenuEnabled",\
"mfInsertMenuItems",\
"mfCheckMenuItems",\
"mfAddClassChoosePage",\
"mfPrivateFreeMenu",\
"mfPrivateCheckMenuItems",\
"mfOpenObjectWithParam",\
"mfCheckSkriptMenuItems",\
"mfExecuteRexxFile")
};
//"mfCheckSkriptMenuItems",\
  //"mfExecuteRexxFile")
#endif


























