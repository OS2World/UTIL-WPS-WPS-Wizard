#define INCL_WINWORKPLACE
#define INCL_WIN
#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_REXXSAA
#define INCL_GPI

#include <os2.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include "htmlwnd.h"
#include "defines.h"
#include "sys_funcs.h"

//extern HWND hwndList;

static PFNWP g_oldStatic=NULLHANDLE;
ULONG  g_ulStaticDataOffset;
ULONG  ulQWP_WNDDATA;

static ULONG ulHorzFontRes=120; /* May also be 96 */

/* Distance of text to text window border */
#define LEFT_PADDING      2
#define BOTTOM_PADDING    5

/* Heading is size of normal text multiplied by this factor */
#define HEADING_FACTOR 1.3

/* Styles for sections */
#define MAX_SECTION_STYLE 2
STYLE  sectionStyles[MAX_SECTION_STYLE]={0, 0, 10, 0};

#if 0
/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This function may be used to write data into the */
/* traplog file. The syntax is similar to printf(). */
/*                                                  */
/* @@REMARKS                                        */
/*                                                  */
/* The filename of the traplog is defined in the    */
/* include file :hp2.excptLogName.h:ehp2.           */
/*                                                  */
/*                                                  */
/*                                                  */
/*!!*************************************************/
void SysWriteToTrapLog(const char* chrFormat, ...)
{
  char buffer[10000];

    va_list arg_ptr;

    va_start (arg_ptr, chrFormat);
    vsprintf(buffer, chrFormat, arg_ptr);
    va_end (arg_ptr);
    WinSendMsg(hwndList, LM_INSERTITEM, MPFROMSHORT(LIT_END), buffer);
}
#endif
void HlpWriteToTrapLog(const char* chrFormat, ...);


#define HASHSIZE 101
static ULONG calculateHash(char * theString)
{
  ULONG ulHash=0;

  if(theString) {
    /* skip leading white space */
    while(*theString==' ')
      theString++;
    for(ulHash=0; *theString!='\0' && *theString!=' ' && *theString!='>'; theString++)
      ulHash=tolower(*theString)+31*ulHash;
  
  }

  return ulHash%HASHSIZE;
}

#define HASHSIZE2 123
static ULONG calculateHash2(char * theString , char** chrCur)
{
  ULONG ulHash=0;

  if(theString) {
    /* skip leading white space */
    while(*theString==' ')
      theString++;
    for(ulHash=0;*theString!='\0' && *theString!=' ' && *theString!='>'; theString++)
      ulHash=tolower(*theString)+37*ulHash;
  }
  *chrCur=theString;
  return ulHash%HASHSIZE2;
}

#define HASH_H1            0x3131
#define HASH_H1_END        0x3232
#define HASH_LINK          0x3333
#define HASH_LINK_END      0x3434
#define HASH_BREAK         0x3535
#define HASH_PARAGRAPH     0x3636
#define HASH_RULER         0x3737
#define HASH_IMAGE         0x3838  /* Not yet */
#define HASH_SECTION       0x3939
#define HASH_SECTION_END   0x3410
#define HASH_UNDERLINED    0x3511
#define HASH_UNDERLINED_END 0x3612

#define HASH_BOLD          0x3713
#define HASH_BOLD_END      0x3814
#define HASH_COLOR         0x3915
#define HASH_COLOR_END     0x3a16

/* HTML tags */
#define HASH_HTML_LINK        0x6161 /* a   */
#define HASH_HTML_LINK_END    0x2772 /* /a  */
#define HASH_HTML_H1          0x2954 /* h1  */
#define HASH_HTML_H1_END      0x3d62 /* /h1 */
#define HASH_HTML_BREAK       0x1532 /* br  */
#define HASH_HTML_PARAGRAPH   0x0b70 /* p   */
#define HASH_HTML_RULER       0x051a /* hr  */
#define HASH_HTML_BOLD        0x6262 /* b   */
#define HASH_HTML_BOLD_END    0x2873 /* /b  */
#define HASH_HTML_FONT        0x0231 /* font */
#define HASH_HTML_FONT_END    0x201b /* /font */

#define HASH_HTML_LT          0x1e35 /* lt */
#define HASH_HTML_GT          0x4d72 /* gt */

LONG parseFontColor(char *chrCur)
{
  char *chrClose;
  char *chrColor;

  if((chrClose=strchr(chrCur, '>'))==NULLHANDLE)
    return -1; /* No closing tag, this must be an error */

  if(chrClose-chrCur>200)
    return -1; /* Prevent buffer overflow attack. The font tag can't be that long... */

  *chrClose=0;
  if((chrColor=strstr(strlwr(chrCur), "color"))==NULLHANDLE) {
    *chrClose='>';
    return -1; /* No color attribute, this must be an error */
  }
  *chrClose='>';

  /* color attribute found */
  chrColor+=strlen("color");
  while(*chrColor==' ' || *chrColor=='=' || *chrColor=='"' || *chrColor=='#') /* skip whitespace */
    chrColor++;

  //  SysWriteToTrapLog(chrColor);
  return strtoul(chrColor, NULLHANDLE, 16);
}

/* This function checks the type of a mark and parses the contents if any.
   The format of a mark is <x y> where x, y are numbers. */
char* parseMark(int iLength, char *chrPos,   TAGINFO *tagInfo)
{
  char *chrStart=chrPos;

  *chrStart++; /* move to mark */

  while(*chrPos!=0)
    {
      switch(*chrPos)
        {
        case '>':
          /* found end of mark */
          {
            char *chrCur;
            ULONG ulHash;

            chrPos++; /* Jump behind the tag */

            /* Get a hash for the current tag */
            ulHash=(calculateHash(chrStart) << 8) +  calculateHash2(chrStart, &chrCur);

            tagInfo->ulParam=0;
            tagInfo->lColor=-1; /* Default (-1): use text color of window. If a color tag is given
                                 this will contain the RGB color. */

            switch(ulHash)
              {
              case HASH_SECTION:
                // case MARK_SECTION_END:
                tagInfo->ulMark=MARK_SECTION;
                tagInfo->ulParam=atoi(chrCur);
                break;
              case HASH_LINK:
              case HASH_HTML_LINK: /* a */
                tagInfo->ulMark=MARK_LINK;
                tagInfo->ulParam=atoi(chrCur);
                break;
              case HASH_H1:
              case HASH_HTML_H1:/* h1 */
                tagInfo->ulMark=MARK_H1;
                /* Get param number. Format of link is: <3 x> with x a number */
                tagInfo->ulParam=atoi(chrCur);
                break;
              case HASH_COLOR:
                tagInfo->ulMark=MARK_COLOR;
                /* Get param number. Format of color is: <y x> with x a number */
                //tagInfo->ulParam=strtoul(chrCur, NULLHANDLE, 16);
                tagInfo->lColor=strtoul(chrCur, NULLHANDLE, 16);
                break;
              case HASH_SECTION_END:
                tagInfo->ulMark=MARK_SECTION_END;
                break;
              case HASH_LINK_END:
              case HASH_HTML_LINK_END:  /* /a */
                tagInfo->ulMark=MARK_LINK_END;
                break;
              case HASH_H1_END:
              case HASH_HTML_H1_END:   /* /h1 */
                tagInfo->ulMark=MARK_H1_END;
                break;
              case HASH_BREAK:
              case HASH_HTML_BREAK: /* br */
                tagInfo->ulMark=MARK_BREAK;
                break;
              case HASH_PARAGRAPH:
              case HASH_HTML_PARAGRAPH: /* p */
                tagInfo->ulMark=MARK_PARAGRAPH;
                break;
              case HASH_RULER:
              case HASH_HTML_RULER: /* hr */
                tagInfo->ulMark=MARK_RULER;
                break;
              case HASH_UNDERLINED:
                tagInfo->ulMark=MARK_UNDERLINED;
                break;
              case HASH_UNDERLINED_END:
                tagInfo->ulMark=MARK_UNDERLINED_END;
                break;
              case HASH_HTML_FONT:
                /* Only color is supported as an attribute for the <font> tag. */
                tagInfo->ulMark=MARK_COLOR;
                /* Get param number. Format of color is: <font color=x> with x a hexadecimal number */
                tagInfo->lColor=parseFontColor(chrCur);
                break;
              case HASH_HTML_BOLD:
                tagInfo->ulMark=MARK_BOLD;
                break;
              case HASH_HTML_BOLD_END:
                tagInfo->ulMark=MARK_BOLD_END;
                break;
              case HASH_HTML_FONT_END:
                /* Only color is supported as an attribute for the <font> tag. */
                tagInfo->ulMark=MARK_COLOR_END;
                break;
              case HASH_HTML_LT:
                /* Only color is supported as an attribute for the <font> tag. */
                tagInfo->ulMark=HASH_HTML_LT;
                break;
              case HASH_HTML_GT:
                /* Only color is supported as an attribute for the <font> tag. */
                tagInfo->ulMark=HASH_HTML_GT;
                break;

              default:
                tagInfo->ulMark=MARK_TEXT;
                break;
              }
            return chrPos;
          }
        default:
          break;
        }
      chrPos++; /* Next char */
    }
  tagInfo->ulMark=MARK_TEXT;
  tagInfo->ulParam=0;
  return chrPos;
}


