.******************************************
:h2 res=5100.REXX callbacks
:p.
REXX callbacks are run during processing of different object methods. Depending on the return code of
the callback the processing will continue with the parent method or the return code is used by the WPS. 

:p.
Currently callbacks for the following WPS methods are available.
:ul.
.*:li.:link reftype=hd res=5200.wpCopiedFromTemplate:elink.
:li.:link reftype=hd res=5210./DRAGOVER (wpDragOver):elink.
:li.:link reftype=hd res=5290./FILTERMENU (wpFilterMenu):elink.
:li.:link reftype=hd res=5220./FILTERPOPUP  (wpFilterPopupMenu):elink.
:li.:link reftype=hd res=5240./MENUITEMSELECTED (wpMenuItemSelected):elink.
:li.:link reftype=hd res=5300./MODIFYMENU (wpModifyMenu):elink.
:li.:link reftype=hd res=5230./MODIFYPOPUP wpModifyPopupMenu:elink.
:li.:link reftype=hd res=5250./OPENFOLDER (wpOpen):elink. (for folder objects)
.* Last used id 5300
:eul.
:p.
:p.
In additon the following callbacks are provided.
:ul.
:li.:link reftype=hd res=5270./FOLDEROPENED (folder opened):elink.
:li.:link reftype=hd res=5320./FOLDERINFO:elink.
:li.:link reftype=hd res=5330./INFOWINDOWEVENT:elink.
:li.:link reftype=hd res=5280./OBJECTHINT (object hint):elink.
:li.:link reftype=hd res=5310./OBJECTINFO (object info for folder information window):elink.
:li.:link reftype=hd res=5260./TOOLBAREVENT (Tool bar item accessed):elink.
:eul.

:p.
:p.
ARG(1) specifies which callback is currently called e.g..
:xmp.
ARG(1)="/DRAGOVER"
:exmp.
:p.
To use the REXX callback interface it must be enabled in the main WPS-Wizard settings notebook.

:h3 res=5110
width=100%
.System and user REXX callbacks
:p.
Whenever the supported
methods are called the system loads and executes a provided REXX script. After installation there're three default scripts
available.

:ul.
:li.:link reftype=hd res=7030.object.rx:elink.
:p.This script is used whenever methods of WPS object are called.
:li.:link reftype=hd res=7010.folder.rx:elink.
:p.This script is executed when a folder object is accessed. First this script will be run and depending on the return
code of the particular method the flow of execution will go to the object.rx script because every folder is of course also a WPS object.
:li.:link reftype=hd res=7020.datafile.rx:elink.
:p.This script will be executed for datafiles. Similar to folder scripts the object.rx script may also be called.
:eul.
:p.
These scripts are located in the :hp2.bin:ehp2. directory of the WPS-Wizard installation.
:p.
The information window of folders is filled by special skripts in the :hp2.bin\fldrinfo:ehp2. directory. These scripts
are executed the same way like normal skripts but only when new information has to be shown.
:p.
Instead of the default scripts it's possible to have :link reftype=hd res=5120.class specific callback scripts:elink.. Furthermore every object can use it's own
:link reftype=hd res=5130.specific script:elink.. Only one of these three scripts is actually invoked. The
search order is the following.
:ol.
:li.Try to run object specific script.
:li.If there's no object script run class specific script.
:li.If there's no class script run the default script.
:eol.



:h3 res=5120
width=100%
.Class specific callback
:p.
A class specific callback script is a script with a special name. It's the class name with the .rx extension.
For example:

:ul.
:li.:hp2.CWMediaFolder.rx:ehp2.
:p.This script is specific for media folder objects.
:li.:hp2.mmmp3.rx:ehp2.
:p.This script is specific for MMMP3 (MP3 files) objects.
:eul. 
 
:nt.
The WPS-Wizard installation must be on a file system supporting long filenames to use this feature.
:ent.

Put the class specific skript into the :hp2.bin:ehp2. directory of the WPS-wizard installation. If you want to have
class specific information in the information area of folders put a class specific skript into :hp2.bin\fldrinfo:ehp2.


:h3 res=5130
width=100%
.Object specific callbacks
:p.
Every object may use it's own callback script which may perform special actions. Object scripts may
be located anywhere. To use such a script just drop it on the :hp2.REXX script:ehp2. page of the
properties notebook.
:p.
You may also use a setup string to attach a script to an object.
:p.
:xmp.
rc=SysSetObjectData(object, "WIZSETREXXSCRIPT=x&colon.\path\rexxscript.rx")
:exmp.

