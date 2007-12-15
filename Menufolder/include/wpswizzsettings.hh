#ifndef _WPSWIZZSETTINGS_HH
#define _WPSWIZZSETTINGS_HH


#pragma SOMAsDefault(on)
#include <som.hh>
#include <somcls.hh>
#pragma SOMNoMangling(on)
#include <wpfolder.hh>
#include <wppgm.hh>

#pragma SOMAsDefault (off)
class CONTEXTITEM;

class CWMenuFolder;

#define KEY_ENABLED 1

#if 0	
class MENUFOLDERINFO
{
public:
 	SOMClass * somClass;
 	SOMClass * tempClass;
 	CWMenuFolder *thisPtr;
	PFNWP oldContainerProc;
	BOOL tempMenuEnabled;
};
#endif
class WPSWIZZDLL;

class WPSWIZZDLL
{
private:
  WPSWIZZDLL * wwdNextDLL;
public:
  int wwdAddDLL(char * chrDLLName){return 0;};
  int wwdRemoveDLL(char * chrDLLName){return 0;};
};

#pragma SOMAsDefault(pop)


class M_CWWizzSettings:public M_WPAbstract
{
#pragma SOMClassName(*,"M_CWWizzSettings")
#pragma SOMClassVersion(*,1,2)
#pragma SOMCallstyle(oidl)
public:
	virtual ULONG wpclsQueryStyle();
	virtual PSZ  wpclsQueryTitle();
  	virtual ULONG wpclsQueryIconData(PICONINFO pIconInfo);
  	virtual ULONG wpclsQueryDefaultHelp(PULONG HelpPanelId,PSZ HelpLibrary);
  virtual void wpclsInitData();
  virtual void wpclsUnInitData();
#pragma SOMReleaseOrder()
};

class CWWizzSettings:public WPAbstract
{
#pragma SOMClassName(*,"CWWizzSettings")
#pragma SOMMetaClass(*,"M_CWWizzSettings")
#pragma SOMClassVersion(*,1,2)
#pragma SOMCallstyle(oidl)
private:

public:
  virtual BOOL wpSetup(PSZ pSetupString);
  virtual BOOL wpAddSettingsPages(HWND hwndNotebook);
  virtual ULONG wpAddObjectWindowPage(HWND hwndNotebook);
  virtual void wpInitData();	
  virtual BOOL wpSaveState();
  virtual ULONG wsAddFileOptionPage(HWND hwndNotebook);
  //virtual ULONG wsCallEnhancerFuncs(WPObject* wpObject, ULONG ulWhichFuncs, ULONG *ulReturnFromFunc, 
  //                                ULONG *ul1, ULONG *ul2, ULONG *ul3, ULONG *ul4, ULONG *ul5, ULONG *ul6);
  //  virtual ULONG wsCallEnhancerFuncs(WPObject* wpObject, ULONG ulWhichFuncs, ULONG ul1, ULONG ul2, ULONG ul3, ULONG ul4, ULONG ul5);
#pragma SOMReleaseOrder(\
"wsAddFileOptionPage")

};

#endif


























