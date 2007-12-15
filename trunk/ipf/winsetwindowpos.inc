.*************** winSetWindowPos ***************************************
:h3 res=2210
width=30%
.winSetWindowPos
:link reftype=hd res=2211
auto dependent group=2.
:p.
:link reftype=hd res=2211.Syntax:elink.
.br
:link reftype=hd res=2215.Parameters:elink.
.br
:link reftype=hd res=2213 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=2204 dependent.Remarks:elink.

.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=2211
x=30%
width=70%
hide
group=2
.winSetWindowPos - Syntax
:xmp.
/* This function sets the window position, size
    and zorder-position of a window */

HWND  windowHandle      /* Handle of the window  */
HWND  hwndInsertBehind  /* Window behind which :hp1.windowHandle:ehp1. is placed */
LONG  x                 /* X position            */
LONG  y                 /* Y position            */
LONG  cx                /* Width                 */
LONG  cy                /* Height                */
ULONG fFlags            /* Window-positioning options */
BOOL  :link reftype=hd res=2213 dependent.fSuccess:elink.    /* Success indicator */
:p.
fSuccess=WPSWizCallWinFunc("winSetWindowPos", 
                            windowHandle , hwndInsertBehind,
                            x, y, cx, cy, fFlags);
:exmp.

:h3 res=2215
x=30%
width=70%
group=3
hide
.winSetWindowPos - Parameters
:p.
:parml tsize=10.
:pt.:hp2.windowHandle:ehp2. (numeric)
:pd.Handle to the window. This is the Presentation Manager HWND.
:pt.:hp2.hwndInsertBehind:ehp2. (numeric)
:pd.Window handle of the window behind which :hp1.windowHandle:ehp1. is placed.
:pt.:hp2.x:ehp2. (numeric)
:pd.New x-position.
:pt.:hp2.y:ehp2. (numeric)
:pd.New y-position.
:pt.:hp2.cx:ehp2. (numeric)
:pd.New width.
:pt.:hp2.cy:ehp2. (numeric)
:pd.New height.
:pt.:hp2.fFlags:ehp2. (numeric)
:pd.Window-positioning options. The following options may be ored together.
:parml tsize=3.
:pt.SWP_SIZE   0x0001
:pd.Change the window size.
:pt.SWP_MOVE   0x0002
:pd.Change the window position.
:pt.SWP_ZORDER 0x0004  
:pd.Change the window z-order.
:eparml.
:pt.:hp2.fSuccess:ehp2. (numeric)
:pd.Success indicator.
:eparml.

:h3 res=2213
x=30%
width=70% height=35%
group=3
hide
.winSetWindowPos - Return value
:p.
:hp2.fSuccess:ehp2.(numeric)
.br
Success indicator.
:parml tsize=10.
:pt.1
:pd.Successful completion
:pt.0
:pd.Error occurred 
:eparml.



:h3 res=2214
x=30%
width=70%
hide
group=2
.winSetWindowPos - Remarks
:p.
Depending on :hp1.fFlags:ehp1. only some of the given parameters are used. Nevertheless it's
necessary to call the function with the whole parameter set. You may set unused parameters to zero.


