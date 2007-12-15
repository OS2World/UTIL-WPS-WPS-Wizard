.**************** WizQueryDrives **************************************
:h3 res=31150
width=30%
.WizQueryDrives
:link reftype=hd res=31151
auto dependent group=2.
:p.
:link reftype=hd res=31151.Syntax:elink.
.br
:link reftype=hd res=31152 dependent.Parameters:elink.
.br
:link reftype=hd res=31153 dependent.Returns:elink.
.br
:link reftype=hd res=31154 dependent.Remarks:elink.
.br
:link reftype=hd res=31156 dependent.Example:elink.

:h3 res=31151
x=30%
width=70%
height=100%
hide
group=2
.WizQueryDrives - Syntax
:xmp.
/* This method gets the fixed, removable or
   remote drives in the system. */

start       /* Drive to begin the query */
which       /* String specifying which kind of drive should be queried */
:p.
rc=WizQueryDrives([start] [, which]);
:exmp.


:h3 res=31152
x=30%
width=70%
height=100%
hide
group=2
.WizQueryDrives - Parameters
:p.
:parml tsize=10.
:pt.:hp2.start:ehp2. (string)
:pd.A string specifying the drive to start the query. If omitted drive :hp2.c&colon.:ehp2.
is used.
:pt.:hp2.which:ehp2. (string)
:pd.A string specifying which kind of drives should be queried. If it is omitted
all drives are returned.
:parml tsize=3.
:pt."fixed"
:pd.Query only fixed disks.
:pt."remote"
:pd.Query remote drives.
:pt."removable"
:pd.Query removable drives.
:eparml.
:eparml.

:h3 res=31153
x=30%
width=70% height=35%
group=3
hide
.WizQueryDrives - Return value
:p.
:hp2.rc:ehp2. REXX-string
:p.
String containing all found drives. For example:

:xmp.
e: f: g: h: i: m: n:
:exmp.


:h3 res=31154
x=30%
width=70%
height=100%
hide
group=2
.WizQueryDrive - Remarks
:p.
If all drives or removable drives are queried and no disk is inserted into
a drive (e.g. CDROM drive) the system will show the eror popup window.

:h3 res=31156
x=30%
width=70%
height=100%
hide
group=2
.WizQueryDrives - Example
:p.

:xmp.

/* Query the fixed disks in the system. */

[...]
SELECT
WHEN ARG(1)="/GADGETSTARTED" THEN DO
  theObject=ARG(2)

  /* Get number of fixed drives starting with c: */
  allDrives=WizQueryDrives("c:", "fixed")
  numDrives=WORDS(allDrives)

[...]
:exmp.

