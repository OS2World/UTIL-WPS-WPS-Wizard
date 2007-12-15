#define INCL_PM
#define INCL_DOS
#define INCL_WINWORKPLACE
#define INCL_MMIOOS2
#define INCL_DOSDEVIOCTL
#define INCL_MCIOS2
#define  INCL_ERRORS
#ifdef __INNOTEK_LIBC__
#define INCL_OS2MM
#endif

#include <os2.h>
#ifdef __INNOTEK_LIBC__
#include "cw_pmwp.h"
#endif
#include <stdio.h>
#include <conio.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <string.h>
#include <stdlib.h>
#include "os2me.h"

#define GOGADGET_RESOURCE_TYPE 300
#define GOGADGET_RESOURCE_ID 1

typedef struct pls
{
   ULONG ulCommand;
   ULONG ulOperandOne;
   ULONG ulOperandTwo;
   ULONG ulOperandThree;
} PLAY_LIST_STRUCTURE_T;

#define PLAYING_HAS_STOPPED                               99

PLAY_LIST_STRUCTURE_T apltPlayList[5]=
{
  LOOP_OPERATION,    5, 4, 0,      /* Which line to end on      < Line 0 > */
  DATA_OPERATION,    0, 0, 0,                                /* < Line 1 > */
  MESSAGE_OPERATION, 0, PLAYING_HAS_STOPPED, 0,
  BRANCH_OPERATION,  0, 0, 0,      /* Branch back to Loop       < Line 3 > */
  EXIT_OPERATION,    0, 0, 0
};

MCI_OPEN_PARMS     mciOpenParameters;                /* Open structure.       */
#define MCI_ERROR_STRING_LENGTH                                 128


/******************************************************************************
 * Name        : SetupFileInformation
 *
 * Description : This procedure tells the MCD information about the file
 *               that is about to be played.  The Samples Per Second,
 *               Bits Per Sample, and the number of channels with which
 *               the waveform file was created has to be told to the MCD.
 *
 * MMPM/2 API's: mciSendCommand
 *               MCI_SET
 *
 * Parameters  : None.
 *
 * Return      : None.
 *
 ******************************************************************************/
BOOL SetupFileInformation(   MMAUDIOHEADER* mmAudioHeader )
{
   MCI_WAVE_SET_PARMS mwspWaveFormParameters;   /* Waveform parameters.       */
   ULONG              ulRC=0;                     /* Return code.               */

   /*
    * Fill the structure with zeros.
    */
   memset( &mwspWaveFormParameters,            /* Object to fill with zeros.  */
           0,                                  /* Value to place in object.   */
           sizeof( mwspWaveFormParameters ) ); /* How many zeros's to use.    */

   /*
    * Set structure values for the MCI_SET.
    */
   mwspWaveFormParameters.ulSamplesPerSec =
      mmAudioHeader->mmXWAVHeader.WAVEHeader.ulSamplesPerSec;
   mwspWaveFormParameters.usBitsPerSample =
      mmAudioHeader->mmXWAVHeader.WAVEHeader.usBitsPerSample;
   mwspWaveFormParameters.usChannels     =
      mmAudioHeader->mmXWAVHeader.WAVEHeader.usChannels;
   mwspWaveFormParameters.ulAudio        =
      MCI_SET_AUDIO_ALL;

   /*
    * Set the number of channels.
    */
   ulRC |=
      mciSendCommand(
         mciOpenParameters.usDeviceID,
         MCI_SET,
         MCI_WAIT | MCI_WAVE_SET_CHANNELS,
         (PVOID) &mwspWaveFormParameters,
         0 );

   /*
    * Set the samples per second for the waveform file.
    */
   ulRC |=
      mciSendCommand(
         mciOpenParameters.usDeviceID,
         MCI_SET,
         MCI_WAIT | MCI_WAVE_SET_SAMPLESPERSEC,
         (PVOID) &mwspWaveFormParameters,
         0 );

   /*
    * Set the bits per second for the waveform file.
    */
   ulRC |=
      mciSendCommand(
         mciOpenParameters.usDeviceID,
         MCI_SET,
         MCI_WAIT | MCI_WAVE_SET_BITSPERSAMPLE,
         (PVOID) &mwspWaveFormParameters,
         0 );

   if(ulRC)
     return FALSE;

   return TRUE;
}  /* End of SetupChimeFileInformation. */


