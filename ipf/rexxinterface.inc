
:h1 res=300.REXX interface
:p.
When using the REXX interface scripts have access to
the most important methods of the WPS and SOM helper functions. Additional functions are available for accessing
PM window functions.
:p.
REXX scripts may get called when
:ul.
:li.the user selects a script from the new :link reftype=hd res=5000.Scripts menu item:elink. in the menubar of folders.
Every folder will get a new sub menu in the menu bar which holds the scripts.
Configuration of the Scripts menu is
done similar to context menu configuration by the use of configuration folders.

:li.the WPS calls one of the :link reftype=hd res=5100.REXX callbacks:elink..
These callbacks intercept the processing of WPS methods. By altering parameters
or return values one may change the behaviour of WPS objects. It's similar to creating a child class using C or C++ and the
SOM compiler and overriding methods. For example it's possible to add or remove menu items, prevent the dropping of
particular objects on an object or process menus selected by the user.

:li.a :link reftype=hd res=30000.gadget:elink. is being executed. Gadgets may be used to gather system information and display them in the background
of the desktop. They're REXX scripts and have access to the REXX interface provided by WPS-wizard. 
:eul.


.************ Wrapper functions ******************************
.im wrapperfunctions.inc

.************ Folder scripts *********************************
.im folderscripts.inc


.************ REXX callbacks ******************************
.im rexxcallback.inc

.******************** The supported WPS methods
.im wpsmethods.inc

.******************** WPS class methods 
.im wpsclassmethods.inc

.******************** New methods
:h2 res=1000
width=30%
.New WPS methods
:p.
The following new methods are currently supported. These methods
and functions are introduced by WPS-Wizard. Some are similar to
normal WPS methods but use a simpler interface.
:p.
.br
:link reftype=hd res=1070 dependent.cwCreateLaunchArea:elink.
.br
:link reftype=hd res=1020 dependent.cwIsA:elink.
.br
:link reftype=hd res=1030 dependent.cwIsInstanceOf:elink.
.br
:link reftype=hd res=710 dependent.cwIsObj:elink.
.br
:link reftype=hd res=1080 dependent.cwQueryRealName:elink.
.br
:link reftype=hd res=1090 dependent.wizAddToolBar:elink.

.* Description of the new methods
.im newmethods.inc


.******************** WindowFunctions 
:h2 res=2000
width=30%
.Supported window functions
:p.
The following window functions are currently supported.
:p.
:link reftype=hd res=2180 dependent.drgCheckClass:elink.
.br
:link reftype=hd res=2160 dependent.drgCheckIfFileSysOnly:elink.
.br
:link reftype=hd res=2190 dependent.drgCheckSpecClass:elink.
.br
:link reftype=hd res=2170 dependent.drgQueryNumObjects:elink.
:p.
:link reftype=hd res=2080 dependent.menuCheckForItem:elink.
.br
:link reftype=hd res=2090 dependent.menuCheckItem:elink.
.br
:link reftype=hd res=2290 dependent.menuEnableItem:elink.
.br
:link reftype=hd res=2100 dependent.menuInsertItem:elink.
.br
:link reftype=hd res=2110 dependent.menuInsertSeparator:elink.
.br
.*:link reftype=hd res=2120 dependent.menuQueryItem:elink.
.*.br
:link reftype=hd res=2130 dependent.menuQueryItemCount:elink.
.br
:link reftype=hd res=2140 dependent.menuRemoveItem:elink.
.*.br
.*:link reftype=hd res=2150 dependent.menuSelectItem:elink.
:p.
:link reftype=hd res=2220 dependent.winCreateWindow:elink.
.br
:link reftype=hd res=2050 dependent.winFileDlg:elink.
.br
:link reftype=hd res=2030 dependent.winObjectFromPRec:elink.
.br
:link reftype=hd res=2010 dependent.winQueryContainerHWND:elink.
.br
:link reftype=hd res=2020 dependent.winQueryContainerRecord:elink.
.br
:link reftype=hd res=2040 dependent.winQueryContRecEmphasis:elink.
.br
:link reftype=hd res=2070 dependent.winQueryFolderObjects:elink.
.br
:link reftype=hd res=2060 dependent.winQueryObject:elink.
.br
:link reftype=hd res=2240 dependent.winQueryText:elink.
.br
:link reftype=hd res=2200 dependent.winQueryWindowPos:elink.
.br
:link reftype=hd res=2230 dependent.winSetText:elink.
.br
:link reftype=hd res=2210 dependent.winSetWindowPos:elink.
.br
:link reftype=hd res=2280 dependent.winShowWindow:elink.
.br
:link reftype=hd res=2250 dependent.winWindowFromID:elink.

.*last res: 2290

.*include the function descriptions.
.im windowfunctions.inc

.*******************************
.* Info about how to format text in the info area
.im infoformatting.inc





