/* rexx - Uninstall an icon theme
 
   This script is part of the icon theme package. An add on
   for WPS-Wizard.

   (c) Chris Wohlgemuth 2007-2008

*/

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs


call SysIni "USER" ,"IconTheme", "DELETE:"


exit
