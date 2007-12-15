.****************** wpClose ************************************

:h3 res=450
width=30%
.wpClose
:link reftype=hd res=451
auto dependent group=2.
:p.
:link reftype=hd res=451.Syntax:elink.
.br
:link reftype=hd res=453 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=451
x=30%
width=70%
hide
group=2
.wpClose - Syntax
:xmp.
/* Close all open views of an object */

theObject         /* WPS-object  */
:p.
rc=WPSWizCallWPSFunc("wpClose", theObject);
:exmp.


:h3 res=453
x=30%
width=70% height=35%
group=3
hide
.wpClose - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is 0 if an error occured.

.************** wpCnrRefreshDetails **************************

:h3 res=820
width=30%
.wpCnrRefreshDetails
:link reftype=hd res=821
auto dependent group=2.
:p.
:link reftype=hd res=821.Syntax:elink.
.br
:link reftype=hd res=823 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=824 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=821
x=30%
width=70%
hide
group=2
.wpCnrRefreshDetails - Syntax
:xmp.
/* This method causes the details data of an
   object to be refreshed in all open views. */

theObject         /* WPS-object this method is invoked on */
:p.
newObject=WPSWizCallWPSFunc("wpCnrRefreshDetails", 
                            theObject);
:exmp.

:h3 res=823
x=30%
width=70% height=35%
group=3
hide
.wpCnrRefreshDetails - Return value
:p.
:hp2.rc:ehp2. numeric
There's no return value for this method.

.************** wpCnrSetEmphasis **************************

:h3 res=810
width=30%
.wpCnrSetEmphasis
:link reftype=hd res=811
auto dependent group=2.
:p.
:link reftype=hd res=811.Syntax:elink.
.br
:link reftype=hd res=813 dependent.Returns:elink.
.br
:link reftype=hd res=814 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=811
x=30%
width=70%
hide
group=2
.wpCnrSetEmphasis - Syntax
:xmp.
/* Set the emphasis of an object in all
   containers it's inserted. */

theObject         /* WPS-object this method is invoked on */
emphAttr          /* Emphasis attribute to be changed     */
fTurnOn           /* Set (1) or unset (0) the attribute   */
:p.
rc=WPSWizCallWPSFunc("wpCnrSetEmphasis", theObject,
                            emphAttr, fTurnOn);
:exmp.

:h3 res=813
x=30%
width=70% height=35%
group=3
hide
.wpCnrSetEmphasis - Return value
:p.
:hp2.rc:ehp2. numeric
The return value is 0 in case of an error.

:h3 res=814
x=30%
width=70%
hide
group=2
.wpCnrSetEmphasis - Remarks
:p.
:hp2.emphAttr:ehp2. is a combination of the following attributes.

:ul.
:li.CRA_SELECTED   0x00000001
.br
Selected records.
:li.CRA_CURSORED   0x00000004
.br
Records drawn with a selection cursor.
:li.CRA_INUSE      0x00000008
.br
Records with in-use emphasis
:li.CRA_FILTERED   0x00000010
.br
Filtered records that means hidden from view.
:li.CRA_SOURCE     0x4000
.br
Records drawn with source emphasis.
:li.CRA_DISABLED   0x1000
.br
Disabled records.
:eul.


.************** wpCopyObject **************************

:h3 res=780
width=30%
.wpCopyObject
:link reftype=hd res=781
auto dependent group=2.
:p.
:link reftype=hd res=781.Syntax:elink.
.br
:link reftype=hd res=783 dependent.Returns:elink.
.br
:link reftype=hd res=784 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=781
x=30%
width=70%
hide
group=2
.wpCopyObject - Syntax
:xmp.
/* Copy an object to a new folder */

wpObject         /* WPS-object this method is invoked on */
wpFolder         /* The target folder */
fLock            /* Flag specifying if the object should remain locked */
:p.
newObject=WPSWizCallWPSFunc("wpCopyObject", wpObject, wpFolder, fLock);
:exmp.

:h3 res=783
x=30%
width=70% height=35%
group=3
hide
.wpCopyObject - Return value
:p.
:hp2.newObject:ehp2. numeric
The return value is 0 in case of an error. Otherwise it's a
pointer to the new object.

:h3 res=784
x=30%
width=70%
hide
group=2
.wpCopyObject - Remarks
:p.
Every object maintains a lock counter to prevent the system from making it
dormant. If the lock counter isn't zero an object stays awake even if
the folder it resides in is closed. You may call wpUnlockObject on the
object to unlock it.

.************** wpCreateFromTemplate **************************

:h3 res=840
width=30%
.wpCreateFromTemplate
:link reftype=hd res=841
auto dependent group=2.
:p.
:link reftype=hd res=841.Syntax:elink.
.br
:link reftype=hd res=843 dependent.Returns:elink.
.br
:link reftype=hd res=844 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=841
x=30%
width=70%
hide
group=2
.wpCreateFromTemplate - Syntax
:xmp.
/* Move an object to a new folder */

theObject         /* WPS-object this method is invoked on */
theFolder         /* The target folder */
fLock             /* Flag specifying if the object should remain locked */
:p.
newObject=WPSWizCallWPSFunc("wpCreateFromTemplate", theObject, theFolder, bLock);
:exmp.

:h3 res=843
x=30%
width=70% height=35%
group=3
hide
.wpCreateFromTemplate - Return value
:p.
:hp2.newObject:ehp2. numeric
The return value is 0 in case of an error. Otherwise it's a
pointer to the new object.

:h3 res=844
x=30%
width=70%
hide
group=2
.wpCreateFromTemplate - Remarks
:p.
The new object is a copy of the template object with the style
OBJSTYLE_TEMPLATE cleared.
:p.
Every object maintains a lock counter to prevent the system from making it
dormant. If the lock counter isn't zero an object stays awake even if
the folder it resides in is closed. You may call wpUnlockObject on the
object to unlock it if fLock was set.


.************** wpCreateShadowObject **************************

:h3 res=790
width=30%
.wpCreateShadowObject
:link reftype=hd res=791
auto dependent group=2.
:p.
:link reftype=hd res=791.Syntax:elink.
.br
:link reftype=hd res=793 dependent.Returns:elink.
.br
:link reftype=hd res=794 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=791
x=30%
width=70%
hide
group=2
.wpCreateShadowObject - Syntax
:xmp.
/* Create a shadow of an object */

