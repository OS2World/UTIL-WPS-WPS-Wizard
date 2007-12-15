.**************** drgQueryNumObjects **************************************
:h3 res=2170
width=30%
.drgQueryNumObjects
:link reftype=hd res=2171
auto dependent group=2.
:p.
:link reftype=hd res=2171.Syntax:elink.
.br
:link reftype=hd res=2175.Parameters:elink.
.br
:link reftype=hd res=2173 dependent.Returns:elink.
.br
:link reftype=hd res=2174 dependent.Remarks:elink.
.br
:link reftype=hd res=2176 dependent.Example:elink.


:h3 res=2171
x=30%
width=70%
hide
group=2
.drgQueryNumObjects - Syntax
:xmp.
/* This function checks if the dragged objects
   are all file system objects. */

PDRAGINFO  pDragInfo    /* Pointer to drag info     */
SHORT      sNumObjects  /* Count of objects dragged */
:p.
sNumObjects=WPSWizCallWinFunc("drgQueryNumObjects",
                                pDragInfo);
:exmp.


:h3 res=2175
x=30%
width=70%
hide
group=2
.drgQueryNumObjects - Parameters
:p.
:parml tsize=3.
:pt.:hp2.pDragInfo:ehp2.
:pd.Pointer to drag info. This may be obtained from the arguments given to the callback function.
:pt.:hp2.sNumObjects:ehp2.
:pd.Count of objects dragged.
:eparml.


:h3 res=2173
x=30%
width=70% height=35%
group=3
hide
.drgQueryNumObjects - Return value
:p.
:hp2.sNumObjects:ehp2.(numeric)
:p.
Count of objects dragged.

:h3 res=2174
x=30%
width=70%
hide
group=2
.drgQueryNumObjects - Remarks
:p.
Never create a message box during dragging of objects or PM will block.

:h3 res=2176
x=30%
width=70%
hide
group=2
.drgQueryNumObjects - Example
:xmp.
/* Rexx callback example

   In this example only a single 
   object may be dropped at a time. */

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

    /* Check if only one object is being dragged */
    IF WPSWizCallWinFunc("drgQueryNumObjects", pdraginfo)\=1 THEN
        EXIT(X2D(00200003)) /* Return DOR_NODROP to prevent dropping */

    /* Continue to the parent method */
    EXIT(0)
    END
OTHERWISE
    return 0
END
:exmp.

