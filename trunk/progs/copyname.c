#define INCL_DOSMISC       /* DOS Miscellaneous values */
#define INCL_DOSERRORS     /* DOS Error values         */
#define INCL_DOSFILEMGR
#define INCL_WINCLIPBOARD
#define INCL_PM

#include <os2.h>
#include <stdio.h>
 
void usage(char *progName) {
  printf("Program to copy a given string to the clipboard.\n");
  printf("To be used with WPS-Wizard.\n");
  printf("(c) Chris Wohlgemuth 2001\n");
  printf("http://www.geocities.com/SiliconValley/Sector/5785/\n\n");
      
  fprintf(stderr,"Usage:\n");
  fprintf(stderr,"%s <string>\n",progName);
}

int main(int argc, char* argv[]) 
{
  APIRET  rc                  = NO_ERROR;  /* Return code                    */
  char *pszDest;
  BOOL fSuccess;
  CHAR szClipString[CCHMAXPATH+1]={0};
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;
  ERRORID err;  
    
  if(argc<2) {
    usage(argv[0]);
    exit(1);
  }
  
  hab=WinInitialize(0);
  if(hab) {
    
    /* Get character string (szClipString). */

    strncpy(szClipString,argv[1],CCHMAXPATH);

               
    if (WinOpenClipbrd(hab)) {
      /* Allocate a shared memory object for the text data. */
      if (!(fSuccess = DosAllocSharedMem(
                                         (PVOID)&pszDest,       /* Pointer to shared memory object */
                                         NULL,                  /* Use unnamed shared memory       */
                                         1000,
                                         //  strlen(szClipString)+1,/* Amount of memory to allocate    */
                                         PAG_WRITE  | PAG_READ |          /* Allow write access              */
                                         PAG_COMMIT |           /* Commit the shared memory        */
                                         OBJ_GIVEABLE))) {      /* Make pointer giveable           */
      

            
      
        /* Copy the string to the allocated memory. */
        strcpy(pszDest,szClipString);

        /* Clear old data from the clipboard. */
        WinEmptyClipbrd(hab);
      
        /*
         * Pass the pointer to the clipboard in CF_TEXT format. Notice
         * that the pointer must be a ULONG value.
         */
      
        fSuccess = WinSetClipbrdData(hab, /* Anchor-block handle    */
                                     (ULONG) pszDest,              /* Pointer to text data   */
                                     CF_TEXT,                      /* Data is in text format */
                                     CFI_POINTER);                 /* Passing a pointer      */

      
        if(!fSuccess) {
          err=WinGetLastError(hab);
          sprintf(szClipString,"%x",err);
          //WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,szClipString,"Debug",123,MB_OK);
        }

        /* Close the clipboard. */
        WinCloseClipbrd(hab);
      }
    }

    WinTerminate(hab);
  }


  return 0;
}