/* Go through the text and find the next opening of a mark: '<'. */
char *findNextMark(char *chrPos)
{
  while(*chrPos!=0)
    {
      switch(*chrPos)
        {
        case '<':
          return chrPos; /* Mark opening found */
        default:
          break;
        }
      chrPos++;
    }
  return chrPos;
}

void addToHtmlPartList(HWND hwnd, htmlPart* hTemp)
{
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  if(!wndData)
    return;

  if(!wndData->hPartList)
    {
      wndData->hPartList=hTemp;
      wndData->hpLastInserted=hTemp;
      return;
    }
  wndData->hpLastInserted->hpNext=hTemp;
  wndData->hpLastInserted=hTemp;
}

htmlPart * getHtmlPartList(HWND hwnd)
{
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  if(!wndData)
    return NULLHANDLE; /* There's a problem*/

   return wndData->hPartList;
}

void freeHtmlPartList(HWND hwnd)
{
  htmlPart *htmlTemp;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  if(!wndData)
    return; /* There's a problem*/

  htmlTemp=wndData->hPartList;
  while(htmlTemp)
    {
      htmlPart* hTemp=htmlTemp->hpNext;
      free(htmlTemp);
      htmlTemp=hTemp;
    }
  wndData->hPartList=NULLHANDLE;
}

/*
  Push current format of text (size, color etc.) on a stack. this stack
  is a linked list of FORMATINFO structures.
*/
BOOL pushFormatInfo(HWND hwnd, FORMATINFO formatInfo)
{
  FORMATINFO* fiTemp;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);
  FORMATINFO* hFormatInfo;

  if(!wndData)
    return FALSE; /* There's a problem. No HTML window data found */

  hFormatInfo=wndData->hFormatInfo;

  if((fiTemp=(FORMATINFO*)malloc(sizeof(FORMATINFO)))==NULLHANDLE)
    return FALSE;

  *fiTemp=formatInfo;

  if(!hFormatInfo)
    {
      wndData->hFormatInfo=fiTemp;
      return TRUE;
    }

  fiTemp->fiNext=hFormatInfo;
  wndData->hFormatInfo=fiTemp;  

  return TRUE;
}

/*
  Get format of text from the stack. The stack is a linked list of structures.
*/
BOOL popFormatInfo(HWND hwnd, FORMATINFO* formatInfo)
{
  FORMATINFO* fiTemp;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);
  FORMATINFO* hFormatInfo;

  if(!wndData)
    return FALSE; /* There's a problem */

  hFormatInfo=wndData->hFormatInfo;

  if(!hFormatInfo)
    return FALSE;

  fiTemp=hFormatInfo->fiNext;
  *formatInfo=*hFormatInfo;
  formatInfo->fiNext=NULLHANDLE;

  free(hFormatInfo);

  wndData->hFormatInfo=fiTemp;
  return TRUE;
}

BOOL getFormatInfo(HWND hwnd, FORMATINFO* formatInfo)
{
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);
  FORMATINFO* hFormatInfo;

  if(!wndData)
    return FALSE; /* There's a problem*/

  hFormatInfo=wndData->hFormatInfo;

  if(!hFormatInfo)
    return FALSE;

  *formatInfo=*hFormatInfo;

  return TRUE;
}

void freeFormatInfoList(HWND hwnd)
{
  FORMATINFO* fiTemp;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);
  FORMATINFO* hFormatInfo;

  if(!wndData)
    return; /* There's a problem*/

  hFormatInfo=wndData->hFormatInfo;

  fiTemp=hFormatInfo;

  while(fiTemp)
    {
      FORMATINFO* hFi=fiTemp->fiNext;
      free(fiTemp);
      fiTemp=hFi;
    }  

  wndData->hFormatInfo=NULLHANDLE;
}


htmlPart * checkForLink(HWND hwnd, PPOINTL ptl)
{
  htmlPart *hTemp;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  hTemp=wndData->hPartList;

  /* Travel the list of linked text parts to check if ptl is over a link. */
  while(hTemp)
    {
      RECTL rcl=hTemp->rcl;
      rcl.yTop+=wndData->lYViewPort;
      rcl.yBottom+=wndData->lYViewPort;
      if(WinPtInRect(WinQueryAnchorBlock(HWND_DESKTOP), &rcl, ptl)) {
        if(hTemp->iType==MARK_LINK)
          return hTemp;
      }
      hTemp=hTemp->hpNext;
    }
  return NULLHANDLE;
}

BOOL clickLink(HWND hwnd, PPOINTL ptl)
{
  htmlPart *hTemp;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);
  
  hTemp=wndData->hPartList;

  /* Travel the list of linked text parts to check if ptl is over a link. Then
     return the parameter. */
  while(hTemp)
    {
      RECTL rcl=hTemp->rcl;
      rcl.yTop+=wndData->lYViewPort;
      rcl.yBottom+=wndData->lYViewPort;

      if(WinPtInRect(WinQueryAnchorBlock(HWND_DESKTOP), &rcl, ptl)) {
        if(hTemp->iType==MARK_LINK) {
          return hTemp->ulParam;
        }
      }
      hTemp=hTemp->hpNext;
    }
  return 0;
}

/* returns the string length in points for the given hps. The length
   may change depending on the font size. */
LONG gpiStringLength(HPS hps, LONG lLength, char * chrText)
{
  POINTL aptl[TXTBOX_COUNT];

  GpiQueryTextBox(hps, lLength, chrText, TXTBOX_COUNT, aptl);

  return aptl[TXTBOX_BOTTOMRIGHT].x-aptl[TXTBOX_BOTTOMLEFT].x; 
}

/* returns the string length in chars. Accepts ' ' and '<' as
   string part separators */
LONG newStringLength(HPS hps, LONG lMax, char * chrText)
{
  char * chrPtr;
  LONG rc;

  if((chrPtr=strchr(chrText, ' '))!=NULLHANDLE) {
    rc=chrPtr-chrText+1; /* Move behind the space */
    if(rc<=lMax)
      return rc; 
  }

  if((chrPtr=strchr(chrText, '<'))!=NULLHANDLE)
    return chrPtr-chrText; /* Move behind the space */

  return strlen(chrText);
}

#if 0
LONG getWordLength(LONG lMax, char * chrText)
{
  char * chrPtr;
  LONG rc;

  if((chrPtr=strchr(chrText, ' '))!=NULLHANDLE) {
    rc=chrPtr-chrText;
    if(rc<=lMax)
      return rc; 
  }

  if((chrPtr=strchr(chrText, '<'))!=NULLHANDLE)
    return chrPtr-chrText;

  return strlen(chrText);
}
#endif

/* Find a string which fits into the remaining window space */
LONG getFittingStringLength(HPS hps, LONG lMax, char* chrText, LONG lFreeSpace)
{
  LONG lPtl;
  int a=0;

  /*
    FIXME:
    Very sloooooow, but still useful.
    */
  while((lPtl=gpiStringLength(hps, a+1, chrText))<=lFreeSpace)
    a++;
  return a;
}

ULONG queryHorzFontRes(void)
{
  HDC hdc;
  LONG alArray[1];

  hdc=WinOpenWindowDC(HWND_DESKTOP);
  DevQueryCaps(hdc, CAPS_HORIZONTAL_FONT_RES, 1, alArray);

  return alArray[0];
}

