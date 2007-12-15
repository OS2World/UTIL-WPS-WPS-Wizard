/*                 Uninstall
 *
 *                   (C) Chris Wohlgemuth 1999-2007
*/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

 call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
 call SysLoadFuncs

nlsStrings.=''
call getStrings

'cls'



/* Get curent directory */
curdir = directory()

/* Check if we are uninstalling. This is a two step procedure. First	
unreplace all classes so they may be safely deleted after a WPS restart.	 */
rc=SysIni(, "WPSWizard", "uninstalling")


if rc<>"yes" THEN DO

say nlsStrings.1
say ''
say '>'
parse value SysCurPos() with aktzeile aktspalte
newposition=SysCurPos(aktzeile-1,2)
pull

/* Clear screen */
'cls'

PARSE SOURCE . . theScript

setupstring="EXENAME="||theScript||";MINIMIZED=YES;MINVIEW=HIDE;OBJECTID=<WPSWIZ_UNINSTALL_PART2>;"
rc=SysIni(, "PM_InstallObject", "WPS-Wizard uninstallation Part 2;WPProgram;<WP_START>", setupstring)
rc=SysIni(, "PM_InstallObject", "WPS-Wizard uninstallation Part 2;WPProgram;<WP_START>;UPDATE", "OPEN=DEFAULT")

'bin\repclass WPFolder CWFolder u'
'bin\repclass WPDataFile CWDataFile u'
'bin\repclass WPDisk CWDisk u'
'bin\repclass WPShadow CWShadow u'
'bin\repclass WPObject CWObject u'
'bin\repclass MMImage CWThemeMMImage u'

say nlsStrings.7||'CWLaunchArea'
		if SysDeregisterObjectClass("CWLaunchArea") <>1 then
		do
		  say nlsStrings.8||'CWLaunchArea'
		 end

say nlsStrings.7||'CWMenuFolder'
		if SysDeregisterObjectClass("CWMenuFolder") <>1 then
		do
		  say nlsStrings.8||'CWMenuFolder'
		 end

say nlsStrings.7||'CWWizzSettings'
		if SysDeregisterObjectClass("CWWizzSettings") <>1 then
			say nlsStrings.8||'CWWizzSettings'


say nlsStrings.7||'CWThemeTransient'
		if SysDeregisterObjectClass("CWThemeTransient") <>1 then
		do
		  say nlsStrings.8||'CWFolder'
		 end

say nlsStrings.7||'CWShadow'
		if SysDeregisterObjectClass("CWShadow") <>1 then
		do
		  say nlsStrings.8||'CWShadow'
		 end

say nlsStrings.7||'CWFolder'
		if SysDeregisterObjectClass("CWFolder") <>1 then
		do
		  say nlsStrings.8||'CWFolder'
		 end

say nlsStrings.7||'CWDataFile'
		if SysDeregisterObjectClass("CWDataFile") <>1 then
		do
		  say nlsStrings.8||'CWDataFile'
		 end

say nlsStrings.7||'CWDisk'
		if SysDeregisterObjectClass("CWDisk") <>1 then
		do
		  say nlsStrings.8||'CWDisk'
		 end

say nlsStrings.7||'CWObject'
		if SysDeregisterObjectClass("CWObject") <>1 then
		do
		  say nlsStrings.8||'CWObject'
		 end

say nlsStrings.7||'CWThemeMMImage'
		if SysDeregisterObjectClass("CWThemeMMImage") <>1 then
		do
		  say nlsStrings.8||'CWThemeMMImage'
		 end

rc=SysIni(, "WPSWizard", "uninstalling", "yes")

say ''
SAY nlsStrings.14||' '||nlsStrings.15

say ''
say nlsStrings.13
say ''
say '>'
parse value SysCurPos() with aktzeile aktspalte
newposition=SysCurPos(aktzeile-1,2)
pull


'@bin\wpsreset.exe'
exit
END

/* ******************************************* */
/* Part 2 from here                            */


call SysSleep 5

call SysDestroyObject "<WPSWIZ_UNINSTALL_PART2>"
rc=SysIni(, "WPSWizard", "uninstalling", "DELETE:")


say nlsStrings.2||curdir
say ''

say nlsStrings.9
rc=SysDestroyObject('<MENUITEMS_FOLDER>')
rc=SysDestroyObject('<MENU_WPDataFile>')
rc=SysDestroyObject('<MENU_WPFolder>')
rc=SysDestroyObject('<WPSWIZ_UNINSTALL>')

say nlsStrings.10
/* toolbar buttons */
rc=SysDestroyObject('<WPSWIZ_TBIOPENPARENT>')
rc=SysDestroyObject('<WPSWIZ_TBISETTINGS>')
rc=SysDestroyObject('<WPSWIZ_TBICMDWIN>')
rc=SysDestroyObject('<WPSWIZ_TBICMDWINWIDE>')
rc=SysDestroyObject('<WPSWIZ_TBIREFRESH>')
rc=SysDestroyObject('<WPSWIZ_TBIDETAILS>')
rc=SysDestroyObject('<WPSWIZ_TBIICON>')
rc=SysDestroyObject('<WPSWIZ_TBICMDFULL>')
rc=SysDestroyObject('<WPSWIZ_TBISELECTALL>')
rc=SysDestroyObject('<WPSWIZ_TBIDESELECTALL>')
rc=SysDestroyObject('<WPSWIZ_TBITEMFOLDER>')
/* Toolbar folders */
rc=SysDestroyObject('<WPSWIZ_FLDRACTION>')
rc=SysDestroyObject('<WPSWIZ_FLDRLAUNCHPAD>')

/* Gadgets */
rc=SysDestroyObject('<WPSWIZ_ZINFOGADGET>')
rc=SysDestroyObject('<WPSWIZ_DRIVEINFOGADGET>')
rc=SysDestroyObject('<WPSWIZ_SYSINFOGADGET>')
rc=SysDestroyObject('<WPSWIZ_TIMEGADGET>')
rc=SysDestroyObject('<WPSWIZ_GADGETFOLDER>')

say nlsStrings.3
if SysDestroyObject('<WPSWIZ_SETTINGS>') <>1 then
	say nlsStrings.4

say nlsStrings.5
if SysDestroyObject('<WPSWIZ_TUTORIAL>') <>1 then
	say nlsStrings.6


/* Main folder */
if SysDestroyObject('<WPSWIZ_FOLDER>') <>1 then
	say 'Can''t delete the WPS-Wizard folder!'

/*
say ''
/* The WPS will be restarted now. */
SAY nlsStrings.14

say ''
say nlsStrings.13
say ''
say '>'
parse value SysCurPos() with aktzeile aktspalte
newposition=SysCurPos(aktzeile-1,2)
pull

'@bin\wpsreset.exe'

say ''
say nlsStrings.12
say ''
say '>'
parse value SysCurPos() with aktzeile aktspalte
newposition=SysCurPos(aktzeile-1,2)
pull
*/

exit

/* The following subroutine was appended while building the distribution:

GetStrings:
nlsStrings.1="..."
...
return

The routine adds the NLS strings to this script. For creating other NLS-versions
translate the files in /nls and change the makefile to include the new NLS-version.
*/

