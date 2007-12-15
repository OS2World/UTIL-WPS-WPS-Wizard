.**************** menuCheckItem **************************************
:h3 res=2090
width=30%
.menuCheckItem
:link reftype=hd res=2091
auto dependent group=2.
:p.
:link reftype=hd res=2091.Syntax:elink.
.br
:link reftype=hd res=2095.Parameters:elink.
.br
:link reftype=hd res=2093 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=2094 dependent.Remarks:elink.
.br
:link reftype=hd res=2096 dependent.Example:elink.


:h3 res=2091
x=30%
width=70%
hide
group=2
.menuCheckItem - Syntax
:xmp.
/* This function checks a menu item */

HWND   hwndMenu         /* Menu window handle                     */
USHORT usID             /* Item ID                                */
BOOL   fIncludeSubmenus /* Flag specifying if sub menus should be searched  */
BOOL   fCheck           /* Flag specifying action (check/uncheck) */
BOOL   fSuccess         /* Success indicator                      */
:p.
fSuccess=WPSWizCallWinFunc("menuCheckItem", hwndMenu,
                                  hwndSubMenu, sPosition,
                                  usId, chrMenuText);
:exmp.


:h3 res=2095
x=30%
width=70%
hide
group=2
.menuCheckItem - Parameters
:p.
:parml tsize=3.
:pt.:hp2.hwndMenu:ehp2.
:pd.Menu window handle.
:pt.:hp2.usID:ehp2.
:pd.The ID of the item to be checked or unchecked.
:pt.:hp2.fIncludeSubmenus:ehp2.
:pd.If this is 1 submenus are also searched to find the specified item ID.
:pt.:hp2.fCheck:ehp2.
:pd.If set to 1 the item will be checked. If 0 the item will be unchecked.
:pt.:hp2.fSuccess:ehp2.
:pd.Success indicator.
:eparml.


:h3 res=2093
x=30%
width=70% height=35%
group=3
hide
.menuCheckItem - Return value
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


:h3 res=2094
x=30%
width=70%
hide
group=2
.menuCheckItem - Remarks
:p.
None


:h3 res=2096
x=30%
width=70%
hide
group=2
.menuInsertItem - Example
:xmp.
/* Rexx callback example
   In this example a menu item with the ID 22300
   is inserted when the WPS asks for the popup menu.
   When the user selects this item a beep is
   generated. */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

SELECT
WHEN ARG(1)="/FOLDEROPENED" THEN DO
   EXIT(0)
END
WHEN ARG(1)="/FILTERPOPUP" THEN DO
   EXIT (ARG(3))
END
WHEN ARG(1)="/MODIFYPOPUP" THEN DO
   PARSE VALUE ARG(4) WITH hwndMenu rest
   /* Insert a separator in the menu */
   ret=WPSWizCallWinFunc("menuInsertSeparator", hwndMenu, 0, -1)
   /* Insert the new item */
   ret=WPSWizCallWinFunc("menuInsertItem",  hwndMenu, 0, -1, 22300, "Inserted with ~Rexx")
   /* Check the newly inserted item */
   ret=WPSWizCallWinFunc("menuCheckItem", hwndMenu, 22300, 1, 1)
   EXIT (ARG(3))
END
WHEN ARG(1)="/MENUITEMSELECTED" THEN DO
   /* The user selected the inserted item */
   PARSE VALUE ARG(4) WITH hwndFrame ulMenuId
   if ulMenuID=22300 THEN
      /* Make some noise */
      call beep 1000, 500
      /* Return 1 if you handled this menu item */
      EXIT (1)
   END
   EXIT (0)
END
WHEN ARG(1)="/DRAGOVER" THEN DO
   EXIT(0)
   END
OTHERWISE
   return 0
END
:exmp.