BOOL isBitmapFont(HPS hps, char * chrFacename)
{
  LONG cFonts;
  LONG lTemp=0;
  PFONTMETRICS pfm;
  BOOL bIsBitmapFont=FALSE;
  char * chrName;

  if((chrName=strchr(chrFacename, '.'))!=NULLHANDLE)
    chrName++;;

  /* Get num fonts of this font family */
  cFonts=GpiQueryFonts(hps, QF_PUBLIC, chrName, &lTemp, sizeof(FONTMETRICS), NULL);

  if((pfm=(PFONTMETRICS)malloc(cFonts*sizeof(FONTMETRICS)))!=NULLHANDLE)
    {
      int a=1;

      lTemp=cFonts;
      cFonts=GpiQueryFonts(hps, QF_PUBLIC, chrName, &cFonts, sizeof(FONTMETRICS), (PFONTMETRICS)pfm);


      for(a=0;a<lTemp; a++) {

        if((pfm[a].sNominalPointSize/10 == atol(chrFacename)) && (ulHorzFontRes==pfm[a].sXDeviceRes)) {
#if 0
          SysWriteToTrapLog("Found: %d. Fam: %s, Face: %s, fsDefn: %x, fsType: %x, Points: %d, lEmHeight: %d, weight %d, fsSelection: %x, lAveCharWidth: %d, lMaxBaselineExt %d, sXDeviceRes: %d",
                            a, pfm[a].szFamilyname, pfm[a].szFacename, pfm[a].fsDefn, pfm[a].fsType, pfm[a].sNominalPointSize,
                            pfm[a].lEmHeight,
                            pfm[a].usWeightClass, pfm[a].fsSelection, pfm[a].lAveCharWidth, pfm[a].lMaxBaselineExt,
                            pfm[a].sXDeviceRes);
#endif
          free(pfm);
          return TRUE;
        }
      }

      if(lTemp)
        bIsBitmapFont=!(pfm[0].fsDefn & FM_DEFN_OUTLINE);
      
      free(pfm);
    }
  return bIsBitmapFont;
}


#if 0
void printCurrentFont(HWND hwnd)
{
  char fontName[255]={0};
  ULONG  len;
  ULONG  attrFound;

  // Query the current font
  len=WinQueryPresParam(hwnd,
                        PP_FONTNAMESIZE,0,&attrFound,sizeof(fontName),
                        fontName,0 /*QPF_NOINHERIT*/);
  if(len) {
    SysWriteToTrapLog("Current Font: %s", fontName);
  }
}
void setHeadingFormat(HPS hps, LONG * clrFore)
{
  FATTRS fat;
  SIZEF sizef;

  memset(&fat, 0, sizeof(fat));
  fat.fsSelection=FATTR_SEL_BOLD;
  fat.usRecordLength=sizeof(fat);
  fat.lMaxBaselineExt=12L;
  fat.lAveCharWidth=12L;
  fat.fsFontUse=FATTR_FONTUSE_NOMIX;
  strcpy(fat.szFacename, "WarpSans");

  //fat.fsSelection=0;//FATTR_SEL_BOLD;

  GpiCreateLogFont(hps, NULL, 1L, &fat);
  GpiSetCharSet(hps, 1L);

  //sizef.cx=MAKEFIXED(18,0);
  //sizef.cy=MAKEFIXED(18,0);
  // GpiQueryCharBox(hps, &sizef);

  *clrFore=CLR_BLACK;
  *clrFore=CLR_BLUE;
}
#endif

void setFattr(HPS hps, char * chrFacename, ULONG ulPts, FATTRS* fat)
{
  LONG cFonts;
  LONG lTemp=0;
  PFONTMETRICS pfm;
  ULONG ulCheck=0;

  /* Get num fonts of this font family */
  cFonts=GpiQueryFonts(hps, QF_PUBLIC, chrFacename, &lTemp, sizeof(FONTMETRICS), NULL);

  if((pfm=(PFONTMETRICS)malloc(cFonts*sizeof(FONTMETRICS)))!=NULLHANDLE)
    {
      int a;

      lTemp=cFonts;
      cFonts=GpiQueryFonts(hps, QF_PUBLIC, chrFacename, &cFonts, sizeof(FONTMETRICS), (PFONTMETRICS)pfm);

      if(lTemp>1)
        ulCheck=1;
      if(pfm[ulCheck].fsDefn & FM_DEFN_OUTLINE) {
        free(pfm);
        return; /* Outline fonts don't need lMaxBaselineExt and lAveCharWidth to be set.
                   There's a problem though. When getting some fixed fonts, the first returned family
                   is an outline font (on my system) . The following are bitmap fonts. */
      }

      /* It's a bitmap font. Find the font for the current font resolution */
      for(a=0;a<lTemp; a++) {
        if(pfm[a].sXDeviceRes==ulHorzFontRes && pfm[a].sNominalPointSize/10==ulPts)
          {
            fat->lMaxBaselineExt=pfm[a].lMaxBaselineExt;
            fat->lAveCharWidth=pfm[a].lAveCharWidth;
#if 0
        SysWriteToTrapLog("Chosen: %d. Fam: %s, Face: %s, fsDefn: %x, fsType: %x, Points: %d, lEmHeight: %d, weight %d, fsSelection: %x, lAveCharWidth: %d, lMaxBaselineExt %d, sXDeviceRes: %d",
                          a, pfm[a].szFamilyname, pfm[a].szFacename, pfm[a].fsDefn, pfm[a].fsType, pfm[a].sNominalPointSize,
                          pfm[a].lEmHeight,
                          pfm[a].usWeightClass, pfm[a].fsSelection, pfm[a].lAveCharWidth, pfm[a].lMaxBaselineExt,
                          pfm[a].sXDeviceRes);
#endif
            break;
          }
      }
      free(pfm);
    }
}

#if 0
void queryFonts(HPS hps)
{
  LONG cFonts;
  LONG lTemp=0;
  PFONTMETRICS pfm;
  //  char *theFont="WarpSans";

  /* Get num fonts */
  cFonts=GpiQueryFonts(hps, QF_PUBLIC, NULLHANDLE /*theFont*/, &lTemp, sizeof(FONTMETRICS), NULL);

  if((pfm=(PFONTMETRICS)malloc(cFonts*sizeof(FONTMETRICS)))!=NULLHANDLE)
    {
      int a;
      SysWriteToTrapLog("Num Fonts: %d", cFonts);

      lTemp=cFonts;
      cFonts=GpiQueryFonts(hps, QF_PUBLIC, NULL /*theFont*/, &cFonts, sizeof(FONTMETRICS), (PFONTMETRICS)pfm);

      for(a=0;a<lTemp; a++) {
        SysWriteToTrapLog("%d. Fam: %s, Face: %s, fsDefn: %x, fsType: %x, Points: %d, lEmHeight: %d, weight %d, fsSelection: %x, lAveCharWidth: %d, lMaxBaselineExt %d, sXDeviceRes: %d",
                          a, pfm[a].szFamilyname, pfm[a].szFacename, pfm[a].fsDefn, pfm[a].fsType, pfm[a].sNominalPointSize,
                          pfm[a].lEmHeight,
                          pfm[a].usWeightClass, pfm[a].fsSelection, pfm[a].lAveCharWidth, pfm[a].lMaxBaselineExt,
                          pfm[a].sXDeviceRes);

      }
      free(pfm);
    }
}
#endif

