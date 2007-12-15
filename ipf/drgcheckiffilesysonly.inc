.**************** drgCheckIfFileSysOnly **************************************
:h3 res=2160
width=30%
.drgCheckIfFileSysOnly
:link reftype=hd res=2161
auto dependent group=2.
:p.
:link reftype=hd res=2161.Syntax:elink.
.br
:link reftype=hd res=2165.Parameters:elink.
.br
:link reftype=hd res=2163 dependent.Returns:elink.
.br
:link reftype=hd res=2164 dependent.Remarks:elink.
.br
:link reftype=hd res=2166 dependent.Example:elink.


:h3 res=2161
x=30%
width=70%
hide
group=2
.drgCheckIfFileSysOnly - Syntax
:xmp.
/* This function checks if the dragged objects
   are all file system objects. */

PDRAGINFO  pDragInfo     /* Pointer to drag info    */
BOOL       fFileSysOnly  /* Success indicator       */
:p.
fFileSysOnly=WPSWizCallWinFunc("drgCheckIfFileSysOnly",
                                pDragInfo);
:exmp.


:h3 res=2165
x=30%
width=70%
hide
group=2
.drgCheckIfFileSysOnly - Parameters
:p.
:parml tsize=3.
:pt.:hp2.pDragInfo:ehp2.
:pd.Pointer to drag info. This may be obtained from the arguments given to the callback function.
:pt.:hp2.fFileSysOnly:ehp2.
:pd.1 if all objects are file system objects.
:eparml.


:h3 res=2163
x=30%
width=70% height=35%
group=3
hide
.drgCheckIfFileSysOnly - Return value
:p.
:hp2.fFileSysOnly:ehp2.(numeric)
:p.
Success indicator.
:parml tsize=10.
:pt.1
:pd.All objects are filesystem objects.
:pt.0
:pd.At least one of the dragged objects is not a file system object.
:eparml.

:h3 res=2164
x=30%
width=70%
hide
group=2
.drgCheckIfFileSysOnly - Remarks
:p.
Never create a message box during dragging of objects or PM will block.

:h3 res=2166
x=30%
width=70%
hide
group=2
.drgCheckIfFileSysOnly - Example
:xmp.
/* Rexx callback example
   In this example only file system objects are
   allowed for dropping. */

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
    EXIT (ARG(3))
END
WHEN ARG(1)="/MENUITEMSELECTED" THEN DO
    /* Return 1 if you handle the menu item */
    EXIT (0)
END
WHEN ARG(1)="/DRAGOVER" THEN DO
    PARSE VALUE ARG(4) WITH hwndFrame pdraginfo
    /* Check if the objects are file system objects */
    IF WPSWizCallWinFunc("drgCheckIfFileSysOnly", pdraginfo)\=1 THEN
        EXIT(X2D(00200003)) /* Return DOR_NODROP to prevent dropping */

    /* Continue to the parent method */
    EXIT(0)
    END
OTHERWISE
    return 0
END
:exmp.