theObject         /* WPS-object this method is invoked on */
theFolder         /* The target folder */
fLock             /* Flag specifying if the object should remain locked */
:p.
newObject=WPSWizCallWPSFunc("wpCreateShadowObject", theObject, theFolder, bLock);
:exmp.

:h3 res=793
x=30%
width=70% height=35%
group=3
hide
.wpCreateShadowObject - Return value
:p.
:hp2.newObject:ehp2. numeric
The return value is 0 in case of an error. Otherwise it's a
pointer to the new object.

:h3 res=794
x=30%
width=70%
hide
group=2
.wpCreateShadowObject - Remarks
:p.
Every object maintains a lock counter to prevent the system from making it
dormant. If the lock counter isn't zero an object stays awake even if
the folder it resides in is closed and the object itself is closed.
You may call wpUnlockObject on the object to unlock it.

.****************** wpDelete ************************************

:h3 res=740
width=30%
.wpDelete
:link reftype=hd res=741
auto dependent group=2.
:p.
:link reftype=hd res=741.Syntax:elink.
.br
:link reftype=hd res=743 dependent.Returns:elink.
.br
:link reftype=hd res=744 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=741
x=30%
width=70%
hide
group=2
.wpDelete - Syntax
:xmp.
/* Delete an object with or without
   confirmation                     */

theObject         /* WPS-object  */
flConfirmation    /* Flag specifying the confirmation action */ 
:p.
rc=WPSWizCallWPSFunc("wpDelete",
                    theObject, flConfirmation);
:exmp.


:h3 res=743
x=30%
width=70% height=35%
group=3
hide
.wpDelete - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is 0 in case of an error.

:h3 res=744
x=30%
width=70%
hide
group=2
.wpDelete - Remarks
:p.
The parameter flConfirmation may be one of the following.

:ul.
:li.0
.br
Don't prompt for confirmation.
:li.CONFIRM_DELETE:        1
.br
Prompt for confirmation for all objects.
:li.CONFIRM_DELETEFOLDER   2
.br
Prompt for confirmation for folders only.
:eul.


.****************** wpDeleteContents ************************************

:h3 res=750
width=30%
.wpDeleteContents
:link reftype=hd res=751
auto dependent group=2.
:p.
:link reftype=hd res=751.Syntax:elink.
.br
:link reftype=hd res=753 dependent.Returns:elink.
.br
:link reftype=hd res=754 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=751
x=30%
width=70%
hide
group=2
.wpDeleteContents - Syntax
:xmp.
/* Delete the contents of a folder
   with or without confirmation     */

theFolder         /* Folder object  */
flConfirmation    /* Flag specifying the confirmation action */ 
:p.
rc=WPSWizCallWPSFunc("wpDeleteContents", 
                     theFolder, flConfirmation);
:exmp.


:h3 res=753
x=30%
width=70% height=35%
group=3
hide
.wpDeleteContents - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is one of the following.

:ul.
:li.OK_DELETE:     1
.br
All objects were deleted.
:li.NO_DELETE:     2
.br
At least one object wasn't deleted due to an error.
:li.CANCEL_DELETE: 3
USer cancelled the operation.
:eul.

:h3 res=754
x=30%
width=70%
hide
group=2
.wpDeleteContents - Remarks
:p.
The parameter flConfirmation may be one of the following.

:ul.
:li.0
.br
Don't prompt for confirmation.
:li.CONFIRM_DELETE:        1
.br
Prompt for confirmation for all objects.
:li.CONFIRM_DELETEFOLDER   2
.br
Prompt for confirmation for folders only.
:eul.


.****************** wpHide ************************************

:h3 res=460
width=30%
.wpHide
:link reftype=hd res=461
auto dependent group=2.
:p.
:link reftype=hd res=461.Syntax:elink.
.br
:link reftype=hd res=463 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=461
x=30%
width=70%
hide
group=2
.wpHide - Syntax
:xmp.
/* Hide all open views of an object */

theObject         /* WPS-object  */
:p.
rc=WPSWizCallWPSFunc("wpHide", theObject);
:exmp.


:h3 res=463
x=30%
width=70% height=35%
group=3
hide
.wpHide - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is 0 if an error occured.

.****************** wpIsCurrentDesktop ************************************

:h3 res=800
width=30%
.wpIsCurrentDesktop
:link reftype=hd res=801
auto dependent group=2.
:p.
:link reftype=hd res=801.Syntax:elink.
.br
:link reftype=hd res=803 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=804 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=801
x=30%
width=70%
hide
group=2
.wpIsCurrentDesktop - Syntax
:xmp.
/* Query if this desktop folder is
   the current desktop              */


theDesktopFolder         /* Pointer to folder object  */
 
:p.
rc=WPSWizCallWPSFunc("wpIsCurrentDesktop",
                    theDesktopFolder);
:exmp.


:h3 res=803
x=30%
width=70% height=35%
group=3
hide
.wpIsCurrentDesktop - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is 1 if the folder is the current desktop
otherwise 0.

.***************** wpIsDeleteable *************************************

:h3 res=470
width=30%
.wpIsDeleteable
:link reftype=hd res=471
auto dependent group=2.
:p.
:link reftype=hd res=471.Syntax:elink.
.br
:link reftype=hd res=473 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=471
x=30%
width=70%
hide
group=2
.wpIsDeleteable - Syntax
:xmp.
/* Check if an object is deleteable */

theObject         /* The WPS-object to be checked */
:p.
rc=WPSWizCallWPSFunc("wpIsDeleteable", theObject);
:exmp.


:h3 res=473
x=30%
width=70% height=35%
group=3
hide
.wpIsDeleteable - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is 0 if not deletable.


.************** wpIsLocked **************************

:h3 res=760
width=30%
.wpIslocked
:link reftype=hd res=761
auto dependent group=2.
:p.
:link reftype=hd res=761.Syntax:elink.
.br
:link reftype=hd res=763 dependent.Returns:elink.
.br
:link reftype=hd res=764 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=761
x=30%
width=70%
hide
group=2
.wpIsLocked - Syntax
:xmp.
/* Query if the object can be made dormant */

theObject         /* WPS-object  */
:p.
rc=WPSWizCallWPSFunc("wpIsLocked", theObject);
:exmp.

:h3 res=763
x=30%
width=70% height=35%
group=3
hide
.wpIsLocked - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
:ul.
:li.0: The object is not locked and thus can be made dormant.
:li.1: The object is locked.
:eul.

:h3 res=764
x=30%
width=70%
hide
group=2
.wpIslocked - Remarks
:p.
Every object maintains a lock counter to prevent the system from making it
dormant. If the lock counter isn't zero an object stays awake even if
the folder it resides in is closed.

