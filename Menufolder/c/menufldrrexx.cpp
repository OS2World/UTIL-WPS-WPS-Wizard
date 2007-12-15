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
#include "newfile.h"
#include "newfileDlg.h"
#include "except.h"
#include <rexxsaa.h>                   /* needed for RexxStart()     */
#include <stdio.h>
#include <stdlib.h>
#include <sys\stat.h>
#include "defines.h"

#define  INVALID_ROUTINE 40            /* Raise Rexx error           */
#define  VALID_ROUTINE    0            /* Successful completion      */

#define MAX_VAR_LEN  270
 
typedef struct RxStemData {
    SHVBLOCK shvb;                     /* Request block for RxVar    */
    CHAR varname[MAX_VAR_LEN];         /* Buffer for the variable    */
                                       /* name                       */
    ULONG stemlen;                     /* Length of stem.            */
 } RXSTEMDATA;

#if 0
typedef struct RxStemData {
    SHVBLOCK shvb;                     /* Request block for RxVar    */
    CHAR ibuf[IBUF_LEN];               /* Input buffer               */
    CHAR varname[MAX];                 /* Buffer for the variable    */
                                       /* name                       */
    CHAR stemname[MAX];                /* Buffer for the variable    */
                                       /* name                       */
    ULONG stemlen;                     /* Length of stem.            */
    ULONG vlen;                        /* Length of variable value   */
    ULONG j;                           /* Temp counter               */
    ULONG tlong;                       /* Temp counter               */
    ULONG count;                       /* Number of elements         */
                                       /* processed                  */
} RXSTEMDATA;
#endif

#include "menufolder.hh"

extern HWND g_hwndRexxError;

typedef ULONG (APIENTRY *PFNWPSFUNC)(WPObject* ,...);

void _Optlink rexxThreadFunc (void *arg)
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;
  PREXXTHREADPARMS tPt;
  WPObject *thisPtr;
  HWND hwnd;

  tPt=(PREXXTHREADPARMS) arg;
  if(!tPt)
    return;
  thisPtr=(WPObject*)tPt->thisPtr;    //Pointer auf CWMediaFolder-Object
  if(!somIsObj(thisPtr))
    return;

  hwnd=tPt->hwndFrame;

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {
      char text[1024];      
      
      RXSTRING arg[3];                    /* argument string for REXX  */
      RXSTRING rexxretval;                /* return value from REXX    */
      APIRET   rc;                        /* return code from REXX     */
      SHORT    rexxrc = 0;                /* return code from function */
      char chrThis[20];
      char chrHwnd[20];
      char chrRexxEnv[20]="/EXECUTE";

      if(tPt->chrCommand[0])
        strncpy(chrRexxEnv, tPt->chrCommand, sizeof(chrRexxEnv));

      chrRexxEnv[sizeof(chrRexxEnv)-1]=0;

      sprintf(chrThis, "%d", thisPtr);
      sprintf(chrHwnd, "%d", hwnd);
      
      /* By setting the strlength of the output RXSTRING to zero, we   */
      /* force the interpreter to allocate memory and return it to us. */
      /* We could provide a buffer for the interpreter to use instead. */
      rexxretval.strlength = 0L;          /* initialize return to empty*/
      
      MAKERXSTRING(arg[0], chrRexxEnv, strlen(chrRexxEnv));/* create input argument     */
      MAKERXSTRING(arg[1], chrThis, strlen(chrThis));/* create input argument     */              
      MAKERXSTRING(arg[2], chrHwnd, strlen(chrHwnd));/* create input argument     */

      TRY_LOUD(RX_START) {
        /* Here we call the interpreter.  We don't really need to use    */
        /* all the casts in this call; they just help illustrate         */
        /* the data types used.                                          */
        rc=RexxStart((LONG)       3,             /* number of arguments   */
                     (PRXSTRING)  &arg,          /* array of arguments    */
                     (PSZ)        tPt->rexxSkript,/* name of REXX file     */
                     (PRXSTRING)  0,             /* No INSTORE used       */
                     (PSZ)        "WIZREXX",         /* Command env. name     */
                     (LONG)       RXSUBROUTINE,  /* Code for how invoked  */
                     (PRXSYSEXIT) 0,             /* No EXITs on this call */
                     (PSHORT)     &rexxrc,       /* Rexx program output   */
                     (PRXSTRING)  &rexxretval ); /* Rexx program output   */
        
        if(rc) {
          sprintf(text,"Error in the Rexx skript %s", 
                  tPt->rexxSkript);
          WinSendMsg(WinWindowFromID(g_hwndRexxError, IDLB_REXXERROR), LM_INSERTITEM, MPFROMSHORT(LIT_END), text);

          sprintf(text,"Get more information with 'help REX%04d'.", rc*-1);
          WinSendMsg(WinWindowFromID(g_hwndRexxError, IDLB_REXXERROR), LM_INSERTITEM, MPFROMSHORT(LIT_END), text);

          WinShowWindow(g_hwndRexxError, TRUE);
          WinSetWindowPos(g_hwndRexxError, HWND_TOP, 0, 0, 0, 0,SWP_ZORDER);
          //          WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "", 1234, MB_OK|MB_MOVEABLE|MB_ERROR);
        }
        DosFreeMem(rexxretval.strptr);          /* Release storage given to us by REXX.  */      
      }
      CATCH(RX_START)
        {}END_CATCH;
        
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  thisPtr->wpFreeMem((PBYTE)tPt);
}


