.*************** winQueryWindowPos ***************************************
:h3 res=2200
width=30%
.winQueryWindowPos
:link reftype=hd res=2201
auto dependent group=2.
:p.
:link reftype=hd res=2201.Syntax:elink.
.br
:link reftype=hd res=2205.Parameters:elink.
.br
:link reftype=hd res=2203 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=2204 dependent.Remarks:elink.

.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=2201
x=30%
width=70%
hide
group=2
.winQueryWindowPos - Syntax
:xmp.
/* This function returns the window position, size
    and zorder-position of a window */

HWND windowHandle   /* Handle of the window  */
PSZ  :link reftype=hd res=2203 dependent.pszPosition:elink.    /* String with position  */
:p.
pszPosition=WPSWizCallWinFunc("winQueryWindowPos", 
                    windowHandle);
:exmp.

:h3 res=2205
x=30%
width=70%
group=3
hide
.winQueryWindowPos - Parameters
:p.
:parml tsize=10.
:pt.:hp2.windowHandle:ehp2.(numeric)
:pd.Handle to the window. This is the Presentation Manager HWND.
:pt.:hp2.pszPosition:ehp2.(string)
:pd.Return string with position information. It has the following format.
:xmp.
x-position y-position width height whandle
:exmp.
Whandle is the window handle of the window behind which windowHandle is placed. 
:eparml.

:h3 res=2203
x=30%
width=70% height=35%
group=3
hide
.winQueryWindowPos - Return value
:p.
:parml tsize=10.
:pt.:hp2.pszPosition:ehp2.(string)
:pd.Return string with position information. It has the following format.
:xmp.
x-position y-position width height whandle
:exmp.
Whandle is the window handle of the window behind which :hp1.windowHandle:ehp1. is placed.
:eparml.



:h3 res=2204
x=30%
width=70%
hide
group=2
.winQueryContainerHWND - Remarks
:p.
The frame handle of the media folder is given to the skript as a parameter
when started.


