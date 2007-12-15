.**************** WizStartTimer **************************************
:h3 res=31160
width=30%
.WizStartTimer
:link reftype=hd res=31161
auto dependent group=2.
:p.
:link reftype=hd res=31161.Syntax:elink.
.br
:link reftype=hd res=31162 dependent.Parameters:elink.
.br
:link reftype=hd res=31163 dependent.Returns:elink.
.br
:link reftype=hd res=31164 dependent.Remarks:elink.
.br
:link reftype=hd res=31166 dependent.Example:elink.

:h3 res=31161
x=30%
width=70%
height=100%
hide
group=2
.WizStartTimer - Syntax
:xmp.
/* This function starts a timer for window hwnd. */

hwnd       /* Window handle  */
id         /* Timer id       */
timeout    /* Timerout in ms */
rc         /* Return code    */
:p.
rc=WizStartTimer(hwnd, id, timeout);
:exmp.


:h3 res=31162
x=30%
width=70%
height=100%
hide
group=2
.WizStartTimer - Parameters
:p.
:parml tsize=10.
:pt.:hp2.hwnd:ehp2. (REXX string with numeric value)
:pd.The window handle given to the gadget as ARG(3) when started.
:pt.:hp2.id:ehp2. (REXX string with numeric value)
:pd.ID of the timer. Timer IDs must be below 0x7fff (32767).
 :pt.:hp2.timeout:ehp2. (REXX string with numeric value)
:pd.The timeout in ms.
:pt.:hp2.rc:ehp2. (REXX string with numeric value)
:pd.0 in case of an error, the timer ID otherwise.
:eparml.

:h3 res=31163
x=30%
width=70% height=35%
group=3
hide
.WizStartTimer - Return value
:p.
:hp2.rc:ehp2. REXX string
:p.
0 in case of an error, the timer ID otherwise.


:h3 res=31164
x=30%
width=70%
height=100%
hide
group=2
.WizStartTimer - Remarks
:p.
Timer IDs must be below 0x7fff (32767). A timer always
belongs to a particular window. The number of timers in the 
system is restricted.

:h3 res=31166
x=30%
width=70%
height=100%
hide
group=2
.WizStartTimer - Example
:p.

:xmp.
/* Start a timer sending a message every 1000ms  */
  ret=WizStartTimer( ARG(3), 1, 1000)


/* Gadget message loop */
  DO FOREVER
    ret=WIZGETMESSAGE(ARG(3))
    if ret<>'' THEN 
      INTERPRET "call "ret
  END
:exmp.




