.**************** cwCreateLaunchArea **************************************
:h3 res=1070
width=30%
.cwCreateLaunchArea
:link reftype=hd res=1071
auto dependent group=2.
:p.
:link reftype=hd res=1071.Syntax:elink.
.br
:link reftype=hd res=1072 dependent.Parameters:elink.
.br
:link reftype=hd res=1073 dependent.Returns:elink.
.br
:link reftype=hd res=1074 dependent.Remarks:elink.
.br
:link reftype=hd res=1076 dependent.Example:elink.

:h3 res=1071
x=30%
width=70%
height=100%
hide
group=2
.cwCreateLaunchArea - Syntax
:xmp.
/* This method creates a new launch area window */

theObject   /* Folder object for which the script is running (or any other WPS object) */
winData.    /* Stem holding creation info for the window                  */
fFlag       /* Flag specifying the type of launch area to be created      */
laFolder    /* CWLaunchArea folder holding the objects of the launch area */
:p.
rc=WPSWizCallFunc("cwCreateLaunchArea", 
                        theObject, className);
:exmp.

:h3 res=1072
x=30%
width=70%
height=100%
hide
group=2
.cwCreateLaunchArea - Parameters
:p.
:parml tsize=10.
:pt.:hp2.theObject:ehp2. (numeric)
:pd.Pointer to a WPS object. Normally this is the pointer to the folder the script is running on. 
:pt.:hp2.winData.:ehp2. (Rexx stem)
:pd.
:parml tsize=3.
:pt.winData.0
:pd.Number of parameters in the stem.
:pt.winData.1
:pd.Handle (HWND) of the parent window.
:pt.winData.2
:pd.Handle (HWND) of the owner window. When the owner window is destroyed the owned window will also be destroyed.
:pt.winData.3
:pd.X position. This is relative to the parents lower left corner.
:pt.winData.4
:pd.Y position. This is relative to the parents lower left corner.
:pt.winData.5
:pd.Initial width. If there're already objects in the CWLaunchArea folder the launch area window will automatically be resized. 
:pt.winData.6
:pd.Initial height. The buttons on the launch area will automatically resized to fit into this height.
:pt.winData.7
:pd.A string to be shown as the fly over help on an empty launch area. This parameter may be omitted and a default
string will be shown. When omitted make sure winData.0 is adjusted accordingly.
:eparml.
:pt.:hp2.fFlag:ehp2. (numeric)
:pd.
:parml tsize=3.
:pt.0
:pd.Create a normal launch area to be used like a launchpad.
:pt.1
:pd.Create a launch area holding folder action objects. These objects must be Rexx scripts or pointer to Rexx scripts.
:eparml.
:pt.:hp2.laFolder:ehp2. (string)
:pd.CWLaunchArea folder holding the objects of the launch area. This may be a full path or an object ID.
The folder must have been created before calling the function. 
:eparml.




:h3 res=1073
x=30%
width=70% height=35%
group=3
hide
.cwCreateLaunchArea - Return value
:p.
:hp2.rc:ehp2. string
:p.
The return value is composed of the window handle of the new launch area
and the object handle of the CWLaunchArea folder. The first part is the window handle.
:p.
See the example how to parse the return value.


:h3 res=1074
x=30%
width=70%
height=100%
hide
group=2
.cwCreateLaunchArea - Remarks
:p.
This function creates a launch area window which may hold WPS objects. The
created window may have any parent for example a folder toolbar.

:p.
There're two kinds of launch area.
:ul.
:li.Launch areas holding objects (launchpad), fFlag=0
:li.Launch areas holding folder action objects, fFlag=1. These objects are Rexx scripts
started after clicking and normally change the folder or it's contents.
See :link reftype=hd res=22000.Toolbar:elink. for more information.
:eul.
:p.
After creation the launch area is invisible, It's necessary to call the
:link reftype=hd res=2280.winShowWindow:elink. function to make it visible.
:p.
There is no limit on the count of launch areas per CWLaunchArea folder. All launch areas
share the same objects and will be updated after adding or removing an object from one
of the areas. Ich you want to add different objects to another folder you have to provide
a special CWLaunchArea folder for this folder. 

:p.
Before calling this function the CWLaunchArea folder containing the objects must already
exist. It will not be created during this call. The following is an example how to build
a new CWLaunchArea folder.

:xmp.

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/* Create a new launch area folder */
rc=SysCreateObject("CWLAunchArea", "New launch area", "<WP_DESKTOP>", "OBJECTID=<NEW_LA>;" ,"update")

/* Add some objects to the new launch area. This will not
   create a launch area window. */
rc=SysSetObjectData("<NEW_LA>", "FPOBJECTS=<WP_SYSTEM>,<WP_DRIVES>,x&colon.\the_path\file;");
:exmp.


