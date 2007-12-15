.**************** winQueryFolderObjects **************************************
:h3 res=2070
width=30%
.winQueryFolderObjects
:link reftype=hd res=2071
auto dependent group=2.
:p.
:link reftype=hd res=2071.Syntax:elink.
.br
:link reftype=hd res=2073 dependent.Returns:elink.
.br
:link reftype=hd res=2074 dependent.Remarks:elink.
.br
:link reftype=hd res=2075.Parameters:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=2071
x=30%
width=70%
hide
group=2
.winQueryFolderObjects - Syntax
:xmp.
/* This function returns a record with a certain 
   emphasis from the specified container handle */

frameHandle  /* Handle to the container       */
stem         /* REXX stem variable            */
fEmphasis    /* Emphasis mask or 0            */
className    /* Class used for filtering      */
:link reftype=hd res=2073 dependent.rc:elink.           /* Success indicator             */

rc=WPSWizCallWinFunc("winQueryFolderObjects", 
                     frameHandle, stem,
                     fEmphasis, className);
:exmp.


:h3 res=2073
x=30%
width=70% height=35%
group=3
hide
.winQueryFolderObjects - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
0 in case of error, otherwise <> 0.


:h3 res=2074
x=30%
width=70%
hide
group=2
.winQueryFolderObjects - Remarks
:p.
The frame handle of the folder is given to the skript as a parameter
when started.
:p.
This function can be used to get the objects of an open folder. Using the :hp2.className:ehp2. parameter
it's possible to restrict the output to objects of a particular class.
:p.
Additional information is returned for each found object like title, real name or object ID.
:p.
The function is new with V0.4.0.

:h3 res=2075
x=30%
width=70%
hide
group=2
.winQueryFolderObjects - Parameters
:p.

:parml tsize=10.
:pt.:hp2.frameHandle:ehp2. (numeric)
:pd.Handle to the folder frame window. This is the Presentation Manager HWND.

:pt.:hp2.stem.:ehp2. (REXX stem variable)
:pd.REXX stem variable holding the result. :hp2.stem.0:ehp2. contains the number of found objects. Additional information
is returned for every object. :hp2.idx:ehp2. is the number of the object in the list e.g. stem.3._title. 
:parml tsize=3.
:pt.stem.idx (numeric)
:pd.The WPS object pointer. This pointer can be used to call methods of this object.
:pt.stem.idx._title
:pd.The WPS title of the object. This title is shown on the WPS but isn't necessarily the real name (filename) of the object.

:pt.stem.idx._class
:pd.Class the object belongs to.

:pt.stem.idx._objectid
:pd.ID of the object if any. E.g. <WP_DESKTOP>.

:pt.stem.idx._realname
:pd.The real name of the object as used by the file system. For abstract objects this is an empty string.

:pt.stem.idx._type
:pd.The type of the object. For abstract and folder objects this is an empty string.

:pt.stem.idx._filesystem
:pd.1 if the object is a filesystem object (a folder or file), 0 if the object is an abstract object.
:eparml.

:pt.:hp2.fEmphasis:ehp2. (bitmask)
:pd.Mask to use when searching for objects.
:parml tsize=3.
:pt.ALL            0
:pd.Return all objects.
:pt.CRA_SELECTED   1
:pd.Selected records.
:pt.CRA_CURSORED   4
:pd.Records drawn with a selection cursor.
:pt.CRA_INUSE      8
:pd.Records with in-use emphasis
:pt.CRA_FILTERED   16
:pd.Filtered records that means hidden from view.
:pt.CRA_SOURCE     0x4000
:pd.Records drawn with source emphasis.
:pt.CRA_DISABLED   0x1000
:pd.Disabled records.
:eparml.

:pt.:hp2.className:ehp2. (string)
:pd.Class name used for filtering. Only objects which are an instance of this class or of one of the subclasses are returned.
:p.
For example if className is :hp2.WPFolder:ehp2. only folder objects are returned.
:p.
If this parameter is omitted all objects are returned.
:pt.:hp2.rc:ehp2. (numeric)
:pd.0 in case of error, otherwise <>0.
:eparml.

