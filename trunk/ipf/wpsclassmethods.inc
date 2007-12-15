:h2 res=4000
width=30%
.Supported WPS class methods
:p.
The following WPS class methods are currently supported.
:p.
.br
:link reftype=hd res=4010 dependent.wpclsQueryObject:elink.



.************** wpclsQueryObject **************************

:h3 res=4010
width=30%
.wpclsQueryObject
:link reftype=hd res=4011
auto dependent group=2.
:p.
:link reftype=hd res=4011.Syntax:elink.
.br
:link reftype=hd res=4013 dependent.Returns:elink.
.br
:link reftype=hd res=4014 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=4011
x=30%
width=70%
hide
group=2
.wpclsQueryObject - Syntax
:xmp.
/* Get the object pointer from a persistent
   object handle (HOBJECT)                   */

classObject  /* Class object this method is invoked on     */
hObject      /* Handle to an object. NOT an object pointer */
:p.
theObject=WPSWizCallWPSClsFunc("wpclsQueryObject",
                         classObject, hObject);
:exmp.

:h3 res=4013
x=30%
width=70% height=35%
group=3
hide
.wpclsQueryObject - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is 0 if the object does not exist.

:h3 res=4014
x=30%
width=70%
hide
group=2
.wpclsQueryObject - Remarks
:p.
The returned object is locked by this call. It is necessary to unlock
it after use so it can be made dormant by the system.