:h3 res=5140.Callback reference
:p. 
The following sections describe the provided callbacks.
:ul.
.*:li.:link reftype=hd res=5200.wpCopiedFromTemplate:elink.
:li.:link reftype=hd res=5210./DRAGOVER (wpDragOver):elink.
:li.:link reftype=hd res=5290./FILTERMENU (wpFilterMenu):elink.
:li.:link reftype=hd res=5220./FILTERPOPUP (wpFilterPopupMenu):elink.
:li.:link reftype=hd res=5240./MENUITEMSELECTED (wpMenuItemSelected):elink.
:li.:link reftype=hd res=5300./MODIFYMENU (wpModifyMenu):elink.
:li.:link reftype=hd res=5230./MODIFYPOPUP (wpModifyPopupMenu):elink.
:li.:link reftype=hd res=5250./OPENFOLDER (wpOpen):elink. (for folder objects)
:eul.
:p.
:ul.
:li.:link reftype=hd res=5270./FOLDEROPENED (folder opened):elink.
:li.:link reftype=hd res=5320./FOLDERINFO:elink.
:li.:link reftype=hd res=5330./INFOWINDOWEVENT:elink.
:li.:link reftype=hd res=5280./OBJECTHINT (object hint):elink.
:li.:link reftype=hd res=5310./OBJECTINFO (object info for folder information window):elink.
:li.:link reftype=hd res=5260./TOOLBAREVENT (Tool bar item accessed):elink.
:eul.
.* Last used id 5300



.*:h4 res=5200
.*width=30%
.*.wpCopiedFromTemplate
.*:link reftype=hd res=5201
.*auto dependent group=2.
.*:p.
.*This method is called directly called after an object is created from a template. Use it to customize
.*the newly created object.

.*:h4 res=5201
.*width=70%
.*x=30%
.*hide
.*group=2
.*.Example
.*:p.
.*:xmp.

.*:exmp.


.******************** wpDragOver

:h4 res=5210
width=30%
./DRAGOVER (wpDragOver)
:link reftype=hd res=5211
auto dependent group=2.
:p.
This callback is called whenever objects are dragged over the object this script belongs to.
:p.
:xmp.
ARG(1): "/DRAGOVER"
:exmp.
:p.
This callback is only invoked for data file and folder objects. It can only be used in 
:link reftype=hd res=5130.object specific scripts:elink..

:h4 res=5211
width=70%
x=30%
hide
group=2
.Example
:p.
:xmp.

/* Rexx callback - In this example only single objects may be dropped
   on the folder. These object must be file system objects.
    */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/* The script is called with 4 parameters.

ARG(1): The method indentifier. In this case /DRAGOVER
ARG(2): The object pointer
ARG(3): Reserved
ARG(4): A composite argument consisting of the folder frame
        handle and a pointer to the draginfo (see WPS Programming
        Reference)
*/

NUMERIC DIGITS 15 /* We need this for caculating with hex numbers.
                     Default is 9. */

SELECT
    .
    .
    .
WHEN ARG(1)="/DRAGOVER" THEN DO

    PARSE VALUE ARG(4) WITH hwndFrame pdraginfo

    /* Check number of objects */
    IF WPSWizCallWinFunc("drgQueryNumObjects", pdraginfo)\=1 THEN
        EXIT(X2D(200003)) /* More than one object. Deny dropping. */

    /* Check if the object is a file system object */
    IF WPSWizCallWinFunc("drgCheckClass", pdraginfo, "WPFileSystem")\=1 THEN
        EXIT(X2D(200003)) /* Not a file system object. Deny dropping. */

    EXIT(0) /* Exit 0 to call the parent */
    END
OTHERWISE
    /* Always return ARG(3) for unknown methods */
    return ARG(3)
END
:exmp.


.******************** wpFilterMenu

:h4 res=5290
width=30%
./FILTERMENU (wpFilterMenu)
:link reftype=hd res=5291
auto dependent group=2.
:p.
This method is invoked whenever a menu is requested for an object. Use this method
to remove some of the predefined menu items. This method is more generic than the 
:link reftype=hd res=5220.wpFilterPopupMenu:elink. callback because of the fine grained
control.

:p.
:xmp.
ARG(1): "/FILTERMENU"
:exmp.

:h4 res=5291
width=70%
x=30%
hide
group=2
.Example
:p.
:xmp.

/* Rexx callback - wpFilterMenu
    */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/* The script is called with 4 parameters.

ARG(1): The method indentifier. In this case /FILTERMENU
ARG(2): The object pointer

ARG(4): A composite argument consisting of
        -Reserved
        -hwndCnr
        -fMultiSelect
        -ulMenuType:      which menu
        -ulView:          the current view of the object
        (see WPS Programming Reference)
*/

