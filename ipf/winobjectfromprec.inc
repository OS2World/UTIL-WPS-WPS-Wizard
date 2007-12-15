.*********** winObjectFromPRec *****************
:h3 res=2030
width=30%
.winObjectFromPRec
:link reftype=hd res=2031
auto dependent group=2.
:p.
:link reftype=hd res=2031.Syntax:elink.
.br
:link reftype=hd res=2033 dependent.Returns:elink.
.br
:link reftype=hd res=2034 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=2025.Parameters:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=2031
x=30%
width=70%
hide
group=2
.winObjectFromPRec - Syntax
:xmp.
/* This function returns the object pointer
   from a container record. */

pRec             /* Pointer to a WPS container record */
:p.
pObject=WPSWizCallWinFunc("winObjectFromPRec", 
                         pRec)

:exmp.


:h3 res=2033
x=30%
width=70% height=35%
group=3
hide
.winObjectFromPRec - Return value
:p.
:hp2.ptrRec:ehp2. numeric
:p.
Pointer to an object.


:h3 res=2034
x=30%
width=70%
hide
group=2
.winObjectFromPRec - Remarks
:p.
The record pointer must be queried from a WPS container that means a container
created by a WPS object and filled with WPS objects.





