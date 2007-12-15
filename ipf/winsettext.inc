.*************** winSetText ***************************************
:h3 res=2230
width=30%
.winSetText
:link reftype=hd res=2231
auto dependent group=2.
:p.
:link reftype=hd res=2231.Syntax:elink.
.br
:link reftype=hd res=2235.Parameters:elink.
.br
:link reftype=hd res=2233 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=2204 dependent.Remarks:elink.

.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=2231
x=30%
width=70%
hide
group=2
.winSetText - Syntax
:xmp.
/**************************************/
/* This function sets the window text */
/**************************************/

HWND  hwndWindow      /* Handle of the window  */
PSZ   pszText         /* New window text       */
BOOL  :link reftype=hd res=2233 dependent.fSuccess:elink.        /* Success indicator */
:p.
fSuccess=WPSWizCallWinFunc("winSetText", 
                            hwndWindow, pszText);
:exmp.

:h3 res=2235
x=30%
width=70%
height=100%
group=3
hide
.winSetText - Parameters
:p.
:parml tsize=10.
:pt.:hp2.hwndWindow:ehp2. (numeric)
:pd.Handle to the window. This is the Presentation Manager HWND.
:pt.:hp2.pszText:ehp2. (string)
:pd.New window text.
:pt.:hp2.fSuccess:ehp2. (numeric)
:pd.Success indicator.
:eparml.

:h3 res=2233
x=30%
width=70% height=35%
group=3
hide
.winSetText - Return value
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