void play(char *chrSourceName)
{
  HMMIO hmmio;
  MMIOINFO mmioinfo;
  LONG lBytesRead=0;
  ULONG rc;
  ULONG ulOpenFlags = MCI_WAIT | MCI_OPEN_PLAYLIST | MCI_OPEN_TYPE_ID | MCI_OPEN_SHAREABLE;
  MCI_SET_PARMS mSetParameters={0};   /* Waveform parameters.       */
  int a;
  char* bWaveMem=NULLHANDLE;
  ULONG mciRC;                 /* MCI generic return code variable.          */
  MMAUDIOHEADER mmAudioHeader;
  ULONG ulOff=9936;
  ULONG ulStart=4276;

  /*******************************/
  /* Set up/open the SOURCE file */
  /*******************************/
  memset(&mmioinfo,0, sizeof(mmioinfo));
  mmioinfo.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;
  mmioinfo.ulFlags=MMIO_READ|MMIO_DENYNONE;
    
  if((hmmio=mmioOpen(chrSourceName, &mmioinfo,MMIO_READ))==NULLHANDLE)
    {
      return;
    }
  memset(&mmAudioHeader,0,sizeof(MMAUDIOHEADER));
  rc = mmioGetHeader(hmmio, &mmAudioHeader,sizeof(MMAUDIOHEADER),
                     &lBytesRead, 0, 0);
  if(rc!=MMIO_SUCCESS) {
    mmioClose(hmmio, 0);
    return;
  }

  if((bWaveMem=malloc(mmAudioHeader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes))
     ==NULLHANDLE)
    {
      mmioClose(hmmio, 0);
      return;
    };

  rc=mmioRead(hmmio, bWaveMem, mmAudioHeader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes);
  if(rc==MMIO_ERROR)
    {
      free(bWaveMem);
      mmioClose(hmmio, 0);
      return;
    }

  /* Setup playlist */
  apltPlayList[ 1 ].ulOperandOne = (ULONG)bWaveMem;
  apltPlayList[ 1 ].ulOperandOne += ulStart;
  apltPlayList[ 1 ].ulOperandTwo = mmAudioHeader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes-ulOff;

  /*
   * Open the correct waveform device with the MCI_OPEN
   * message to MCI.
   */
  mciOpenParameters.pszDeviceType    = (PSZ)
    MAKEULONG ( MCI_DEVTYPE_WAVEFORM_AUDIO, 1 );
  /*
   * The address of the buffer containing the chime waveform file.
   */
  mciOpenParameters.pszElementName =
    (PSZ)&apltPlayList[ 0 ];
    
  /*
   * Initialize the MCI_OPEN_PARMS data structure with hwndMainDialogBox
   * as callback handle for MM_MCIPASSDEVICE, then issue the MCI_OPEN
   * command with the mciSendCommand function.  No alias is used.
   */
  mciOpenParameters.hwndCallback  = NULLHANDLE;//hwndMainDialogBox;
  mciOpenParameters.pszAlias      = (CHAR) NULL;
    
  /*
   * Open the waveform file in the playlist mode.
   */
  rc =
    mciSendCommand(
                   0,                           /* We don't know the device yet.        */
                   MCI_OPEN,                    /* MCI message.                         */
                   ulOpenFlags,                 /* Flags for the MCI message.           */
                   (PVOID) &mciOpenParameters,  /* Parameters for the message.          */
                   0 );                         /* Parameter for notify message.        */

  if ( rc != 0 )
    {
      free(bWaveMem);
      mmioClose(hmmio, 0);
      return;
    }


  if(!SetupFileInformation(&mmAudioHeader))
    {
      free(bWaveMem);
      mmioClose(hmmio, 0);
      return;
    }

  mSetParameters.ulAudio=MCI_SET_AUDIO_ALL;
  mSetParameters.ulOver=500;
  mSetParameters.ulLevel=0;
  rc=mciSendCommand(
                    mciOpenParameters.usDeviceID, /* Device to play the chimes.    */
                    MCI_SET,                     /* MCI message.                  */
                    MCI_WAIT|MCI_SET_AUDIO|MCI_OVER|MCI_SET_VOLUME,//MCI_NOTIFY,  /* Flags for the MCI message.    */
                    (PVOID) &mSetParameters,   /* Parameters for the message.   */
                    0 );              /* Parameter for notify message. */

  rc =
    mciSendCommand(
                   mciOpenParameters.usDeviceID, /* Device to play the chimes.    */
                   MCI_PLAY,                     /* MCI message.                  */
                   0,//MCI_NOTIFY,                   /* Flags for the MCI message.    */
                   (PVOID) &mciOpenParameters,   /* Parameters for the message.   */
                   0 );              /* Parameter for notify message. */

  if (!rc)
    {
      for(a=0; a<=10; a++) {
        DosSleep(100);
        
        mSetParameters.ulAudio=MCI_SET_AUDIO_ALL;
        mSetParameters.ulOver=500;
        mSetParameters.ulLevel=100*a/10;
        rc=mciSendCommand(
                          mciOpenParameters.usDeviceID, /* Device to play the chimes.    */
                          MCI_SET,                     /* MCI message.                  */
                          MCI_WAIT|MCI_SET_AUDIO|MCI_OVER|MCI_SET_VOLUME,//MCI_NOTIFY,  /* Flags for the MCI message.    */
                          (PVOID) &mSetParameters,   /* Parameters for the message.   */
                          0 );              /* Parameter for notify message. */
      }
      
      for(a=60; a>=0; a--) {
        DosSleep(250);
        
        //fprintf(stderr, "%d: %d\n", a, mSetParameters.ulLevel);

        mSetParameters.ulAudio=MCI_SET_AUDIO_ALL;
        mSetParameters.ulOver=500;
        mSetParameters.ulLevel=100*a/60;
        rc=mciSendCommand(
                          mciOpenParameters.usDeviceID, /* Device to play the chimes.    */
                          MCI_SET,                     /* MCI message.                  */
                          MCI_WAIT|MCI_SET_AUDIO|MCI_OVER|MCI_SET_VOLUME,//MCI_NOTIFY,  /* Flags for the MCI message.    */
                          (PVOID) &mSetParameters,   /* Parameters for the message.   */
                          0 );              /* Parameter for notify message. */
      }          
      
      DosSleep(5000);

#if 0
      BOOL fQuit=FALSE;
      for(;;)
        {
          DosSleep(100);
          if(_kbhit())
            {
              int chr=_getch();
              /* key pressed */
              if('+'==chr)
                {
                  ulOff+=4;
                  if(ulOff>50000)
                    ulOff=50000;

                }
              else if('-'==chr)
                {
                  ulOff-=4;
                  if(ulOff< 4 )
                    ulOff=0;
                }
              else if('m'==chr)
                {
                  ulStart-=4;
                  if(ulStart< 4 )
                    ulStart=0;
                  ulOff+=4;
                }
              else if('p'==chr)
                {
                  ulStart+=4;
                  if(ulStart> 10000 )
                    ulStart=10000;
                  ulOff-=4;
                }

              else if('q'==chr)
                break;
              fprintf(stderr,"Start: %d %d\n", ulStart, ulOff);
              apltPlayList[ 1 ].ulOperandTwo = mmAudioHeader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes-ulOff; //12000
              apltPlayList[ 1 ].ulOperandOne = (ULONG)bWaveMem+ulStart;
            }
        }
#endif
    }

  mciSendCommand(
                 mciOpenParameters.usDeviceID, /* Device to play the chimes.    */
                 MCI_STOP,                     /* MCI message.                  */
                 MCI_WAIT,//MCI_NOTIFY,                   /* Flags for the MCI message.    */
                 (PVOID) &mciOpenParameters,   /* Parameters for the message.   */
                 0 );              /* Parameter for notify message. */

  mciSendCommand(
                 mciOpenParameters.usDeviceID, /* Device to play the chimes.    */
                 MCI_CLOSE,                     /* MCI message.                  */
                 MCI_WAIT,//MCI_NOTIFY,                   /* Flags for the MCI message.    */
                 (PVOID) &mciOpenParameters,   /* Parameters for the message.   */
                 0 );              /* Parameter for notify message. */


  free(bWaveMem);
  mmioClose(hmmio, 0);
}

