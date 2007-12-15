.**************** WizStopTimer **************************************
:h3 res=31180
width=30%
.WizStopTimer
:link reftype=hd res=31181
auto dependent group=2.
:p.
:link reftype=hd res=31181.Syntax:elink.
.br
:link reftype=hd res=31182 dependent.Parameters:elink.
.br
:link reftype=hd res=31183 dependent.Returns:elink.
.br
:link reftype=hd res=31184 dependent.Remarks:elink.
.br
:link reftype=hd res=31186 dependent.Example:elink.

:h3 res=31181
x=30%
width=70%
height=100%
hide
group=2
.WizStopTimer - Syntax
:xmp.
/* This function stops a timer for window hwnd. */

hwnd       /* Window handle  */
id         /* Timer id       */
rc         /* Return code    */
:p.
rc=WizStopTimer(hwnd, id);
:exmp.


:h3 res=31182
x=30%
width=70%
height=100%
hide
group=2
.WizStopTimer - Parameters
:p.
:parml tsize=10.
:pt.:hp2.hwnd:ehp2. (REXX string with numeric value)
:pd.The window handle given to the gadget as ARG(3) when started.
:pt.:hp2.id:ehp2. (REXX string with numeric value)
:pd.ID of the timer. Timer IDs must be below 0x7fff (32767).
:pt.:hp2.rc:ehp2. (REXX string with numeric value)
:pd.0 in case of an error, 1 otherwise. Check for the error value. An error may for example occur if the timer ID is not valid.
:eparml.

:h3 res=31183
x=30%
width=70% height=35%
group=3
hide
.WizStopTimer - Return value
:p.
:hp2.rc:ehp2. REXX string
:p.
0 in case of an error. Check for the error value. An error may for example occur if the timer ID is not valid.


:h3 res=31184
x=30%
width=70%
height=100%
hide
group=2
.WizStopTimer - Remarks
:p.
Timer IDs must be below 0x7fff (32767). A timer always
belongs to a particular window. The number of timers in the 
system is restricted.

:h3 res=31186
x=30%
width=70%
height=100%
hide
group=2
.WizStopTimer - Example
:p.

:xmp.

  gloabalHWND=ARG(3)

/* Start a timer sending a message every 1000ms  */
  ret=WizStartTimer( ARG(3), 1, 1000)


/* Gadget message loop */
  DO FOREVER
    ret=WIZGETMESSAGE(ARG(3))
    if ret<>'' THEN 
      INTERPRET "call "ret
  END

 [...]

/* Stop a running timer */
  ret=WizStopTimer(globalHWND, 1)

:exmp.




