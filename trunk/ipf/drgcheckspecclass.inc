.**************** drgCheckClass **************************************
:h3 res=2190
width=30%
.drgCheckSpecClass
:link reftype=hd res=2191
auto dependent group=2.
:p.
:link reftype=hd res=2191.Syntax:elink.
.br
:link reftype=hd res=2195.Parameters:elink.
.br
:link reftype=hd res=2193 dependent.Returns:elink.
.br
:link reftype=hd res=2194 dependent.Remarks:elink.
.br
:link reftype=hd res=2196 dependent.Example:elink.


:h3 res=2191
x=30%
width=70%
hide
group=2
.drgCheckSpecClass - Syntax
:xmp.
/* This function checks if the dragged objects
   are all instances of a particular class.
   Instances of a subclass are not allowed. */

PDRAGINFO  pDragInfo     /* Pointer to drag info    */
PSZ        pszClassName  /* Class name to check for */
BOOL       fClassOnly    /* Success indicator       */
:p.
fFileSysOnly=WPSWizCallWinFunc("drgCheckSpecClass",
                                pDragInfo, pszClassName);
:exmp.

:h3 res=2195
x=30%
width=70%
hide
group=2
.drgCheckSpecClass - Parameters
:p.
:parml tsize=3.
:pt.:hp2.pDragInfo:ehp2.
:pd.Pointer to drag info. This may be obtained from the arguments given to the callback function.
:pt.:hp2.pszClassName:ehp2.
:pd.The class name to check the objects against.
:pt.:hp2.fClassOnly:ehp2.
:pd.1 if all objects are instances of the specified class.
:eparml.


:h3 res=2193
x=30%
width=70% height=35%
group=3
hide
.drgCheckSpecClass - Return value
:p.
:hp2.fClassOnly:ehp2.(numeric)
:p.
Success indicator.
:parml tsize=10.
:pt.1
:pd.All objects are instances of the particular class.
:pt.0
:pd.At least one of the dragged objects is an instance of another class or of a subclass.
:eparml.

:h3 res=2194
x=30%
width=70%
hide
group=2
.drgCheckSpecClass - Remarks
:p.
Never create a message box during dragging of objects or PM will block.

:h3 res=2196
x=30%
width=70%
hide
group=2
.drgCheckSpecClass - Example
:xmp.
/* Rexx callback example
   In this example only MMJPG objects are
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
    /* Check if the objects are instances of the class MMJPG */
    IF WPSWizCallWinFunc("drgCheckSpecClass", pdraginfo, "MMJPG")\=1 THEN
        EXIT(X2D(00200003)) /* Return DOR_NODROP to prevent dropping */

    /* Continue to the parent method */
    EXIT(0)
    END
OTHERWISE
    return 0
END
:exmp.