NUMERIC DIGITS 15 /* We need this for caculating with hex numbers.
                     Default is 9. */

SELECT
    .
    .
    .
WHEN ARG(1)="/FILTERMENU" THEN DO
    /* */
    EXIT (ARG(3))
END
OTHERWISE
    /* Always return ARG(3) for unknown methods */
    return ARG(3)
END
:exmp.


.******************** wpFilterPopupMenu

:h4 res=5220
width=30%
./FILTERPOPUP (wpFilterPopupMenu)
:link reftype=hd res=5221
auto dependent group=2.
:p.
This method is invoked whenever a popup menu is requested for an object. Use this method
to remove some of the predefined menu items. If a flag (bit) in the return value is set the corresponding
menu is available.
:p.
:xmp.
ARG(1): "/FILTERPOPUP"
:exmp.

:p.
:xmp.
CTXT_ARRANGE          : 0x020000
        Open arrange 
CTXT_CLOSE            : 0x0008
        Close 
CTXT_COPY             : 0x0100
        Copy 
CTXT_DELETE           : 0x0080
        Delete
CTXT_DETAILS          : 0x004000
        Open details view 
CTXT_FIND             : 0x008000
        Open find 
CTXT_HELP             : 0x0040
        Help 
CTXT_ICON             : 0x001000
        Open icon view 
CTXT_LINK             : 0x0400
        Create shadow 
CTXT_LOCKUP           : 0x100000
        Open lockup 
CTXT_MOVE             : 0x0200
        Move 
CTXT_NEW              : 0x0001
        Create another 
CTXT_OPEN             : 0x0002
        Open 
CTXT_PALETTE          : 0x200000
        Open palette 
CTXT_PRINT            : 0x0020
        Print 
CTXT_PROGRAM          : 0x0800
        Open program 
CTXT_REFRESH          : 0x400000
        Refresh 
CTXT_SELECT           : 0x010000
        Open select 
CTXT_SHUTDOWN         : 0x080000
        Open shutdown 
CTXT_SORT             : 0x040000
        Open sort 
CTXT_SETTINGS         : 0x0010
        Open settings 
CTXT_SWITCHTO         : 0x0004
        Switch to 
CTXT_TREE             : 0x002000
        Open tree view 
CTXT_WINDOW           : 0x0004
        Window 
:exmp.


:h4 res=5221
width=70%
x=30%
hide
group=2
.Example
:p.
:xmp.

/* Rexx callback - wpFilterPopupMenu
    */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/* The script is called with 4 parameters.

ARG(1): The method indentifier. In this case /FILTERPOPUP
ARG(2): The object pointer
ARG(3): Return value of the parent. Removing menu items means
        resetting bits of this ULONG.
ARG(4): A composite argument consisting of
        -ulFlags:  the menu items set by the parent methods
        -hwndCnr
        -fMultiSelect
        (see WPS Programming Reference)
*/

NUMERIC DIGITS 15 /* We need this for caculating with hex numbers.
                     Default is 9. */

SELECT
    .
    .
    .
WHEN ARG(1)="/FILTERPOPUP" THEN DO
    /* Intercept this to remove some items from the popup menu. Removing
       items means resetting bits of the ARG(3) value */
    EXIT (ARG(3))
END
OTHERWISE
    /* Always return ARG(3) for unknown methods */
    return ARG(3)
END
:exmp.


.******************** wpMenuItemSelected

:h4 res=5240
width=30%
./MENUITEMSELECTED (wpMenuItemSelected)
:link reftype=hd res=5241
auto dependent group=2.
:p.
This method is invoked whenever a menu item is selected.
:p.
:xmp.
ARG(1): "/MENUITEMSELECTED"
:exmp.

:h4 res=5241
width=70%
x=30%
hide
group=2
.Example
:p.
:xmp.

/* Rexx callback - wpMenuItemSelected

    In this example a separator and a new menu item are
    inserted into the popup menu. The new menu item is
    checked after insertion. When the user selects this
    new item a beep is produced.
    */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs


NUMERIC DIGITS 15 /* We need this for caculating with hex numbers.
                     Default is 9. */

SELECT
    .
    .
    .
WHEN ARG(1)="/MODIFYPOPUP" THEN DO