void selectHeadingFont(HPS hps, FATTRS *fat, char * fontName)
{
  char * chrName;

  //  queryFonts(hps);

  if((chrName=strchr(fontName, '.'))!=NULLHANDLE)
    chrName++;;

  if(!isBitmapFont(hps, fontName)) {
    /* It's an outline font. */

    fat->szFacename[FACESIZE-1]=0;
    fat->lMaxBaselineExt=0;
    fat->lAveCharWidth=0;

    //SysWriteToTrapLog("     lMaxBaselineExt: %d, lAveCharWidth: %d, %s",
    //fat->lMaxBaselineExt, fat->lAveCharWidth, fat->szFacename);
  }
  else
    {
      LONG cFonts;
      LONG lTemp=0;
      PFONTMETRICS pfm;
      ULONG ulSize=1000;

      /* For bitmap fonts use bitmap 'Helv' as heading */

      /* Get num fonts of this font family */
      cFonts=GpiQueryFonts(hps, QF_PUBLIC, "Helv", &lTemp, sizeof(FONTMETRICS), NULL);
      
      if((pfm=(PFONTMETRICS)malloc(cFonts*sizeof(FONTMETRICS)))!=NULLHANDLE)
        {
          int a;

          lTemp=cFonts;
          cFonts=GpiQueryFonts(hps, QF_PUBLIC, "Helv", &cFonts, sizeof(FONTMETRICS), (PFONTMETRICS)pfm);

          memset(&fat->szFacename, 0, FACESIZE);
          fat->fsSelection=FATTR_SEL_BOLD;
          for(a=0;a<lTemp; a++) {            
            if(!(pfm[a].fsDefn & FM_DEFN_OUTLINE)) {
              /* It's a bitmap font. Find the font for the current font resolution */

              if(pfm[a].sXDeviceRes==ulHorzFontRes && pfm[a].lMaxBaselineExt > fat->lMaxBaselineExt
                 && pfm[a].lMaxBaselineExt < ulSize /* && pfm[a].sNominalPointSize/10==ulPts*/)
                {
                  fat->lMaxBaselineExt=pfm[a].lMaxBaselineExt;
                  fat->lAveCharWidth=pfm[a].lAveCharWidth;
                  ulSize=pfm[a].lMaxBaselineExt; 

                  strncpy((char*)&fat->szFacename, pfm[a].szFacename, sizeof(FACESIZE));
                  fat->szFacename[FACESIZE-1]=0;
                }
            }
          }
          free(pfm);
        }            
    }
  //SysWriteToTrapLog("%s is bitmap font? ->%d", fontName, isBitmapFont(hps, chrName));
}


void setHeadingFormat(HPS hps, LONG * clrFore, HWND hwnd)
{
  ULONG ret;
  HTMLWNDDATA* wndData;

  wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  if(!wndData)
    return;

  if(wndData->fatHeading.lMaxBaselineExt!=0) {
    /* It's a bitmap font */
    ret=GpiCreateLogFont(hps, NULL, 1L, &wndData->fatHeading);
    GpiSetCharSet(hps, 1L);
  }
  else {
    SIZEF sizef;
    /* Size of outline font is changed using the char box */
    GpiQueryCharBox(hps, &sizef);

    sizef.cx=MAKEFIXED(FIXEDINT(sizef.cx)*HEADING_FACTOR, 0);
    sizef.cy=MAKEFIXED(FIXEDINT(sizef.cy)*HEADING_FACTOR, 0);

    GpiSetCharBox(hps, &sizef);
  }
}

void setLinkFormat(HPS hps, LONG * clrFore, HWND hwnd)
{
  FATTRS fat;
  HTMLWNDDATA* wndData;
  ULONG ret;

  wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  if(!wndData)
    return;
  
  memcpy(&fat, &wndData->fatText, sizeof(fat));

  fat.fsFontUse=FATTR_FONTUSE_NOMIX;
  fat.fsSelection=FATTR_SEL_UNDERSCORE;

  ret=GpiCreateLogFont(hps, NULL, 1L, &fat);
  GpiSetCharSet(hps, 1L);

  *clrFore=0x000000ff;
}

void setUnderlineFormat(HPS hps, LONG * clrFore, HWND hwnd)
{
  FATTRS fat;
  HTMLWNDDATA* wndData;
  ULONG ret;

  wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  if(!wndData)
    return;
  
  memcpy(&fat, &wndData->fatText, sizeof(fat));

  fat.fsFontUse=FATTR_FONTUSE_NOMIX;
  fat.fsSelection=FATTR_SEL_UNDERSCORE;

  ret=GpiCreateLogFont(hps, NULL, 1L, &fat);
  GpiSetCharSet(hps, 1L);
}

void setBoldFormat(HPS hps, LONG * clrFore, HWND hwnd)
{
  FATTRS fat;
  HTMLWNDDATA* wndData;
  ULONG ret;

  wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  if(!wndData)
    return;
  
  memcpy(&fat, &wndData->fatText, sizeof(fat));

  fat.fsFontUse=FATTR_FONTUSE_NOMIX;
  fat.fsSelection=FATTR_SEL_BOLD;

  ret=GpiCreateLogFont(hps, NULL, 1L, &fat);
  GpiSetCharSet(hps, 1L);
}

void setDefaultFormat(HPS hps, LONG * clrFore, HWND hwnd)
{
  HTMLWNDDATA* wndData;
  ULONG ret;

  wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  if(!wndData)
    return;
  //  fat.fsFontUse=FATTR_FONTUSE_NOMIX;

  GpiSetCharBox(hps, &wndData->sizef);
  ret=GpiCreateLogFont(hps, NULL, 1L, &wndData->fatText);
  GpiSetCharSet(hps, 1L);
}


/* format text. This function takes the text and splits it in parts which will be formated nicely, so
   the text flows in the window. */
