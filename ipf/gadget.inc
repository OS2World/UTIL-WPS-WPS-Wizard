.*----------------------- +++ h1 +++ ---------------------
:h1 res=30000
width=100%
.Gadget programming
:p.
Gadgets use the :link reftype=hd res=300.REXX interface:elink. provided by WPS-wizard. There're
additional :link reftype=hd res=30400.gadget functions:elink. currently available only to gadget scripts.
:p.
:nt.
A user sees a Gadget as a script or object started like any other program. After invocation something
happens on the desktop. In the following text this will be called a :hp2.Gadget script:ehp2. because
technically a gadget is a single object with several functions callable by the script and drawing on
the desktop background but not a script. A gadget may be the owner of several child gadgets and all
together form a bigger more complicated gadget. To prevent confusion between the user reception of
gadgets and the implementation the latter is called a :hp2.gadget object:ehp2.. 
:ent.
:p.
A gadget script can be divided into four parts.

:ol.
:li.Code to start the script as a gadget
:li.Creation of gadget objects
:li.Message loop
:li.Section with command handlers
:eol.

:p.
:hp7.Code to start the script as a gadget:ehp7.
:p.
Gadget scripts are started like any other REXX script. But to use the REXX interface provided by
WPS-wizard it's necessary for the script to run in the context of the WPS. This is achieved by
using a two step start procedure. The gadget script sends a setup string to the desktop folder  and the
WPS executes the script in an own thread. The following code part shows this.
:p.
:xmp.
/* Gadget script*/

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/* Check if the script was directly started */
IF ARG() = 0 THEN DO
  /* Start the gadget */
  PARSE SOURCE . . theScript
  IF ARG(1)="" THEN
    call SysSetObjectdata "<WP_DESKTOP>", "WIZLAUNCHGADGET="theScript	
  Exit(0) /* Terminate the script. The WPS will restart it as a gadget */
END

NUMERIC DIGITS 15 /* We need this for calculating with hex numbers */

SELECT
WHEN ARG(1)="/GADGETSTARTED" THEN DO
    theObject=ARG(2)
    hwnd=ARG(3)

    /* Create your gadgets here */

[...]

END
OTHERWISE
    /* We shouldn't end here... */
    Exit(0)
END

:exmp.
Because gadget scripts are finally started by the WPS but not by the command shell it's possible
to start them from every program or REXX script just by sending the appropriate setup
string to the desktop folder. 
:p.
The WPS gives several parameters to the script when it is started. These parameters must be used
when calling WPS-wizard functions.
:p.
:parml tsize=10.
:pt.:hp2.ARG(1):ehp2.
:pd.Always :hp2./GADGETSTARTED:ehp2. when the script is started as a gadget.
:pt.:hp2.ARG(2):ehp2.
:pd.WPS object pointer of the desktop folder <WP_DESKTOP>. 
:pt.:hp2.ARG(3):ehp2.
:pd.Window handle of an invisible object window. This handle must be used when getting messages in the message loop of the gadget. 
:eparml.
:p.
:hp7.Creation of gadget objects:ehp7.
:p.
Gadget objects are created by calling :link reftype=hd res=31140.WizCreateGadget(parent, stem., name):elink.. 
:p.
Normally a gadget object lives directly on the desktop. In that case the parent is :hp2.DESKTOP:ehp2..
Gadget objects may be grouped together by specifying the same parent gadget object while creating new
gadget object. By doing that all child gadget objects are moved when the parent
gadget (which owns the childs) is moved. See the Drive information or System information script
for an example. Only special gadget objects may own other gadget objects. These special gadgets
are called container gadgets. Currently the GROUP_GADGET and the IMAGE_GADGET are the available container gadgets.
:p.
The following code creates a gadget object living on the desktop:
:xmp.
/* Gadget create info */
  thestem._x=x                 /* x  */
  thestem._y=y                 /* y  */
  thestem._cx=cx               /* cx */
  thestem._cy=cy               /* cy */
  thestem._type=HTML_GADGET    /* Gadget type */
  thestem._hwnd=ARG(3)         /* hwnd */
  thestem._flags=GSTYLE_POPUP  /* We want a popup menu */
  thestem._font="12.Helv"      /* font */