#define HASHSIZE 101
ULONG calculateHash(char * theString)
{
  ULONG ulHash=0;

  if(theString)
    for(ulHash=0;*theString!='\0'; theString++)
      ulHash=*theString+31*ulHash;
  
  return ulHash%HASHSIZE;
}

#define HASHSIZE2 123
ULONG calculateHash2(char * theString)
{
  ULONG ulHash=0;

  if(theString)
    for(ulHash=0;*theString!='\0'; theString++)
      ulHash=*theString+37*ulHash;
  
  return ulHash%HASHSIZE;
}

void wpObjectError(char *chrMethod)
{
  char text[200];
  sprintf(text, "Method %s: object pointer is not valid.", chrMethod);

  WinSendMsg(WinWindowFromID(g_hwndRexxError, IDLB_REXXERROR), LM_INSERTITEM, MPFROMSHORT(LIT_END), text);
  WinShowWindow(g_hwndRexxError, TRUE);
  WinSetWindowPos(g_hwndRexxError, HWND_TOP, 0, 0, 0, 0,SWP_ZORDER);
#if 0
  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "Object error",
                1234, MB_OK|MB_MOVEABLE);
#endif
}

void numParameterError(char *chrMethod, ULONG ulReq, ULONG ulFound)
{
  char text[200];

  sprintf(text, "%s: wrong number of parameters.", chrMethod);
  WinSendMsg(WinWindowFromID(g_hwndRexxError, IDLB_REXXERROR), LM_INSERTITEM, MPFROMSHORT(LIT_END), text);

  sprintf(text, "Required: %d, found: %d", ulReq, ulFound);
  WinSendMsg(WinWindowFromID(g_hwndRexxError, IDLB_REXXERROR), LM_INSERTITEM, MPFROMSHORT(LIT_END), text);

  WinShowWindow(g_hwndRexxError, TRUE);
  WinSetWindowPos(g_hwndRexxError, HWND_TOP, 0, 0, 0, 0,SWP_ZORDER);
}

#define RXERR_METHODNAME_FORMAT "%s: \"%s\" is unknown."
void rxerrMethodNameError(char *chrRexxFunction, char *chrMethodName)
{
  char *text;

  if((text=(char*)malloc(strlen(chrRexxFunction)+strlen(chrMethodName)+strlen(RXERR_METHODNAME_FORMAT)))!=NULLHANDLE) {
    sprintf(text, RXERR_METHODNAME_FORMAT, chrRexxFunction, chrMethodName);
    
    WinSendMsg(WinWindowFromID(g_hwndRexxError, IDLB_REXXERROR), LM_INSERTITEM, MPFROMSHORT(LIT_END), text);
    WinShowWindow(g_hwndRexxError, TRUE);
    WinSetWindowPos(g_hwndRexxError, HWND_TOP, 0, 0, 0, 0,SWP_ZORDER);
    free(text);
  }
}