.****************** wpLockObject ************************************

:h3 res=720
width=30%
.wpLockObject
:link reftype=hd res=721
auto dependent group=2.
:p.
:link reftype=hd res=721.Syntax:elink.
.br
:link reftype=hd res=723 dependent.Returns:elink.
.br
:link reftype=hd res=724 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=721
x=30%
width=70%
hide
group=2
.wpLockObject - Syntax
:xmp.
/* Increase the lock counter of an object */

theObject         /* WPS-object  */
:p.
rc=WPSWizCallWPSFunc("wpLockObject", theObject);
:exmp.


:h3 res=723
x=30%
width=70% height=35%
group=3
hide
.wpLockObject - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
There's no  return value for this method.

:h3 res=724
x=30%
width=70%
hide
group=2
.wpLockObject - Remarks
:p.
Every object maintains a lock counter to prevent the system from making it
dormant. If the lock counter isn't zero an object stays awake even if
the folder it resides in is closed.

.************** wpMoveObject **************************

:h3 res=770
width=30%
.wpMoveObject
:link reftype=hd res=771
auto dependent group=2.
:p.
:link reftype=hd res=771.Syntax:elink.
.br
:link reftype=hd res=773 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=764 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=771
x=30%
width=70%
hide
group=2
.wpMoveObject - Syntax
:xmp.
/* Move an object to a new folder */

theObject         /* WPS-object        */
theFolder         /* The target folder */
:p.
rc=WPSWizCallWPSFunc("wpMoveObject", theObject, theFolder);
:exmp.

:h3 res=773
x=30%
width=70% height=35%
group=3
hide
.wpMoveObject - Return value
:p.
:hp2.rc:ehp2. numeric
The return value is 0 in case of an error.


.************** wpPopulate **************************

:h3 res=830
width=30%
.wpPopulate
:link reftype=hd res=831
auto dependent group=2.
:p.
:link reftype=hd res=831.Syntax:elink.
.br
:link reftype=hd res=833 dependent.Returns:elink.
.br
:link reftype=hd res=834 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=831
x=30%
width=70%
hide
group=2
.wpPopulate - Syntax
:xmp.
/* Move an object to a new folder */

theFolder         /* Folder this method is invoked on */
fFoldersOnly      /* Populate with folders only (1)
                     or with all objects (0)          */
:p.
rc=WPSWizCallWPSFunc("wpPopulate", theFolder,
                    0, 0, fFoldersOnly);
:exmp.

:h3 res=833
x=30%
width=70% height=35%
group=3
hide
.wpPopulate - Return value
:p.
:hp2.rc:ehp2. numeric
The return value is 0 in case of an error. Otherwise it's a
pointer to the new object.

:h3 res=834
x=30%
width=70%
hide
group=2
.wpPopulate - Remarks
:p.
Each object in the folder is locked by this method call.
:p.
The folder flags indicate the population state of the folder.

.***************** wpQueryAttr *************************************

:h3 res=480
width=30%
.wpQueryAttr
:link reftype=hd res=481
auto dependent group=2.
:p.
:link reftype=hd res=481.Syntax:elink.
.br
:link reftype=hd res=483 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=481
x=30%
width=70%
hide
group=2
.wpQueryAttr - Syntax
:xmp.
/* Query the file attributes of an object */

theObject         /* The WPS-object to be checked
                     Must be of class WPFileSystem  */
:p.
rc=WPSWizCallWPSFunc("wpQueryAttr", theObject);
:exmp.


:h3 res=483
x=30%
width=70% height=35%
group=3
hide
.wpQueryAttr - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is a bit mask of the file attributes.


.******************************************************

:h3 res=490
width=30%
.wpQueryConcurrentView
:link reftype=hd res=491
auto dependent group=2.
:p.
:link reftype=hd res=491.Syntax:elink.
.br
:link reftype=hd res=493 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=491
x=30%
width=70%
hide
group=2
.wpQueryConcurrentView - Syntax
:xmp.
/* Query the concurrent view behavior of an object */

theObject         /* The WPS-object to be checked */
:p.
ulView=WPSWizCallWPSFunc("wpQueryConcurrentView", 
                         theObject);
:exmp.


:h3 res=493
x=30%
width=70% height=55%
group=3
hide
.wpQueryConcurrentView - Return value
:p.
:hp2.ulView:ehp2. numeric
:p.
The return value is one of the following:

:ul.
:li.CCVIEW_DEFAULT: 0
.br
Concurrent view is set to system default.
:li.CCVIEW_ON:      1
.br
Concurrent view is off.
:li.CCVIEW_OFF:     2
.br
Concurrent view is off.
:eul.



.******************************************************

:h3 res=500
width=30%
.wpQueryConfirmations
:link reftype=hd res=501
auto dependent group=2.
:p.
:link reftype=hd res=501.Syntax:elink.
.br
:link reftype=hd res=503 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=501
x=30%
width=70%
hide
group=2
.wpQueryConfirmations - Syntax
:xmp.
/* Query which confirmations are set for this
   object */

theObject         /* The WPS-object on which the method is invoked */
:p.
ulFlag=WPSWizCallWPSFunc("wpQueryConfirmations", theObject);
:exmp.


:h3 res=503
x=30%
width=70% height=55%
group=3
hide
.wpQueryConfirmations - Return value
:p.
:hp2.ulFlag:ehp2. numeric
:p.
The return value is one of the following.
:ul.
:li.0
.br
No confirmation set.
:li.CONFIRM_DELETE             0x0001
.br
Prompt for all objects.
:li.CONFIRM_DELETEFOLDER       0x0002
.br
Prompt for folders only.
:li.CONFIRM_RENAMEFILESWITHEXT 0x0004
.br
Prompt when renaming files with extension.
:li.CONFIRM_KEEPASSOC          0x0008
.br
Prompt if file to be renamed has an association depending on the extension.
:li.CONFIRM_ACTION             0x0010
.br
Prompt when moving, copying or creating shadows.
:li.CONFIRM_PROGRESS           0x0020
.br
Display a dialog box when performing a copy, move etc.
:li.CONFIRM_ALLOWFORCEDELETE   0x0040
.br
Allow forced delete. Must be set when CONFIRM_FORCEDELETE is set.
:li.CONFIRM_FORCEDELETE        0x0080
.br
Prompt for confirmation of forced deletion.
:eul.

The values are returned as decimal numbers.