/* Create gadget on the desktop */
  rc=WizCreateGadget("DESKTOP", "thestem." , "timeGadget")

:exmp.
:p.
In the next example a group gadget object is created which owns several child gadget objects.
:xmp.
  /* Create a group gadget */
  stem._x=30                 /* x  */
  stem._y=100                /* y  */
  stem._cx=360               /* cx */
  stem._cy=100               /* cy */
  stem._type=GROUP_GADGET    /* Gadget type */
  stem._hwnd=ARG(3)          /* hwnd */
  stem._flags=GSTYLE_POPUP 

/* Create the gadget on the desktop */
  ret=WIZCREATEGADGET("DESKTOP", "stem.", groupGadget)

  /* Create a memory bar gadget owned by the group gadget */
  stem._x=50               /* x  */
  stem._y=110              /* y  */
  stem._cx=320             /* cx */
  stem._cy=cy              /* cy */
  stem._type=BAR_GADGET    /* Gadget type */
  stem._hwnd=ARG(3)        /* hwnd */
  stem._flags=0            /* No popup menu for this child */

/* Create the child gadget */
  ret=WIZCREATEGADGET("groupGadget", "stem." , "membargadget")

  /* Create a bootdrive bar gadget owned by the group gadget */
  stem._x=50               /* x  */
  stem._y=140              /* y  */
  stem._cx=320             /* cx */
  stem._cy=cy              /* cy */
  stem._type=BAR_GADGET    /* Gadget type */
  stem._hwnd=ARG(3)        /* hwnd */
  stem._flags=0            /* No popup menu for this child */

/* Create the child gadget */
  ret=WIZCREATEGADGET("groupGadget", "stem.", driveGadget)

:exmp.

:p.
Every gadget supports several functions for setting e.g. the gadget objects position, the color
or the font. These functions are called like normal REXX functions. The function name used
during the call is built from the gadget object name and the gadgets function.
:p.
For example if the gadget function :hp2.Color():ehp2. of the gadget object :hp2.myGadget:ehp2.
shall be called in the script this would result in the following line:

:xmp.
  ret=myGadget.Color()

:exmp.
The following code shows how to set the colors of the bar gadget object created in the previous example.

:xmp.
/* Set foreground color (text) and background color (bar) */
  call groupGadget.drivegadget.color X2D('dddddd')
  call groupGadget.drivegadget.backcolor X2D('8080ff')
:exmp.

:p.
:hp7.Message loop:ehp7.
:p.
Every gadget script :hp2.must:ehp2. maintain a message loop. It is necessary because gadgets have a
hidden window and messages sent to this window by the system are only handled if the gadget
regulary calls the :hp2.WIZGETMESSAGE():ehp2. function. Without a message loop the systems
user interface may eventually freeze.
:p.
Despite this more technical consideration a missing message loop would prevent that the user
can interact with a gadget. 
:p.
Example of a message loop:
:xmp.

/* Gadget message loop */
  DO FOREVER
    ret=WIZGETMESSAGE(ARG(3))
    if ret<>'' THEN 
      INTERPRET "call "ret
  END
:exmp.

:p.
:hp7.Section with command handlers:ehp7.
:p.
If gadgets support popup menus (as specified during the creation process) the gadget script
must contain handlers for the messages sent by the system. If these handlers are omitted
the script will terminate with an error as soon as the system tries to deliver the message.
:p.
During script execution several gadget objects may be created. Handlers are specific for a
particular gadget object which has a distinctive name. To route the messages to the right
handler these must be named accordingly. 
:p.
For example if the gadget object is named 
:xmp.
  myGadget
