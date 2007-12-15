.*************** winCreateWindow ***************************************
:h3 res=2220
width=30%
.winCreateWindow
:link reftype=hd res=2221
auto dependent group=2.
:p.
:link reftype=hd res=2221.Syntax:elink.
.br
:link reftype=hd res=2225.Parameters:elink.
.br
:link reftype=hd res=2223 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=2204 dependent.Remarks:elink.
.br
:link reftype=hd res=5251 dependent.Example:elink.

:h3 res=2221
x=30%
width=70%
hide
group=2
.winCreateWindow - Syntax
:xmp.
/*********************************************/
/* This function creates a window or control */
/*********************************************/

STEM windowData     /* A REXX stem with create information */
PSZ  pszText        /* Windowtext               */
HWND hwndWindow     /* Handle of created window */
:p.
hwndWindow=WPSWizCallWinFunc("winCreateWindow", 
                    windowData, pszText);
:exmp.

:h3 res=2225
x=30%
width=70% height=100%
group=2
hide
.winQueryWindowPos - Parameters
:p.
:parml tsize=10.
:pt.:hp2.windowData:ehp2.(STEM)
:pd.REXX stem which holds the information necessary to build the window.
:parml tsize=3.
:pt.:hp2.windowData.0:ehp2.
:pd.Count of stem elements.
:pt.:hp2.windowData.1:ehp2.
:pd.Parent window handle. This may be for example the handle of a folder tool bar.
:pt.:hp2.windowData.2:ehp2.
:pd.:link reftype=hd res=2228 dependent.Window class:elink.. One of the predefined PM window classes e.g.
button, entry field or static.
:pt.:hp2.windowData.3:ehp2.
:pd.:link reftype=hd res=2227 dependent.Window flags:elink. e.g. WS_VISIBLE. See PM Programming Reference for more info.
:pt.:hp2.windowData.4:ehp2.
:pd.X position.
:pt.:hp2.windowData.5:ehp2.
:pd.Y position.
:pt.:hp2.windowData.6:ehp2.
:pd.Width.
:pt.:hp2.windowData.7:ehp2.
:pd.Height.
:pt.:hp2.windowData.8:ehp2.
:pd.ID. This ID must be greater than 0.
:pt.:hp2.windowData.9:ehp2.
:pd.Icon to be used as a label for buttons. If this is an empty string the button will use the window text.
The full path to the icon file must be provided.
:p.For window classes other than buttons this must be an empty string.
:pt.:hp2.windowData.10:ehp2.
:pd.Fly over help text for buttons. If this is an empty string no fly over help is available for the button
:p.For window classes other than buttons this must be an empty string.
:eparml.
:pt.:hp2.pszText:ehp2.(string)
:pd.Text to be used for the window. For buttons this is the label etc. 
:pt.:hp2.hwndWindow:ehp2.(numeric)
:pd.Handle to the window. This is the Presentation Manager HWND.
:eparml.

:h3 res=2223
x=30%
width=70% height=35%
group=3
hide
.winCreateWindow - Return value
:p.
:hp2.hwndWindow:ehp2. (numeric)
:p.
Window handle of the newly created window.
:p.
:parml tsize=10.
:pt.0
:pd.Window not created.
:pt.Other
:pd.Window handle.
:eparml.



:h3 res=2224
x=30%
width=70%
hide
group=2
.winCreateWindow - Remarks
:p.
None


:h3 res=2227
x=70%
width=30% height=100%
group=3
hide
.Window flags
:p.
Standard window flags
:p.
WS_VISIBLE                 0x80000000L
.br
WS_DISABLED                0x40000000L
.br
WS_CLIPCHILDREN            0x20000000L
.br
WS_CLIPSIBLINGS            0x10000000L
.br
WS_PARENTCLIP              0x08000000L
.br
WS_SAVEBITS                0x04000000L
.br
WS_SYNCPAINT               0x02000000L
.br
WS_MINIMIZED               0x01000000L
.br
WS_MAXIMIZED               0x00800000L
.br
WS_GROUP                   0x00010000L
.br
WS_TABSTOP                 0x00020000L
.br
WS_MULTISELECT             0x00040000L
:p.
Static control styles
:p.
SS_TEXT                    0x00000001
.br
SS_GROUPBOX                0x00000002
.br
Horizontal positioning of text
.br
DT_LEFT                    0x00000000
.br
DT_CENTER                  0x00000100
.br
DT_RIGHT                   0x00000200
.br
Vertical positioning of text
.br
DT_TOP                     0x00000000
.br
DT_VCENTER                 0x00000400
.br
DT_BOTTOM                  0x00000800
.br
DT_WORDBREAK               0x00004000
.br
:p.
Button control styles
:p.
BS_NOPOINTERFOCUS          0x00000800
.br
BS_NOBORDER                0x00001000
.br
BS_NOCURSORSELECT          0x00002000
.br


:h3 res=2228
x=70%
width=30% height=100%
group=3
hide
.Window class
:p.
WC_FRAME                   0xffff0001
.br
WC_COMBOBOX                0xffff0002
.br
WC_BUTTON                  0xffff0003
.br
WC_STATIC                  0xffff0005
.br
WC_ENTRYFIELD              0xffff0006
.br
WC_LISTBOX                 0xffff0007
.br
WC_MLE                     0xffff000A
.br
WC_SPINBUTTON              0xffff0020
.br