#define RXERR_PARMVALUE_FORMAT "%s: Parameter %d is invalid."
void rxerrParameterValueError(char *chrMethodName, ULONG ulParmNum)
{
  char *text;

  if((text=(char*)malloc(strlen(chrMethodName)+strlen(RXERR_PARMVALUE_FORMAT)+10))!=NULLHANDLE) {
    sprintf(text, RXERR_PARMVALUE_FORMAT, chrMethodName, ulParmNum);
    
    WinSendMsg(WinWindowFromID(g_hwndRexxError, IDLB_REXXERROR), LM_INSERTITEM, MPFROMSHORT(LIT_END), text);
    WinShowWindow(g_hwndRexxError, TRUE);
    WinSetWindowPos(g_hwndRexxError, HWND_TOP, 0, 0, 0, 0,SWP_ZORDER);
    free(text);
  }
}

#define RXERR_ERRORWITHMETHOD_FORMAT "%s: %s%s"
void rxerrPrintErrorTextWithMethod(char *chrMethodName, char *chrText1, char *chrText2)
{
  char *text;

  if((text=(char*)malloc(strlen(chrMethodName)+strlen(chrText1)+strlen(chrText2)+10))!=NULLHANDLE) {
    sprintf(text, RXERR_ERRORWITHMETHOD_FORMAT, chrMethodName, chrText1, chrText2);
    
    WinSendMsg(WinWindowFromID(g_hwndRexxError, IDLB_REXXERROR), LM_INSERTITEM, MPFROMSHORT(LIT_END), text);
    WinShowWindow(g_hwndRexxError, TRUE);
    WinSetWindowPos(g_hwndRexxError, HWND_TOP, 0, 0, 0, 0,SWP_ZORDER);
    free(text);
  }
}

#define RXERR_GENERALERROR_FORMAT "%s%s%s"
void rxerrPrintErrorText(char *chrText0, char *chrText1, char *chrText2)
{
  char *text;

  if((text=(char*)malloc(strlen(chrText0)+strlen(chrText1)+strlen(chrText2)+10))!=NULLHANDLE) {
    sprintf(text, RXERR_GENERALERROR_FORMAT, chrText0, chrText1, chrText2);
    
    WinSendMsg(WinWindowFromID(g_hwndRexxError, IDLB_REXXERROR), LM_INSERTITEM, MPFROMSHORT(LIT_END), text);
    WinShowWindow(g_hwndRexxError, TRUE);
    WinSetWindowPos(g_hwndRexxError, HWND_TOP, 0, 0, 0, 0,SWP_ZORDER);
    free(text);
  }
}

#if 0
void generalRexxError(char *chrMethod)
{
  char text[200];
  sprintf(text, "%s: parameter error.", chrMethod);

  WinSendMsg(WinWindowFromID(g_hwndRexxError, IDLB_REXXERROR), LM_INSERTITEM, MPFROMSHORT(LIT_END), text);
  WinShowWindow(g_hwndRexxError, TRUE);
  WinSetWindowPos(g_hwndRexxError, HWND_TOP, 0, 0, 0, 0,SWP_ZORDER);
}
#endif