:h3 res=1076
x=30%
width=70%
height=100%
hide
group=3
.cwCreateLaunchArea - Example
:p.
This example adds a toolbar to a folder and creates a launch area
on the toolbar.
:xmp.

WHEN ARG(1)="/OPENFOLDER" THEN DO   /* The folder just opened */

  if WANTTOOLBAR=NO THEN EXIT(0)

  /* Get the currently running script */
  PARSE SOURCE . . rxfile

  PARSE VALUE ARG(4) WITH hwndFrame ulView

  /* Size folder so toolbar fits */
  PARSE VALUE WPSWizCallWinFunc("winQueryWindowPos", hwndFrame) WITH x y cx cy rest

  if cy<FOLDER_MIN_SIZE_Y then
    ret=WPSWizCallWinFunc("winSetWindowPos", hwndFrame, 0, x, y-FOLDER_MIN_SIZE_Y+cy, cx, FOLDER_MIN_SIZE_Y, 3)
  /* Call folder method to add tool bar */
  hwndToolbar=WPSWizCallFunc("wizAddToolBar", ARG(2), hwndFrame, 36, rxfile)

  winData.0=6
  winData.1=hwndToolbar    /* Parent window */
  winData.2=hwndToolbar    /* Owner  */
  winData.3=550                            /* x      */
  winData.4=0                             /* y      */
  winData.5=36                            /* width  */
  winData.6=36                            /* height */
  rc=WPSWizCallFunc("cwCreateLaunchArea",  ARG(2), "winData." , 0, "<CWLA_FOLDER>")
  PARSE VAR rc hwndLA object

  if hwndLA<>0 THEN
    rc=WPSWizCallWinFunc("winShowWindow", hwndLA , 1)
  EXIT(0)
END

:exmp.

.**************** cwIsA **************************************
:h3 res=1020
width=30%
.cwIsA
:link reftype=hd res=1021
auto dependent group=2.
:p.
:link reftype=hd res=1021.Syntax:elink.
.br
:link reftype=hd res=1023 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=1014 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=1021
x=30%
width=70%
hide
group=2
.cwIsA - Syntax
:xmp.
/* This method checks if the object is an instance
   of the class or of one of it's subclasses */

theObject   /* The object to be checked */
className   /* Name of a class */
:p.
rc=WPSWizCallFunc("cwIsA", 
                        theObject, className);
:exmp.


:h3 res=1023
x=30%
width=70% height=35%
group=3
hide
.cwIsA - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
If the object isn't an instance of the class or a subclass
0 is returned. If the class can't be found on the system
:hp2.ERROR_CLASS&colon.:ehp2. is returned.


.************** cwIsInstanceOf ****************************************

:h3 res=1030
width=30%
.cwIsInstanceOf
:link reftype=hd res=1031
auto dependent group=2.
:p.
:link reftype=hd res=1031.Syntax:elink.
.br
:link reftype=hd res=1033 dependent.Returns:elink.
.br
:link reftype=hd res=1034 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=1031
x=30%
width=70%
hide
group=2
.cwIsInstanceOf - Syntax
:xmp.
/* This method checks if the object is an instance
   of the particular class */

theObject   /* The object to be checked */
className   /* Name of a class */
:p.
rc=WPSWizCallFunc("cwIsInstanceOf", 
                        theObject, className);
:exmp.


:h3 res=1033
x=30%
width=70% height=35%
group=3
hide
.cwIsInstanceOf - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
If the object isn't an instance of the class
0 is returned. If the class can't be found on the system
:hp2.ERROR_CLASS&colon.:ehp2. is returned.

:h3 res=1034
x=30%
width=70%
hide
group=2
.cwIsInstanceOf - Remarks
:p.
This method checks if the object is an instance of a particular class.
For example given the following list of classes.
:p.
ParentClass->CWAudio->CWMP3->ChildClass
:p.
If the object is an instance of CWMP3 a call with class name
:hp2.CWAudio:ehp2. would return 0.
:p.
Use :link reftype=hd res=1020.cwIsA:elink. if you want to check for
such cases. 


.***************************************************
:h3 res=710
width=30%
.cwIsObj
:link reftype=hd res=711
auto dependent group=2.
:p.
:link reftype=hd res=711.Syntax:elink.
.br
:link reftype=hd res=713 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=711
x=30%
width=70%
hide
group=2
.cwIsObj - Syntax
:xmp.
/* Check if an object pointer is valid */

theObject         /* Object to be checked */
:p.
bValid=WPSWizCallFunc("cwIsObj", theObject);
:exmp.


:h3 res=713
x=30%
width=70% height=35%
group=3
hide
.cwIsObj - Return value
:p.
:hp2.bValid:ehp2. numeric
:p.
The return value is numeric. It's 0 if the parameter isn't an object pointer.


