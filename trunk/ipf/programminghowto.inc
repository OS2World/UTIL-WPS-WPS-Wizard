.*---------------------- h1 -------------------------------
:h1 res=23000  width=100% height=100%.Programming tutorials
:p.
The following chapter contains examples and tutorials showing how to program with
and for WPS-wizard.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=23100 x=left  width=35% height=100% group=1.Creating new action objects
:link reftype=hd res=23110
auto dependent group=2.
:p.
Folder action objects are basically small Rexx scripts executed when clicked. These
scripts are called with three parameters:

:ol.
:li./EXECUTE
:li.Pointer to the folder object
:li.Frame window handle of the folder
:eol.
:p.
These parameters are the same as for normal :link reftype=hd res=5000.folder scripts:elink..
:p.
:sl.
:li.:link reftype=hd res=23110.Creating a script:elink.
:li.:link reftype=hd res=23120.Object installation:elink.
:li.:link reftype=hd res=23130.Adding object hints:elink.
:esl.
.*-----------------------------------------------------

.*-----------------------------------------------------
:h3 res=23110 x=right  width=65% height=100% group=2.Creating a script
:p.
The following is a script for opening the parent folder. You may find it
in the :hp2.bin\toolbar:ehp2. subdirectory of the WPS-wizard installation.
As you can see it's short and uses one of the functions introduced by WPS-wizard.
:xmp. There's no limitation on use of Rexx functions in such a script.

/* Open parent of a folder */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/*
  Parameters:

  ARG(1): /EXECUTE
  ARG(2): folder object
  ARG(3): folder window handle (frame)
*/

/*
  WPSWizCallFunc("cwQueryRealName", ARG(2), 1) queries the full path of the folder object.
  This function is provided by WPS-wizard.

  SysSetObjectData(<path>, "MENUITEMSELECTED=714") selects the menu item 714 of an open folder.
  This menu item is "Open parent".
*/

/* Open the parent folder */
ret=SysSetObjectData(WPSWizCallFunc("cwQueryRealName", ARG(2), 1), "MENUITEMSELECTED=714")

exit
:exmp.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h3 res=23120 x=right  width=65% height=100% group=2.Object installation
:p.
The toolbar may be changed using the mouse. Program objects pointing to the scripts
are located in a folder inside the WPS-wizard folder on the desktop for easy access. Newly created
objects may be placed into that central folder but may also reside elsewhere on disk. When
creating an action button be sure to provide a basic installation script so an appropriate
program object is created for the user.
:p.
:artwork  align=center name='tbconfigure.bmp'.
:p.
:ul.
:li.The default action objects folder has the ID :hp2.<WPSWIZ_TBITEMFOLDER>:ehp2..
:li.Name the program object sensible. The name will be shown as the flyover help of the button.
:li.Use an object ID for your program object so uninstallation and hints are possible.
:li.Provide a useful icon. 
:eul.
:p.
:p.
The following is an example for a basic install script.
:xmp.

/* Action object installation */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/*
  Configuration parameters. Make sure the object ID is the last parameter.

 */
conf="EXENAME=path_to_script;ICONFILE=path\icon.ico;OBJECTID=<MY_NEW_OBJ>;"

rc=SysCreateObject("WPProgram", "Object title", "<WPSWIZ_TBITEMFOLDER>", conf , 'update')
:exmp.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h3 res=23130 x=right  width=65% height=100% group=2.Adding object hints
:p.
Object hints are shown when the mouse pointer remains over an object. This
feature must be enabled in the main settings notebook.
:p.
You may add hints for your action objects by adding the necessary information to
the main hint database. This database is a normal INI file located in the :hp2.\help:ehp2.
directory of the WPS-wizard installation.
:p.
:hp2.Locating the hint database:ehp2.
:p.
The hint database may be located after the WPS-wizard DLL is found.
:p.
:xmp.
/* Check for WPS-Wizard installation */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs
ret=SysQueryClassList('classes.')
found=0
DO cls=1 to classes.0
  theclass=WORD(classes.cls, 1)
  thePath=SUBWORD(classes.cls, 2)
  IF TRANSLATE(theClass)='CWOBJECT' THEN DO
    found=1	
    LEAVE
  END
END

/* Extract the base path */
basepath=FILESPEC('d', thePath)||FILESPEC('p', thePath)

/* Check if DLL is in a sub dir */
IF TRANSLATE(RIGHT(basepath, 5)) = "\DLL\" THEN
DO
  /* Yes, so walk one dir up to the real installation base */
  basePath=LEFT(basepath, LENGTH(basePath)-4)
END

/* Add help dir and hint database name */
hintIni=basepath||"help\OBJHINTS.INI"

:exmp.

:p.
:hp2.Adding the hint to the database:ehp2.
:p.
:xmp.

hintText="This is the hint text for object <MY_NEW_OBJ>"

/* Write hint into database */
ret=SysIni(hintIni, "<MY_NEW_OBJ>", "hint", hintText)

:exmp.


