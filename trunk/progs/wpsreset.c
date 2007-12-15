#define INCL_DOS
#define INCL_DOSERRORS

#define INCL_WIN
#include <os2.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  HAB   hab;
  HMQ   hmq;
  PTIB ptib;
  PPIB ppib;
  
  if(DosGetInfoBlocks(&ptib, &ppib)==NO_ERROR)
    {
      /*      printf("Got info block...\n");*/
      ppib->pib_ultype=3; /* Morph into PM, so creating of message queue succedes */
    }
  
  if((hab=WinInitialize(0))!=NULLHANDLE)
    {
      if((hmq=WinCreateMsgQueue(hab, 0))!=NULLHANDLE) {
        PRFPROFILE prfProfiles;
        static char chrUser[CCHMAXPATH]={0}; 
        static char chrSystem[CCHMAXPATH]={0};
        
        prfProfiles.cchUserName=CCHMAXPATH;
        prfProfiles.pszUserName=chrUser;
        prfProfiles.cchSysName=CCHMAXPATH;
        prfProfiles.pszSysName=chrSystem;
        
        if(PrfQueryProfile(hab, &prfProfiles)) {
          /* Got profiles */
          printf("\nResetting WPS now...\n");  
          PrfReset(hab, &prfProfiles);
        }
        WinDestroyMsgQueue(hmq);
      }
      WinTerminate(hab);
    }
  
  return (0);
}

