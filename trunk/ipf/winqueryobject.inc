.**************** winQueryObject **************************************
:h3 res=2060
width=30%
.winQueryObject
:link reftype=hd res=2061
auto dependent group=2.
:p.
:link reftype=hd res=2061.Syntax:elink.
.br
:link reftype=hd res=2063 dependent.Returns:elink.
.br
:link reftype=hd res=2064 dependent.Remarks:elink.
.br
:link reftype=hd res=2065.Parameters:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=2061
x=30%
width=70%
hide
group=2
.winQueryObject - Syntax
:xmp.
/* This function returns the persistent
   handle to an object                   */

nameID   /* ID or fully qualified path to the object */
:p.
hObject=WPSWizCallWinFunc("winQueryObject", 
                         nameID);
:exmp.


:h3 res=2063
x=30%
width=70% height=35%
group=3
hide
.winQueryObject - Return value
:p.
:hp2.hObject:ehp2. numeric
:p.
Object handle. This is NOT a pointer to an object!
:p.
If the object can't be found or awaked 0 is returned.


:h3 res=2064
x=30%
width=70%
hide
group=2
.winQueryObject - Remarks
:p.
A valid object id must start with :hp2.<:ehp2. and end with :hp2.>:ehp2..

:h3 res=2065
x=30%
width=70%
hide
group=2
.winQueryObject - Parameters
:p.
:ul.
:li.:hp2.nameID:ehp2.
.br
An object ID or the fully qualified path.
:eul.