.******************************************************

:h3 res=510
width=30%
.wpQueryContent
:link reftype=hd res=511
auto dependent group=2.
:p.
:link reftype=hd res=511.Syntax:elink.
.br
:link reftype=hd res=513 dependent.Returns:elink.
.br
:link reftype=hd res=514 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=511
x=30%
width=70%
hide
group=2
.wpQueryContent - Syntax
:xmp.
/* Query the contents of a folder */

theFolder         /* The folder to be checked */
theObject         /* Previous object or NULL */
option            /* option, which object to query 
                     (QC_FIRST, QC_NEXT, QC_LAST)
:p.
object=WPSWizCallWPSFunc("wpQueryContent", 
                         theFolder, theObject, option);
:exmp.


:h3 res=513
x=30%
width=70% height=55%
group=3
hide
.wpQueryContent - Return value
:p.
:hp2.object:ehp2. numeric
:p.
Pointer to the requested object.


:h3 res=514
x=30%
width=70% 
group=2
hide
.wpQueryContent - Remarks
:p.
Specify QC_FIRST (0) to get the first object in the folder, QC_LAST (2) for the last. When
using QC_NEXT (1) then :hp2.theObject:ehp2. points to the previous object otherwise this
parameter is ignored.
:p.
The order of the objects returned is the order in the internal list of the folder. It's no
necessarily the order in which the objects appear in the container.  



.******************************************************

:h3 res=520
width=30%
.wpQueryDefaultView
:link reftype=hd res=521
auto dependent group=2.
:p.
:link reftype=hd res=521.Syntax:elink.
.br
:link reftype=hd res=523 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=521
x=30%
width=70%
hide
group=2
.wpQueryDefaultView - Syntax
:xmp.
/* Query the default view of an object */

theObject         /* The WPS-object to be checked */
:p.
ulView=WPSWizCallWPSFunc("wpQueryDefaultView", 
                         theObject);
:exmp.


:h3 res=523
x=30%
width=70% height=55%
group=3
hide
.wpQueryDefaultView - Return value
:p.
:hp2.ulView:ehp2. numeric
:p.
The return value is one of the following:

:ul.
:li.OPEN_CONTENTS: 1
.br
Open in contents view (icon).
:li.OPEN_DEFAULT:  0
.br
Same as doubleclick.
:li.OPEN_DETAILS:  102
.br
Details view.
:li.OPEN_HELP:     3  
.br
Display the help panel.
:li.OPEN_RUNNING:  4  
.br
Execute the object.
:li.OPEN_SETTINGS: 2  
.br
Open the settings of the object.
:li.OPEN_TREE:     101
.br
Open the tree view.
:li.OPEN_UNKNOWN:  -1  
.br
Unknown view.
:li.OPEN_USER:     0x6500 or higher
.br
A user view introduced by a subclass.
:eul.



.******************************************************

:h3 res=530
width=30%
.wpQueryDisk
:link reftype=hd res=531
auto dependent group=2.
:p.
:link reftype=hd res=531.Syntax:elink.
.br
:link reftype=hd res=533 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=531
x=30%
width=70%
hide
group=2
.wpQueryDisk - Syntax
:xmp.
/* Query the disk object a file system
   object is located on */

theObject         /* The file system object to be checked */
:p.
Disk=WPSWizCallWPSFunc("wpQueryDisk", 
                         theObject);
:exmp.


:h3 res=533
x=30%
width=70% height=55%
group=3
hide
.wpQueryDisk - Return value
:p.
:hp2.Disk:ehp2. numeric
:p.
The return value is a pointer to the disk object of the drive the file system object is
located on.

.******************************************************

:h3 res=540
width=30%
.wpQueryEASize
:link reftype=hd res=541
auto dependent group=2.
:p.
:link reftype=hd res=541.Syntax:elink.
.br
:link reftype=hd res=543 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=541
x=30%
width=70%
hide
group=2
.wpQueryEASize - Syntax
:xmp.
/* Query the size of the EAs of a file system
   object */

theObject         /* The file system object  */
:p.
eaSize=WPSWizCallWPSFunc("wpQueryEASize", 
                         theObject);
:exmp.


:h3 res=543
x=30%
width=70% height=55%
group=3
hide
.wpQueryEASize - Return value
:p.
:hp2.eaSize:ehp2. numeric
:p.
The return value is the size of the extended attributes of a file.


.******************************************************

:h3 res=550
width=30%
.wpQueryFileSize
:link reftype=hd res=551
auto dependent group=2.
:p.
:link reftype=hd res=551.Syntax:elink.
.br
:link reftype=hd res=553 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=551
x=30%
width=70%
hide
group=2
.wpQueryFileSize - Syntax
:xmp.
/* Query the size file */

theObject         /* The file system object  */
:p.
size=WPSWizCallWPSFunc("wpQueryFileSize", 
                         theObject);
:exmp.


:h3 res=553
x=30%
width=70% height=55%
group=3
hide
.wpQueryFileSize - Return value
:p.
:hp2.size:ehp2. numeric
:p.
The return value is the size of the file.


.**************** wpQueryFldrDetailsClass **************************************

:h3 res=900
width=30%
.wpQueryFldrDetailsClass
:link reftype=hd res=901
auto dependent group=2.
:p.
:link reftype=hd res=901.Syntax:elink.
.br
:link reftype=hd res=903 dependent.Returns:elink.
.br
:link reftype=hd res=904 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=901
x=30%
width=70%
hide
group=2
.wpQueryFldrDetailsClass - Syntax
:xmp.
/* Query the details class of a folder */

WPFolder   somSelf  /* The folder on which the method is invoked */
M_WPObject class    /* Pointer to the class object */
:p.
class=WPSWizCallWPSFunc("wpQueryFldrDetailsClass", 
                         somSelf );
:exmp.


:h3 res=903
x=30%
width=70% height=55%
group=3
hide
.wpQueryFldrDetailsClass - Return value
:p.
:hp2.font:ehp2. numeric
:p.
The return value is a pointer to the class object.


:h3 res=904
x=30%
width=70%
hide
group=2
.wpQueryFldrDetailsClass - Remarks
:p.
The details class for folders can be set using :link reftype=hd res=910.wpSetFldrDetailsClass:elink..


.******************************************************

:h3 res=560
width=30%
.wpQueryFldrFlags
:link reftype=hd res=561
auto dependent group=2.
:p.
:link reftype=hd res=561.Syntax:elink.
.br
:link reftype=hd res=563 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=561
x=30%
width=70%
hide
group=2
.wpQueryFldrFlags - Syntax
:xmp.
/* Query the flags of a folder */