LONG rxGetLongFromStem(RXSTRING args, ULONG ulTail, ULONG* ulError)
{
  char text[20];
  RXSTEMDATA ldp={0};

  *ulError=0;
  
                                       /* Initialize data area       */
  strcpy(ldp.varname, args.strptr);
  ldp.stemlen = args.strlength;
  strupr(ldp.varname);                 /* uppercase the name         */

  if (ldp.varname[ldp.stemlen-1] != '.')
    ldp.varname[ldp.stemlen++] = '.';

  /* Add tail number to stem */  
  sprintf(ldp.varname+ldp.stemlen, "%d", ulTail);

  ldp.shvb.shvnext = NULL;                           /* Only one request block */
  ldp.shvb.shvname.strptr = ldp.varname;             /* Var name               */
  ldp.shvb.shvname.strlength = strlen(ldp.varname);  /* RxString length        */
  ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;

  ldp.shvb.shvcode = RXSHV_SYFET;                    /* Get a value            */
  ldp.shvb.shvret = 0;

  /* The value field of the request block is empty so REXX will allocate memory for us
     which holds the value. */

  /* Now get the value */
  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN) {
    *ulError=1;
    return 0;      /* error on non-zero          */
  }

  if(sizeof(text) > ldp.shvb.shvvalue.strlength) {
    strncpy(text, ldp.shvb.shvvalue.strptr, ldp.shvb.shvvalue.strlength);
    text[ldp.shvb.shvvalue.strlength]=0;
  }
  else
    strncpy(text, ldp.shvb.shvvalue.strptr, sizeof(text));
  text[sizeof(text)-1]=0;

  /* Free mem allocated for us by REXX */
  DosFreeMem(ldp.shvb.shvvalue.strptr);

  return atol(text);
}

LONG rxGetLongFromStem2(RXSTRING args, char * chrTail, ULONG* ulError)
{
  char text[20];
  RXSTEMDATA ldp={0};

  *ulError=0;
  
                                       /* Initialize data area       */
  strcpy(ldp.varname, args.strptr);
  ldp.stemlen = args.strlength;
  strupr(ldp.varname);                 /* uppercase the name         */

  if (ldp.varname[ldp.stemlen-1] != '.')
    ldp.varname[ldp.stemlen++] = '.';

  /* Add tail  to stem */  
  strncat(ldp.varname+ldp.stemlen, chrTail, sizeof(ldp.varname)-ldp.stemlen);
  ldp.varname[sizeof(ldp.varname)-1]=0;

  ldp.shvb.shvnext = NULL;                           /* Only one request block */
  ldp.shvb.shvname.strptr = ldp.varname;             /* Var name               */
  ldp.shvb.shvname.strlength = strlen(ldp.varname);  /* RxString length        */
  ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;

  ldp.shvb.shvcode = RXSHV_SYFET;                    /* Get a value            */
  ldp.shvb.shvret = 0;

  /* The value field of the request block is empty so REXX will allocate memory for us
     which holds the value. */

  /* Now get the value */
  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN) {
    *ulError=1;
    return 0;      /* error on non-zero          */
  }

  if(sizeof(text) > ldp.shvb.shvvalue.strlength) {
    strncpy(text, ldp.shvb.shvvalue.strptr, ldp.shvb.shvvalue.strlength);
    text[ldp.shvb.shvvalue.strlength]=0;
  }
  else
    strncpy(text, ldp.shvb.shvvalue.strptr, sizeof(text));
  text[sizeof(text)-1]=0;

  /* Free mem allocated for us by REXX */
  DosFreeMem(ldp.shvb.shvvalue.strptr);

  return atol(text);
}

LONG rxSetLongInStem(RXSTRING args, ULONG ulTail, LONG lValue)
{
  char text[20];
  RXSTEMDATA ldp={0};

  sprintf(text, "%ld", lValue);
                                       /* Initialize data area       */
  strcpy(ldp.varname, args.strptr);
  ldp.stemlen = args.strlength;
  strupr(ldp.varname);                 /* uppercase the name         */

  if (ldp.varname[ldp.stemlen-1] != '.')
    ldp.varname[ldp.stemlen++] = '.';

  /* Add tail number to stem */  
  sprintf(ldp.varname+ldp.stemlen, "%d", ulTail);

  ldp.shvb.shvnext = NULL;                           /* Only one request block */
  ldp.shvb.shvname.strptr = ldp.varname;             /* Var name               */
  ldp.shvb.shvname.strlength = strlen(ldp.varname);  /* RxString length        */
  ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;
  /* Set the value of the var */
  ldp.shvb.shvvalue.strptr = text;
  ldp.shvb.shvvalue.strlength = strlen(text);
  ldp.shvb.shvvaluelen = ldp.shvb.shvvalue.strlength;
  ldp.shvb.shvcode = RXSHV_SET;
  ldp.shvb.shvret = 0;
  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN) {
    return INVALID_ROUTINE;      /* error on non-zero          */
  }

  return VALID_ROUTINE;
}

