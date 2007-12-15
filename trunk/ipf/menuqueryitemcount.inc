.**************** menuQueryItemCount **************************************
:h3 res=2130
width=30%
.menuQueryItemCount
:link reftype=hd res=2131
auto dependent group=2.
:p.
:link reftype=hd res=2131.Syntax:elink.
.br
:link reftype=hd res=2135.Parameters:elink.
.br
:link reftype=hd res=2133 dependent.Returns:elink.
.br
:link reftype=hd res=2134 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=2136 dependent.Example:elink.


:h3 res=2131
x=30%
width=70%
hide
group=2
.menuQueryItemCount - Syntax
:xmp.
/* This function inserts a sparator into a menu */

HWND   hwndMenu        /* Menu window handle              */
SHORT  sNumItems       /* Number of menu items            */
:p.
sNumItems=WPSWizCallWinFunc("menuQueryItemCount",
                                  hwndMenu);
:exmp.


:h3 res=2135
x=30%
width=70%
hide
group=2
.menuQueryItemCount - Parameters
:p.
:parml tsize=3.
:pt.:hp2.hwndMenu:ehp2.
:pd.Menu window handle.
:pt.:hp2.sNumItems:ehp2.
:pd.Number of items in the menu.
:eparml.


:h3 res=2133
x=30%
width=70% height=35%
group=3
hide
.menuQueryItemCount - Return value
:p.
:hp2.sNumItems:ehp2.(numeric)
:p.
Number of items in the menu.


:h3 res=2134
x=30%
width=70%
hide
group=2
.menuQueryItemCount - Remarks
:p.
None
