#ifndef _CONTEXTITEM_HH
#define _CONTEXTITEM_HH

#pragma SOMAsDefault (off)

class CONTEXTITEM;

class CONTEXTITEM
{
public:
	MENUITEM menuItem;
	ULONG ulSubMenuID;//if <>0 destinationsubmenu
	char objectName[CCHMAXPATH];//Name of Object
	HOBJECT  hObject;
	WPObject *wpObject;//WPS-Objecthandle
	CONTEXTITEM *previousItem;
	CONTEXTITEM *nextItem;
	CONTEXTITEM *subMenu;
};

#pragma SOMAsDefault (pop)


#endif
