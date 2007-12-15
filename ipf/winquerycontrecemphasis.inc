.**************** winQueryContRecEmphasis **************************************
:h3 res=2040
width=30%
.winQueryContRecEmphasis
:link reftype=hd res=2041
auto dependent group=2.
:p.
:link reftype=hd res=2041.Syntax:elink.
.br
:link reftype=hd res=2043 dependent.Returns:elink.
.br
:link reftype=hd res=2044 dependent.Remarks:elink.
.br
:link reftype=hd res=2045.Parameters:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=2041
x=30%
width=70%
hide
group=2
.winQueryContRecEmphasis - Syntax
:xmp.
/* This function returns a record with a certain 
   emphasis from the specified container handle */

containerHandle  /* Handle to the container       */
pRec             /* Pointer to previous item or 0 */
fEmphasis        /* Emphasis mask                 */
:p.
ptrRec=WPSWizCallWinFunc("winQueryContRecEmphasis", 
                         containerHandle, pRec,
                         fEmphasis);
:exmp.


:h3 res=2043
x=30%
width=70% height=35%
group=3
hide
.winQueryContRecEmphasis - Return value
:p.
:hp2.ptrRec:ehp2. numeric
:p.
Pointer to a container record or 0. In case of an error -1 is returned.


:h3 res=2044
x=30%
width=70%
hide
group=2
.winQueryContRecEmphasis - Remarks
:p.
The frame handle of the media folder is given to the skript as a parameter
when started.

:h3 res=2045
x=30%
width=70%
hide
group=2
.winQueryContRecEmphasis - Parameters
:p.
:ul.
:li.:hp2.containerHandle:ehp2.
.br
Handle to a container.
:li.:hp2.pRec:ehp2.
.br
Pointer to previous item or CMA_FIRST (0x0010).
:li.:hp2.fEmphasis:ehp2.
.br
Enumeration order.
:sl.
:li.CRA_SELECTED   1
.br
Selected records.
:li.CRA_CURSORED   4
.br
Records drawn with a selection cursor.
:li.CRA_INUSE      8
.br
Records with in-use emphasis
:li.CRA_FILTERED   16
.br
Filtered records that means hidden from view.
:li.CRA_SOURCE     0x4000
.br
Records drawn with source emphasis.
:li.CRA_DISABLED   0x1000
.br
Disabled records.
:esl.
:eul.
