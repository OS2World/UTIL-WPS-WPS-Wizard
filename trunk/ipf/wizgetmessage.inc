.**************** WizGetMessage **************************************
:h3 res=31130
width=30%
.WizGetMessage
:link reftype=hd res=31131
auto dependent group=2.
:p.
:link reftype=hd res=31131.Syntax:elink.
.br
:link reftype=hd res=31132 dependent.Parameters:elink.
.br
:link reftype=hd res=31133 dependent.Returns:elink.
.br
:link reftype=hd res=31134 dependent.Remarks:elink.
.br
:link reftype=hd res=31136 dependent.Example:elink.

:h3 res=31131
x=30%
width=70%
height=100%
hide
group=2
.WizGetMessage - Syntax
:xmp.
/* This function gets messages from the system. For
   example popup menu selections or gadget activation. */

hwnd       /* Window handle */
:p.
rc=WizGetMessage(hwnd);
:exmp.


:h3 res=31132
x=30%
width=70%
height=100%
hide
group=2
.WizGetMessage - Parameters
:p.
:parml tsize=10.
:pt.:hp2.hwnd:ehp2. (REXX string)
:pd.The window handle given to the gadget as ARG(3) when started.
:eparml.

:h3 res=31133
x=30%
width=70% height=35%
group=3
hide
.WizGetMessage - Return value
:p.
:hp2.rc:ehp2. REXX string
:p.
A string which contains the name of the handler function and parameters. See
the example how to use it.


:h3 res=31134
x=30%
width=70%
height=100%
hide
group=2
.WizGetMessage - Remarks
:p.
Every gadget :hp2.must:ehp2. maintain a message loop. Otherwise the user can't close or move
the gadget and worse the PM may block if the user tries to do so.

:h3 res=31136
x=30%
width=70%
height=100%
hide
group=2
.WizGetMessage - Example
:p.

:xmp.
/* 
   Example of dispatching messages gathered in a gadgets
   message loop.
*/

/* Gadget message loop */
  DO FOREVER
    ret=WIZGETMESSAGE(ARG(3))
    if ret<>'' THEN 
      INTERPRET "call "ret
  END
:exmp.