theFolder         /* The folder object  */
:p.
flags=WPSWizCallWPSFunc("wpQueryFldrFlags", 
                         theFolder);
:exmp.


:h3 res=563
x=30%
width=70% height=55%
group=3
hide
.wpQueryFldrFlags - Return value
:p.
:hp2.flags:ehp2. numeric
:p.
The return value is a bitmask of the following.

:ul.
:li.FOI_POPULATEDWITHALL     0x0001
.br
The folder is populated with all objects.
:li.FOI_POPULATEDWITHFOLDERS 0x0002
.br
The folder is only populated with folders it contains.
:li.FOI_WORKAREA             0x0004
.br
The folder is a workarea folder.
:eul.

.******************************************************

:h3 res=570
width=30%
.wpQueryFldrFont
:link reftype=hd res=571
auto dependent group=2.
:p.
:link reftype=hd res=571.Syntax:elink.
.br
:link reftype=hd res=573 dependent.Returns:elink.
.br
:link reftype=hd res=574 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=571
x=30%
width=70%
hide
group=2
.wpQueryFldrFont - Syntax
:xmp.
/* Query the font of a folder view */

thefolder         /* The folder on which the method is invoked */
fldrView          /* One of OPEN_CONTENTS (1), OPEN_DETAILS (102) or OPEN_TREE (101) */
:p.
font=WPSWizCallWPSFunc("wpQueryFldrFont", 
                         theFolder, fldrView);
:exmp.


:h3 res=573
x=30%
width=70% height=55%
group=3
hide
.wpQueryFldrFont - Return value
:p.
:hp2.font:ehp2. string
:p.
The return value is font of the specified folder view for example :hp2.10.Helv:ehp2..


:h3 res=574
x=30%
width=70%
hide
group=2
.wpQueryFldrFont - Remarks
:p.
There's only one font per view. All concurrent views share the same font.


.**************** wpQueryFldrSortClass **************************************

:h3 res=890
width=30%
.wpQueryFldrSortClass
:link reftype=hd res=891
auto dependent group=2.
:p.
:link reftype=hd res=891.Syntax:elink.
.br
:link reftype=hd res=893 dependent.Returns:elink.
.br
:link reftype=hd res=894 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=891
x=30%
width=70%
hide
group=2
.wpQueryFldrSortClass - Syntax
:xmp.
/* Query the folder sort class */

WPFolder   somSelf  /* The folder on which the method is invoked */
M_WPObject class    /* Pointer to the class object */
:p.
class=WPSWizCallWPSFunc("wpQueryFldrSortClass", 
                         somSelf );
:exmp.


:h3 res=893
x=30%
width=70% height=55%
group=3
hide
.wpQueryFldrSortClass - Return value
:p.
:hp2.font:ehp2. numeric
:p.
The return value is a pointer to the class object.


:h3 res=894
x=30%
width=70%
hide
group=2
.wpQueryFldrSortClass - Remarks
:p.
The default sort class for folders is WPFileSystem.


.******************************************************

:h3 res=580
width=30%
.wpQueryFolder
:link reftype=hd res=581
auto dependent group=2.
:p.
:link reftype=hd res=581.Syntax:elink.
.br
:link reftype=hd res=583 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=581
x=30%
width=70%
hide
group=2
.wpQueryFolder - Syntax
:xmp.
/* Query the folder an  objects resides in */

theObject         /* A WPS object  */
:p.
theFolder=WPSWizCallWPSFunc("wpQueryFolder", 
                         theObject);
:exmp.

:h3 res=583
x=30%
width=70% height=55%
group=3
hide
.wpQueryFolder - Return value
:p.
:hp2.theFolder:ehp2. numeric
:p.
The return value is the folder a given object resides in. The pointer
may be used with other methods.


.******************************************************

:h3 res=590
width=30%
.wpQueryHandle
:link reftype=hd res=591
auto dependent group=2.
:p.
:link reftype=hd res=591.Syntax:elink.
.br
:link reftype=hd res=593 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=591
x=30%
width=70%
hide
group=2
.wpQueryHandle - Syntax
:xmp.
/* Query the persistent handle of an object */

theObject         /* A WPS object  */
:p.
handle=WPSWizCallWPSFunc("wpQueryHandle", 
                         theObject);
:exmp.


:h3 res=593
x=30%
width=70% height=55%
group=3
hide
.wpQueryHandle - Return value
:p.
:hp2.handle:ehp2. numeric
:p.
The return value is a persistent handle to the given object. This
handle is unique on the system.


.******************************************************

:h3 res=600
width=30%
.wpQueryObjectID
:link reftype=hd res=601
auto dependent group=2.
:p.
:link reftype=hd res=601.Syntax:elink.
.br
:link reftype=hd res=603 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=314 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=601
x=30%
width=70%
hide
group=2
.wpQueryObjectID - Syntax
:xmp.
/* Query the ID of an object */

theObject         /* A WPS object  */
:p.
id=WPSWizCallWPSFunc("wpQueryObjectID", 
                         theObject);
:exmp.


:h3 res=603
x=30%
width=70% height=55%
group=3
hide
.wpQueryObjectID - Return value
:p.
:hp2.id:ehp2. string
:p.
The return value is the ID of the given object or NULL. This
ID is unique on the system for example <WP_DESKTOP>.

.******************************************************

:h3 res=610
width=30%
.wpQueryShadowedObject
:link reftype=hd res=611
auto dependent group=2.
:p.
:link reftype=hd res=611.Syntax:elink.
.br
:link reftype=hd res=613 dependent.Returns:elink.
.br
:link reftype=hd res=614 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=611
x=30%
width=70%
hide
group=2
.wpQueryShadowedObject - Syntax
:xmp.
/* Query the object a shadow links to */

theShadow         /* A shadow object  */
fLock             /* lock flag 0/1*/
:p.
object=WPSWizCallWPSFunc("wpQueryShadowedObject", 
                         theShadow, fLock);
:exmp.


:h3 res=613
x=30%
width=70% height=55%
group=3
hide
.wpQueryShadowedObject - Return value
:p.
:hp2.object:ehp2. numeric
:p.
The return value is the object the shadow links to.


:h3 res=614
x=30%
width=70%
hide
group=2
.wpQueryShadowedObject - Remarks
:p.
If fLock is 0 the queried object will go dormant when the object and the folder
the object resides in are closed. Otherwise the object stays awake until a 
wpUnlockObject call is made. Awake objects stay in memory while dormant objects release
their resources when not longer accessed.


