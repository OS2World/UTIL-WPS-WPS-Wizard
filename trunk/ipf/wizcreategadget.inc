.**************** WizCreateGadget **************************************
:h3 res=31140
width=30%
.WizCreateGadget
:link reftype=hd res=31141
auto dependent group=2.
:p.
:link reftype=hd res=31141.Syntax:elink.
.br
:link reftype=hd res=31142 dependent.Parameters:elink.
.br
:link reftype=hd res=31143 dependent.Returns:elink.
.br
:link reftype=hd res=31144 dependent.Remarks:elink.
.br
:link reftype=hd res=31146 dependent.Example:elink.

:h3 res=31141
x=30%
width=70%
height=100%
hide
group=2
.WizCreateGadget - Syntax
:xmp.
/* Create a new gadget. */

parent      /* Parent gadget */
createStem  /* Stem containing the creation information */
name        /* String specifying the name of the gadget */
:p.
rc=WizCreateGadget(parent, createStem, name);
:exmp.


:h3 res=31142
x=30%
width=70%
height=100%
hide
group=2
.WizCreateGadget - Parameters
:p.
:parml tsize=10.
:pt.:hp2.parent:ehp2. (string)
:pd.A string containing the gadgets parent name. If the gadget is living directly on the
desktop (is not owned by another gadget) use the name :hp2.DESKTOP:ehp2..
:pt.:hp2.createStem:ehp2. (stem)
:pd.REXX stem containing the information for the create process.

:parml tsize=3.
:pt.createStem._x
:pd.The x position of the gadget in desktop coordinates.
:pt.createStem._y
:pd.The x position of the gadget in desktop coordinates.
:pt.createStem._cx
:pd.Width of the new gadget.
:pt.createStem._cy
:pd.Height of the new gadget.
:pt.createStem._type
:pd.The type of gadget to be created.
:pt.createStem._hwnd
:pd.Window handle given to the gadget when started.
:pt.createStem._flags
:pd.Flags determining the behaviour and appearance of the gadget.
:pt.createStem._font
:pd.The font to be used in text gadgets. This parameter my be omitted.
:eparml
.
:pt.:hp2.name:ehp2. (string)
:pd.The name of the new gadget. It is used to identify a gadget when messages have to
be dispatched to the appropriate handler. 
:eparml.

:h3 res=31143
x=30%
width=70% height=35%
group=3
hide
.WizCreateGadget - Return value
:p.
:hp2.rc:ehp2. REXX string
:p.
1 if the gadget was successfully created, otherwise 0.


:h3 res=31144
x=30%
width=70%
height=100%
hide
group=2
.WizCreateGadget - Remarks
:p.
A gadget object living on the desktop always has :hp2.DESKTOP:ehp2. as parent. It's
not possible to use every gadget object as a parent. Gadget objects usable as a parent
must be container gadgets. As of V0.5.0 of WPS-Wizard the only container gadget is
the :hp2.GROUP_GADGET:ehp2..
:p.
The :hp2._flags:ehp2. parameter may be used to change the behaviour and appearance
of a gadget. To specify more than one flag add them together as shown in the following
code example.

:xmp.
  stem._x=30                /* x  */
  stem._y=100               /* y  */
  stem._cx=360              /* cx */
  stem._cy=100              /* cy */
  stem._type=HTML_GADGET    /* Gadget type */
  stem._hwnd=ARG(3)         /* hwnd */
  stem._flags=GSTYLE_POPUP + GSTYLE_NORESIZE
  stem._font="10.Helv"      /* font */

:exmp.

The following flags are supported by every gadget.

:parml tsize=10.
:pt.:hp2.GSTYLE_POPUP:ehp2.
:pd.The gadget supports popup menus. When specified the gadget script must contain a popup
handler and also a command handler. If omitted it's impossible to close a gadget.
:pt.:hp2.GSTYLE_NOACTIVATE:ehp2.
:pd.The gadget can't be activated by the user. Use with care because this means the user
can't close, move or size the gadget. 
:pt.:hp2.GSTYLE_NORESIZE:ehp2.
:pd.The gadget can't be resized by the user. There's no way to change this after the gadget
was created.
:eparml.
:p.
Refer to the individual gadgets for information about additional flags.


:h3 res=31146
x=30%
width=70%
height=100%
hide
group=2
.WizCreateGadget - Example
:p.

:xmp.

/* Gadget create info */
  thestem._x=10                /* x  */
  thestem._y=10                /* y  */
  thestem._cx=100              /* cx */
  thestem._cy=200              /* cy */
  thestem._type=HTML_GADGET    /* Gadget type */
  thestem._hwnd=ARG(3)         /* hwnd */
  thestem._flags=GSTYLE_POPUP  /* We want a popup menu */
  thestem._font="12.Helv"      /* font */

/* Create gadget on the desktop */
  rc=WizCreateGadget("DESKTOP", "thestem." , "timeGadget")

:exmp.