LONG doFormat(HPS hps, HWND hwnd, LONG lLength, char * chrText, RECTL* rcl,   TAGINFO *tagInfo, LONG lCharsDone,  HTMLWNDDATA* wndData)
{
  POINTL aptl[TXTBOX_COUNT];
  POINTL ptl, ptlMove;
  LONG clrFore;
  LONG lTotalLength=lLength;
  htmlPart* hTemp=NULLHANDLE;
  LONG lPtl;
  LONG lMark=tagInfo->ulMark;
  FORMATINFO fi={0};
  htmlPart *hPrev;

  ULONG  ulFormat=0;

  getFormatInfo( hwnd, &fi);
  hPrev=getHtmlPartList(hwnd);

  GpiQueryTextBox(hps, 1, "*", TXTBOX_COUNT, aptl); /* Get size of a character. Not accurate but sufficient. */

  GpiQueryCurrentPosition(hps, &ptl); /* Current position in the hps */
  ptlMove=ptl;

//  if(!lCharsDone)
  if(!(wndData->fFlags & HTML_STARTED_FORMAT))
    {
      /* First invocation. Fill format info with data for text  */
      fi.cyChar=aptl[TXTBOX_TOPLEFT].y-aptl[TXTBOX_BOTTOMLEFT].y;
      fi.xPos=LEFT_PADDING;
      fi.iType=MARK_TEXT;   /* Current format is for text */
      ptl.x=fi.xPos;
      rcl->yTop-=fi.cyChar;
      ptl.y=rcl->yTop;
      GpiMove(hps, &ptl);
      /* Save current formatting info on the format stack. */
      pushFormatInfo(hwnd, fi);
      wndData->fFlags |= HTML_STARTED_FORMAT;
    }

  switch(lMark)
    {
    case MARK_H1:
      {
        setHeadingFormat(hps, &clrFore, hwnd);

        GpiQueryTextBox(hps, 1, "*", TXTBOX_COUNT, aptl);
        fi.cyChar=aptl[TXTBOX_TOPLEFT].y-aptl[TXTBOX_BOTTOMLEFT].y; /* New font size */
        fi.iType=MARK_H1;
        ptl.x=rcl->xLeft;

        //        if(lCharsDone) /* Only move curser down when already printed a line */
        if(wndData->fFlags & HTML_NOTFIRST_LINE) {
          rcl->yTop-=(fi.cyChar*HEADING_FACTOR);
        }
        else
          rcl->yTop-=1;
        ptl.y=rcl->yTop;
        GpiMove(hps, &ptl); /* Move cursor */
        //      pushFormatInfo(hwnd, fi);
        break;
      }
    case MARK_H1_END:
      {
        GpiQueryTextBox(hps, 1, "*", TXTBOX_COUNT, aptl);
        fi.cyChar=aptl[TXTBOX_TOPLEFT].y-aptl[TXTBOX_BOTTOMLEFT].y;
        
        //      ptl.x=rcl->xLeft;
        ptl.x=fi.xPos;
        /* !!!!!!!!!!!!!!!! */
        //        rcl->yTop-=(fi.cyChar*1.5);

        ptl.y=rcl->yTop;
        GpiMove(hps, &ptl);
        
        GpiSetCharSet(hps, LCID_DEFAULT); /* Default font  */
        GpiDeleteSetId(hps, LCID_ALL);

        setDefaultFormat(hps, &clrFore,  hwnd);

        /* Get previous formatting */   
        //    popFormatInfo(hwnd, &fi);

        /* ???????? */
        //if(!lLength)
        //  return lLength;
        
        /* FIXME: use format stack here!! */
        //lMark=0; /* Set to text */
        break;
      }
    case MARK_SECTION:
      {
        int idx=0;

        if(tagInfo->ulParam<=MAX_SECTION_STYLE)
          idx=tagInfo->ulParam;
        fi.xPos+=sectionStyles[idx].cx;
        ptl.x=fi.xPos;
        GpiMove(hps, &ptl);
        
        pushFormatInfo(hwnd, fi);

        if(!lLength)
          return lLength;
        break;
      }
    case MARK_SECTION_END:
      popFormatInfo(hwnd, &fi);
      
      if(!lLength)
        return lLength;
      /* FIXME: use format stack here!! */
      lMark=0; /* Set to text */
      break;
    case MARK_LINK:
      {
        setLinkFormat(hps, &clrFore, hwnd);
        break;
      }
    case MARK_LINK_END:
      {
        break;
      }
    case MARK_COLOR: /* No need to set hps color here. The htmlPart will contain the color info. */
      break;
    case MARK_UNDERLINED:
      setUnderlineFormat(hps, &clrFore, hwnd);
      ulFormat|=FORMAT_UNDERLINED;
      break;
    case MARK_UNDERLINED_END:
      ulFormat&=~FORMAT_UNDERLINED;
      break;
    case HASH_HTML_LT:
    case HASH_HTML_GT:

      break;
    case MARK_PARAGRAPH:
     ptl.x=fi.xPos;

     //if(lCharsDone)/* Only move curser down when already printed a line */
     if(wndData->fFlags & HTML_NOTFIRST_LINE) {
       rcl->yTop-=fi.cyChar*1.5;
     }
     else
       rcl->yTop-=1;

     ptl.y=rcl->yTop;
     GpiMove(hps, &ptl);
     if(!lLength)
       return lLength;
     /* FIXME: use format stack here!! */
     lMark=0; /* Set to text */
     break;
    case MARK_BREAK:
      //      ptl.x=rcl->xLeft;
      ptl.x=fi.xPos;
      rcl->yTop-=fi.cyChar;
      ptl.y=rcl->yTop;
      GpiMove(hps, &ptl);
      /* if lLength!=0 then no starting tag for following text. Only the <Break> tag given. */
      if(!lLength)
        return lLength;
      /* FIXME: use format stack here!! */
      lMark=0; /* Set to text */
      break;
    case MARK_BOLD:
      setBoldFormat(hps, &clrFore, hwnd);
      ulFormat|=FORMAT_BOLD;
      break;
    case MARK_BOLD_END:
      ulFormat&=~FORMAT_BOLD;
      break;
    case MARK_RULER:
      {
        /* Draw ruler */
        ptl.x=rcl->xRight*0.1;
        rcl->yTop-=fi.cyChar*1; /* Move into next line */
        ptl.y=rcl->yTop;

        /* Create a part for the ruler */
        hTemp=(htmlPart*)malloc(sizeof(htmlPart));
        memset(hTemp, 0, sizeof(htmlPart));
        hTemp->iType=lMark;

        WinSetRect(WinQueryAnchorBlock(HWND_DESKTOP), &hTemp->rcl,
                   ptl.x, ptl.y,
                   rcl->xRight*0.9, ptl.y+2 );

        hTemp->ptl=ptl;
        hTemp->chrText=chrText;
        hTemp->iLength=rcl->xRight*0.8;
        hTemp->ulParam=tagInfo->ulParam;
        hTemp->hpPrev=hPrev;
        hPrev=hTemp;
        addToHtmlPartList(hwnd, hTemp);

        ptl.x=fi.xPos;
        rcl->yTop-=fi.cyChar*0.2; /* Move cursor into next line */
        ptl.y=rcl->yTop;

        GpiMove(hps, &ptl);

        /* if lLength!=0 then no starting tag for following text. Only the <Break> tag given. */
        if(!lLength)
          return lLength;

        /* FIXME: use format stack here!! */
        lMark=0; /* Set to text */
        break;
      }
    default:
      GpiSetCharSet(hps, LCID_DEFAULT); /* Default font  */
      GpiDeleteSetId(hps, LCID_ALL);
      clrFore=CLR_BLACK;                /* Default color */
      if(!lLength)
        return lLength;
      /* FIXME: use format stack here!! */
      lMark=0; /* Set to text */
      break;
    }

  GpiSetColor(hps, clrFore);

  switch(lMark)
    {
    case HASH_HTML_LT:
      lPtl=gpiStringLength(hps, 1, "<" );
      break;
    case HASH_HTML_GT:
      lPtl=gpiStringLength(hps, 1, ">" );
      break;
    default:
      /* Get length of string in pts */
      lPtl=gpiStringLength(hps, lLength, chrText );
    }

  if( lPtl <= (rcl->xRight-ptl.x)) {
    /* String fits into line */
    hTemp=(htmlPart*)malloc(sizeof(htmlPart));
    memset(hTemp, 0, sizeof(htmlPart));
    hTemp->iType=lMark; /* Type of the text part */
    WinSetRect(WinQueryAnchorBlock(HWND_DESKTOP), &hTemp->rcl,
               ptl.x, ptl.y,
               ptl.x+ lPtl, ptl.y+fi.cyChar );
    hTemp->ptl=ptl;
    hTemp->lPts=lPtl;
    hTemp->ulParam=tagInfo->ulParam;
    hTemp->lColor=tagInfo->lColor;
    hTemp->ulFormat=ulFormat;
    hTemp->hpPrev=hPrev;
    hPrev=hTemp;

    switch(lMark)
      {
      case HASH_HTML_LT:
        hTemp->iLength=1;
        hTemp->chrText= "<";
        break;
      case HASH_HTML_GT:
        hTemp->iLength=1;
        hTemp->chrText= ">" ;
        break;
      default:
        hTemp->chrText=chrText;
        hTemp->iLength=lLength;
      }
    addToHtmlPartList(hwnd, hTemp);

    ptl.x+=lPtl;
    GpiMove(hps, &ptl);

    wndData->fFlags|=HTML_NOTFIRST_LINE; /* We outputed some text, so allow cursor movement
                                            into next line */
    return lLength;
  }
  else
    {
      LONG lDone=0;
      LONG lPtl;
      /* String is to long. Split in words and try to draw them */
     
      /* Get next word */
      while((lLength=newStringLength( hps, lTotalLength-lDone, chrText)) && lDone+lLength<=lTotalLength)
        {
          lPtl=gpiStringLength(hps, lLength, chrText);
          GpiQueryCurrentPosition(hps, &ptl);
          ptlMove=ptl;

          if(lPtl > (rcl->xRight-ptl.x))
            {
              /* Word doesn't fit. Check if it fits into a whole line */
              if(lPtl <= (rcl->xRight-fi.xPos))
                {
                  /* Yes, does fit. Move to next line and print it  */
                  ptl.x=fi.xPos;
                  rcl->yTop-=fi.cyChar;
                  ptl.y=rcl->yTop;  
                  GpiMove(hps, &ptl);
                  ptlMove=ptl;

                  ptlMove.x+=lPtl;
                  GpiMove(hps, &ptlMove);
                }
              else {
                /* No, too long for a whole line so start writing in current line and wrap */
                /* Get the amount of chars which do fit */
                lLength=getFittingStringLength(hps, lTotalLength-lDone, chrText, rcl->xRight-ptl.x);

                if(lLength) {
                  //GpiCharString(hps, lLength, chrText);
                  lPtl=gpiStringLength(hps, lLength, chrText);
                  ptlMove.x+=lPtl;
                  GpiMove(hps, &ptlMove);
                }
                else
                  {
                    /* Not enough space for a single char, move to next line */
                    ptl.x=fi.xPos;
                    rcl->yTop-=fi.cyChar;
                    ptl.y=rcl->yTop;  
                    GpiMove(hps, &ptl);
                  }
              }
            }
          else {
            /* The current string fits into the remaining space */
            ptlMove.x+=lPtl;
            GpiMove(hps, &ptlMove);
          }
          /* Crate the part info */
          hTemp=(htmlPart*)malloc(sizeof(htmlPart));
          memset(hTemp, 0, sizeof(htmlPart));
          hTemp->iType=lMark;
          WinSetRect(WinQueryAnchorBlock(HWND_DESKTOP), &hTemp->rcl,
                     ptl.x, ptl.y,
                     ptl.x+lPtl, ptl.y+fi.cyChar );

          hTemp->ptl=ptl;
          hTemp->lPts=lPtl;
          hTemp->ulParam=tagInfo->ulParam;
          hTemp->lColor=tagInfo->lColor;
          hTemp->ulFormat=ulFormat;
          hTemp->hpPrev=hPrev;
          hPrev=hTemp;

          switch(lMark)
            {
            case HASH_HTML_LT:
              hTemp->iLength=1;
              hTemp->chrText= "<";
              break;
            case HASH_HTML_GT:
              hTemp->iLength=1;
              hTemp->chrText= ">";
              break;
            default:
              hTemp->chrText=chrText;
              hTemp->iLength=lLength;
            }

          addToHtmlPartList(hwnd, hTemp);
          wndData->fFlags|=HTML_NOTFIRST_LINE; /* We outputed some text, so allow cursor movement
                                                  into next line */

          lDone+=lLength;
          chrText+=lLength; /* Move write mark in string */
        }
      return lDone;
    }
  return 0;
}


