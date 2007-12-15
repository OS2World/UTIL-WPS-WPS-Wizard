:h3 res=410
width=30%
.somGetClass
:link reftype=hd res=411
auto dependent group=2.
:p.
:link reftype=hd res=411.Syntax:elink.
.br
:link reftype=hd res=413 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=411
x=30%
width=70%
hide
group=2
.somGetClass - Syntax
:xmp.
/* Get the class object from an object */

theObject         /* Object for which the class is requested */
:p.
classObject=WPSWizCallWPSFunc("somGetClass", theObject);
:exmp.


:h3 res=413
x=30%
width=70% height=35%
group=3
hide
.somGetClass - Return value
:p.
:hp2.classObject:ehp2. numeric
:p.
The return value is numeric. It's a pointer to the class object for the object
:hp2.theObject:ehp2.

.******************************************************

:h3 res=420
width=30%
.somGetClassName
:link reftype=hd res=421
auto dependent group=2.
:p.
:link reftype=hd res=421.Syntax:elink.
.br
:link reftype=hd res=423 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=421
x=30%
width=70%
hide
group=2
.somGetClassName - Syntax
:xmp.
/* Get the class name an object is an
   instance of. */

theObject         /* Object for which the class name is requested */
:p.
className=WPSWizCallWPSFunc("somGetClassName", theObject);
:exmp.


:h3 res=423
x=30%
width=70% height=35%
group=3
hide
.somGetClassName - Return value
:p.
:hp2.className:ehp2. string
:p.
The return value is the name of the class the object 
:hp2.theObject:ehp2. is an instance of.


.******************************************************

:h3 res=430
width=30%
.somGetname
:link reftype=hd res=431
auto dependent group=2.
:p.
:link reftype=hd res=431.Syntax:elink.
.br
:link reftype=hd res=433 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=431
x=30%
width=70%
hide
group=2
.somGetname - Syntax
:xmp.
/* Get the class name from an class pointer */

theClass         /* Class object for which the name is requested */
:p.
className=WPSWizCallWPSFunc("somGetname", theClass);
:exmp.


:h3 res=433
x=30%
width=70% height=35%
group=3
hide
.somGetname - Return value
:p.
:hp2.className:ehp2. string
:p.
The return value is the name of the class 
:hp2.theClass:ehp2..


.******************************************************

:h3 res=440
width=30%
.somGetVersionNumbers
:link reftype=hd res=441
auto dependent group=2.
:p.
:link reftype=hd res=441.Syntax:elink.
.br
:link reftype=hd res=443 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=441
x=30%
width=70%
hide
group=2
.somGetVersionNumbers - Syntax
:xmp.
/* Get the class version from an class pointer */

theClass         /* Class object for which version is requested */
:p.
classVersion=WPSWizCallWPSFunc("somGetVersionNumbers", theClass);
:exmp.


:h3 res=443
x=30%
width=70% height=35%
group=3
hide
.somGetVersionNumbers - Return value
:p.
:hp2.classVersion:ehp2. string
:p.
The return value is the major.minor version of the class for example :hp2.1.3:ehp2..