.******************************************************

:h3 res=620
width=30%
.wpQueryStyle
:link reftype=hd res=621
auto dependent group=2.
:p.
:link reftype=hd res=621.Syntax:elink.
.br
:link reftype=hd res=623 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=614 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=621
x=30%
width=70%
hide
group=2
.wpQueryStyle - Syntax
:xmp.
/* Query the style of an object */

theObject         /* An object  */
:p.
style=WPSWizCallWPSFunc("wpQueryStyle", 
                         theObject);
:exmp.


:h3 res=623
x=30%
width=70% height=55%
group=3
hide
.wpQueryStyle - Return value
:p.
:hp2.style:ehp2. numeric
:p.
The return value is a bitmask of the style flags of an object.
:ul.
:li.OBJSTYLE_NOMOVE          2
.br
Object cannot be moved.
:li.OBJSTYLE_NOLINK          4
.br
Shadows can't be created
:li.OBJSTYLE_NOCOPY          8
.br
Object cannot be copied.
:li.OBJSTYLE_TEMPLATE        32
.br
The object is a template.
:li.OBJSTYLE_NODELETE        64
.br
The object cannot be deleted.
:li.OBJSTYLE_NOPRINT         128
.br
The object cannot be printed.
:li.OBJSTYLE_NODRAG          256
.br
The object cannot be dragged.
:li.OBJSTYLE_NOTVISIBLE      512
.br
The object is hidden.
:li.OBJSTYLE_NOSETTINGS      1024
.br
There're not settings for this object.
:li.OBJSTYLE_NORENAME        2048
.br
The object cannot be renamed.
:li.OBJSTYLE_NODROPON        8192
.br
Other objects can't be dropped on this object.
:li.OBJSTYLE_CUSTOMICON      16384
.br
Icon is destroyed when object goes dormant. 
:eul.

.******************************************************

:h3 res=630
width=30%
.wpQueryTitle
:link reftype=hd res=631
auto dependent group=2.
:p.
:link reftype=hd res=631.Syntax:elink.
.br
:link reftype=hd res=633 dependent.Returns:elink.
.br
:link reftype=hd res=634 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=631
x=30%
width=70%
hide
group=2
.wpQueryTitle - Syntax
:xmp.
/* Query the title of an object */

theObject         /* An object  */
:p.
title=WPSWizCallWPSFunc("wpQueryTitle", 
                         theObject);
:exmp.


:h3 res=633
x=30%
width=70% height=55%
group=3
hide
.wpQueryTitle - Return value
:p.
:hp2.title:ehp2. string
:p.
This call returns the title of the object as a string.

:h3 res=634
x=30%
width=70%
hide
group=2
.wpQueryTitle - Remarks
:p.
The title may differ from the real name of an object. For example
FAT-drives don't support long file names but the WPS may take
them from the EAs.


.******************************************************

:h3 res=640
width=30%
.wpQueryType
:link reftype=hd res=641
auto dependent group=2.
:p.
:link reftype=hd res=641.Syntax:elink.
.br
:link reftype=hd res=643 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=634 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=641
x=30%
width=70%
hide
group=2
.wpQueryType - Syntax
:xmp.
/* Query the type of an object */

theObject         /* An object  */
:p.
type=WPSWizCallWPSFunc("wpQueryType", 
                         theObject);
:exmp.


:h3 res=643
x=30%
width=70% height=55%
group=3
hide
.wpQueryType - Return value
:p.
:hp2.type:ehp2. string
:p.
The return string contains a list of types delimited with linefeed characters.
For example :hp2.Plain Text\nC Code:ehp2..

.******************************************************

:h3 res=650
width=30%
.wpRefresh
:link reftype=hd res=651
auto dependent group=2.
:p.
:link reftype=hd res=651.Syntax:elink.
.br
:link reftype=hd res=653 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=634 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=651
x=30%
width=70%
hide
group=2
.wpRefresh - Syntax
:xmp.
/* Force a file system object to reread its information */

theObject         /* A file system object */
view              /* The view to be refreshed (OPEN_CONTENTS, OPEN_TREE, OPEN_DETAILS or any other defined view) */
reserved          /* reserved parameter, must be 0 */
:p.
rc=WPSWizCallWPSFunc("wpRefresh", 
                         theObject, view, reserved);
:exmp.


:h3 res=653
x=30%
width=70% height=55%
group=3
hide
.wpRefresh - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
Returns 0 if an error occurs.


.******************************************************

:h3 res=660
width=30%
.wpSaveDeferred
:link reftype=hd res=661
auto dependent group=2.
:p.
:link reftype=hd res=661.Syntax:elink.
.br
:link reftype=hd res=663 dependent.Returns:elink.
.br
:link reftype=hd res=664 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=661
x=30%
width=70%
hide
group=2
.wpSaveDeferred - Syntax
:xmp.
/* Save the instance data of an object asynchronously */

theObject         /* A WPS object */
:p.
rc=WPSWizCallWPSFunc("wpSaveDeferred", 
                         theObject);
:exmp.


:h3 res=663
x=30%
width=70% height=55%
group=3
hide
.wpSaveDeferred - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
Returns 0 if an error occurs.


:h3 res=664
x=30%
width=70%
hide
group=2
.wpSaveDeferred - Remarks
:p.
The instance data is saved on a separate thread in the background when the system is idle.
If it's necessary to save critical data immediately use wpSaveImmediate.


:h3 res=670
width=30%
.wpSaveImmediate
:link reftype=hd res=671
auto dependent group=2.
:p.
:link reftype=hd res=671.Syntax:elink.
.br
:link reftype=hd res=673 dependent.Returns:elink.
.br
:link reftype=hd res=674 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=671
x=30%
width=70%
hide
group=2
.wpSaveImmediate - Syntax
:xmp.
/* Save the instance data of an object synchronously */

theObject         /* A WPS object */
:p.
rc=WPSWizCallWPSFunc("wpSaveImmediate", 
                         theObject);
:exmp.


:h3 res=673
x=30%
width=70% height=55%
group=3
hide
.wpSaveImmediate - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
Returns 0 if an error occurs.


:h3 res=674
x=30%
width=70%
hide
group=2
.wpSaveImmediate - Remarks
:p.
The instance data is saved immediate when calling this method. In general
wpSaveDeferred should be used because when using that method the data is written
when the system is idle.

