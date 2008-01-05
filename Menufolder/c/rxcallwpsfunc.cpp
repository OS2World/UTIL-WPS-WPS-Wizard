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
void numParameterError(char *chrMethod, ULONG ulReq, ULONG ulFound);
void rxerrMethodNameError(char *chrRexxFunction, char *chrMethodName);

void HlpWriteToTrapLog(const char* chrFormat, ...);

/*
  This is the handling routine for the "WPSWizCallWPSFunc" Rexx function.
 */
ULONG EXPENTRY rxCallWPSFunc(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring)
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

      if((procAddr=(PFNWPSFUNC) somResolveByName((SOMObject*)wpObject, argv[0].strptr))==NULL) {
        sprintf(retstring->strptr, "ERROR_METHOD:");
        retstring->strlength=strlen(retstring->strptr);
      }
      else {
        switch(ulHash)
          {
            /* WPS method without a parameter returning ULONG or WPObject* */

            /* WPObject */
          case 0x1c11: /* wpClose                 */
          case 0x064a: /* wpCnrRefreshDetails     */
          case 0x214d: /* wpHide                  */
          case 0x2948: /* wpIsDeleteable          */
          case 0x3608: /* wpIsLocked              */
          case 0x5d32: /* wpLockObject            */
          case 0x105d: /* wpQueryConfirmations    */
          case 0x5464: /* wpQueryConcurrentView   */
          case 0x2c4e: /* wpQueryDefaultView      */
          case 0x404d: /* wpQueryFolder           */
          case 0x4114: /* wpQueryHandle           */
          case 0x124c: /* wpQueryIcon             */
          case 0x2a15: /* wpQueryStyle            */
          case 0x0b0b: /* wpSaveDeferred          */
          case 0x0225: /* wpSaveImmediate         */
          case 0x4034: /* wpUnlockObject          */
            /* WPFileSystem */
          case 0x535e: /* wpQueryAttr             */
          case 0x435f: /* wpQueryDisk */
          case 0x050c: /* wpQueryEASize           */   
          case 0x2b63: /* wpQueryFileSize         */
            /* WPFolder */
          case 0x0900: /* wpQueryFldrDetailsClass */
          case 0x320c: /* wpQueryFldrFlags        */
          case 0x0c11: /* wpQueryFldrSortClass    */

          case 0x2548: /* wpIsCurrentDesktop      */
          case 0x3c4a: /* somGetClass             */
            {
              sprintf(retstring->strptr,"%d",procAddr(wpObject));
              retstring->strlength=strlen(retstring->strptr);
              break;            
            }
            /* Functions without params returning a string */
            /* WPFileSystem */
          case 0x1d5a:  /* wpQueryType           */
            /* WPObject */
          case 0x3406: /* somGetClassName        */
          case 0x172a: /* somGetname             */
          case 0x0553: /* wpQueryTitle           */
          case 0x3c35: /* wpQueryObjectID        */
            {
              //HlpWriteToTrapLog("%s, 2: %s\n", wpObject->wpQueryObjectID(), procAddr(wpObject));
              sprintf(retstring->strptr,"%s",procAddr(wpObject));
              //HlpWriteToTrapLog(retstring->strptr);
              //HlpWriteToTrapLog("\n");
              retstring->strlength=strlen(retstring->strptr);
              //HlpWriteToTrapLog("%d\n", retstring->strlength);
              break;
            } 
          case 0x4c06: /* somGetVersionNumbers */
            {
              LONG lMajor;
              LONG lMinor;
              
              procAddr(wpObject, &lMajor, &lMinor);

              sprintf(retstring->strptr,"%d.%d", lMajor, lMinor);
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
            /* Functions with one (ULONG) param returning ULONG, BOOL or WPObject* */
            /* WPObject */
          case 0x5c01: /* wpDelete              */
          case 0x162e: /* wpMoveObject          */
          case 0x191c: /* wpSetDefaultView      */
          case 0x1b09: /* wpSetFldrDetailsClass */
          case 0x1208: /* wpSetFldrSortClass    */
          case 0x0045: /* wpSwitchTo            */
            /* WPFolder */
          case 0x5a0d: /* wpDeleteContents      */

            /* WPShadow */
          case 0x000f: /* wpSetLinkToObject     */
          case 0x3853: /* wpQueryShadowedObject */
          case 0x053a: /* wpSetStyle */
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

            /* Functions with one (ULONG) param returning a string: */
          case 0x3100: /* wpQueryFldrFont */
            {
              ULONG ulParam1;
              
              if(argc<3) {
                numParameterError(argv[0].strptr, 3, argc);
                ulReturn=40;
                break;
              }
              ulParam1=atol(argv[2].strptr);
              sprintf(retstring->strptr,"%s",procAddr(wpObject, ulParam1));
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
            /* Function with two ULONG (or alike) params returning ULONG */
          case 0x4b31: /* wpCreateFromTemplate */
            /* WPFolder */
          case 0x4a58: /* wpCnrSetEmphasis     */
          case 0x311e: /* wpCopyObject         */
          case 0x611d: /* wpCreateShadowObject */
          case 0x612b: /* wpQueryContent       */
          case 0x043f: /* wpRefresh            */
          case 0x5202: /* wpModifyStyle        */
              {
                ULONG ulParam1;
                ULONG ulParam2;
                
                if(argc<4) {
                  numParameterError(argv[0].strptr, 4, argc);
                  ulReturn=40;
                  break;
                }
                ulParam1=atol(argv[2].strptr);
                ulParam2=atol(argv[3].strptr);
                
                sprintf(retstring->strptr,"%d", procAddr(wpObject, ulParam1, ulParam2));
                retstring->strlength=strlen(retstring->strptr);
                break;
              }
            /* Function with three ULONG (or alike) params returning ULONG */
          case 0x1a54: /* wpPopulate      */
          case 0x634d: /* wpViewObject    */
            {
              ULONG ulParam1;
              ULONG ulParam2;
              
              if(argc<5) {
                numParameterError(argv[0].strptr, 5, argc);
                ulReturn=40;
                break;
              }
              ulParam1=atol(argv[2].strptr);
              ulParam2=atol(argv[3].strptr);
              
              sprintf(retstring->strptr,"%d", procAddr(wpObject, ulParam1, ulParam2, atol(argv[4].strptr)));
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
            /* Function with a string parameter returning ULONG or BOOL */
          case 0x375e: /* wpSetType */
          case 0x5d21: /* wpSetObjectID */
            {
              if(argc<3) {
                numParameterError(argv[0].strptr, 3, argc);
                ulReturn=40;
                break;
              }
              sprintf(retstring->strptr,"%d", procAddr(wpObject, argv[2].strptr));
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
          default:
            rxerrMethodNameError("WPSWizCallWPSFunc", argv[0].strptr);
            ulReturn=40;
          }/* switch */
      }/* else if((procAddr=(PFNWPSFUNC) somResolveByName(wpObject, argv[1].strptr))==NULL) */
    }/* if(somIsObj(wpObject)) */
    else {
      /* wpObject is not valid */
      wpObjectError(argv[0].strptr);
      ulReturn=40;
    }
    
#if 0
    sprintf(text, "WPSFunc %s Hash: %x, Hash2: %x",argv[0].strptr, ulHash, calculateHash2(argv[0].strptr));
    WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "",
                  1234, MB_OK|MB_MOVEABLE);
#endif
       
  }
  CATCH(RX)
    {}END_CATCH;

  return ulReturn;
}
/*

Functions without params returning ULONG, BOOL or WPObject*:

wpQueryFileSize:       0x2b        0x63
wpQueryFldrFlags :     0x32       0x0c
wpQueryFolder:         0x40       0x4d
wpQueryHandle:         0x41      0x14
wpQueryStyle:          0x2a        hash2: 0x15     !!!!!
wpQueryEASize:         0x5         hash2: 0x0c     !!!!!
wpQueryDefaultView:    0x2c       0x4e
wpQueryConfirmations:  0x10       0x5d
wpQueryConcurrentView: 0x54       0x64
wpQueryAttr:           0x53       0x5e
wpIsDeleteable:        0x29       0x48
wpHide:                0x21       0x4d
wpClose:               0x1c       0x11
wpQueryDisk:           0x435f
0x0b:  wpSaveDeferred             0x0b 
0x2: wpSaveImmediate              0x25
0x3c: somGetClass                   hash2: 0x4a     !!!!!
0x4c: somGetVersionNumbers       hash2: 0x06

Functions without params returning a string:
0x5: wpQueryTitle     hash2: 0x53           !!!!!!
0x1d:  wpQueryType               0x5a
0x3c: wpQueryObjectID              hash2:   0x35      !!!!!!
0x34: somGetClassName          hash2: 0x06
0x17: somGetname               hash2: 0x2a

Functions with one (ULONG) param returning ULONG, BOOL or WPObject*:
wpSetDefaultView:      0x19      0x1c
wpSetLinkToObject:     0x0       0x0f
wpSetStyle:            0x5      hash2: 0x3a    !!!!!

somIsObj               0x512c




Done:
0x172a: somGetname
0x3c4a: somGetClass
0x3406: somGetClassName

0x311e: wpCopyObject
0x064a: wpCnrRefreshDetails
0x4a58: wpCnrSetEmphasis
0x4b31: wpCreateFromTemplate
0x611d: wpCreateShadowObject
0x5c01: wpDelete
0x5a0d: wpDeleteContents
0x2548: wpIsCurrentDesktop
0x2948: wpIsDeleteable        
0x3608: wpIsLocked
0x5d32: wpLockObject
0x162e: wpMoveObject
0x1a54: wpPopulate
0x535e: wpQueryAttr
0x5464: wpQueryConcurrentView
0x105d: wpQueryConfirmations 
0x612b: wpQueryContent
0x435f: wpQueryDisk
0x2c4e: wpQueryDefaultView
0x050c: wpQueryEASize
0x2b63: wpQueryFileSize
0x0900: wpQueryFldrDetailsClass
0x320c: wpQueryFldrFlags
0x3100: wpQueryFldrFont
0x0c11: wpQueryFldrSortClass
0x404d: wpQueryFolder
0x4114: wpQueryHandle
0x3c35: wpQueryObjectID
0x3853: wpQueryShadowedObject
0x2a15: wpQueryStyle
0x0553: wpQueryTitle
0x1d5a: wpQueryType
0x043f: wpRefresh
0x0b0b: wpSaveDeferred
0x0225: wpSaveImmediate
0x191c: wpSetDefaultView
0x1b09: wpSetFldrDetailsClass
0x1208: wpSetFldrSortClass
0x000f: wpSetLinkToObject
0x5d21: wpSetObjectID
0x053a: wpSetStyle
0x375e: wpSetType
0x0045: wpSwitchTo
0x4034: wpUnlockObject
0x634d: wpViewObject


***********************
To document:

0x124c: wpQueryIcon
0x5202: wpModifyStyle

**********************


Not yet done:
0x375e wpSetRealName

0x0112: wpCreateAnother

0x502e: wpCreateShadowObjectExt
0x5448: wpDisplayHelp
0x2033: wpIsDetailsColumnVisible
0x1139: wpIsSortAttribAvailable
0x0630: wpModifyFldrFlags

0x1a15: wpModuleForClass
0x0547: wpOpen
case 0x0758:  wpContainsFolders     

QueryActionButtons


 */


