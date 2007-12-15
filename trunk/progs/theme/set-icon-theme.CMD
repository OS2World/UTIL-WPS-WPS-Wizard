/* rexx - Create a folder with objects showing all the icons
   we have.
 
   This script is part of the icon theme package. An add on
   for WPS-Wizard.

   (c) Chris Wohlgemuth 2007

*/

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

oldName=DIRECTORY()||"\CLASS-IMAGE.INI"
newName=DIRECTORY()||"\wpswiz-theme.ini"

/* Small trick. The script is started with a parameter when the restart
   message shall be displayed. */
If ARG()<>0 THEN call showMsg

PARSE SOURCE . . theScript

if stream(newname, "C", "query exists") <> "" THEN
DO
SAY "Set as new icon theme..."

call myExit
END

if stream(oldname, "C", "query exists") <> "" THEN
DO
 /* Rename ini file */
 SAY "Copying INI file..."
 '@copy "'||oldname||'" wpswiz-theme.ini'
END

SAY "Set as new icon theme..."

call myExit

exit


myExit:

call SysIni "USER" ,"IconTheme", "IconFolder", DIRECTORY()||'00'x

SAY "You have to restart the WPS now."
'PMREXX /Q '||theScript||' msg'

exit


showMsg:

rc=RxMessageBox( "The WPS must be restarted so the new icons are loaded."||'0a'x||'0a'x||'Restart the WPS now?', "Icon installation", "YESNO")

IF rc==6 THEN
DO
	/* Find WPS wizard */
	wpswiz=''
	call SysQueryClassList "liste."
	do i = 1 to liste.0
	     IF WORD(liste.i, 1) =="CWObject" THEN
	       wpswiz=WORD(liste.i, 2)	     
	END

	IF wpswiz=='' THEN DO
		call RxMessageBox "WPS-Wizard seems not to be installed. Exiting..."
		exit
	END
	wpsresetexe=FILESPEC("drive", wpswiz)||FILESPEC("path", wpswiz)||"bin\wpsreset.exe"
	'cmd.exe /C '||wpsresetexe||' 1>NUL 2>NUL'
END
rc=0
exit
