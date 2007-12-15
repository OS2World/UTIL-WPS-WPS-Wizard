.**************** menuCheckForItem **************************************
:h3 res=2080
width=30%
.menuCheckForItem
:link reftype=hd res=2081
auto dependent group=2.
:p.
:link reftype=hd res=2081.Syntax:elink.
.br
:link reftype=hd res=2085.Parameters:elink.
.br
:link reftype=hd res=2083 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=2084 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=2086 dependent.Example:elink.


:h3 res=2081
x=30%
width=70%
hide
group=2
.menuCheckForItem - Syntax
:xmp.
/* This function checks if a menu contains the
   specified item */

HWND   hwndMenu         /* Menu window handle          */
USHORT usID             /* Item ID                     */
BOOL   fIncludeSubmenus /* Flag specifying if sub menus should be searched  */
BOOL   fFound           /* Success indicator           */
:p.
fFound=WPSWizCallWinFunc("menuCheckForItem", hwndMenu
                          usId, fIncludeSubmenus);
:exmp.


:h3 res=2085
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
:pd.The ID of the item to search for.
:pt.:hp2.fIncludeSubmenus:ehp2.
:pd.If this is 1 submenus are also searched to find the specified item ID. Set it to 0 otherwise.
:pt.:hp2.fFound:ehp2.
:pd.Success indicator.
:eparml.


:h3 res=2083
x=30%
width=70% height=35%
group=3
hide
.menuCheckForItem - Return value
:p.
:hp2.fFound:ehp2.(numeric)
.br
Success indicator.
:parml tsize=3.
:pt.1
:pd.Successful completion
:pt.0
:pd.Error occurred 
:eparml.


:h3 res=2084
x=30%
width=70%
hide
group=2
.menuRemoveItem - Remarks
:p.
None