/* The script is called with 4 parameters.

ARG(1): The method indentifier. In this case /MODIFYPOPUP
ARG(2): The object pointer
ARG(3): Return value of the parent.
ARG(4): A composite argument consisting of
        -hwndMenu: menu handle
        -hwndCnr:  container handle
        -iPosition
        (see WPS Programming Reference)
*/

    /* Insert new items to the popup menu */
    PARSE VALUE ARG(4) WITH hwndMenu rest
    /* Add amenu separator at the end of the menu */
    ret=WPSWizCallWinFunc("menuInsertSeparator", hwndMenu, 0, -1)
    /* Insert new menu item with id 26000.
       User menu IDs must be above  25856 (WPMENUID_USER) */
    ret=WPSWizCallWinFunc("menuInsertItem",  hwndMenu, 0, -1, 26000, "Inserted with ~Rexx")
    /* Check the new item */
    ret=WPSWizCallWinFunc("menuCheckItem", hwndMenu, 26000, 1, 1)

    EXIT (ARG(3)) /* Return parent result */
END
WHEN ARG(1)="/MENUITEMSELECTED" THEN DO

/* The script is called with 4 parameters.

ARG(1): The method indentifier. In this case /MENUITEMSELECTED
ARG(2): The object pointer
ARG(3): Reserved
ARG(4): A composite argument consisting of
        -hwndFrame: frame handle
        -ulMenuId:  menu ID
        (see WPS Programming Reference)
*/

	/* This is called when the user selects a menu item */
	PARSE VALUE ARG(4) WITH hwndFrame ulMenuId
    /* The user selected the new menu item */
	if ulMenuID=26000 THEN DO
		call beep 1000, 500
        /* Return 1 if you handle this menu item */
		EXIT(1)
	END
	EXIT (0)
END
OTHERWISE
    /* Always return ARG(3) for unknown methods */
    return ARG(3)
END
:exmp.


.******************** wpModifyMenu

:h4 res=5300
width=30%
./MODIFYMENU (wpModifyPopupMenu)
:link reftype=hd res=5301
auto dependent group=2.
:p.
This callback is specific to V0.4.0 or higher.
:p.
This method is invoked whenever a menu is requested for an object. Use this method
to remove or add menu items in popup and pulldown menues.
:p.
:xmp.
ARG(1): "/MODIFYMENU"
:exmp.
:p.
Added user menu IDs must be higher than 25856 ( 0x6500, WPMENUID_USER).

:h4 res=5301
width=70%
x=30%
hide
group=2
.Example
:p.
:xmp.

/* Rexx callback - wpModifyMenu

    In this example a separator and a new menu item are
    inserted into the view menu. The new menu item is
    checked after insertion. When the user selects this
    new item a beep is produced.
    */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs


NUMERIC DIGITS 15 /* We need this for caculating with hex numbers.
                     Default is 9. */

SELECT
    .
    .
    .
WHEN ARG(1)="/MODIFYMENU" THEN DO

/* The script is called with 4 parameters.

ARG(1): The method indentifier. In this case /MODIFYPOPUP
ARG(2): The object pointer
ARG(3): Return value of the parent.
ARG(4): A composite argument consisting of
        -hwndMenu: menu handle
        -hwndCnr:  container handle
        -iPosition
        -ulMenuType:  the requested menu
                      MENU_OBJECTPOPUP       0x00000001
                      MENU_OPENVIEWPOPUP     0x00000002
                      MENU_FOLDERPULLDOWN    0x00000005
                      MENU_VIEWPULLDOWN      0x00000006
                      MENU_HELPPULLDOWN      0x00000007
                      MENU_EDITPULLDOWN      0x00000008
                      MENU_SELECTEDPULLDOWN  0x00000009                      
        -ulView:      the current view of the object
        (see WPS Programming Reference)
*/

    /* Insert new items to the popup menu */
    PARSE VALUE ARG(4) WITH hwndMenu 

	IF ulMenuType=6 THEN DO
      /* Add amenu separator at the end of the menu */
      ret=WPSWizCallWinFunc("menuInsertSeparator", hwndMenu, 0, -1)
      /* Insert new menu item with id 26000.
         User menu IDs must be above  25856 (WPMENUID_USER) */
      ret=WPSWizCallWinFunc("menuInsertItem",  hwndMenu, 0, -1, 26000, "Inserted with ~Rexx")
      /* Check the new item */
      ret=WPSWizCallWinFunc("menuCheckItem", hwndMenu, 26000, 1, 1)
    END
    EXIT (ARG(3)) /* Return parent result */
END
WHEN ARG(1)="/MENUITEMSELECTED" THEN DO

/* The script is called with 4 parameters.

ARG(1): The method indentifier. In this case /MENUITEMSELECTED
ARG(2): The object pointer
ARG(3): Reserved
ARG(4): A composite argument consisting of
        -hwndFrame: frame handle
        -ulMenuId:  menu ID
        (see WPS Programming Reference)
*/

	/* This is called when the user selects a menu item */
	PARSE VALUE ARG(4) WITH hwndFrame ulMenuId
    /* The user selected the new menu item */
	if ulMenuID=26000 THEN
		call beep 1000, 500
        /* Return 1 if you handle this menu item */
		EXIT(1)
	END
	EXIT (0)
