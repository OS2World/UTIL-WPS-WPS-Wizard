.************ Folder scripts *********************************
:h2 res=5000.Folder scripts
:p.
Adding scripts to a folder is simply done by putting shadows of them or
program objects into the
script folder.
:p.
The configuration folder has a special ID :hp2.<SKRIPTS_WPFolder>:ehp2. so WPS-Wizard may find it.
Similar to context menus you may have different script folders for different folder classes in
addition to the main folder scripts. These additional configuration folders must have an ID
composed the following way :hp2.<SKRIPTS_Folder-class-name>:ehp2..
:p.
For example

:ul.
:li.<SKRIPTS_CWMediaFolder>
:li.<SKRIPTS_CWAudioFolder>
:eul.

:p.
You may use the following REXX script to create a new script folder.

:xmp.
/* rexx - Create a new script configuration folder */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs


rc=SysCreateObject("CWMenuFolder","Skripts WPFolder", '<WP_DESKTOP>','HIDECLASSPAGE=1;OBJECTID=<SKRIPTS_WPFolder>', 'update')

IF rc=0 THEN DO
   SAY 'Error!'
   '@PAUSE'
   Exit
END
:exmp.
:p.
When a script is executed three parameters are given to it.

:ul.
:li.ARG(1): /EXECUTE
.br
You should always check this parameter to make sure the script executes in the right
context.
:li.ARG(2): a pointer to the folder object the script is executed from. This pointer may be used to access folder methods.
:li.ARG(3): a handle to the folders frame. This is a HWND which may be used with PM functions.
:eul.
