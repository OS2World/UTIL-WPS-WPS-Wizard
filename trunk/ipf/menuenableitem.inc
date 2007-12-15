.**************** menuEnableItem **************************************
:h3 res=2290
width=30%
.menuEnableItem
:link reftype=hd res=2291
auto dependent group=2.
:p.
:link reftype=hd res=2291.Syntax:elink.
.br
:link reftype=hd res=2295.Parameters:elink.
.br
:link reftype=hd res=2293 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=2294 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=2296 dependent.Example:elink.


:h3 res=2291
x=30%
width=70%
hide
group=2
.menuEnableItem - Syntax
:xmp.
/* This function enables or disables a menu item */

HWND   hwndMenu         /* Menu window handle                      */
USHORT usID             /* Item ID                                 */
BOOL   fEnable          /* Flag specifying action (enable/disable) */
BOOL   fSuccess         /* Success indicator                       */
:p.
fSuccess=WPSWizCallWinFunc("menuEnableItem", hwndMenu,
                                  usId, fEnable);
:exmp.


:h3 res=2295
x=30%
width=70%
hide
group=2
.menuEnableItem - Parameters
:p.
:parml tsize=3.
:pt.:hp2.hwndMenu:ehp2.
:pd.Menu window handle.
:pt.:hp2.usID:ehp2.
:pd.The ID of the item to be enabled or disabled.
:pt.:hp2.fEnable:ehp2.
:pd.If set to 1 the item will be enabled. If 0 the item will be disabled.
:pt.:hp2.fSuccess:ehp2.
:pd.Success indicator.
:eparml.


:h3 res=2293
x=30%
width=70% height=35%
group=3
hide
.menuEnableItem - Return value
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


:h3 res=2294
x=30%
width=70%
hide
group=2
.menuEnableItem - Remarks
:p.
None



