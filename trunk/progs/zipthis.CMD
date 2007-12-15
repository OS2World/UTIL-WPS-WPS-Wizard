/*                 Zip the given file
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

PARSE ARG fileName

IF fileName="" THEN exit

IF SysFileTree(fileName, file.,'F')\=0 THEN exit

IF file.0\=1 THEN exit

'zip -9 -j "'filespec('N',fileName)'.zip" "'filename'"'

exit

