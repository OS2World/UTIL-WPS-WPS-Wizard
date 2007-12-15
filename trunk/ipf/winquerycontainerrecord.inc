.*************** winQueryContainerRecord ******************************
:h3 res=2020
width=30%
.winQueryContainerRecord
:link reftype=hd res=2021
auto dependent group=2.
:p.
:link reftype=hd res=2021.Syntax:elink.
.br
:link reftype=hd res=2023 dependent.Returns:elink.
.br
:link reftype=hd res=2024 dependent.Remarks:elink.
.br
:link reftype=hd res=2025.Parameters:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=2021
x=30%
width=70%
hide
group=2
.winQueryContainerRecord - Syntax
:xmp.
/* This function returns the requested item from
    the specified container handle from */

containerHandle  /* Handle to the container              */
pRec             /* Pointer to previous item or 0        */
cmd              /* Value specifying which item to query */
fsSearch         /* Enumeration order                    */
:p.
ptrRec=WPSWizCallWinFunc("winQueryContainerRecord", 
                         containerHandle, pRec,
                         cmd, fsSearch);
:exmp.


:h3 res=2023
x=30%
width=70% height=35%
group=3
hide
.winQueryContainerRecord - Return value
:p.
:hp2.ptrRec:ehp2. numeric
:p.
Pointer to a container item.


:h3 res=2024
x=30%
width=70%
hide
group=2
.winQueryContainerRecord - Remarks
:p.
The frame handle of the media folder is given to the skript as a parameter
when started.

:h3 res=2025
x=30%
width=70%
hide
group=2
.winQueryContainerRecord - Parameters
:p.
:ul.
:li.:hp2.containerHandle:ehp2.
.br
Handle to a container.
:li.:hp2.pRec:ehp2.
.br
Pointer to previous item or 0. This parameter is ignored if CMA_FIRST or CMA_LAST is specified.
:li.:hp2.cmd:ehp2.
.br
Command value specifying which container item to retrieve.
:sl.
:li.CMA_FIRST  0x0010
.br
Get first container record.
:li.CMA_LAST   0x0020
.br
Get last container record.
:li.CMA_PREV   0x0080
.br
Get previous container record of :hp1.pRec:ehp1..
:li.CMA_NEXT   0x0100
.br
Get next container record after :hp1.pRec:ehp1..
:esl.
:li.:hp2.fsSearch:ehp2.
.br
Enumeration order.
:sl.
:li.CMA_ITEMORDER  1
.br
Query records in item order.
:li.CMA_ZORDER     8
.br
Query records in z-order. The last record is the last to be drawn. This flag
is only valid in icon view.
:esl.

:eul.