/*************************************************************/
/*                                                           */
/* This function returns a string from a stem. The caller    */
/* must free the returned string with free()                 */
/*                                                           */
/*************************************************************/
PSZ rxGetStringFromStem(RXSTRING args, ULONG ulTail, ULONG* ulError)
{
  RXSTEMDATA ldp={0};
  char *pText;  
  *ulError=0;
  
                                       /* Initialize data area       */
  strcpy(ldp.varname, args.strptr);
  ldp.stemlen = args.strlength;
  strupr(ldp.varname);                 /* uppercase the name         */

  if (ldp.varname[ldp.stemlen-1] != '.')
    ldp.varname[ldp.stemlen++] = '.';

  /* Add tail number to stem */  
  sprintf(ldp.varname+ldp.stemlen, "%d", ulTail);

  ldp.shvb.shvnext = NULL;                           /* Only one request block */
  ldp.shvb.shvname.strptr = ldp.varname;             /* Var name               */
  ldp.shvb.shvname.strlength = strlen(ldp.varname);  /* RxString length        */
  ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;

  ldp.shvb.shvcode = RXSHV_SYFET;                    /* Get a value            */
  ldp.shvb.shvret = 0;

  /* The value field of the request block is empty so REXX will allocate memory for us
     which holds the value. */

  /* Now get the value */
  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN) {
    *ulError=1;
    return 0;      /* error on non-zero          */
  }

  if((pText=(char*)malloc(ldp.shvb.shvvalue.strlength+1))!=NULLHANDLE)
     {
       strncpy(pText, ldp.shvb.shvvalue.strptr, ldp.shvb.shvvalue.strlength);
       pText[ldp.shvb.shvvalue.strlength]=0;
     }
  /* Free mem allocated for us by REXX */
  DosFreeMem(ldp.shvb.shvvalue.strptr);

  return pText;
}

/*************************************************************/
/*                                                           */
/* This function returns a string from a stem. The caller    */
/* must free the returned string with free()                 */
/*                                                           */
/*************************************************************/
PSZ rxGetStringFromStem2(RXSTRING args, char *chrTail, ULONG* ulError)
{
  RXSTEMDATA ldp={0};
  char *pText;  
  *ulError=0;
  
                                       /* Initialize data area       */
  strcpy(ldp.varname, args.strptr);
  ldp.stemlen = args.strlength;
  strupr(ldp.varname);                 /* uppercase the name         */

  if (ldp.varname[ldp.stemlen-1] != '.')
    ldp.varname[ldp.stemlen++] = '.';

  /* Add tail to stem */  
  sprintf(ldp.varname+ldp.stemlen, "%s", chrTail);

  ldp.shvb.shvnext = NULL;                           /* Only one request block */
  ldp.shvb.shvname.strptr = ldp.varname;             /* Var name               */
  ldp.shvb.shvname.strlength = strlen(ldp.varname);  /* RxString length        */
  ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;

  ldp.shvb.shvcode = RXSHV_SYFET;                    /* Get a value            */
  ldp.shvb.shvret = 0;

  /* The value field of the request block is empty so REXX will allocate memory for us
     which holds the value. */

  /* Now get the value */
  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN) {
    *ulError=1;
    return 0;      /* error on non-zero          */
  }

  if((pText=(char*)malloc(ldp.shvb.shvvalue.strlength+1))!=NULLHANDLE)
     {
       strncpy(pText, ldp.shvb.shvvalue.strptr, ldp.shvb.shvvalue.strlength);
       pText[ldp.shvb.shvvalue.strlength]=0;
     }
  /* Free mem allocated for us by REXX */
  DosFreeMem(ldp.shvb.shvvalue.strptr);

  return pText;
}