END
OTHERWISE
    /* Always return ARG(3) for unknown methods */
    return ARG(3)
END
:exmp.



.******************** wpModifyPopupMenu

:h4 res=5230
width=30%
./MODIFYPOPUP wpModifyPopupMenu)
:link reftype=hd res=5231
auto dependent group=2.
:p.
This method is invoked whenever a popup menu is requested for an object. Use this method
to remove or add menu items.
:p.
:xmp.
ARG(1): "/MODIFYPOPUP"
:exmp.
:p.
Added user menu IDs must higher than 25856 ( 0x6500, WPMENUID_USER).

:h4 res=5231
width=70%
x=30%
hide
group=2
.Example
:p.
:xmp.

/* Rexx callback - wpModifyPopupMenu

    In this example a separator and a new menu item are
    inserted into the popup menu. The new menu item is
    checked after insertion. When the user selects this
    new item a beep is produced.
    */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs


NUMERIC DIGITS 15 /* We need this for caculating with hex numbers.
                     Default is 9. */

SELECT
    .
    .
    .
WHEN ARG(1)="/MODIFYPOPUP" THEN DO

/* The script is called with 4 parameters.

ARG(1): The method indentifier. In this case /MODIFYPOPUP
ARG(2): The object pointer
ARG(3): Return value of the parent.
ARG(4): A composite argument consisting of
        -hwndMenu: menu handle
        -hwndCnr:  container handle
        -iPosition
        (see WPS Programming Reference)
*/

    /* Insert new items to the popup menu */
    PARSE VALUE ARG(4) WITH hwndMenu rest
    /* Add amenu separator at the end of the menu */
    ret=WPSWizCallWinFunc("menuInsertSeparator", hwndMenu, 0, -1)
    /* Insert new menu item with id 26000.
       User menu IDs must be above  25856 (WPMENUID_USER) */
    ret=WPSWizCallWinFunc("menuInsertItem",  hwndMenu, 0, -1, 26000, "Inserted with ~Rexx")
    /* Check the new item */
    ret=WPSWizCallWinFunc("menuCheckItem", hwndMenu, 26000, 1, 1)

    EXIT (ARG(3)) /* Return parent result */
END
WHEN ARG(1)="/MENUITEMSELECTED" THEN DO

/* The script is called with 4 parameters.

ARG(1): The method indentifier. In this case /MENUITEMSELECTED
ARG(2): The object pointer
ARG(3): Reserved
ARG(4): A composite argument consisting of
        -hwndFrame: frame handle
        -ulMenuId:  menu ID
        (see WPS Programming Reference)
*/

	/* This is called when the user selects a menu item */
	PARSE VALUE ARG(4) WITH hwndFrame ulMenuId
    /* The user selected the new menu item */
	if ulMenuID=26000 THEN
		call beep 1000, 500
        /* Return 1 if you handle this menu item */
		EXIT(1)
	END
	EXIT (0)
END
OTHERWISE
    /* Always return ARG(3) for unknown methods */
    return ARG(3)
END
:exmp.


.******************** wpOpen (for folder objects)

:h4 res=5250
width=30%
./OPENFOLDER (wpOpen for folder objects)
:link reftype=hd res=5251
auto dependent group=2.
:p.
This callback is invoked when a folder just opened. The folder isn't populated yet.
This means no objects are yet inserted into the folder container.
:p.
:xmp.
ARG(1): "/OPENFOLDER"
:exmp.

:h4 res=5251
width=70%
x=30%
hide
group=2
.Example
:p.
:xmp.
    /* 
    In this example a toolbar is added to the
    folder frame.
    The user selection of the toolbar items is handled.
    */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/* Defines for window classes */
WC_BUTTON=ffff0003
WC_STATIC=ffff0005
WC_ENTRYFIELD=ffff0006
WC_MLE=ffff000A

NUMERIC DIGITS 15 /* We need this for caculating with hex numbers.
                     Default is 9. */

SELECT
    .
    .
    .
