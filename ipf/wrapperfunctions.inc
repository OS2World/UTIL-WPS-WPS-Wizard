.************ Wrapper functions ******************************
:h2 res=4900.Wrapper functions
:p.
Methods and functions are called using wrapper functions provided by WPS-Wizard. There're four
of these wrappers.

:table cols='20 35'.
:row.
:c.:link reftype=hd res=310.WPSWizCallWPSFunc:elink.
:c.Call one of the standard WPS methods.
:row.
:c.:link reftype=hd res=340.WPSWizCallWPSClsFunc:elink.
:c.Call one of the standard WPS class methods.
:row.
:c.:link reftype=hd res=320.WPSWizCallFunc:elink.
:c.Call one of the methods introduced by WPS-Wizard.
:row.
:c.:link reftype=hd res=330.WPSWizCallWinFunc:elink.
:c.Call a function dealing with PM.
:etable.
:p.
The script runs in the context of the WPS which is a PM program on a separate thread. You may
use additional REXX dlls available. The name of the REXX skript does not matter but the file
must as usual start with a comment.
:p.


.*********** WPSWizCallWPSFunc **************************

:h3 res=310
width=30%
.WPSWizCallWPSFunc
:link reftype=hd res=311
auto dependent group=2.

:p.
This REXX function is used to call a standard WPS function.
:p.
:link reftype=hd res=311.Syntax:elink.
.*.br
.*:link reftype=hd res=312.Parameters:elink.
.br
:link reftype=hd res=313 dependent.Returns:elink.
.br
:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.br
:link reftype=hd res=316 dependent.Example:elink.


:h3 res=311
x=30%
width=70%
hide
group=2
.WPSWizCallWPSFunc - Syntax
:xmp.
/* Call a WPS method from REXX */

:link reftype=hd res=1311.wpsMethodName:elink.  /* The name of a WPS method */
Object         /* Object on which the method is invoked */
param1         /* First method parameter */
param2         /* Second method parameter */
:p.
rc=WPSWizCallWPSFunc(wpsMethodName, Object, 
                 param1, param2, ...);
:exmp.

:h3 res=1311
x=30% 
width=70% height=35%
group=3
hide
.wpsMethodName
:p.
The name of the method to be invoked. This name is
case sensitive!

:h3 res=313
x=30%
width=70% height=35%
group=3
hide
.WPSWizCallWPSFunc - Return value
:p.
The return value depends on the WPS method being called. It may be a string or a
numeric value. See the method description for more info.
:p.
If the method can't be found the string :hp2.ERROR_METHOD&colon.:ehp2. is returned.

:h3 res=314
x=30%
width=70%
group=2
hide
.WPSWizCallWPSFunc - Remarks
:p.
The method to be called must be a member of the object :hp2.Object:ehp2. An error occurs if
the object does not support the requested method. For example a WPFolder object does
not support the same methods as a WPDataFile object.

:h3 res=316
x=30%
width=70%
group=2
hide
.WPSWizCallWPSFunc - Example
:p.
:xmp.
/* This example queries the size of a file */
/* and displays it in a message box        */

theSize=WPSWizCallWPSFunc("wpQueryFileSize", theObject)

rc=RxMessageBox("The size of the file is: "||theSize)


:exmp.

.*********** WPSWizCallWPSClsFunc **************************

:h3 res=340
width=30%
.WPSWizCallWPSClsFunc
:link reftype=hd res=341
auto dependent group=2.

:p.
This REXX function is used to call a standard WPS class function.
:p.
:link reftype=hd res=341.Syntax:elink.
.*.br
.*:link reftype=hd res=342.Parameters:elink.
.br
:link reftype=hd res=343 dependent.Returns:elink.
.br
:link reftype=hd res=344 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=345.Usage:elink.
.br
:link reftype=hd res=346 dependent.Example:elink.


:h3 res=341
x=30%
width=70%
hide
group=2
.WPSWizCallWPSClsFunc - Syntax
:xmp.
/* Call a WPS class method from REXX */

:link reftype=hd res=1311.wpsMethodName:elink.  /* The name of a WPS method */
classObject    /* Class object on which the method is invoked */
param1         /* First method parameter */
param2         /* Second method parameter */
:p.
rc=WPSWizCallWPSClsFunc(wpsMethodName, classObject, 
                 param1, param2, ...);
:exmp.


:h3 res=343
x=30%
width=70% height=35%
group=3
hide
.WPSWizCallWPSClsFunc - Return value
:p.
The return value depends on the WPS class method being called. It may be a string or a
numeric value. See the method description for more info.
:p.
If the method can't be found the string :hp2.ERROR_METHOD&colon.:ehp2. is returned.

:h3 res=344
x=30%
width=70%
group=2
hide
.WPSWizCallWPSClsFunc - Remarks
:p.
The method to be called must be a member of the object :hp2.classObject:ehp2. An error occurs if
the object does not support the requested method. For example a WPFolder object does
not support the same methods as a WPDataFile object.

:h3 res=346
x=30%
width=70%
group=2
hide
.WPSWizCallWPSClsFunc - Example
:p.
:xmp.
/* Get the object pointer from an persistent
   object handle (HOBJECT)                   */