LONG rxSetStringInStem(RXSTRING args, ULONG ulTail, char* chrTailString, char* theString)
{
  RXSTEMDATA ldp={0};
  
  if(!theString)
    theString="";

                                       /* Initialize data area       */
  strcpy(ldp.varname, args.strptr);
  ldp.stemlen = args.strlength;
  strupr(ldp.varname);                 /* uppercase the name         */

  if (ldp.varname[ldp.stemlen-1] != '.')
    ldp.varname[ldp.stemlen++] = '.';

  /* add tailString if any, else only tail */
  if(chrTailString) {
    sprintf(ldp.varname+ldp.stemlen, "%d.%s", ulTail, chrTailString);
  }
  else   /* Add tail number to stem */
    sprintf(ldp.varname+ldp.stemlen, "%d", ulTail);

  ldp.shvb.shvnext = NULL;                           /* Only one request block */
  ldp.shvb.shvname.strptr = ldp.varname;             /* Var name               */
  ldp.shvb.shvname.strlength = strlen(ldp.varname);  /* RxString length        */
  ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;
  /* Set the value of the var */
  ldp.shvb.shvvalue.strptr = theString;
  ldp.shvb.shvvalue.strlength = strlen(theString);
  ldp.shvb.shvvaluelen = ldp.shvb.shvvalue.strlength;
  ldp.shvb.shvcode = RXSHV_SYSET;
  ldp.shvb.shvret = 0;

  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN) {
    return INVALID_ROUTINE;      /* error on non-zero          */
  }

  return VALID_ROUTINE;
}

LONG rxSetLongInRexxVar(char* chrVar, ULONG ulVal)
{
  RXSTEMDATA ldp={0};
  char valString[34];

  if(!chrVar || !strlen(chrVar))
    return INVALID_ROUTINE;      /* error on non-zero          */

                                         /* Initialize data area       */
  strcpy(ldp.varname, chrVar);
  ldp.stemlen = strlen(chrVar);
  strupr(ldp.varname);                 /* uppercase the name         */



  ldp.shvb.shvnext = NULL;                           /* Only one request block */
  ldp.shvb.shvname.strptr = ldp.varname;             /* Var name               */
  ldp.shvb.shvname.strlength = strlen(ldp.varname);  /* RxString length        */
  ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;
  /* Set the value of the var */
  ldp.shvb.shvvalue.strptr =   _ultoa(ulVal, valString, 10);  /* Make a string from the var */
  ldp.shvb.shvvalue.strlength = strlen(valString);
  ldp.shvb.shvvaluelen = ldp.shvb.shvvalue.strlength;
  ldp.shvb.shvcode = RXSHV_SYSET;
  ldp.shvb.shvret = 0;

  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN) {
    return INVALID_ROUTINE;      /* error on non-zero          */
  }

  return VALID_ROUTINE;
}



LONG rxSetStringInRexxVar(char* chrVarName, char* theString)
{
  RXSTEMDATA ldp={0};
  
  if(!theString)
    theString="";
                                       /* Initialize data area       */
  strncpy(ldp.varname, chrVarName, sizeof(ldp.varname));
  ldp.varname[sizeof(ldp.varname)-1]=0;
  ldp.stemlen = strlen(chrVarName);
  strupr(ldp.varname);                 /* uppercase the name         */

  ldp.shvb.shvnext = NULL;                           /* Only one request block */
  ldp.shvb.shvname.strptr = ldp.varname;             /* Var name               */
  ldp.shvb.shvname.strlength = strlen(ldp.varname);  /* RxString length        */
  ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;
  /* Set the value of the var */
  ldp.shvb.shvvalue.strptr = theString;
  ldp.shvb.shvvalue.strlength = strlen(theString);
  ldp.shvb.shvvaluelen = ldp.shvb.shvvalue.strlength;
  ldp.shvb.shvcode = RXSHV_SYSET;
  ldp.shvb.shvret = 0;

  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN) {
    return INVALID_ROUTINE;      /* error on non-zero          */
  }

  return VALID_ROUTINE;
}


/*
WinSendMsg(hwndContainer,CM_QUERYRECORDEMPHASIS, MPFROMSHORT(CMA_FIRST),
                                              MPFROMSHORT(CRA_SELECTED));
                                              */