BOOL  getFileFromResource(HMODULE hmod, ULONG ulType, ULONG ulID, char* chrFileName)
{
  PVOID offset=NULLHANDLE;
  ULONG ulSize;
  size_t written;
  FILE *file;
  char* ptr;
  int a;

  if(DosQueryResourceSize(hmod, ulType, ulID, &ulSize)!=NO_ERROR)
    return FALSE;

  if(DosGetResource(hmod, ulType, ulID, &offset)!=NO_ERROR)
    return FALSE;

  /* Now create the file */
  if((file=fopen(chrFileName, "wb"))==NULLHANDLE)
    {
      DosFreeResource(offset);
      return FALSE;
    } 
  written=fwrite( offset, 1, ulSize, file);
  fclose(file);

  //  if(written!=ulSize)
  //   fprintf(stderr, "fwrite() error. written: %d of %d\n", written, ulSize);

  DosFreeResource(offset);
  return TRUE;
}

#ifdef __INNOTEK_LIBC__
void goGadgetGoThreadFunc (void *arg)
#else
void _Optlink goGadgetGoThreadFunc (void *arg)
#endif
{
  char* chrTemp;
  FILE *file;

  if((chrTemp=getenv("tmp"))==NULLHANDLE)
    if((chrTemp=getenv("temp"))==NULLHANDLE)
      chrTemp=getenv("tmp");

  if((chrTemp=tempnam(NULL, ""))!=NULLHANDLE)
    {
      if(getFileFromResource((HMODULE) arg ,GOGADGET_RESOURCE_TYPE, GOGADGET_RESOURCE_ID, chrTemp))
        play(chrTemp);
      remove(chrTemp);
      free(chrTemp);
    }

  return;
}