classObject  /* Class object this method is invoked on     */
hObject      /* Handle to an object. NOT an object pointer */
:p.
theObject=WPSWizCallWPSClsFunc("wpclsQueryObject",
                         classObject, hObject);
:exmp.

.*************** WPSWizCallFunc ********************************

:h3 res=320
width=30%
.WPSWizCallFunc
:link reftype=hd res=321
auto dependent group=2.
:p.
Use this function to call one of the new WPS methods introduced by WPS-Wizard or a
helper method.
:p.
:link reftype=hd res=321 dependent.Syntax:elink.
.*.br
.*:link reftype=hd res=322.Parameters:elink.
.br
:link reftype=hd res=323 dependent.Returns:elink.
.br
:link reftype=hd res=324 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=325.Usage:elink.
.br
:link reftype=hd res=326.Example:elink.

:h3 res=321
x=30%
width=70%
group=2
hide
.WPSWizCallFunc - Syntax
:xmp.
/* Call a method from REXX */

methodName  /* The name of a method */
Object         /* Object on which the method is invoked */
param1         /* First method parameter */
param2         /* Second method parameter */
:p.
rc=WPSWizCallFunc(methodName, Object, 
                 param1, param2, ...);
:exmp.


:h3 res=322
x=30%
width=70%
group=2
hide
.WPSWizCallFunc - Parameters
:p.

:h3 res=323
x=30%
width=70% height=35%
group=3
hide
.WPSWizCallFunc - Return value
:p.
The return value depends on the method being called. It may be a string or a
numeric value. See the method description for more info.
:p.
If the method can't be found the string :hp2.ERROR_METHOD&colon.:ehp2. is returned.

:h3 res=324
x=30%
width=70%
group=2
hide
.WPSWizCallFunc - Remarks
:p.
The method to be called must be a member of the object :hp2.Object:ehp2. An error occurs if
the object does not support the requested method. For example a CWMediaFolder object does
not support the same methods as a CWAudio object.

:h3 res=325
x=30%
width=70%
group=2
hide
.WPSWizCallFunc - Usage
:p.

:h3 res=326
x=30%
width=70%
group=2
hide
.WPSWizCallFunc - Example
:xmp.
/* This example queries the first object in  */
/* a CWMediaFolder, gets the linked file and */
/* queries the name of the artist from the   */
/* ID3 tag.                                  */

/* Get the first object of folder theFolder */
theObject=WPSWizCallWPSFunc("wpQueryContent", theFolder, 0, QC_FIRST)

/* Get the linked file system object from the shadow.
   cwGetFileSystemObject is a member of the
   CWMediaFolder class. */
theFileObj=WPSWizCallFunc("cwGetFileSystemObject", theFolder, theObject)

/* Get the artist name. cwmmQueryTrackInfo is a member of the
   CWAudio class. Every object in the CWMediaFolder is usually
   a shadow to an object instance of a subclass of CWAudio */
theName=WPSWizCallFunc("cwmmQueryTrackInfo", theFileObj, 2)

/* Display the name */
rc=RxMessageBox("The artist name : "||theName)
:exmp.


.********** WPSWizCallWinFunc *********************

:h3 res=330
width=30%
.WPSWizCallWinFunc
:link reftype=hd res=331
auto dependent group=2.
:p.
This REXX function is used to call functions handling PM calls.
:p.
:link reftype=hd res=331.Syntax:elink.
.br
:link reftype=hd res=332.Parameters:elink.
.br
:link reftype=hd res=333.Returns:elink.
.br
.*:link reftype=hd res=334.Remarks:elink.
.*.br
.*:link reftype=hd res=335.Usage:elink.
.*.br
.*:link reftype=hd res=336.Example:elink.


:h3 res=331
x=30%
width=70%
group=2
hide
.WPSWizCallWinFunc - Syntax
:p.
:xmp.
/* Call a PM function from REXX */

winFunctionName  /* The name of PM function   */
param1           /* First function parameter  */
param2           /* Second function parameter */
rc               /* Returned value            */
:p.
rc=WPSWizCallWinFunc(winFunctionName,
                     param1, param2, ...);
:exmp.

:h3 res=332
x=30%
width=70%
group=2
hide
.WPSWizCallWinFunc - Parameters
:p.
:parml tsize=3.
:pt.:hp2.winFunctionName:ehp2.
:pd.The name of the function to be called. This name is case sensitive!
:pt.:hp2.param1:ehp2.
:pd.First function parameter.
:pt.:hp2.param2:ehp2.
:pd.Second function parameter.
:pt.:hp2....:ehp2.
:pd.More parameters
:eparml.

:h3 res=333
x=30%
width=70%
group=2
hide
.WPSWizCallWinFunc - Returns
:p.
The returned value depends on the function which was called. It may be a numeric or boolean value or
a string.

:h3 res=334
x=30%
width=70%
group=2
hide
.WPSWizCallWinFunc - Remarks
:p.
None

:h3 res=335
x=30%
width=70%
group=2
hide
.WPSWizCallWinFunc - Usage
:p.

:h3 res=336
x=30%
width=70%
group=2
hide
.WPSWizCallWinFunc - Example
:p.
