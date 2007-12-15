.*************** winWindowFromID ***************************************
:h3 res=2250
width=30%
.winWindowFromID
:link reftype=hd res=2251
auto dependent group=2.
:p.
:link reftype=hd res=2251.Syntax:elink.
.br
:link reftype=hd res=2255.Parameters:elink.
.br
:link reftype=hd res=2253 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=2204 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=2251
x=30%
width=70%
hide
group=2
.winWindowFromID - Syntax
:xmp.
/****************************************/
/* This function gets the window handle */
/* of a child window.                   */
/****************************************/

HWND   hwndParent      /* Handle of parent window  */
USHORT wndID           /* Window ID                */
HWND   :link reftype=hd res=2253 dependent.hwndChild:elink.       /* Success indicator        */
:p.
hwndChild=WPSWizCallWinFunc("winWindowFromID", hwndParent,
                            wndID );
:exmp.

:h3 res=2255
x=30%
width=70%
group=3
hide
.winWindowFromID - Parameters
:p.
:parml tsize=10.
:pt.:hp2.hwndParent:ehp2. (numeric) - input
:pd.Handle to the parent window. This is the Presentation Manager HWND.
:pt.:hp2.wndID:ehp2. (numeric) - input
:pd.Window ID to search for.
:pt.:hp2.hwndChild:ehp2. (numeric) - returns
:pd.Window handle of the window with ID :hp1.wndID:ehp1..
:eparml.

:h3 res=2253
x=30%
width=70% height=35%
group=3
hide
.winWindowFromID - Return value
:p.
:hp2.hwndChild:ehp2.(numeric)
:p.
Window handle of the window with ID :hp1.wndID:ehp1.. This is the Presentation Manager HWND.