:exmp.
the popup menu handler must be named
:xmp.
  myGadget.onPopup:
:exmp.
:p.
Gadget objects may be owned by other (named) gadget objects. In that case the gadget objects form
a chain of gadget object. For example if a gadget object is created on the desktop named
:hp2.groupGadget:ehp2. and another gadget object is created with the name :hp2.myGadget:ehp2. and
the groupGadget as parent the resulting chain is the following.
:xmp.
  groupGadget->myGadget
:exmp.
:p.
Handler names for myGadget mirror the gadget object chain. They contain all names of the chain divided
by a point.
:xmp.
  groupGadget.myGadget.onPopup:
:exmp. 
.*--------------------------------------------


.*--------------------------------------------
:h2 res=30400
width=100%
.Gadget functions
:p.
Every gadget supports a range of common functions. These are listed below. In addition gadgets
may support more functions for special needs. For example the ICON_GADGET has a function to set the
icon file to be used when drawing the gadget object. For these additional functions see
the section about the different gadget types.
:p.
Most functions can be used to set and to query a value. If no parameter is given when the
function is called the current value is returned. If a parameter is provided this parameter will
be used as the new value and the old one is returned.
:p.
This returns the current foreground color of a gadget:
:xmp.
  currentColor=myGadget.Color()

:exmp.
This sets a new foreground color:
:xmp.
  previousColor=myGadget.Color(X2D('dddddd'))

:exmp.
:p.
:hp7.Gadget functions&colon.:ehp7.
:parml tsize=10.
:pt.:hp2.Color([newColor]):ehp2.
:pd.Set and query the foreground colour.
:pt.:hp2.Backcolor([newColor]):ehp2.
:pd.Set and query the background colour.
:pt.:hp2.Text(newText):ehp2.
:pd.Set the gadget objects text. Currently it's not possible to query a gadget objects text.
:pt.:hp2.Font([newFont]):ehp2.
:pd.Set and query the font.
:pt.:hp2.Style( newStyle, mask):ehp2.
:pd.Set and query the style of the gadget object. The style is determined by the :hp2._flags:ehp2.
parameter during creation of a gadget object. Using this call it's possible to change for example the
GSTYLE_NOACTIVATE style of a gadget object later on to prevent that a user changes child gadget 
objects owned by a group gadget object. Only bits contained in the mask will be changed.
:pt.:hp2.Position([x, y, cx, cy]):ehp2.
:pd.Returns a string in the format:
:xmp.
  x y cx cy
:exmp.
:eparml.
:p.
Gadgets may call the following functions. These
are currently not available to other scripts. Main difference
to the other WPS-wizard functions is that :hp2.not:ehp2. a wrapper
is called with the name and the parameters but the function directly.
Another difference is that the function names are not case sensitive.
:p.
.br
:link reftype=hd res=31140 dependent.WizCreateGadget:elink.
.br
:link reftype=hd res=31120 dependent.WizDestroyGadget:elink.
.br
:link reftype=hd res=31130 dependent.WizGetMessage:elink.
.br
:link reftype=hd res=31150 dependent.WizQueryDrives:elink.
.br
:link reftype=hd res=31100 dependent.WizQueryFreeMem:elink.
.br
:link reftype=hd res=31170 dependent.WizQueryImageInfo:elink.
.br
:link reftype=hd res=31110 dependent.WizQueryUptime:elink.
.br
:link reftype=hd res=31160 dependent.WizStartTimer:elink.
.br
:link reftype=hd res=31180 dependent.WizStopTimer:elink.
.br


.* last used ID: 31180

.* Include the descriptions for gadgetfuncs
.im gadgetfunctions.inc

.*--------------------------------------------


