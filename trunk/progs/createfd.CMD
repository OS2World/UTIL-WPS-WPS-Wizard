/*                 Create the default WPFolder config folder
 *
 *                   (C) Chris Wohlgemuth 2001-2002
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

/*
DO a=1 to nlsStrings.0
  SAY nlsStrings.a
END
*/

/* Get curent directory */
curdir = directory()

resdir=FILESPEC("drive", curdir)||FILESPEC("path", curdir)||'res'

/* "Creating configuration folder..." */
say nlsStrings.1

/* Title: "Menu for folders" */ 
setupstring='CLEARMENUCACHE=1;DEFAULTVIEW=ICON;SHOWALLINTREEVIEW=YES;ICONVIEW=NONFLOWED,MINI;OBJECTID=<MENU_WPFolder>'
if SysCreateObject("CWMenuFolder", nlsStrings.4, "<WPSWIZ_CONFIG>",setupstring,"update")<>1 then
             do
                say ''
                say nlsStrings.2
             end

/* Title: "Copy ~filename" */
setupstring='EXENAME='curdir'\copyname.exe;PARAMETERS="%*";MINVIEW=HIDE;CCVIEW=YES;MINIMIZED=YES;ICONFILE='resdir'\MWFOL.ico'
if SysCreateObject("WPProgram",nlsStrings.5,"<MENU_WPFolder>",setupstring,"update")<>1 then
                say nlsStrings.3
/* "~Zip this tree" */
setupstring='EXENAME='curdir'\ziptree.cmd;PARAMETERS="%*";MINVIEW=HIDE;CCVIEW=YES;ICONFILE='resdir'\MWFOL.ico;MINIMIZED=YES'
if SysCreateObject("WPProgram", nlsStrings.6, "<MENU_WPFolder>",setupstring,"update")<>1 then
                say nlsStrings.3
/* "~Drives" */
setupstring='SHADOWID=<WP_DRIVES>;OBJECTID=<WPSWIZ_MENU_DRIVES>'
if SysCreateObject("WPShadow", nlsStrings.7,"<MENU_WPFolder>",setupstring,"update")<>1 then
                say nlsStrings.3

/* Befehlszeilen Untermenu */
setupstring='DEFAULTVIEW=ICON;SHOWALLINTREEVIEW=YES;ICONVIEW=NONFLOWED,MINI;OBJECTID=<SUBMENU_CMDLINES>'
if SysCreateObject("CWMenuFolder",nlsStrings.8,"<MENU_WPFolder>",setupstring,"update") = 1 then
             do
	/* Build Menuitems */
	/* "OS/2-~Window" */
	setupstring='EXENAME=*;PARAMETERS=%;PROGTYPE=WINDOWABLEVIO;CCVIEW=YES;'
	if SysCreateObject("WPProgram",nlsStrings.9,"<SUBMENU_CMDLINES>",setupstring,"update")<>1 then
                say nlsStrings.3
	/* "OS/2-~Fullscreen" */
	setupstring='EXENAME=*;PARAMETERS=%;PROGTYPE=FULLSCREEN;CCVIEW=YES;'
	if SysCreateObject("WPProgram",nlsStrings.10,"<SUBMENU_CMDLINES>",setupstring,"update")<>1 then
                say nlsStrings.3
	/* "D~OS-Window" */
	setupstring='EXENAME=*;PARAMETERS=%;PROGTYPE=WINDOWEDVDM;CCVIEW=YES;'
	if SysCreateObject("WPProgram",nlsStrings.11,"<SUBMENU_CMDLINES>",setupstring,"update")<>1 then
                say nlsStrings.3
	/* "DOS-Full~screen" */
	setupstring='EXENAME=*;PARAMETERS=%;PROGTYPE=VDM;CCVIEW=YES;'
	if SysCreateObject("WPProgram",nlsStrings.12,"<SUBMENU_CMDLINES>",setupstring,"update")<>1 then
                say nlsStrings.3

            end

/* Script folder */
/* Title: "Scripts for folders" */ 
setupstring='CLEARMENUCACHE=1;SHOWALLINTREEVIEW=YES;DEFAULTVIEW=ICON;ICONVIEW=NONFLOWED,MINI;HIDECLASSPAGE=1;OBJECTID=<SKRIPTS_WPFolder>'
if SysCreateObject("CWMenuFolder", nlsStrings.13, "<WPSWIZ_CONFIG>",setupstring,"update")<>1 then
             do
                say ''
                say nlsStrings.2
             end
/* "How to ~use scripts" */
setupstring='EXENAME='curdir'\scripts;MINVIEW=HIDE;CCVIEW=YES;MINIMIZED=YES'
if SysCreateObject("WPProgram", nlsStrings.14, "<SKRIPTS_WPFolder>",setupstring,"update")<>1 then
                say nlsStrings.3
/* "Example ~1" */
setupstring='EXENAME='curdir'\example1.cmd;MINVIEW=HIDE;CCVIEW=YES;MINIMIZED=YES'
if SysCreateObject("WPProgram", nlsStrings.15, "<SKRIPTS_WPFolder>",setupstring,"update")<>1 then
                say nlsStrings.3
/* "Example ~2" */
setupstring='EXENAME='curdir'\example2.cmd;MINVIEW=HIDE;CCVIEW=YES;MINIMIZED=YES'
if SysCreateObject("WPProgram", nlsStrings.16, "<SKRIPTS_WPFolder>",setupstring,"update")<>1 then
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