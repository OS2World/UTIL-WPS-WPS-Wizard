/* Example: Get some information about the
   objects in a folder */

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
rc=RxMessageBox("This skript will gather some information about the objects in this folder.", "Information",'OK')

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

/* Now check all objects */
DO WHILE object\=0
	allObjects.0=allObjects.0+1
	a=allObjects.0
	allObjects.a._obj=object
	call checkClass allObjects.0 /* see below */
	/* Get next object */
	object=WPSWizCallWPSFunc("wpQueryContent", mFldr, object, QC_NEXT)
END

/* Show what we've found */
theString=allObjects.0||" Objects in this folder."||'0d'X||'0d'X
theString=theString||allObjects._folder||" folders"||'0d'X
theString=theString||allObjects._file||" files"||'0d'X
theString=theString||allObjects._shadow||" shadows and"||'0d'X
theString=theString||allObjects._other||" other objects"||'0d'X||'0d'X||"   "
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
checkClass:

a=ARG(1)

ret=WPSWizCallFunc("cwIsA", allObjects.a._obj, "WPFolder")
IF ret=1 THEN DO
	allObjects.a._class='WPFolder'
	allObjects._folder=allObjects._folder+1
	RETURN
END

ret=WPSWizCallFunc("cwIsA", allObjects.a._obj, "WPDataFile")
IF ret=1 THEN DO
	allObjects.a._class='WPDataFile'
	allObjects._file=allObjects._file+1
	RETURN
END

ret=WPSWizCallFunc("cwIsA", allObjects.a._obj, "WPShadow")
IF ret=1 THEN DO
	allObjects.a._class='WPShadow'
	allObjects._shadow=allObjects._shadow+1
	RETURN
END

allObjects.a._class='other'
allObjects._other=allObjects._other+1

return