WHEN ARG(1)="/OPENFOLDER" THEN DO   /* The folder just opened */

    /* The script is called with 4 parameters.

    ARG(1): The method indentifier. In this case /OPENFOLDER
    ARG(2): The object pointer
    ARG(3): Reserved
    ARG(4): A composite argument consisting of
        -hwndFrame: frame handle
        -ulView:  viewid
        (see WPS Programming Reference)
    */

    /* Get the currently running script. The script is located in the
       bin directory of the WPS-Wizard installation */

    PARSE SOURCE . . rxfile
    /* Build the res dir from the script location. The res dir contains icons for the tool bar buttons */
    theDir=FILESPEC("d",rxFile)||FILESPEC("p",rxFile) /* Get base dir (x&colon.\...\base\bin\) */
    theDir=LEFT(theDir, LENGTH(theDir)-1)             /* remove last backslash (x&colon.\...\base\bin) */
    theDir=FILESPEC("d",theDir)||FILESPEC("p",theDir)||"res\" /* Add res dir to base path: (x&colon.\...\base\res\) */

    /* Call folder method to add tool bar */
    PARSE VALUE ARG(4) WITH hwndFrame ulView
    hwndToolbar=WPSWizCallFunc("wizAddToolBar", ARG(2), hwndFrame, 36, rxfile)

    /* Create some buttons */
    winData.0=10
    winData.1=hwndToolbar               /* Parent window */
    winData.2=x2D(ffff0003)             /* WC_BUTTON  */
    winData.3=X2D(80000000)             /* WS_VISIBLE */
    winData.4=10                        /* x      */
    winData.5=3                         /* y      */
    winData.6=30                        /* width  */
    winData.7=30                        /* height */
    winData.8=1                         /* ID   */

    /* Create 6 buttons on the tool bar */
    DO a=6 to 1 by -1
        winData.4=8+(a-1)*34            /* Horizontal position */
        winData.8=0+a                   /* ID                  */

        /* Specify icon file and fly over help text */
        SELECT
        WHEN a=1 THEN DO
            winData.9=theDir"parent.ICO" /* Icon */
            winData.10="Open parent"     /* Bubble help text */
        END
        WHEN a=2 THEN DO
            winData.9=theDir"properties.ICO" /* Icon */
            winData.10="Open properties" /* Bubble help text */
        END
        WHEN a=3 THEN DO
            winData.9="" /* No icon */
            winData.10="Change to Details" /* Bubble help text */
        END
        WHEN a=4 THEN DO
            winData.9="" /* No icon */
            winData.10="Change to Icon" /* Bubble help text */
        END
        WHEN a=5 THEN DO
            winData.9=theDir"config.ICO" /* Icon */
            winData.10="Open config folder" /* Bubble help text */
        END
        WHEN a=6 THEN DO
            winData.9=theDir"cmdwin.ICO" /* Icon */
            winData.10="Open command window" /* Bubble help text */
        END
        OTHERWISE 
            winData.10="" /* Default is no fly over help */
        END
        /* Create the button */
        rc=WPSWizCallWinFunc("winCreateWindow", "winData." , "")
    END

    /* Create an entry field */
    winData.2=x2D(WC_ENTRYFIELD)
    winData.3=X2D(80000008)  /* WS_VISIBLE+ES_MARGIN */
    winData.4=250
    winData.5=10
    winData.6=90
    winData.7=18
    winData.8=idEntry
    /* Create the windoe */
    rc=WPSWizCallWinFunc("winCreateWindow", "winData." , "")

    /* Create the find button */
    winData.2=x2D(ffff0003)  /* Button control */
    winData.3=X2D(80000000)  /* WS_VISIBLE */
    winData.4=350
    winData.5=2
    winData.6=60
    winData.7=32
    winData.8=50
    winData.9=""       /* No icon */
    winData.10="Search in help libraries"         /* Bubble help text */
    rc=WPSWizCallWinFunc("winCreateWindow", "winData." , "Find...")
    EXIT(0)