.******************************************************
:h3 res=1080
width=30%
.cwQueryRealName
:link reftype=hd res=1081
auto dependent group=2.
:p.
:link reftype=hd res=1081.Syntax:elink.
.br
:link reftype=hd res=1085 dependent.Parameters:elink.
.br
:link reftype=hd res=1083 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=1074 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=1081
x=30%
width=70%
hide
group=2
.cwQueryRealName - Syntax
:xmp.
/* This method queries the real name of an object */

wpObject    /* Pointer to an object on which the method is invoked */
flag        /* Flag specifying if full path is queried */
:p.
name=WPSWizCallFunc("cwQueryRealName", 
                    wpObject, flag);
:exmp.


:h3 res=1085
x=30%
width=70%
hide
group=2
.cwQueryRealName - Parameters
:p.
:parml tsize=3.
:pt.:hp2.wpObject:ehp2.
:pd.Pointer to the object.
:pt.:hp2.flag:ehp2.
:pd.If flag is 1
the full qualified path is returned. If flag is 0 only the name of the
object is returned.
:pt.:hp2.name:ehp2.
:pd.The real name of the object as used by the file system.
:p.
In case of an error :hp2.ERROR&colon.:ehp2. is returned.
:eparml.

:h3 res=1083
x=30%
width=70% height=35%
group=3
hide
.cwQueryRealName - Return value
:p.
:hp2.info:ehp2. string
:p.
The real name of the object as used by the file system. If flag is 1
the full qualified path is returned. If flag is 0 only the name of the
object is returned.
:p.
In case of an error :hp2.ERROR&colon.:ehp2. is returned.


.*************** wizAddToolBar ***************************************
:h3 res=1090
width=30%
.wizAddToolBar
:link reftype=hd res=1091
auto dependent group=2.
:p.
:link reftype=hd res=1091.Syntax:elink.
.br
:link reftype=hd res=1095 dependent.Parameters:elink.
.br
:link reftype=hd res=1093 dependent.Returns:elink.
.br
:link reftype=hd res=1094 dependent.Remarks:elink.
.br
:link reftype=hd res=5251 dependent.Example:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=1091
x=30%
width=70%
hide
group=2
.wizAddToolBar - Syntax
:xmp.
/**************************************************/
/* This method adds a tool bar to a folder        */
/* window.                                        */
/* It's introduced by the WPS-Wizard folder class */
/* CWFolder.                                      */
/**************************************************/
WPObject wpFolder      /* Pointer to the folder on which the method is invoked */
HWND     hwndFrame     /* Frame handle of the open folder                      */
ULONG    ulHeight      /* Height of the tool bar                               */
PSZ      pszRexxScript /* Rexx script which handles user interaction           */
HWND     hwndTB        /* Window handle of the created toolbar                 */
:p.
hwndTB=WPSWizCallFunc("wizAddToolBar", wpFolder,
                     hwndFrame, ulHeight, pszRexxScript);
:exmp.


:h3 res=1095
x=30%
width=70%
hide
group=2
.wizAddToolBar - Parameters
:p.
:parml tsize=3.
:pt.:hp2.wpFolder:ehp2.
:pd.Pointer to the folder object. This is given as the second Parameter (ARG(2)) to a callback.
:pt.:hp2.hwndFrame:ehp2.
:pd.The frame handle of the open folder.
:pt.:hp2.ulHeight:ehp2.
:pd.Height of the tool bar. The height of the bar can't be changed after creation.
:pt.:hp2.pszRexxScript:ehp2.
:pd.Absolute path to a REXX script which handles user input. This may be a completely independend
script.
:pt.:hp2.hwndTB:ehp2.
:pd.Window handle of the newly created tool bar.
:p.
:parml tsize=10.
:pt.0
:pd.Tool bar not created.
:pt.Other
:pd.Tool bar handle.
:eparml.

:eparml.


:h3 res=1093
x=30%
width=70% height=35%
group=3
hide
.wizAddToolBar - Return value
:p.
:hp2.hwndTB:ehp2. (numeric)
:p.
Window handle of the newly created tool bar.
:p.
:parml tsize=10.
:pt.0
:pd.Tool bar not created.
:pt.Other
:pd.Tool bar handle.
:eparml.



:h3 res=1094
x=30%
width=70%
hide
group=2
.wizAddToolBar - Remarks
:p.
This method may only be called from the folder :link reftype=hd res=5250.wpOpen:elink. callback.
:p.
The returned handle :hp1.hwndTB:ehp1. can be used to create windows on the tool bar. The REXX script
given to the method can be located anywhere. It doesn't has to be the same script which contains
the :link reftype=hd res=5250.wpOpen:elink. callback.





