.**************** WizQueryFreeMem **************************************
:h3 res=31100
width=30%
.WizQueryFreeMem
:link reftype=hd res=31101
auto dependent group=2.
:p.
:link reftype=hd res=31101.Syntax:elink.
.br
:link reftype=hd res=31102 dependent.Parameters:elink.
.br
:link reftype=hd res=31103 dependent.Returns:elink.
.br
:link reftype=hd res=31104 dependent.Remarks:elink.
.br
:link reftype=hd res=31106 dependent.Example:elink.

:h3 res=31101
x=30%
width=70%
height=100%
hide
group=2
.WizQueryFreeMem - Syntax
:xmp.
/* This method gets the amount of free physical memory
    or free virtual memory in the system  */

which       /* String specifying if physical or virtual memory should be queried */
:p.
rc=WizQueryFreeMem(which);
:exmp.


:h3 res=31102
x=30%
width=70%
height=100%
hide
group=2
.WizQueryFreeMem - Parameters
:p.
:parml tsize=10.
:pt.:hp2.which:ehp2. (string)
:pd.A string specifying which kind of memory should be queried.
:parml tsize=3.
:pt."physical"
:pd.Query the free physical memory.
:pt."virtual"
:pd.Query the free virtual memory.

:eparml.
:eparml.

:h3 res=31103
x=30%
width=70% height=35%
group=3
hide
.WizQueryFreeMem - Return value
:p.
:hp2.rc:ehp2. REXX-string
:p.
The amount of free physical or free virtual memory in the system.


:h3 res=31104
x=30%
width=70%
height=100%
hide
group=2
.WizQueryFreeMem - Remarks
:p.
The amount of free memory changes constantly depending
on system conditions.

:h3 res=31106
x=30%
width=70%
height=100%
hide
group=2
.WizQueryFreeMem - Example
:p.

:xmp.
/* Query the free physical memory */

ret=WizQueryFreeMem("physical")
:exmp.

