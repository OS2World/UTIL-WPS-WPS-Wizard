/*
 * This file is (C) Chris Wohlgemuth 1999/2001
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
#define INCL_DOS
#define INCL_DOSDEVIOCTL
#define INCL_DOSERRORS
#define INCL_WIN
#define INCL_WINWORKPLACE

#include <os2.h>

//#include "newfile.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>//for strcpy
#include <stdio.h>   // for sprintf
#include <stdlib.h>

    #ifndef BS_NOTEBOOKBUTTON
        #define BS_NOTEBOOKBUTTON          8L
    #endif


#if  __cplusplus
extern "C" {
#endif

/****************************************************
 *                                                  *
 * This funktion returns the running OS version:    *
 *                                                  *
 * 30: Warp 3, 40 Warp 4 etc.                       *
 *                                                  *
 ****************************************************/
ULONG cwQueryOSRelease(void)
{
  static ULONG ulVersionMinor=0;

  if(!ulVersionMinor)
    if(DosQuerySysInfo(QSV_VERSION_MINOR, QSV_VERSION_MINOR, &ulVersionMinor, sizeof(ulVersionMinor)))
      ulVersionMinor=30;/* Default Warp 3 */

  return ulVersionMinor;
}

/****************************************************
 *                                                  *
 * This function moves the default buttons off      *
 * notebook pages on Warp 4.                        *
 * usDelta specifies the units all controls have    *
 * to be moved down. usID is the ID of the last     *
 * button that has to be moved on the main frame    *
 * of the notebook. This value is a threshold.      *
 *                                                  *
 ****************************************************/
BOOL cwMoveNotebookButtonsWarp4(HWND hwndDlg, USHORT usID, USHORT usDelta)
{
  if (cwQueryOSRelease()>=40) {
    HENUM henum;
    HWND hwnd;
    
    /* Move the default notebook buttons */
    if((henum=WinBeginEnumWindows(hwndDlg))!=NULLHANDLE) {
      while((hwnd=WinGetNextWindow(henum))!=NULLHANDLE) {
        if(WinQueryWindowUShort(hwnd,QWS_ID)<=usID)
          WinSetWindowBits(hwnd, QWL_STYLE,
                           BS_NOTEBOOKBUTTON, BS_NOTEBOOKBUTTON);
        else {
          SWP swp;
          POINTL ptl= {0};
          ptl.y=usDelta;

          WinMapDlgPoints(hwndDlg, &ptl, 1, TRUE);
          /* Move all other controls */
          if(WinQueryWindowPos(hwnd, &swp))
            WinSetWindowPos(hwnd, NULLHANDLE, swp.x, swp.y-ptl.y,0,0, SWP_MOVE);
        }
      }
      WinEndEnumWindows(henum);
    }
  }
  return TRUE;
}

BOOL winhAssertWarp4Notebook(HWND hwndDlg,
                             USHORT usID,
                             ULONG ulDelta)
{
  return cwMoveNotebookButtonsWarp4(hwndDlg, usID, (USHORT) ulDelta);
}


void sendCommandToObject(char* chrObject, char* command)
{
  HOBJECT hObject;

  hObject=WinQueryObject(chrObject);

  if(hObject!=NULLHANDLE) {
    WinSetObjectData(hObject, command);
  }
}

static USHORT queryShiftState(void)
{
    USHORT     usRc=0;
    HFILE      hf;
    ULONG      ulAction;

    if (NO_ERROR==DosOpen("KBD$", &hf, &ulAction, 0, FILE_NORMAL,
                          OPEN_ACTION_OPEN_IF_EXISTS, /* fsOpenFlags */
                          OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE,
                          NULLHANDLE))
      {
        SHIFTSTATE      ss;
        ULONG           cbDataLen = sizeof(ss);
        
        if (NO_ERROR == DosDevIOCtl(hf, IOCTL_KEYBOARD, KBD_GETSHIFTSTATE,
                                    NULL, 0, NULL, &ss, cbDataLen, &cbDataLen))
          {
            usRc=ss.fsState;
          }/* DosDevIOCtl() */
        DosClose(hf);
      }/* DosOpen() */

    return usRc;
}

/*
  This function returns true if the Alt and the Ctrl keys are currently pressed.
 */
BOOL SysQueryAltCtrlPressed(void)
{
  /* Either Alt and Ctrl key */
  return ((queryShiftState() & 12)!=0);
}


#if __cplusplus
}
#endif

#if 0
PSZ substitutePlaceHolder(PSZ orgPSZ, PSZ pathPSZ, char* resultString, int iSize)
{ //Replace %* in orgPSZ-String.
	char * pBuchst;
	
    *resultString=0;
   
	if((pBuchst=strchr(orgPSZ,'%'))!=NULL){
      if(*(pBuchst+1)=='*') {
        if((iSize-(pBuchst-orgPSZ))>0)
          strncat(resultString,orgPSZ,pBuchst-orgPSZ);
        pBuchst+=2;
        if((iSize-2-strlen(pathPSZ))>0)
          strcat(resultString,pathPSZ);
        if((iSize-strlen(pBuchst))>0)
          strcat(resultString,pBuchst);
		}
      else
        strncpy(resultString, orgPSZ, iSize-1);
	}
    else
      strncpy(resultString, orgPSZ, iSize-1);
	return resultString;	
}
#endif

PSZ deleteTilde(PSZ orgPSZ, char *resultString, int iSize)
{
	char * pBuchst;
	int iLen;

    strncpy(resultString, orgPSZ, iSize);
	if((pBuchst=strchr(orgPSZ,'~'))!=NULL){
      *pBuchst=0;
      strncpy(resultString, orgPSZ, iSize-1);
      iLen=strlen(resultString);
      strncat(resultString,pBuchst +1,iSize-iLen-1);
      *pBuchst='~';
	}
	return resultString;	
}









