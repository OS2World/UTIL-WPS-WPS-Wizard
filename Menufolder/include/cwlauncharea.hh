#ifndef _CWLAUNCHAREA_HH
#define _CWLAUNCHAREA_HH


#pragma SOMAsDefault(on)
#include <som.hh>
#include <somcls.hh>
#pragma SOMNoMangling(on)
#include <menufolder.hh>

/* Flag marking, this launch area holds action buttons. This is important
   for allowing drag and drop of creating the right context menu.
   */
#define CWLA_HOLDS_ACTIONBUTTONS  0x00000001

class CWLaunchArea;
class CWLAObject;

#pragma SOMAsDefault (off)
/*
  Used to track open launch areas
 */
typedef struct _CWLAVIEW
{
  USEITEM useItem;
  VIEWITEM viewItem;
  WPObject *wpOwnerObject;
}CWLAVIEW;
	
#pragma SOMAsDefault(pop)

/* The launcharea is a child of a menu folder because we may use all the
   REXX handling and object starting stuff of the menu folder. 
   */
class M_CWLaunchArea:public M_CWMenuFolder
{
#pragma SOMClassName(*,"M_CWLaunchArea")
#pragma SOMClassVersion(*,1,2)
#pragma SOMCallstyle(oidl)
public:
	virtual PSZ  wpclsQueryTitle();
	virtual void  wpclsInitData();
#pragma SOMReleaseOrder()
};

class CWLaunchArea:public CWMenuFolder
{
#pragma SOMClassName(*,"CWLaunchArea")
#pragma SOMMetaClass(CWLaunchArea,"M_CWLaunchArea")
#pragma SOMClassVersion(CWLaunchArea,1,2)
#pragma SOMCallstyle(oidl)
private:
  ULONG ulNumObjects;
  ULONG fl; /* Flags, e.g. 1 if launcharea holds action buttons. */
  CWLAObject *cwlaObjectList;
  PSZ pszFlyOver;
public:
  virtual void wpInitData();
  virtual void wpUnInitData();
  virtual HWND wpOpen(HWND hwndCnr, ULONG ulView, ULONG ulParam);
  virtual BOOL wpSetup(PSZ pSetupString);
  virtual ULONG mfAddClassChoosePage(HWND hwndNotebook);
  virtual ULONG cwQueryNumObjects();
  virtual BOOL cwSetLaunchAreaPos(HWND hwndLA, HWND hwndInsertBehind, LONG x, LONG y, LONG cx, LONG cy, ULONG fl);
  virtual BOOL cwBuildObjectList();
  virtual BOOL cwAddObject( WPObject* wpObject, int iPos, HWND hwndLA);
  virtual BOOL cwRemoveObject( CWLAObject* cwlaObject, HWND hwndLA);
  virtual void cwRefreshLaunchArea(HWND hwndLA);
  virtual void cwFreeObjectList();
  virtual void cwShowLaunchArea(HWND hwndLA, BOOL bShow);
  virtual HWND cwCreateLaunchArea(HWND hwndParent, HWND hwndOwner, LONG x, LONG y, LONG cx, LONG cy, ULONG fl);
  virtual void cwSetFlyOverText(PSZ pText);
  virtual PSZ cwQueryFlyOverText();
  virtual void cwSetOwnerObject(HWND hwndLA, WPObject *wpObject);
  virtual WPObject *cwQueryOwnerObject(HWND hwndLA);
  virtual BOOL cwQueryLaunchAreaHoldsActionButtons();
#pragma SOMReleaseOrder(\
"cwQueryNumObjects",\
"cwSetLaunchAreaPos",\
"cwBuildObjectList",\
"cwAddObject",\
"cwRemoveObject",\
"cwFreeObjectList",\
"cwRefreshLaunchArea",\
"cwShowLaunchArea",\
"cwCreateLaunchArea",\
"cwSetFlyoverText",\
"cwQueryFlyoverText",\
"cwSetOwnerObject", \
"cwQueryOwnerObject", \
"cwQueryLaunchAreaHoldsActionButtons")
};



class CWLAObject : public SOMObject {

	#pragma SOMClassName(*, "CWLAObject")
	#pragma SOMNoMangling(*)
	#pragma SOMNonDTS(*)

     #pragma SOMClassVersion (CWLAObject, 1, 2)
     #pragma SOMCallstyle (oidl)
     #pragma SOMAsDefault(off)
     #pragma SOMAsDefault(pop)
  public :
     virtual CWLAObject* cwQueryNextObject();
     virtual CWLAObject* cwSetNextObject(CWLAObject* cwlaObject);
     virtual HPOINTER cwQueryObjectIcon();
     virtual BOOL cwSetWPObject(WPObject* wpObject_);
     virtual WPObject* cwQueryWPObject();
     virtual BOOL cwSetObjectHandle(HOBJECT hObject_);
     virtual HOBJECT cwQueryObjectHandle();
     virtual BOOL cwSetLaunchArea(CWLaunchArea* cwLA);
     virtual CWLaunchArea* cwQueryLaunchArea();
  private :
     CWLAObject *cwLAONext;
     WPObject *wpObject;
     HOBJECT hObject;
     char chrName[260];
     CWLaunchArea *cwlaParent;
     #pragma SOMReleaseOrder ( \
			      "cwQueryNextObject", \
			      "cwSetNextObject", \
			      "cwQueryObjectIcon", \
			      "cwSetWPObject", \
			      "cwQueryWPObject", \
			      "cwSetObjectHandle", \
			      "cwQueryObjectHandle", \
			      "cwSetLaunchArea", \
			      "cwQueryLaunchArea")
};


#endif /* _CWLAUNCHAREA_HH */


























