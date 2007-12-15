/* Example: Get the total size of the file objects
   in a folder using WPS methods */

/* Load Rexxutil functions */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

QC_FIRST=0
QC_NEXT=1
QC_LAST=2

/* Check if we are executing */
if ARG(1)\='/EXECUTE' THEN EXIT

/* The folder running this skript */
mFldr=ARG(2)
/* And the frame handle */
frameHandle=ARG(3)

/* Get the script file path */
PARSE SOURCE . . filePath

/* Say what we're doing */
rc=RxMessageBox("This script will get the total size of all file object in this folder using WPS methods.", "Information",'OK')

/* Clear cars */
allObjects.=''
allObjects.0=0
allObjects._file=0
allObjects._folder=0
allObjects._shadow=0
allObjects._other=0

/* Get first object */
object=WPSWizCallWPSFunc("wpQueryContent", mFldr, 0, QC_FIRST)

if object=0 THEN DO
	/* No object so quit */
	rc=RxMessageBox("No objects in this folder.", "Information",'OK')
	EXIT
END

/* Now check all objects for their class */
DO WHILE object\=0
	allObjects.0=allObjects.0+1
	a=allObjects.0
	allObjects.a._obj=object
	call checkFile allObjects.0 /* see below */
	/* Get next object */
	object=WPSWizCallWPSFunc("wpQueryContent", mFldr, object, QC_NEXT)
END

totalSize=0
/* Get the size of the file system objects */
DO a=1 to allObjects.0
	IF allObjects.a._class='WPDataFile' THEN DO
		/* Get the size of the object */
		totalSize=totalSize+WPSWizCallWPSFunc("wpQueryFileSize", allObjects.a._obj)
	END
END
/* Show what we've found */
theString=allObjects._file||" files in the folder."||'0d'X||'0d'X
theString=theString||"Total size is: "||totalSize||' Bytes'||'0d'X||'  '
rc=RxMessageBox(theString, "Information",'OK')

/* Ask if user want to see the script */
rc=RxMessageBox("Do you want to see this script?", "Question",'YESNO','Question')
IF rc='6' THEN DO
	ADDRESS CMD 'e.exe "'||filePath||'"'
END

/* Ok done */
EXIT

/**********************/

/* Check the class of the object */
checkFile:

a=ARG(1)

ret=WPSWizCallFunc("cwIsA", allObjects.a._obj, "WPDataFile")
IF ret=1 THEN DO
	allObjects.a._class='WPDataFile'
	allObjects._file=allObjects._file+1
	RETURN
END

allObjects.a._class='other'
allObjects._other=allObjects._other+1

return