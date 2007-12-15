/* rexx - Create a folder with objects showing all the icons
   we have.
 
   This script is part of the icon theme package. An add on
   for WPS-Wizard.

   (c) Chris Wohlgemuth 2007

*/

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

theFolder=ARG(1)
theFolder=STRIP(TRANSLATE(theFolder,' ','"'))
if theFolder="" THEN DO
	SAY "Drop the icon root folder (64x64) here."
	'pause'
	EXIT
END
theFolder=theFolder||"\"
SAY theFolder

INIFILE=theFolder||"wpswiz-theme.ini"
SAY inifile

ul=LENGTH(theFolder)

rc=SysFileTree(theFolder, "files", "SFO");

DO a=1 to files.0
	IF TRANSLATE(RIGHT(files.a, 4)) == ".PNG" THEN
	DO
	/* Find last '\' */
	fname=RIGHT(files.a, LENGTH(files.a)-LASTPOS('\', files.a))
	fname=LEFT(fname, LENGTH(fname)-4)
	rc=SysIni(INIFILE, fname, "Image", RIGHT(files.a, LENGTH(files.a)-ul)||'00'x)
	END
END

exit


