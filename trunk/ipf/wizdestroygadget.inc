.**************** WizDestroyGadget **************************************
:h3 res=31120
width=30%
.WizDestroyGadget
:link reftype=hd res=31121
auto dependent group=2.
:p.
:link reftype=hd res=31121.Syntax:elink.
.br
:link reftype=hd res=31122 dependent.Parameters:elink.
.br
:link reftype=hd res=31123 dependent.Returns:elink.
.br
:link reftype=hd res=31124 dependent.Remarks:elink.
.br
:link reftype=hd res=31126 dependent.Example:elink.

:h3 res=31121
x=30%
width=70%
height=100%
hide
group=2
.WizDestroyGadget - Syntax
:xmp.
/* This function destroys a previously created gadget */

theGadget       /* Gadget name */
:p.
rc=WizDestroyGadget(theGadget);
:exmp.


:h3 res=31122
x=30%
width=70%
height=100%
hide
group=2
.WizDestroyGadget - Parameters
:p.
:parml tsize=10.
:pt.:hp2.theGadget:ehp2. (string)
:pd.The name of the gadget to be destroyed.
:eparml.

:h3 res=31123
x=30%
width=70% height=35%
group=3
hide
.WizDestroyGadget - Return value
:p.
:hp2.rc:ehp2. REXX-string
:p.
1 if successfull, 0 otherwise.


:h3 res=31124
x=30%
width=70%
height=100%
hide
group=2
.WizDestroyGadget - Remarks
:p.
It is an error to destroy a gadget which doesn't exist.

:h3 res=31126
x=30%
width=70%
height=100%
hide
group=2
.WizDestroyGadget - Example
:p.

:xmp.
/* Destroy a gadget. This code is an example for a command
   handler. The handler is called when the user selects a
   manu item of the popup menu of gadget "timeGadget". */

timegadget.onCommand:
/*
  ARG(1): hwnd of client area
  ARG(2): ID
  ARG(3): source (menu or button)
*/
  PARSE SOURCE . . theScript
  IF ARG(2)=1 THEN DO
    call SysIni iniFile, APPKEY, TIMEPOS,  timeGadget.position()
    call SysIni iniFile, APPKEY, TIMEFONT, timeGadget.font()
    rc=wizDestroyGadget("timegadget")
    exit(0)
  END
return

:exmp.