static void formatText(HWND hwnd, RECTL *rcl)
{
  LONG lLength;
  LONG  lTotalLength;
  char *chrPos;
  char *chrEnd;
  LONG lDone=0;
  RECTL rclTemp;
  HPS hps;
  HTMLWNDDATA* wndData;
  /* For getting display font */
  char fontName[255];
  ULONG  len;
  ULONG  attrFound;

  wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  if(!wndData)
    return ;

  /* Reset data */
  freeHtmlPartList(hwnd);   /* New formatting so free what we might have */
  wndData->fFlags=0;

  if(!wndData->chrText)
    return;

  lTotalLength=strlen(wndData->chrText);
  chrPos=wndData->chrText; /* current position in string is first char */
  //rcl->xLeft+=2;
  rclTemp=*rcl;

  rclTemp.xLeft+=LEFT_PADDING;

  hps=WinGetPS(hwnd);

  /* Get font attributes of window text */
  memset(&wndData->fatText, 0, sizeof(FATTRS));
  wndData->fatText.usRecordLength=sizeof(FATTRS);
  // Query the current window font. It is set in WM_CREATE
  len=WinQueryPresParam(hwnd,
  //len=WinQueryPresParam(WinQueryWindow(hwnd, QW_PARENT),
                        PP_FONTNAMESIZE,0,&attrFound,sizeof(fontName),
                        fontName, 0 /*QPF_NOINHERIT*/);
  //HlpWriteToTrapLog("Font: %s\n", fontName);
  if(len) {
    char *chrFont;
    
    if((chrFont=strchr(fontName, '.'))!=NULLHANDLE) {
      chrFont++;
      strcpy(wndData->fatText.szFacename, chrFont);
    }    
    /* Fill fat.lMaxBaselineExt and fat.lAveCharWidth */
    setFattr(hps, wndData->fatText.szFacename, atol(fontName), &wndData->fatText);
  }

  GpiQueryCharBox(hps, &wndData->sizef);

  /* Select a font for headings */
  memcpy(&wndData->fatHeading, &wndData->fatText, sizeof(FATTRS));
  selectHeadingFont(hps, &wndData->fatHeading, fontName);


  /* Skip whitespace */
  while(*chrPos==' ' && *chrPos!=0)
    {
      chrPos++;
      lDone++;
    }

  while(lDone<=lTotalLength && *chrPos!=0)
    {
      TAGINFO tagInfo={0};

      /* Check if starting with a mark */
      if(*chrPos=='<')
        chrPos=parseMark(lLength, chrPos, &tagInfo); /* Start of next text part in chrPos afterwards */
      //SysWriteToTrapLog("chrPos: %s", chrPos);      

      /* Get next mark '<' */
      chrEnd=findNextMark(chrPos);

      //SysWriteToTrapLog("chrEnd: %s", chrEnd);      
      lLength=chrEnd-chrPos; /* Length of text to be drawn */
      //SysWriteToTrapLog("Length: %d", lLength);      
      /* Text part found or single tag like end tag followed immediatly from another tag */
      lDone+=doFormat(hps, hwnd, lLength, chrPos, &rclTemp, &tagInfo, lDone, wndData);
      chrPos=chrEnd;
    } /* while */
  
  /* Vertical size of whole text */
  wndData->ulCyText=rcl->yTop-rclTemp.yTop+BOTTOM_PADDING; /* Add some space at the bottom of the text */
  if(wndData->ulCyText>rcl->yTop) {
    /* Set scrollbar range and position */
    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETSCROLLBAR, MPFROMSHORT(1),
               MPFROM2SHORT( 1,wndData->ulCyText-rcl->yTop));
    
    /* Set size of thumb */
    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETTHUMBSIZE,
               MPFROM2SHORT( rcl->yTop, wndData->ulCyText), 0L);
    
    WinShowWindow(WinWindowFromID(hwnd, FID_VERTSCROLL), TRUE);
  }
  else {
    WinShowWindow(WinWindowFromID(hwnd, FID_VERTSCROLL), FALSE);
    wndData->lYViewPort=0;
  }
  WinReleasePS(hps);

  /* Clean up */
  freeFormatInfoList(hwnd);
}



/*
  FIXME: Use position info for optimized drawing instead of relying on
  system clipping in HPS.
 */
void drawItNow(HWND hwnd, HPS hps, RECTL *rcl, LONG x, LONG y)
{
  htmlPart *hTemp;
  LONG clrFore;
  LONG curColor;
  POINTL ptl, ptlUnder;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);
  BOOL bHeading=FALSE;


  if(!wndData)
      return;

  /* */
  GpiCreateLogColorTable(hps, 0, LCOLF_RGB, 0, 0, NULLHANDLE);

  hTemp=wndData->hPartList;

  curColor=wndData->lForeColor;
  clrFore=wndData->lForeColor;

  /* Set font */
  setDefaultFormat(hps, &clrFore,  hwnd);
  while(hTemp)
    {
      switch(hTemp->iType)
        {
        case MARK_H1:
          {
            if(!bHeading) {
              setHeadingFormat(hps, &clrFore, hwnd);
              bHeading=TRUE;
            }
            break;
          }
        case MARK_H1_END:
          setDefaultFormat(hps, &clrFore,  hwnd);
          bHeading=FALSE;
          break;
        case MARK_LINK:
          {
            setLinkFormat(hps, &clrFore, hwnd);
            break;
          }
        case MARK_LINK_END:
          {
            GpiSetCharSet(hps, LCID_DEFAULT);
            GpiDeleteSetId(hps, LCID_ALL);
            //            GpiSetColor(hps, CLR_GREEN /*curColor*/);
               clrFore=curColor;
            break;
          }
        case MARK_UNDERLINED:
          {
            setUnderlineFormat(hps, &clrFore, hwnd);
            break;
          }
        case MARK_UNDERLINED_END:
          {
            GpiSetCharSet(hps, LCID_DEFAULT);
            GpiDeleteSetId(hps, LCID_ALL);
            break;
          }
        case MARK_BOLD:
          setBoldFormat(hps, &clrFore, hwnd);
          break;
        case MARK_BOLD_END:
          GpiSetCharSet(hps, LCID_DEFAULT);
          GpiDeleteSetId(hps, LCID_ALL);
          break;
        case MARK_COLOR:
          clrFore=hTemp->lColor;
          curColor=clrFore;
          break;
        case MARK_RULER:
          {
            /* Draw ruler */
            clrFore=0x777777;                /* Default color */
            GpiSetColor(hps, clrFore);
            
            ptl=hTemp->ptl;
            ptl.y+=wndData->lYViewPort+y;
            ptl.x+=x;

            GpiMove(hps, &ptl);
            ptl.x+=hTemp->iLength;
            GpiLine(hps, &ptl);

            clrFore=0xffffff;                /* Default color */
            GpiSetColor(hps, clrFore);
            ptl.y-=1;
            GpiMove(hps, &ptl);
            ptl.x-=hTemp->iLength;
            GpiLine(hps, &ptl);

            hTemp=hTemp->hpNext;
            continue;
          }
        default:
          //          GpiSetCharSet(hps, LCID_DEFAULT);
          //     GpiDeleteSetId(hps, LCID_ALL);
          clrFore=wndData->lForeColor;
          //          setDefaultFormat(hps, &clrFore,  hwnd);
          curColor=clrFore;
          break;
        }

      GpiSetColor(hps, clrFore);

      ptl=hTemp->ptl;
      ptlUnder=ptl;
      ptl.y+=wndData->lYViewPort+y;
      ptl.x+=x;

      GpiMove(hps, &ptl);
      GpiCharString(hps, hTemp->iLength, hTemp->chrText);

      hTemp=hTemp->hpNext;
    }
  return;
}


