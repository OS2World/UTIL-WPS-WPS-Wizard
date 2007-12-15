.**************** menuInsertSeparator **************************************
:h3 res=2110
width=30%
.menuInsertSeparator
:link reftype=hd res=2111
auto dependent group=2.
:p.
:link reftype=hd res=2111.Syntax:elink.
.br
:link reftype=hd res=2115.Parameters:elink.
.br
:link reftype=hd res=2113 dependent.Returns:elink.
.br
:link reftype=hd res=2114 dependent.Remarks:elink.
.br
:link reftype=hd res=2116 dependent.Example:elink.


:h3 res=2111
x=30%
width=70%
hide
group=2
.menuInsertSeparator - Syntax
:xmp.
/* This function inserts a sparator into a menu */

HWND   hwndMenu        /* Menu window handle              */
HWND   hwndSubMenu     /* Submenu window handle           */
SHORT  sPosition       /* Position to insert the new item */
SHORT  sIndexInserted  /* Index of inserted item          */
:p.
sIndexInserted=WPSWizCallWinFunc("menuInsertSeparator", hwndMenu,
                                  hwndSubMenu, sPosition);
:exmp.


:h3 res=2115
x=30%
width=70%
hide
group=2
.menuInsertItem - Parameters
:p.
:parml tsize=3.
:pt.:hp2.hwndMenu:ehp2.
:pd.Menu window handle.
:pt.:hp2.hwndSubMenu:ehp2.
:pd.Submenu window handle.
:p.
This may be 0 if the separator is inserted into a main menu.
:pt.:hp2.sPosition:ehp2.
:pd.Position where the item will be inserted. If the position is MIT_END (-1) the item will be inserted at the end of the menu.
:pt.:hp2.sIndexInserted:ehp2.
:pd.The zero-based position the item is inserted.
:eparml.


:h3 res=2113
x=30%
width=70% height=35%
group=3
hide
.menuInsertSeparator - Return value
:p.
:hp2.sIndexInserted:ehp2.(numeric)
.br
The index of the inserted item.
:parml tsize=10.
:pt.-1 (MIT_MEMERROR)
:pd.No space for new item
:pt.-1 (MIT_ERROR)
:pd.Other error 
:pt.other
:pd.Zero based index of the offset of the item in the menu.
:eparml.


:h3 res=2114
x=30%
width=70%
hide
group=2
.menuInsertSeparator - Remarks
:p.
If the position is MIT_END (-1) the separator will be inserted at the end of the menu.


:h3 res=2116
x=30%
width=70%
hide
group=2
.menuInsertSeparator - Example
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
