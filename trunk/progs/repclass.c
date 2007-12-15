#define INCL_DOSMISC       /* DOS Miscellaneous values */
#define INCL_DOSERRORS     /* DOS Error values         */
#define INCL_DOSFILEMGR
#define INCL_WINWORKPLACE

#include <os2.h>
#include <stdio.h>
 
void usage(char *progName) {
  printf("Program to replace a WPS class.\n");
  printf("(c) Chris Wohlgemuth 2001\n");
  printf("http://www.geocities.com/SiliconValley/Sector/5785/\n\n");
      
  fprintf(stderr,"Usage:\n");
  fprintf(stderr,"%s <Class to replace> <Replacement class> [u]\n",progName);
  fprintf(stderr,"If 'u' is given the class will be unreplaced.\n\n");

}

int main(int argc, char* argv[])  {
	

	APIRET  rc                  = NO_ERROR;  /* Return code                    */
    BOOL bReplace=TRUE;
	
    if(argc<3) {
      usage(argv[0]);
      exit(1);
    }
    if(argc==4) 
      if(argv[3]=="u")
        bReplace=FALSE;
	
    if(WinReplaceObjectClass(argv[1] , argv[2], bReplace)){
      printf("Class replaced.\n");
      printf("You have to reboot so the change can take effect.\n");
    }
    else{
      printf("!! Couldn't replace the class !!.\n\n");
      DosBeep(100,200);
      return 1;
	}
	printf("Done.\n\n");
	
	return 0;
}