#define WPS_WIZARD
MRESULT EXPENTRY htmlProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
    {

    case WM_MOUSEMOVE:
      {
        POINTL ptlTemp;
        static POINTL ptl;
        htmlPart *hTemp;

        ptlTemp.x=SHORT1FROMMP(mp1);
        ptlTemp.y=SHORT2FROMMP(mp1);

        if(ptlTemp.x!=ptl.x || ptlTemp.y!=ptl.y) {
          ptl=ptlTemp;

          if((hTemp=checkForLink(hwnd,  &ptlTemp))!=NULLHANDLE) {
            HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);
            /* Mouse is over a link */
            if(wndData->hPtr)
              WinSetPointer(HWND_DESKTOP, wndData->hPtr);
            //          else
            //            WinSetPointer(HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP, SPTR_MOVE, FALSE));
            // hTemp=getHeadHtmlPart(hTemp);
            // drawLinkSelected
            return MRTRUE;
          }
          break;
        }
        return MRTRUE;
        //      break;
      }
    case WM_BUTTON1DOWN:
      {
        POINTL ptl;

        ptl.x=SHORT1FROMMP(mp1);
        ptl.y=SHORT2FROMMP(mp1);
        break;
      }
    case WM_BUTTON1CLICK:
      {
        POINTL ptl;
        ULONG ulLink;
        ptl.x=SHORT1FROMMP(mp1);
        ptl.y=SHORT2FROMMP(mp1);
        
        if((ulLink=clickLink(hwnd,  &ptl))!=0) {
          WinSendMsg(WinQueryWindow(hwnd, QW_OWNER), WM_COMMAND, MPFROMSHORT((SHORT)ulLink),
                     MPFROM2SHORT(CMDSRC_OTHER, TRUE));

          return MRTRUE;
        }

        return MRFALSE;
      }
    case WM_PAINT:
      {
        RECTL rcl;
        HPS hps;
        ULONG  attrFound;
        RGB bg;
        LONG lBg;
        HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

        hps=WinBeginPaint(hwnd, NULLHANDLE, &rcl);

        WinQueryWindowRect(hwnd, &rcl);

        GpiCreateLogColorTable(hps, LCOL_PURECOLOR, LCOLF_RGB, 0, 0, NULL);

        /* Prevent overpainting of scrollbar */
        if(WinIsWindowVisible(WinWindowFromID(hwnd, FID_VERTSCROLL)))
          rcl.xRight-=wndData->lVertBarWidth;

#ifdef WPS_WIZARD
        // Query the current background colour (inherited from parent)
        if(WinQueryPresParam(WinQueryWindow(WinQueryWindow(hwnd, QW_PARENT), QW_PARENT),//hwnd,
                              PP_BACKGROUNDCOLOR,0,&attrFound,sizeof(bg),
                             &bg, 0 /*QPF_NOINHERIT*/)) {
          lBg=(bg.bRed<<16)+(bg.bGreen<<8) + bg.bBlue;
          /* Set it */
          WinFillRect(hps, &rcl, (LONG) lBg);
          //     WinFillRect(hps, &rcl, 0x00FFFFFF);
        }
#else
        // Query the current background colour (inherited from parent)
        if(WinQueryPresParam(/*WinQueryWindow(WinQueryWindow(hwnd, QW_PARENT), QW_PARENT),*/hwnd,
                             PP_BACKGROUNDCOLOR,0,&attrFound,sizeof(bg),
                             &bg, 0 /*QPF_NOINHERIT*/)) {
          lBg=(bg.bRed<<16)+(bg.bGreen<<8) + bg.bBlue;
          /* Set it */
          WinFillRect(hps, &rcl, (LONG) lBg);
        }
#endif
        else
          WinFillRect(hps, &rcl, 0x00FFFFFF);
        
        drawItNow(hwnd, hps, &rcl, 0, 0);

        WinEndPaint(hps);
        return MRFALSE;
      }
    case WM_PRESPARAMCHANGED:
      {
        switch(LONGFROMMP(mp1))
          {
#if 0
          case PP_BACKGROUNDCOLOR:
            {
              ULONG  attrFound;
              RGB bg;

              if(WinQueryPresParam(hwnd,
                                   PP_BACKGROUNDCOLOR,0,&attrFound,sizeof(bg),
                                   &bg, QPF_NOINHERIT))                
                // Set the background colour of parent
                WinSetPresParam(WinQueryWindow(hwnd, QW_PARENT),
                                PP_BACKGROUNDCOLOR,(ULONG)sizeof(RGB) , &bg);            
            break;
            }
#endif
          case PP_FOREGROUNDCOLOR:
            {
              ULONG  attrFound;
              LONG fg=0;
              HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

              if(WinQueryPresParam(hwnd,
                                   PP_FOREGROUNDCOLOR,0,&attrFound,sizeof(fg),
                                   &fg, QPF_NOINHERIT))                

                wndData->lForeColor=(LONG)fg;
            break;
            }

          case PP_FONTNAMESIZE:
            {
              HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

              if(wndData) {
                RECTL rcl;          

                WinQueryWindowRect(hwnd, &rcl);
                rcl.xRight-= wndData->lVertBarWidth;
                
                formatText( hwnd,  &rcl);
                
                WinInvalidateRect(hwnd, NULLHANDLE, TRUE);
              }
            }
          default:
            break;
          }
        break;
      }
      /* Vertical slider */
    case WM_VSCROLL:
      {
        switch(SHORT1FROMMP(mp1))
          {
          case FID_VERTSCROLL:
            {
              HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

              switch(SHORT2FROMMP(mp2))
                {
                case SB_LINEDOWN:
                  {
                    RECTL rcl;
                    WinQueryWindowRect(hwnd, &rcl);
                    wndData->lYViewPort+=10;//wndData->ulCyText;
                    if(wndData->lYViewPort+rcl.yTop>wndData->ulCyText)
                      wndData->lYViewPort=wndData->ulCyText-rcl.yTop;
                    rcl.xRight-=wndData->lVertBarWidth;
                    WinInvalidateRect(hwnd, &rcl, TRUE);
                    /* Position slider */
                    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETSCROLLBAR,
                               MPFROMSHORT(1+wndData->lYViewPort),
                               MPFROM2SHORT( 1,wndData->ulCyText-rcl.yTop));
                  break;
                  }
                case SB_LINEUP:
                  {
                    RECTL rcl;
                    WinQueryWindowRect(hwnd, &rcl);
                    wndData->lYViewPort-=10;//wndData->ulCyText;
                    if(wndData->lYViewPort<0)
                      wndData->lYViewPort=0;
                    rcl.xRight-=wndData->lVertBarWidth;
                    WinInvalidateRect(hwnd, &rcl, TRUE);
                    /* Position slider */
                    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETSCROLLBAR,
                               MPFROMSHORT(1+wndData->lYViewPort),
                               MPFROM2SHORT( 1,wndData->ulCyText-rcl.yTop));
                  break;
                  }
                case SB_PAGEDOWN:
                  {
                    RECTL rcl;
                    WinQueryWindowRect(hwnd, &rcl);
                    wndData->lYViewPort+=rcl.yTop;//wndData->ulCyText;
                    if(wndData->lYViewPort+rcl.yTop>wndData->ulCyText)
                      wndData->lYViewPort=wndData->ulCyText-rcl.yTop;
                    rcl.xRight-=wndData->lVertBarWidth;
                    WinInvalidateRect(hwnd, &rcl, TRUE);
                    /* Position slider */
                    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETSCROLLBAR,
                               MPFROMSHORT(1+wndData->lYViewPort),
                               MPFROM2SHORT( 1,wndData->ulCyText-rcl.yTop));
                  break;
                  }
                case SB_PAGEUP:
                  {
                    RECTL rcl;
                    WinQueryWindowRect(hwnd, &rcl);
                    wndData->lYViewPort-=rcl.yTop;//wndData->ulCyText;
                    if(wndData->lYViewPort<0)
                      wndData->lYViewPort=0;
                    rcl.xRight-=wndData->lVertBarWidth;
                    WinInvalidateRect(hwnd, &rcl, TRUE);
                    /* Position slider */
                    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETSCROLLBAR,
                               MPFROMSHORT(1+wndData->lYViewPort),
                               MPFROM2SHORT( 1,wndData->ulCyText-rcl.yTop));
                    break;
                  }
                  /* User moved slider and released the button */
                case SB_SLIDERPOSITION:
                  {
                    RECTL rcl;
                    WinQueryWindowRect(hwnd, &rcl);
                    wndData->lYViewPort=SHORT1FROMMP(mp2);
                    if(wndData->lYViewPort<0)
                      wndData->lYViewPort=0;
                    else if(wndData->lYViewPort+rcl.yTop>wndData->ulCyText)
                      wndData->lYViewPort=wndData->ulCyText-rcl.yTop;

                    rcl.xRight-=wndData->lVertBarWidth;
                    WinInvalidateRect(hwnd, &rcl, TRUE);
                    /* Position slider */
                    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETSCROLLBAR,
                               MPFROMSHORT(1+wndData->lYViewPort),
                               MPFROM2SHORT( 1,wndData->ulCyText-rcl.yTop));
                    break;
                  }
                case SB_SLIDERTRACK:
                  {
                    RECTL rcl;
                    WinQueryWindowRect(hwnd, &rcl);
                    wndData->lYViewPort=SHORT1FROMMP(mp2);
                    if(wndData->lYViewPort<0)
                      wndData->lYViewPort=0;
                    else if(wndData->lYViewPort+rcl.yTop>wndData->ulCyText)
                      wndData->lYViewPort=wndData->ulCyText-rcl.yTop;

                    rcl.xRight-=wndData->lVertBarWidth;
                    WinInvalidateRect(hwnd, &rcl, TRUE);
                    /* Position slider */
                    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETSCROLLBAR,
                               MPFROMSHORT(1+wndData->lYViewPort),
                               MPFROM2SHORT( 1,wndData->ulCyText-rcl.yTop));
                    break;
                  }
                default:
                  break;
                }
              break;
            }
          default:
            break;
          }

      break;
      }
    case WM_SETWINDOWPARAMS:
      {
      PWNDPARAMS pwp=(PWNDPARAMS)PVOIDFROMMP(mp1);
      RECTL rcl;
      HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

      if(!pwp)
        break;

      /* New text */
      if(pwp->fsStatus & WPM_TEXT)
        {
          WinQueryWindowRect(hwnd, &rcl);
          
          if(wndData) {
            if(wndData->chrText)
              free(wndData->chrText);
            
            wndData->chrText=NULLHANDLE;
            
            if(pwp->cchText) {
              
              /* Only if strlen != 0 */
              wndData->chrText=(char*)malloc(pwp->cchText+1); /* Don't know if terminating 0 is included... */
              if(wndData->chrText) {
                strncpy(wndData->chrText, pwp->pszText, pwp->cchText);
                /* Terminate string */
                wndData->chrText[pwp->cchText]=0;
              }
            }
            rcl.xRight-= wndData->lVertBarWidth;
            
            formatText( hwnd,  &rcl);
            
            WinInvalidateRect(hwnd, NULLHANDLE, TRUE);
          }
        }
      break;
      }
    case WM_WINDOWPOSCHANGED:
      {
        PSWP pswp;
        PSWP pswpNew=(PSWP)PVOIDFROMMP(mp1);
        pswp=pswpNew++;

        if(!pswp)
          break;

        /* Only size */
        if((pswp->fl & SWP_SIZE)/*&& pswp->cx!=pswpNew->cx*/) {
          HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

          if(wndData) {
            RECTL rcl={0};

            /* Only reformat if width changes */
            //if(pswp->cx!=pswpNew->cx) {
              //wndData->ulCyText

              WinQueryWindowRect(hwnd, &rcl);
              rcl.xRight=pswp->cx-wndData->lVertBarWidth;
              rcl.yTop=pswp->cy;
              if(wndData->chrText)
                formatText(hwnd,  &rcl);
              /* Position scrollbar */        
              WinSetWindowPos(WinWindowFromID(hwnd, FID_VERTSCROLL),
                              NULLHANDLE, rcl.xRight, 0, wndData->lVertBarWidth, rcl.yTop, SWP_MOVE|SWP_SIZE);
              //}
          }
        }
        break;
      }
    case WM_CREATE:
      {
        FRAMECDATA frameCtrl={0};
        RECTL rcl;
        HTMLWNDDATA* wndData;
        MRESULT mr;
        PCREATESTRUCT pCreate=(PCREATESTRUCT)PVOIDFROMMP(mp2);

        ULONG ulVBar=WinQuerySysValue(HWND_DESKTOP, SV_CXVSCROLL);
        mr=g_oldStatic( hwnd, msg, mp1, mp2);

        /* Create scrollbar */
        frameCtrl.cb=sizeof(frameCtrl);
        frameCtrl.flCreateFlags=FCF_VERTSCROLL;
        WinCreateFrameControls(hwnd, &frameCtrl, "");
        
        /* Position Framecontrol */        
        WinQueryWindowRect( hwnd, &rcl);
        WinSetWindowPos(WinWindowFromID(hwnd, FID_VERTSCROLL),
                        NULLHANDLE, rcl.xRight-ulVBar, 0, ulVBar, rcl.yTop, SWP_MOVE|SWP_SIZE);
        
        wndData=(HTMLWNDDATA*)malloc(sizeof(HTMLWNDDATA));
        if(wndData) {
          char * fontName="9.WarpSans";

          memset(wndData, 0, sizeof(HTMLWNDDATA));          
          /* Scrollbar width */
          wndData->lVertBarWidth=ulVBar;
          WinSetWindowULong(hwnd, ulQWP_WNDDATA, (ULONG)wndData);

          // Set the font
          WinSetPresParam(hwnd,
                          PP_FONTNAMESIZE,(ULONG) strlen(fontName)+1, /* +1 because of terminating 0 */
                          fontName);

          if(pCreate->pszText && strlen(pCreate->pszText)) {
            wndData->chrText=(char*)malloc(strlen(pCreate->pszText)+1); 
            if(wndData->chrText)
              strncpy(wndData->chrText, pCreate->pszText, strlen(pCreate->pszText));
            rcl.xRight-= wndData->lVertBarWidth;
            formatText( hwnd, &rcl);
          }
        }
        return mr;
      break;
      }
    case WM_DESTROY:
      {
        HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

        if(wndData)
          {
            freeHtmlPartList(hwnd);
            free(wndData);
          }
        break;
      }
    case WM_APPTERMINATENOTIFY:
      {
        switch(SHORT2FROMMP(mp1))
          {
          case HTML_SET_HANDPTR:
            {
              HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

              if(!wndData)
                break;
              wndData->hPtr=(HPOINTER)LONGFROMMP(mp2);
              break;
            }     
          default:
            break;
          }
        return MRFALSE;
      }
    default:
      break;
  }
  if(g_oldStatic)
    return g_oldStatic( hwnd, msg, mp1, mp2);
  return WinDefWindowProc( hwnd, msg, mp1, mp2);
}

BOOL registerHtmlWnd(void)
{
  CLASSINFO ci;

  /* Get on screen font resolution */
  ulHorzFontRes=queryHorzFontRes();
  /* Register the new text control */

  if(WinQueryClassInfo(WinQueryAnchorBlock(HWND_DESKTOP),
                       WC_STATIC,
                       &ci))
    {
      g_ulStaticDataOffset=ci.cbWindowData;
      g_oldStatic=ci.pfnWindowProc;
      ulQWP_WNDDATA=g_ulStaticDataOffset;
      
      
      if(WinRegisterClass(WinQueryAnchorBlock(HWND_DESKTOP),
                          (PSZ)HTML_WND,
                          htmlProc,
                          (ci.flClassStyle) &~(CS_PUBLIC|CS_PARENTCLIP),
                          ci.cbWindowData+sizeof(void*)))
        {
          /* */
          return TRUE;
        }
    }
  return FALSE;
}