.******************************************************

:h3 res=680
width=30%
.wpSetDefaultView
:link reftype=hd res=681
auto dependent group=2.
:p.
:link reftype=hd res=681.Syntax:elink.
.br
:link reftype=hd res=683 dependent.Returns:elink.
.br
:link reftype=hd res=684 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=681
x=30%
width=70%
hide
group=2
.wpSetDefaultView - Syntax
:xmp.
/* Set the default view of an object */

theObject         /* A WPS object */
view              /* The new default view */
:p.
rc=WPSWizCallWPSFunc("wpSetDefaultView", 
                         theObject, view);
:exmp.


:h3 res=683
x=30%
width=70% height=55%
group=3
hide
.wpSetDefaultView - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
Returns 0 if an error occurs.


:h3 res=684
x=30%
width=70%
hide
group=2
.wpSetDefaultView - Remarks
:p.
The object may be set to one of the following views.

:ul.
:li.OPEN_CONTENTS: 1
.br
Open in contents view (icon).
:li.OPEN_DEFAULT:  0
.br
Same as doubleclick.
:li.OPEN_DETAILS:  102
.br
Details view.
:li.OPEN_HELP:     3  
.br
Display the help panel.
:li.OPEN_RUNNING:  4  
.br
Execute the object.
:li.OPEN_SETTINGS: 2  
.br
Open the settings of the object.
:li.OPEN_TREE:     101
.br
Unknown view.
:li.OPEN_USER:     0x6500 or higher
.br
A user view introduced by a subclass.
:eul.


.**************** wpSetFldrDetailsClass **************************************

:h3 res=910
width=30%
.wpSetFldrDetailsClass
:link reftype=hd res=911
auto dependent group=2.
:p.
:link reftype=hd res=911.Syntax:elink.
.br
:link reftype=hd res=913 dependent.Returns:elink.
.br
:link reftype=hd res=914 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=911
x=30%
width=70%
hide
group=2
.wpSetFldrDetailsClass - Syntax
:xmp.
/* Set the details class of a folder */

WPFolder   somSelf  /* The folder on which the method is invoked */
M_WPObject class    /* Pointer to the class object               */
BOOL       rc       /* Success indicator                         */
:p.
rc=WPSWizCallWPSFunc("wpSetFldrDetailsClass", 
                         somSelf, class );
:exmp.

:h3 res=913
x=30%
width=70% height=55%
group=3
hide
.wpSetFldrDetailsClass - Return value
:p.
:hp2.font:ehp2. numeric
:p.
The return value is zero in case of an error.

:h3 res=914
x=30%
width=70%
hide
group=2
.wpSetFldrDetailsClass - Remarks
:p.
Use :link reftype=hd res=900.wpQueryFldrDetailsClass:elink. to query the details class of a folder.


.**************** wpSetFldrSortClass **************************************

:h3 res=920
width=30%
.wpSetFldrSortClass
:link reftype=hd res=921
auto dependent group=2.
:p.
:link reftype=hd res=921.Syntax:elink.
.br
:link reftype=hd res=923 dependent.Returns:elink.
.br
:link reftype=hd res=924 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=921
x=30%
width=70%
hide
group=2
.wpSetFldrSortClass - Syntax
:xmp.
/* Set the sort class of a folder */

WPFolder   somSelf  /* The folder on which the method is invoked */
M_WPObject class    /* Pointer to the class object               */
BOOL       rc       /* Success indicator                         */
:p.
rc=WPSWizCallWPSFunc("wpSetFldrSortClass", 
                         somSelf, class );
:exmp.

:h3 res=923
x=30%
width=70% height=55%
group=3
hide
.wpSetFldrSortClass - Return value
:p.
:hp2.font:ehp2. numeric
:p.
The return value is zero in case of an error.

:h3 res=924
x=30%
width=70%
hide
group=2
.wpSetFldrSortClass - Remarks
:p.
Use wpQueryFldrSortClass to query the sort class of a folder.


.******************************************************

:h3 res=690
width=30%
.wpSetLinkToObject
:link reftype=hd res=691
auto dependent group=2.
:p.
:link reftype=hd res=691.Syntax:elink.
.br
:link reftype=hd res=693 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=684 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=691
x=30%
width=70%
hide
group=2
.wpSetLinkToObject - Syntax
:xmp.
/* This method links the shadow to 
   the specified object */

theShadow    /* Shadow object on which the method is invoked */
theObject    /* A WPS object to link to */
:p.
rc=WPSWizCallWPSFunc("wpSetLinkToObject", 
                     theShadow, theObject);
:exmp.


:h3 res=693
x=30%
width=70% height=55%
group=3
hide
.wpSetLinkToObject - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
Returns 0 if an error occurs.

.************** wpSetObjectID **************************

:h3 res=870
width=30%
.wpSetObjectID
:link reftype=hd res=871
auto dependent group=2.
:p.
:link reftype=hd res=871.Syntax:elink.
.br
:link reftype=hd res=873 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=874 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=871
x=30%
width=70%
hide
group=2
.wpSetObjectID - Syntax
:xmp.
/* Set the object ID of an object */

theObject         /* Object this method is invoked on */
newId             /* New object ID   */
:p.
rc=WPSWizCallWPSFunc("wpSetObjectID", theObject,
                       newID);
:exmp.

:h3 res=873
x=30%
width=70% height=35%
group=3
hide
.wpSetObjectID - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is 0 in case of an error.


.************** wpSetStyle ****************************************

:h3 res=700
width=30%
.wpSetStyle
:link reftype=hd res=701
auto dependent group=2.
:p.
:link reftype=hd res=701.Syntax:elink.
.br
:link reftype=hd res=703 dependent.Returns:elink.
.br
:link reftype=hd res=704 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=701
x=30%
width=70%
hide
group=2
.wpSetStyle - Syntax
:xmp.
/* Set the style of an object */

theObject     /* A WPS object */
newStyle      /* The new style of the object */
:p.
rc=WPSWizCallWPSFunc("wpSetStyle", 
                     theObject, newStyle);
:exmp.


:h3 res=703
x=30%
width=70% height=55%
group=3
hide
.wpSetStyle - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
Returns 0 if an error occurs.

