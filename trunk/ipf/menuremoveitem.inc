.**************** menuRemoveItem **************************************
:h3 res=2140
width=30%
.menuRemoveItem
:link reftype=hd res=2141
auto dependent group=2.
:p.
:link reftype=hd res=2141.Syntax:elink.
.br
:link reftype=hd res=2145.Parameters:elink.
.br
:link reftype=hd res=2143 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=2144 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=2146 dependent.Example:elink.


:h3 res=2141
x=30%
width=70%
hide
group=2
.menuRemoveItem - Syntax
:xmp.
/* This function removes an item from a menu */

HWND   hwndMenu         /* Menu window handle          */
USHORT usID             /* Item ID                     */
BOOL   fIncludeSubmenus /* Flag specifying if sub menus should be searched  */
SHORT  sItemsLeft       /* Count of remaining items    */
:p.
sItemsLeft=WPSWizCallWinFunc("menuRemoveItem", hwndMenu
                              usId, fIncludeSubmenus);
:exmp.


:h3 res=2145
x=30%
width=70%
hide
group=2
.menuRemoveItem - Parameters
:p.
:parml tsize=3.
:pt.:hp2.hwndMenu:ehp2.
:pd.Menu window handle.
:pt.:hp2.usID:ehp2.
:pd.The ID of the item to be removed.
:pt.:hp2.fIncludeSubmenus:ehp2.
:pd.If this is 1 submenus are also searched to find the specified item ID.
:pt.:hp2.sItemsLeft:ehp2.
:pd.Count of remaining items after removal.
:eparml.


:h3 res=2143
x=30%
width=70% height=35%
group=3
hide
.menuRemoveItem - Return value
:p.
:hp2.sItemsLeft:ehp2.(numeric)
:p.
Count of remaining items after removal.

:h3 res=2144
x=30%
width=70%
hide
group=2
.menuRemoveItem - Remarks
:p.
None




