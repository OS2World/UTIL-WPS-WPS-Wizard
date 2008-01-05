/*
 * This file is (C) Chris Wohlgemuth 2002
 * It is part of the WPS-Wizard package
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
#define INCL_SW
#define INCL_REXXSAA
#define INCL_WIN
#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_GPI

#include <os2.h>
#include "newfileDlg.h"
#include "except.h"
#include <rexxsaa.h>                   /* needed for RexxStart()     */
#include <stdio.h>
#include <stdlib.h>
#include <sys\stat.h>
#include "defines.h"

#include "menufolder.hh"

typedef ULONG (APIENTRY *PFNWPSFUNC)(WPObject* ,...);

ULONG calculateHash(char * theString);
ULONG calculateHash2(char * theString);
void wpObjectError(char *chrMethod);
//void parameterError(char *chrMethod);
void numParameterError(char *chrMethod, ULONG ulReq, ULONG ulFound);
void rxerrMethodNameError(char *chrRexxFunction, char *chrMethodName);

/*
  This is the handling routine for the "WPSWizCallWPSClsFunc" Rexx function.
 */
ULONG EXPENTRY rxCallWPSClassFunc(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring)
{
  char text[200];
  ULONG ulHash;
  ULONG ulReturn;
  WPObject* wpObject;
  WPFileSystem* wpFSObject;


  /* At least WPObject* and name of method */
  if(argc<2)
    return 40;

  TRY_LOUD(RX) {

    ulHash=calculateHash(argv[0].strptr)<<8;
    ulHash+=calculateHash2(argv[0].strptr);
    ulReturn=0;

    wpObject=(WPObject*)atol(argv[1].strptr);
    if(somIsObj(wpObject)) {
      PFNWPSFUNC procAddr;

      if((procAddr=(PFNWPSFUNC) somResolveByName(wpObject, argv[0].strptr))==NULL) {

        sprintf(retstring->strptr, "ERROR_METHOD:");
        retstring->strlength=strlen(retstring->strptr);
      }
      else {
        switch(ulHash)
          {
            /* WPS method without a parameter returning ULONG or WPObject* */
            /* WPObject */
#if 0
          case 0x512c: /* somIsObj                */               
            {
              sprintf(retstring->strptr,"%d",procAddr(wpObject));
              retstring->strlength=strlen(retstring->strptr);
              break;            
            }
            /* Functions without params returning a string */
            /* WPFileSystem */
          case 0x172a: /* somGetname             */
            {
              sprintf(retstring->strptr,"%s",procAddr(wpObject));
              retstring->strlength=strlen(retstring->strptr);
              break;
            } 
#endif
            /* Functions with one (ULONG) param returning ULONG, BOOL or WPObject* */
          case 0x3601: /* wpclsQueryObject */
            {
              ULONG ulParam1;
              
              if(argc<3) {
                numParameterError(argv[0].strptr, 3, argc);
                ulReturn=40;
                break;
              }
              ulParam1=atol(argv[2].strptr);
              sprintf(retstring->strptr,"%d",procAddr(wpObject, ulParam1));
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
          default:
            rxerrMethodNameError("WPSWizCallWPSClsFunc", argv[0].strptr);
            ulReturn=40;
          }/* switch */
      }/* else if((procAddr=(PFNWPSFUNC) somResolveByName(wpObject, argv[1].strptr))==NULL) */
    }/* if(somIsObj(wpObject)) */
    else {
      /* wpObject is not valid */
      wpObjectError(argv[0].strptr);
      ulReturn=40;
    }
    
    /*  sprintf(text, "%s Hash: %x, Hash2: %x",argv[0].strptr, ulHash, calculateHash2(argv[0].strptr));
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "",
        1234, MB_OK|MB_MOVEABLE);
        */ 
  }
  CATCH(RX)
    {}END_CATCH;

  return ulReturn;
}

/*
0x3601: wpclsQueryObject

 */
