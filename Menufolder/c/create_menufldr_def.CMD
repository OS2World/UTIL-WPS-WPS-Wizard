/* */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs
SAY "Def file to create: "ARG(1)
deffile=ARG(1)
if ARG(1)="" then deffile="tst.def"

theVersion=VALUE("WPSWIZSVERSION", ,"OS2ENVIRONMENT")
if theVersion="" THEN theVersion="0.0.0"

/* File defining the DLL exports */
exportsfile="EXPORTS.DEF"

def="; MENUFOLDER.DEF: DEF-file for menufldr.dll"||'0d'x||'0a'x
def=def||";"||'0d'x||'0a'x
def=def||";                    (C) Chris Wohlgemuth 1996-"||WORD(DATE(), 3)  ||'0d'x||'0a'x
def=def||";/*"||'0d'x||'0a'x
def=def||"; * This program is free software; you can redistribute it and/or modify"||'0d'x||'0a'x
def=def||"; * it under the terms of the GNU General Public License as published by"||'0d'x||'0a'x
def=def||"; * the Free Software Foundation; either version 2, or (at your option)"||'0d'x||'0a'x
def=def||"; * any later version."||'0d'x||'0a'x
def=def||"; *"||'0d'x||'0a'x
def=def||"; * This program is distributed in the hope that it will be useful,"||'0d'x||'0a'x
def=def||"; * but WITHOUT ANY WARRANTY; without even the implied warranty of"||'0d'x||'0a'x
def=def||"; * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"||'0d'x||'0a'x
def=def||"; * GNU General Public License for more details."||'0d'x||'0a'x
def=def||"; *"||'0d'x||'0a'x
def=def||"; * You should have received a copy of the GNU General Public License"||'0d'x||'0a'x
def=def||"; * along with this program; see the file COPYING.  If not, write to"||'0d'x||'0a'x
def=def||"; * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA."||'0d'x||'0a'x
def=def||"; */"||'0d'x||'0a'x
def=def||";/*"||'0d'x||'0a'x
def=def||"; * If you need another license for your project/product contact me at"||'0d'x||'0a'x
def=def||"; * "||'0d'x||'0a'x
def=def||"; * http://www.os2world.com/cdwriting"||'0d'x||'0a'x
def=def||"; * http://www.geocities.com/SiliconValley/Sector/5785/"||'0d'x||'0a'x
def=def||"; */"||'0d'x||'0a'x

def=def||'0d'x||'0a'x

def=def||"LIBRARY MENUFLDR INITGLOBAL"||'0d'x||'0a'x
def=def||"DESCRIPTION '$@#(C) C. Wohlgemuth 1996-"
def=def||WORD(DATE(),3)||":"||LEFT(theVersion,3)||"        #@##1## "||DATE("E")||" "||TIME("N")||"        My ;-)  ::::"||WORD(TRANSLATE(theVersion," ", "."),3)||"::@@Menu folder (WPS-Wizard) V"||theVersion||"'"||'0d'x||'0a'x



'@type NUL > 'deffile
/* Write header */
call lineout deffile, def
/* Write the exports */
DO WHILE LINES(exportsfile)
	call lineout deffile, LINEIN(exportsfile)
END
call stream exportsfile, "C", "close"
call stream deffile, "C", "close"

SAY "New MENUFOLDER.DEF created for V"||theVersion||"."
exit
