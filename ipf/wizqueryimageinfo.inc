.**************** WizQueryImageInfo **************************************
:h3 res=31170
width=30%
.WizQueryImageInfo
:link reftype=hd res=31171
auto dependent group=2.
:p.
:link reftype=hd res=31171.Syntax:elink.
.br
:link reftype=hd res=31172 dependent.Parameters:elink.
.br
:link reftype=hd res=31173 dependent.Returns:elink.
.br
:link reftype=hd res=31174 dependent.Remarks:elink.
.br
:link reftype=hd res=31176 dependent.Example:elink.

:h3 res=31171
x=30%
width=70%
height=100%
hide
group=2
.WizQueryImageInfo - Syntax
:xmp.
/* This function queries information about an image file */

fileName    /* Image file name */

:p.
rc=WizQueryImageInfo(fileName);
:exmp.


:h3 res=31172
x=30%
width=70%
height=100%
hide
group=2
.WizQueryImageInfo - Parameters
:p.
:parml tsize=10.
:pt.:hp2.fileName:ehp2. (string)
:pd.Name of the image file.
:eparml.

:h3 res=31173
x=30%
width=70% height=35%
group=3
hide
.WizQueryImageInfo - Return value
:p.
:hp2.rc:ehp2. REXX-string
:p.
:parml tsize=10.
:pt.0
:pd.Error occurred.
:pt.Rexx string
:pd.The string is put together from the following information. :hp2.width:ehp2., :hp2.height:ehp2., :hp2.bitcount.:ehp2.
:eparml.

:h3 res=31174
x=30%
width=70%
height=100%
hide
group=2
.WizQueryImageInfo - Remarks
:p.
In case of an error 0 is returned. This function may be used for any image format which is supported by MMOS2.

:h3 res=31176
x=30%
width=70%
height=100%
hide
group=2
.WizQueryImageInfo - Example
:p.

:xmp.
/* Query some information about an image file */

ret=WizQueryImageInfo("x&colon.\path_to_file")

IF ret=0 THEN SAY "Error occurred."
ELSE
  SAY "Width is: "||WORD(ret, 1)
:exmp.

