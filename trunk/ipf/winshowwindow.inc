.*************** winShowWindow ***************************************
:h3 res=2280
width=30%
.winShowWindow
:link reftype=hd res=2281
auto dependent group=2.
:p.
:link reftype=hd res=2281.Syntax:elink.
.br
:link reftype=hd res=2285.Parameters:elink.
.br
:link reftype=hd res=2283 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=2204 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:p.
This function was introduced with V0.4.1 of WPS-Wizard.
:h3 res=2281
x=30%
width=70%
hide
group=2
.winShowWindow - Syntax
:xmp.
/**************************************/
/* This function shows/hides a window */
/**************************************/

HWND  hwndWindow      /* Handle of the window  */
BOOL  :link reftype=hd res=2286 dependent.bShow:elink.           /* Show/hide             */
BOOL  :link reftype=hd res=2283 dependent.fSuccess:elink.        /* Success indicator */
:p.
fSuccess=WPSWizCallWinFunc("winShowWindow", 
                            hwndWindow, bShow);
:exmp.

:h3 res=2285
x=30%
width=70%
height=100%
group=3
hide
.winShowWindow - Parameters
:p.
:parml tsize=10.
:pt.:hp2.hwndWindow:ehp2. (numeric)
:pd.Handle to the window. This is the Presentation Manager HWND.
:pt.:hp2.bShow:ehp2. (numeric)
:pd.1 show window, 0 hide window.
:pt.:hp2.fSuccess:ehp2. (numeric)
:pd.Success indicator.
:eparml.

:h3 res=2283
x=30%
width=70% height=35%
group=3
hide
.winShowWindow - Return value
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


:h3 res=2286
x=30%
width=70% height=35%
group=3
hide
.winShowWindow parameter - bShow
:p.
:hp2.bShow:ehp2.(numeric)
.br
Show/hide window.
:parml tsize=10.
:pt.1
:pd.Show window.
:pt.0
:pd.Hide window.
:eparml.