:h3 res=704
x=30%
width=70%
hide
group=2
.wpSetStyle - Remarks
:p.
The following flags may be ored together.
:ul.
:li.OBJSTYLE_NOMOVE          2
.br
Object cannot be moved.
:li.OBJSTYLE_NOLINK          4
.br
Shadows can't be created
:li.OBJSTYLE_NOCOPY          8
.br
Object cannot be copied.
:li.OBJSTYLE_TEMPLATE        32
.br
The object is a template.
:li.OBJSTYLE_NODELETE        64
.br
The object cannot be deleted.
:li.OBJSTYLE_NOPRINT         128
.br
The object cannot be printed.
:li.OBJSTYLE_NODRAG          256
.br
The object cannot be dragged.
:li.OBJSTYLE_NOTVISIBLE      512
.br
The object is hidden.
:li.OBJSTYLE_NOSETTINGS      1024
.br
There're not settings for this object.
:li.OBJSTYLE_NORENAME        2048
.br
The object cannot be renamed.
:li.OBJSTYLE_NODROPON        8192
.br
Other objects can't be dropped on this object.
:li.OBJSTYLE_CUSTOMICON      16384
.br
Icon is destroyed when object goes dormant. 
:eul.


.************** wpSetType **************************

:h3 res=880
width=30%
.wpSetType
:link reftype=hd res=881
auto dependent group=2.
:p.
:link reftype=hd res=881.Syntax:elink.
.br
:link reftype=hd res=883 dependent.Returns:elink.
.br
:link reftype=hd res=884 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=881
x=30%
width=70%
hide
group=2
.wpSetType - Syntax
:xmp.
/* Set the type of a filesystem object */

WPFileSystem somSelf     /* Object this method is invoked on */
String       newType     /* String with new type(s)          */
:p.
rc=WPSWizCallWPSFunc("wpSetType", somSelf,
                       newType);
:exmp.

:h3 res=883
x=30%
width=70% height=35%
group=3
hide
.wpSetType - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is 0 in case of an error.

:h3 res=884
x=30%
width=70%
hide
group=2
.wpSetType - Remarks
:p.
This method sets the .TYPE attribute of the file system object.
:p.
The string is a list of types. Each type is separated by a line feed character. For example:
:p.
:xmp.
newType="Plain Text\nC code"
:exmp.


.************** wpSwitchTo **************************

:h3 res=860
width=30%
.wpSwitchTo
:link reftype=hd res=861
auto dependent group=2.
:p.
:link reftype=hd res=861.Syntax:elink.
.br
:link reftype=hd res=863 dependent.Returns:elink.
.br
:link reftype=hd res=864 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=861
x=30%
width=70%
hide
group=2
.wpSwitchTo - Syntax
:xmp.
/* Display an open object view */

theObject         /* Object this method is invoked on */
view              /* The view to surface */
:p.
rc=WPSWizCallWPSFunc("wpSwitchTo", 
                       theObject, view);
:exmp.

:h3 res=863
x=30%
width=70% height=35%
group=3
hide
.wpSwitchTo - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is 0 in case of an error.

:h3 res=864
x=30%
width=70%
hide
group=2
.wpSwitchTo - Remarks
:p.
The view may be one of the following.

:ul.
:li.OPEN_CONTENTS: 1
.br
Open in contents view (icon).
:li.OPEN_DEFAULT:  0
.br
Same as doubleclick.
:li.OPEN_DETAILS:  102
.br
Details view.
:li.OPEN_HELP:     3  
.br
Only for palette objects.
:li.OPEN_RUNNING:  4  
.br
Execute the object.
:li.OPEN_SETTINGS: 2  
.br
Open the settings of the object.
:li.OPEN_TREE:     101
.br
Unknown view.
:li.OPEN_USER:     0x6500 or higher
.br
A user view introduced by a subclass.
:eul.



.************** wpUnlockObject **************************

:h3 res=730
width=30%
.wpUnlockObject
:link reftype=hd res=731
auto dependent group=2.
:p.
:link reftype=hd res=731.Syntax:elink.
.br
:link reftype=hd res=733 dependent.Returns:elink.
.br
:link reftype=hd res=734 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=731
x=30%
width=70%
hide
group=2
.wpUnlockObject - Syntax
:xmp.
/* Decrease the lock counter of an object */

theObject         /* WPS-object  */
:p.
rc=WPSWizCallWPSFunc("wpUnlockObject", theObject);
:exmp.

:h3 res=733
x=30%
width=70% height=35%
group=3
hide
.wpUnlockObject - Return value
:p.
:hp2.rc:ehp2. numeric
:p.
The return value is 0 in case of an error.

:h3 res=734
x=30%
width=70%
hide
group=2
.wpUnlockObject - Remarks
:p.
Every object maintains a lock counter to prevent the system from making it
dormant. If the lock counter isn't zero an object stays awake even if
the folder it resides in is closed.

.************** wpViewObject **************************

:h3 res=850
width=30%
.wpViewObject
:link reftype=hd res=851
auto dependent group=2.
:p.
:link reftype=hd res=851.Syntax:elink.
.br
:link reftype=hd res=853 dependent.Returns:elink.
.br
:link reftype=hd res=854 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=851
x=30%
width=70%
hide
group=2
.wpViewObject - Syntax
:xmp.
/* Open an object or display an open
   object view if according to the
   view settings.                     */

theObject         /* Object this method is invoked on */
view              /* The view to open/surface */
param             /* optional parameter (0)   */
:p.
hwnd=WPSWizCallWPSFunc("wpViewObject", theObject,
                    0, view, param);
:exmp.

:h3 res=853
x=30%
width=70% height=35%
group=3
hide
.wpViewObject - Return value
:p.
:hp2.hwnd:ehp2. numeric
:p.
The return value is 0 in case of an error. Otherwise it's the
handle to the objects window.

:h3 res=854
x=30%
width=70%
hide
group=2
.wpViewObject - Remarks
:p.
Depending on the settings of the object a new view is opened or an
already existing view will be brought to the foreground.
:p.
The view may be one of the following.

:ul.
:li.OPEN_CONTENTS: 1
.br
Open in contents view (icon).
:li.OPEN_DEFAULT:  0
.br
Same as doubleclick.
:li.OPEN_DETAILS:  102
.br
Details view.
:li.OPEN_HELP:     3  
.br
Display the help panel.
:li.OPEN_PALETTE:  121
.br
Only for palette objects.
:li.OPEN_RUNNING:  4  
.br
Execute the object.
:li.OPEN_SETTINGS: 2  
.br
Open the settings of the object.
:li.OPEN_TREE:     101
.br
Unknown view.
:li.OPEN_USER:     0x6500 or higher
.br
A user view introduced by a subclass.
:eul.