END
WHEN ARG(1)="/TOOLBAREVENT" THEN DO
    /* An event from the folder tool bar */

    /* This callback is called with only 3 parameters!
 
    ARG(1): The callback indentifier. In this case /TOOLBAREVENT
    ARG(2): The object pointer
    ARG(3): A composite argument

    */

    PARSE VALUE ARG(3) WITH cmd id hwnd hwndFrame
    /* cmd: the event. Currently only COMMAND 
       id:  id of the window (button) which caused the event
           hwnd: window handle of window 'id'
           hwndFrame: frame handle of the folder with this toolbar
    */
    if cmd="COMMAND" THEN DO
    SELECT
    WHEN id=1 THEN /* Open parent */
        ret=SysSetObjectData(WPSWizCallFunc("cwQueryRealName", ARG(2), 1), "MENUITEMSELECTED=714")
    WHEN id=2 THEN /* Open properties */
        ret=SysSetObjectData(WPSWizCallFunc("cwQueryRealName", ARG(2), 1), "MENUITEMSELECTED=112")
    WHEN id=3 THEN /* Change to details */
        ret=SysSetObjectData(WPSWizCallFunc("cwQueryRealName", ARG(2), 1), "MENUITEMSELECTED=718")
    WHEN id=4 THEN /* Change to icon */
        ret=SysSetObjectData(WPSWizCallFunc("cwQueryRealName", ARG(2), 1), "MENUITEMSELECTED=716")
    WHEN id=5 THEN /* Open system configuration */
        ret=SysSetObjectData("<WP_CONFIG>", "OPEN=DEFAULT")
    WHEN id=6 THEN /* Open a command line window */
        ret=SysSetObjectData("<WP_OS2WIN>", "OPEN=DEFAULT")
    WHEN id=50 THEN DO /* Find button clicked */
        /* get handle of entry field */
        hwndEntry=WPSWizCallWinFunc("winWindowFromID", hwnd , idEntry)
        /* Get text from entry field */
        searchText=WPSWizCallWinFunc("winQueryText", hwndEntry )
            if searchText="" THEN DO
            call rxMessagebox "Insert a word to search for." , "Information" 
            EXIT(0)
        END 
        /* Open the topic */
        ADDRESS CMD 'help 'searchText		
    END
    OTHERWISE
        EXIT(0)
    END
    END /* if COMMAND */
    EXIT(0)
END

OTHERWISE
    /* Always return ARG(3) for unknown methods */
    return ARG(3)
END
:exmp.


.******************** Folder open

:h4 res=5270
width=30%
./FOLDEROPENED (Folder is open)
:link reftype=hd res=5271
auto dependent group=2.
:p.
This callback is invoked when the folder is open and fully populated. This means all objects are
awake and inserted into the container.
:xmp.
ARG(1): "/FOLDEROPENED"
:exmp.


:h4 res=5271
width=70%
x=30%
hide
group=2
.Example
:p.
:xmp.
    /* 
        Rexx callback - Folder is open
    */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs


NUMERIC DIGITS 15 /* We need this for caculating with hex numbers.
                     Default is 9. */

    SELECT
    .
    .
    .
    WHEN ARG(1)="/FOLDEROPENED" THEN DO

    /* The script is called with 4 parameters.

    ARG(1): The method indentifier. In this case /FOLDEROPENED
    ARG(2): The object pointer
    ARG(3): Reserved
    ARG(4): Composite argument consisting of
            -hwndFrame      (frame handle )
            -ulView         (see WPS Programming Reference)
    */

        /* This is called when the folder is opened and fully populated. That means all objects are awake */
        PARSE VALUE ARG(4) WITH hwndFrame ulView .
        .
        .
        .
        EXIT(0)
    END

    OTHERWISE
        /* Always return ARG(3) for unknown methods */
        return ARG(3)
    END
:exmp.


.************ /FOLDERINFO ****************
:h4 res=5320
width=30%
./FOLDERINFO (folder info)
:link reftype=hd res=5311
auto dependent group=2.
:p.
This callback is specific to V0.4.0 or higher.
:p.
This callback is invoked when information about the currently open folder should be displayed. The returned
string will be shown in the top part of the folder information window. Links created during this callback must
have an ID below or equal 1000.
:p.
More about links and text formatting may be found :link reftype=hd res=5200.here:elink.

:xmp.
ARG(1): "/FOLDERINFO"
:exmp.


.************ /INFOWINDOWEVENT ****************
:h4 res=5330
width=30%
./INFOWINDOWEVENT
:link reftype=hd res=5311
auto dependent group=2.
:p.
This callback is specific to V0.4.0 or higher.
:p.
This callback is invoked when the user clicked a link created during the processing of the :hp2./OBJECTINFO:ehp2. or
:hp2./FOLDERINFO:ehp2. callback.


:xmp.
ARG(1): "/INFOWINDOWEVENT"
:exmp.

.************ Object hint ****************
:h4 res=5280
width=30%
./OBJECTHINT (object hint)
:link reftype=hd res=5281
auto dependent group=2.
:p.
This callback is invoked when a fly over hint for an object should be created. The returned
string will be shown in a window.
:xmp.
ARG(1): "/OBJECTHINT"
:exmp.


:h4 res=5281
width=70%
x=30%
hide
group=2
.Example
:p.
:xmp.
/* rexx -  This script calls the external script 'objecthint.rx'
   to create the string which will be shown as the fly over hint */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/* ARG(2) is the object pointer      */
/* ARG(3) is the default return code for other callbacks */

SELECT
  .
  .
  .
