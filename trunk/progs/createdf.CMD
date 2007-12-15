/*                 Create the default WPDataFile config folder
 *
 *                   (C) Chris Wohlgemuth 2001
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

/* Get curent directory */
curdir = directory()


resdir=FILESPEC("drive", curdir)||FILESPEC("path", curdir)||'res'

say nlsStrings.1
setupstring='CLEARMENUCACHE=1;SHOWALLINTREEVIEW=YES;DEFAULTVIEW=ICON;ICONVIEW=NONFLOWED,MINI;OBJECTID=<MENU_WPDataFile>'
if SysCreateObject("CWMenuFolder",nlsStrings.4, "<WPSWIZ_CONFIG>",setupstring,"update")<>1 then
             do
                say ''
                say nlsStrings.2
             end

setupstring='EXENAME='curdir'\copyname.exe;PARAMETERS="%*";MINVIEW=HIDE;CCVIEW=YES;MINIMIZED=YES;ICONFILE='resdir'\MWDAT.ico'
if SysCreateObject("WPProgram", nlsStrings.5,"<MENU_WPDataFile>",setupstring,"update")<>1 then
              say nlsStrings.3
setupstring='EXENAME='curdir'\zipthis.cmd;PARAMETERS=%*;MINVIEW=HIDE;CCVIEW=YES;ICONFILE='resdir'\MWDAT.ico;MINIMIZED=YES'
if SysCreateObject("WPProgram",nlsStrings.6,"<MENU_WPDataFile>",setupstring,"update")<>1 then
              say nlsStrings.3

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