.*--------------------------------------------
:h2 res=30500
width=100%
.Command handlers
:p.
Command handlers are called in reponse to an event or user action. For example a script may
request regular timer events or a user may select an item of a popup menu. Handlers are only
necessary if a script allows actions causing some event. For example a gadget not allowing
any user interaction (because it can't be activated) doesn't need a popup menu handler. As
a consequence it also doesn't need a command handler because no source for a command is
available. The handlers are normal REXX functions which are called from within the scripts
message loop with or without parameters.
:p.
Command handlers have a name built from the names of a gadget (or a gadget chain) and the
particular event or message. So distinctive handlers may be created for every single gadget
object. The case of the handler name is not important. Every command handler 
.br
The following is an example for a popup event handler for the gadget object :hp2.myGadget:ehp2.
which is owned by the group gadget object :hp2.groupGadget:ehp2..
:xmp.
groupgadget.myGadget.onpopup: 

  /* Handle a request for a context menu here... */

return
:exmp.
:p.
The following command handlers may be used in a gadget script.

:parml tsize=10.
:pt.:hp2.onCommand:ehp2.
:pd.Called from the message loop whenever the user selected an item in the popup menu of
the gadget object.
:parml tsize=3.
:pt.ARG(1)
:pd.Window handle (hwnd) of the gadget objects client area.
:pt.ARG(2)
:pd.ID of the menu item.
:pt.ARG(3)
:pd.Reserved
:eparml.

:pt.:hp2.onPopup:ehp2.
:pd.The user requested a context menu on an activated gadget object. During the handler a popup menu may
be built an presented to the user. If no menu is desired the handler may just return.
:p.
The handler is called with three parameters.
:parml tsize=3.
:pt.ARG(1)
:pd.Window handle (hwnd) of the gadget objects client area.
:pt.ARG(2)
:pd.x position of the mouse pointer at the time the button was clicked in window coordinates.
:pt.ARG(3)
:pd.y position of the mouse pointer at the time the button was clicked in window coordinates.
:eparml.
:pt.:hp2.onTimer:ehp2.
:pd.If the script started a timer regular timer events are sent to the message loop. In response
to these events the onTimer handler is called.
:parml tsize=3.
:pt.ARG(1)
:pd.Timer ID as specified when creating the timer.
:eparml.
:pt.:hp2.:ehp2.
:pd.

:eparml.


.*--------------------------------------------


.*--------------------------------------------
:h2 res=30100
width=100%
.Gadget types
:p.
The following gadgets may be created:

:ul.
:li.:link reftype=hd res=30110.BAR_GADGET:elink.
:p.A gadget displaying a bar to show a current value. For example the free space on a drive. 
:li.:link reftype=hd res=30120.GROUP_GADGET:elink.
:p.Use this to group several gadgets together. When the group gadget is moved all the owned
gadget are also moved.
:li.:link reftype=hd res=30130.HTML_GADGET:elink.
:p.This gadget displays formated text.
:li.:link reftype=hd res=30140.ICON_GADGET:elink.
:p.A gadget only displaying an icon.
:li.:link reftype=hd res=30150.IMAGE_GADGET:elink.
:p.A gadget displaying an image.
:eul.
.*--------------------------------------------


.*----------------- BAR_GADGET ---------------------------
:h3 res=30110
width=100%
.BAR_GADGET
:p.The BAR_GADGET displays a bar which length represents an arbitrary value, for example the free memory in
the system. The length of the bar is proportional to the value in relation to a maximum.
A Border may or may not be drawn around the bar.
Several different border styles are possible which may be set during creation or later using the :hp2.Style():ehp2.
gadget function. A text may be shown on the bar which can be set using the normal :hp2.text():ehp2. function. 
:p.
:hp7.Styles:ehp7.
:p.
The BAR_GADGET supports the following additional styles&colon.
:p.
:parml tsize=10.
:pt.:hp2.GSTYLE_BORDER:ehp2.
:pd.Draw a border around the bar. This border will have a raised appearance.

:pt.:hp2.GSTYLE_SUNKENBORDER:ehp2.
:pd.Draw a sunken border around the bar.

:pt.:hp2.GSTYLE_COLORBORDER:ehp2.
:pd.Draw a border in the color of the bar.

:pt.:hp2.GSTYLE_FORECLRBORDER:ehp2.
:pd.Draw a border using the text color.
:eparml.
:p.
If none of the border styles is set the bar will be drawn without a border.
:p.
:hp7.Additional functions:ehp7.
:p.
The BAR_GADGET supports a function to set the bar value in addition to all the normal gadget functions.
:p.
:parml tsize=10.
:pt.:hp2.BarValue(currentValue, maxValue):ehp2.
:pd.The function returns 1 for success, 0 otherwise.
:eparml.
:p.
:hp7.Remarks:ehp7.
:p.
This gadget supports drag and drop of colours for changing the bar (background) and the text colour.
.*--------------------------------------------


.*------------- GROUP_GADGET -------------------------------
:h3 res=30120
width=100%
.GROUP_GADGET
:p.
The group gadget is a container gadget. This means a group gadget object  can be parent of several other gadget objects.
When the group gadget object is moved all owned childs will also move.
.br
Because the group gadget allows to be drawn with  a border around it may also be used as a plain "border gadget".
:p.
:hp7.Styles:ehp7.
:p.
The GROUP_GADGET supports the following additional styles&colon.
:p.
:parml tsize=10.
:pt.:hp2.GSTYLE_BORDER:ehp2.
:pd.Draw a border around the gadget object. This border will have a raised appearance.

:pt.:hp2.GSTYLE_SUNKENBORDER:ehp2.
:pd.Draw a sunken border around the gadget object.

:pt.:hp2.GSTYLE_COLORBORDER:ehp2.
:pd.Draw a border in the color of the background.
:eparml.
:p.
If none of the border styles is set no border will be drawn.
:p.
:hp7.Additional functions:ehp7.
:p.
This gadget has no additional functions.
:p.
:hp7.Remarks:ehp7.
:p.
This gadget supports drag and drop of colours for changing the border (drop background color).
.*--------------------------------------------


.*---------------- HTML_GADGET ----------------------------
:h3 res=30130
width=100%
.HTML_GADGET
:p.
A HTML_GADGET shows formatted test. See the chapter :link reftype=hd res=5200.Information window:elink. for a
description of the supported tags.
:p.
:hp7.Styles:ehp7.
:p.
This gadget supports no additional styles.
:p.
:hp7.Additional functions:ehp7.
:p.
This gadget has no additional functions.
:p.
:hp7.Remarks:ehp7.
:p.
This gadget supports drag and drop of colours for changing the text colour. Be aware that the text may contain
some color tags. These tags will not be overriden.
.*--------------------------------------------


.*---------------- ICON_GADGET ----------------------------
:h3 res=30140
width=100%
.ICON_GADGET
:p.
This gadget shows an OS/2 icon.
:p.
:hp7.Styles:ehp7.
:p.
This gadget supports no additional styles.
:p.
:hp7.Additional functions:ehp7.
:p.
:parml tsize=10.
:pt.:hp2.Icon(<path to icon file>):ehp2.
:pd.This sets the path to the icon to be drawn.
:eparml.
:p.
:hp7.Remarks:ehp7.
:p.
Depending on the size of the gadget object the small icon will be drawn or the big one. If the gadget object
is smaller than the normal icon size it will automatically draw the small representation. 
.*--------------------------------------------


.*---------------- IMAGE_GADGET ----------------------------
:h3 res=30150
width=100%
.IMAGE_GADGET
:p.
This gadget shows an image file. It is a container gadget thus may be parent of other gadgets. 
:p.
:hp7.Styles:ehp7.
:p.
The IMAGE_GADGET supports the following additional styles&colon.
:p.
:parml tsize=10.
:pt.:hp2.GSTYLE_BORDER:ehp2.
:pd.Draw a border around the gadget object. This border will have a raised appearance.

:pt.:hp2.GSTYLE_SUNKENBORDER:ehp2.
:pd.Draw a sunken border around the gadget object.

:pt.:hp2.GSTYLE_COLORBORDER:ehp2.
:pd.Draw a border in the color of the background.
:eparml.
:p.
If none of the border styles is set no border will be drawn.
:p.
:hp7.Additional functions:ehp7.
:p.
:parml tsize=10.
:pt.:hp2.Image(<path to image file>):ehp2.
:pd.This sets the path to the image to be drawn.
:eparml.
:p.
:hp7.Remarks:ehp7.
:p.
Any image file supported by MMOS2 may be used.
.*--------------------------------------------


.*--------------------------------------------
:h2 res=30200
width=100%
.Predefined variables
:p.
When a gadget is started a couple of REXX variables are set up by the system.

:p.
:parml tsize=10.
:pt.:hp2.system.icon.width:ehp2.
:pd.The icon width in pixel.
:pt.:hp2.system.icon.height:ehp2.
:pd.The icon height in pixel.

:pt.:hp2.system.path.bootdrive:ehp2.
:pd.The drive the system booted from e.g. :hp2.d&colon.:ehp2..
:pt.:hp2.system.path.wpswizard:ehp2.
:pd.WPS-wizard installation directory.
:pt.:hp2.system.path.wpswizardini:ehp2.
:pd.The WPS-wizard ini file. Gadgets may save their configuration data in this file if they
don't chose to use a private file.
:pt.:hp2.system.path.wpswizardhints:ehp2.
:pd.The hint database of WPS-wizard. This INI file may also contain strings for different languages.

:pt.:hp2.system.physicalmemory:ehp2.
:pd.Installed physical memory.

:pt.:hp2.system.screen.width:ehp2.
:pd.The screen width in pixel.
:pt.:hp2.system.screen.height:ehp2.
:pd.The screen height in pixel.

:eparml.

.*:parml tsize=3.
.*--------------------------------------------


.*--------------------------------------------
:h2 res=30600
width=100%
.Using dynamic gadget names
:p.
If gadgets are created dynamically their name may not be known until execution time
of the script. But as shown before gadget functions must be called using the full
gadget name. To allow function calling on such gadget objects a special function
is provided by WPS-wizard.

:xmp.
  ret= DoForGadget( gadget.function, param1, param2,...)
:exmp.
:p.
The :hp2.gadget.function:ehp2. parameter is the name normally used when calling functions on gadget
objects, for example&colon.
:xmp.
  groupGadget.mygadget.text
:exmp.
:p.
The nice thing is, this name is now a parameter to a function and may be dynamically built
during execution as shown in the following example.

:xmp.

  /* Save position for all gadgets in INI file */
  DO a=1 TO numdrives
     /* The following is the same call as "ret=groupGadget.driveA.position()"
        with 'A' being a number from 1 to numDrives. */
     ret=DoForGadget("groupGadget.drive"||a||".position")
     call SysIni iniFile, APPKEY, DRIVEPOS||a, ret
  END
:exmp.

.*--------------------------------------------


.*--------------------------------------------
:h2 res=30300
width=100%
.Gadget example
:p.
This is an example for a gadget showing the current time and date on the desktop.
:xmp.

/*
 rexx -  Time gadget showing the current data and time on the desktop
         background
*/

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/* Check if the script was directly started */
IF ARG() = 0 THEN DO
  /* Start the gadget */
  PARSE SOURCE . . theScript
  call SysSetObjectdata "<WP_DESKTOP>", "WIZLAUNCHGADGET="theScript	
  Exit(0)
END


NUMERIC DIGITS 15 /* We need this for calculating with hex numbers */

SELECT
WHEN ARG(1)="/GADGETSTARTED" THEN DO
  theObject=ARG(2)

  SIGNAL ON SYNTAX NAME errorHandler
  SIGNAL ON ERROR NAME errorHandler
  SIGNAL ON FAILURE NAME errorHandler

  /* Info for storing data in WPS-wizard ini file */
  INIFile=system.path.wpswizardini
  APPKEY="timegadget"
  TIMEPOS="timepos"
  TIMEFONT="timefont"

  defaultPos=system.screen.width-220||" 100 200 150"

  PARSE VAR defaultPos x y cx cy rest

/* Gadget create info */
  thestem._x=x                 /* x  */
  thestem._y=y                 /* y  */
  thestem._cx=cx               /* cx */
  thestem._cy=cy               /* cy */
  thestem._type=HTML_GADGET    /* Gadget type */
  thestem._hwnd=ARG(3)         /* hwnd */
  thestem._flags=GSTYLE_POPUP  /* We want a popup menu */
  thestem._font="12.Helv"      /* font */

/* Get saved position if any */
  ret=Sysini(INIFile, APPKEY, TIMEPOS)
  IF  ret >< "ERROR:" THEN DO
    PARSE VAR ret thestem._x thestem._y thestem._cx thestem._cy rest
  END
/* Get saved font if any */
  ret=Sysini(INIFile, APPKEY, TIMEFONT)
  IF  ret >< "ERROR:" THEN DO
    thestem._font=ret
  END

/* Create gadget on the desktop */
  rc=WizCreateGadget("DESKTOP", "thestem." , "timeGadget")

/* Set gadget text */
  gadgetText='<p><font color="#00dddddd">'||TIME()||'</font></p><p><font color="#00ff0000">'||DATE()||"</font></p>"
  call timeGadget.text gadgetText

/* Start a timer sending a message every 1000ms  */
  ret=WizStartTimer( ARG(3), 10, 1000)

/* Gadget message loop */
  DO FOREVER
    ret=WIZGETMESSAGE(ARG(3))
    if ret<>'' THEN 
      INTERPRET "call "ret
  END

  EXIT(0) /* We shouldn't end here */
END
OTHERWISE
  /* We shouldn't end here... */
  Exit(0)
END
exit(0) /* We shouldn't end here */


/* The user selected a menu item in the popup menu. This handler must
   always be provided when a gadget object supports popup menus. */
timegadget.onCommand:
/*
	ARG(1): hwnd of client area
	ARG(2): ID
	ARG(3): source (menu or button)
*/
  IF ARG(2)=1 THEN DO
    /* User selected close */

    /* Save current position and font */
    call SysIni iniFile, APPKEY, TIMEPOS,  timeGadget.position()
    call SysIni iniFile, APPKEY, TIMEFONT, timeGadget.font()
    /* Destroy the gadget */
    rc=wizDestroyGadget("timegadget")
    exit(0)
  END
return

/* Called from the message loop when the user requests a popup menu */
timegadget.onPopUp:
/*
  ARG(1): hwnd of client area
  ARG(2): x
  ARG(3): y
*/
  menu.0=1
  menu.1="Close Gadget" /* The id of this item is 1 */

  menu._x=ARG(2)
  menu._y=ARG(3)

  ret=WPSWizCallWinFunc("menuPopupMenu", ARG(1), 'menu.')
return

/* This handler is called periodically when a timer message is received */
onTimer:
  newText='<p><font color="#00dddddd">'||TIME()||'</font></p><p><font color="#00ff0000">'||DATE()||"</font></p>"
  IF newText><gadgetText THEN DO
    gadgetText=newText
    call timeGadget.text gadgetText
  END
return

quit:
exit(0)

errorHandler:
  PARSE SOURCE . . theScript

  ret=WPSWizGadgetFunc("cwDisplayRexxError", "")
  ret=WPSWizGadgetFunc("cwDisplayRexxError", theScript||": ")
  ret=WPSWizGadgetFunc("cwDisplayRexxError", "Error in line "||SIGL)

exit(0)
:exmp.