WHEN ARG(1)="/OBJECTHINT" THEN DO
    PARSE SOURCE . . theScript
    thePath=FILESPEC("drive",theScript)||FILESPEC("path", theScript)||"objecthint.rx"
    theHintCmd='call  "'||thePath||'" 'ARG(2)
    interpret theHintCmd
    EXIT result" "
END

OTHERWISE
   /* Always return ARG(3) for unknown methods */
    return ARG(3)
END

:exmp.

.************ /OBJECTINFO ****************
:h4 res=5310
width=30%
./OBJECTINFO (object info)
:link reftype=hd res=5311
auto dependent group=2.
:p.
This callback is specific to V0.4.0 or higher.
:p.
This callback is invoked when a information about a selected object in a folder should be displayed. The returned
string will be shown in the bottom part of the folder information window. Links created during this callback must
have an ID greate than 1000.
:p.
More about links and text formatting may be found :link reftype=hd res=5200.here:elink.
:xmp.
ARG(1): "/OBJECTINFO"
:exmp.


:h4 res=5311
width=70%
x=30%
hide
group=2
.Example
:p.
:xmp.
/* rexx -  Default REXX script handling folder info requests */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

theObject=ARG(2)

SELECT
WHEN ARG(1)="/OBJECTINFO" THEN DO
    /* Get title of object */
    theString="<1 0>"||TRANSLATE(WPSWizCallWPSFunc("wpQueryTitle", theObject), ' ',D2C(10) )||"<2>"

    /* Find object info in ini */
    PARSE SOURCE . . theScript
    iniPath=getIni()
    theId=WPSWizCallWPSFunc("wpQueryObjectID", theObject)
    if theId\="(null)" THEN DO
        iniValue=SysIni(iniPath, theId, "hint")
        if iniValue\="ERROR:" then DO
            iniValue=TRANSLATE(iniValue, ' ',D2C(10) )
            theString=theString||"<0>"||iniValue||"<0>"
            EXIT theString||" "
        END
    END
    EXIT theString||" "
END
WHEN ARG(1)="/FOLDERINFO" THEN DO
    PARSE SOURCE . . theScript

    theString="<1 1>"||TRANSLATE(WPSWizCallFunc("cwQueryRealName", ARG(2), 0), ' ',D2C(10) )||"<2><0>"

    call getNLS
    /* Find object info in ini */
    iniPath=getIni()
    theId=WPSWizCallWPSFunc("wpQueryObjectID", theObject)
    if theId\="(null)" THEN DO
        iniValue=SysIni(iniPath, theId, "hint")
        if iniValue\="ERROR:" then DO
            iniValue=TRANSLATE(iniValue, ' ',D2C(10) )
            theString=theString||LEFT(iniValue,LENGTH(iniValue)-1)||"<7>"
        END
    END
    /* Add folder actions for this folder */

    theString=theString||"<9 1> <3 1>"||nls._drives||"<4><6>"
    /* Links must have an ID below or equal to 1000. */
    theString=theString||"<3 2>"||nls._sysconf||"<4><6>"
    theString=theString||"<3 3>"||nls._wpswiz||"<4><10><7>"
    DROP nls.

    EXIT theString" "
END
WHEN ARG(1)="/INFOWINDOWEVENT" THEN DO
    /* This callback is called with only 3 parameters!
 
    ARG(1): The callback indentifier. In this case /INFOWINDOWEVENT
    ARG(2): The object pointer
    ARG(3): Deafult return code
    ARG(4): A composite argument

    */

    PARSE VALUE ARG(4) WITH cmd id hwndInfo hwndFrame object
    /* cmd: the event. Currently only COMMAND 
           id:  id of the link which caused the event
           hwnd: window handle of window holding the info text with the link
           hwndFrame: frame handle of the folder
           object:    Currently selected object
	*/
    SELECT
    WHEN id=1 then call sysSetObjectData "<WP_DRIVES>", "OPEN=DEFAULT"

    WHEN id=2 then call sysSetObjectData "<WP_CONFIG>", "OPEN=DEFAULT"

    WHEN id=3 then call sysSetObjectData "<WPSWIZ_FOLDER>", "OPEN=DEFAULT"
    OTHERWISE
        NOP
    END
END
OTHERWISE
    return ARG(3)
END

EXIT ARG(3)
:exmp.


.******************** Toolbar event

:h4 res=5260
width=30%
./TOOLBAREVENT (Toolbar event)
:link reftype=hd res=5251
auto dependent group=2.
:p.
This callback is invoked when the user interacts with the buttons and windows of the
folder toolbar.
:p.
:xmp.
ARG(1): "/TOOLBAREVENT"
:exmp.






