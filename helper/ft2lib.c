#define INCL_DOSMODULEMGR     /* Module Manager values */
#define INCL_DOSERRORS        /* Error values */
#define INCL_PM
#define INCL_GPI
#define INCL_DOS
#include <stdlib.h>
#include <stdio.h>
#include <os2.h>
#include <ft2lib.h>

ULONG fFt2LibInit=FALSE;
PSZ pszFt2Error;

void APIENTRY Ft2EnableFontEngine(BOOL fEnable);

APIRET Ft2LibInit(void);

APIRET (* APIENTRY pfnFt2EnableFontEngine)(BOOL fEnable);
HPS (*APIENTRY pfnFt2CreatePS)(HAB hab, HDC hdc, PSIZEL psizlSize, ULONG flOptions);
BOOL (*APIENTRY pfnFt2DestroyPS)(HPS hps);


void APIENTRY Ft2EnableFontEngine(BOOL fEnable)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2EnableFontEngine)
    pfnFt2EnableFontEngine(fEnable);
}


/** @defgroup ft2lib_comgpi     Common GPI Control APIs
 * @{ */
HPS     APIENTRY Ft2CreatePS(HAB hab, HDC hdc, PSIZEL psizlSize, ULONG flOptions)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CreatePS)
    return pfnFt2CreatePS(hab, hdc, psizlSize, flOptions);
  else
    return GpiCreatePS(hab,hdc, psizlSize, flOptions);
}

BOOL    APIENTRY Ft2DestroyPS(HPS hps)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2DestroyPS)
    return pfnFt2DestroyPS(hps);
  else
    return GpiDestroyPS(hps);
}

BOOL    (*APIENTRY pfnFt2Associate)(HPS hps, HDC hdc);
BOOL    APIENTRY Ft2Associate(HPS hps, HDC hdc)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2Associate)
    return pfnFt2Associate(hps,hdc);
  else
    return GpiAssociate(hps,hdc);
}

BOOL    (*APIENTRY pfnFt2RestorePS)(HPS hps, LONG lPSid);
BOOL    APIENTRY Ft2RestorePS(HPS hps, LONG lPSid)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2RestorePS)
    return pfnFt2RestorePS(hps, lPSid);
  else
    return GpiRestorePS(hps, lPSid);
}

LONG    (*APIENTRY pfnFt2SavePS)(HPS hps);
LONG    APIENTRY Ft2SavePS(HPS hps)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SavePS)
    return pfnFt2SavePS(hps);
  else
    return GpiSavePS(hps);
}

BOOL    (*APIENTRY pfnFt2Erase)(HPS hps);
BOOL    APIENTRY Ft2Erase(HPS hps)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2Erase)
    return pfnFt2Erase(hps);
  else
    return GpiErase(hps);
}

/** @} */


/** @defgroup ft2lib_globalprim Global Primitive APIs
 * @{ */
BOOL    (*APIENTRY pfnFt2SetColor)(HPS hps, LONG lColor);
BOOL    APIENTRY Ft2SetColor(HPS hps, LONG lColor)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetColor)
    return pfnFt2SetColor(hps, lColor);
  else
    return GpiSetColor(hps, lColor);
}

LONG    (*APIENTRY pfnFt2QueryColor)(HPS hps);
LONG    APIENTRY Ft2QueryColor(HPS hps)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryColor)
    return pfnFt2QueryColor(hps);
  else
    return GpiQueryColor(hps);
}

BOOL    (*APIENTRY pfnFt2SetAttrs)(HPS hps, LONG lPrimType, ULONG flAttrMask, ULONG flDefMask, PBUNDLE ppbunAttrs);
BOOL    APIENTRY Ft2SetAttrs(HPS hps, LONG lPrimType, ULONG flAttrMask, ULONG flDefMask, PBUNDLE ppbunAttrs)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetAttrs)
    return pfnFt2SetAttrs(hps, lPrimType, flAttrMask, flDefMask, ppbunAttrs);
  else
    return GpiSetAttrs(hps, lPrimType, flAttrMask, flDefMask, ppbunAttrs);
}

BOOL    (*APIENTRY pfnFt2SetBackColor)(HPS hps, LONG lColor);
BOOL    APIENTRY Ft2SetBackColor(HPS hps, LONG lColor)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetBackColor)
    return pfnFt2SetBackColor(hps, lColor);
  else
    return GpiSetBackColor(hps, lColor);
}

LONG    (*APIENTRY pfnFt2QueryBackColor)(HPS hps);
LONG    APIENTRY Ft2QueryBackColor(HPS hps)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryBackColor)
    return pfnFt2QueryBackColor(hps);
  else
    return GpiQueryBackColor(hps);
}

BOOL    (*APIENTRY pfnFt2SetMix)(HPS hps, LONG lMixMode);
BOOL    APIENTRY Ft2SetMix(HPS hps, LONG lMixMode)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetMix)
    return pfnFt2SetMix(hps, lMixMode);
  else
    return GpiSetMix(hps, lMixMode);
}

BOOL    (*APIENTRY pfnFt2SetBackMix)(HPS hps, LONG lMixMode);
BOOL    APIENTRY Ft2SetBackMix(HPS hps, LONG lMixMode)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetBackMix)
    return pfnFt2SetBackMix(hps, lMixMode);
  else
    return GpiSetBackMix(hps, lMixMode);
}

BOOL    (*APIENTRY pfnFt2SetCurrentPosition)(HPS hps, PPOINTL pptlPoint);
BOOL    APIENTRY Ft2SetCurrentPosition(HPS hps, PPOINTL pptlPoint)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetCurrentPosition)
    return pfnFt2SetCurrentPosition(hps, pptlPoint);
  else
    return GpiSetCurrentPosition(hps, pptlPoint);
}

BOOL    (*APIENTRY pfnFt2QueryCurrentPosition)(HPS hps, PPOINTL pptlPoint);
BOOL    APIENTRY Ft2QueryCurrentPosition(HPS hps, PPOINTL pptlPoint)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryCurrentPosition)
    return pfnFt2QueryCurrentPosition(hps, pptlPoint);
  else
    return GpiQueryCurrentPosition(hps, pptlPoint);
}

LONG    (*APIENTRY pfnFt2SetPel)(HPS hps, PPOINTL pptlPoint);
LONG    APIENTRY Ft2SetPel(HPS hps, PPOINTL pptlPoint)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetPel)
    return pfnFt2SetPel(hps, pptlPoint);
  else
    return GpiSetPel(hps, pptlPoint);
}

LONG    (*APIENTRY pfnFt2QueryPel)(HPS hps, PPOINTL pptlPoint);
LONG    APIENTRY Ft2QueryPel(HPS hps, PPOINTL pptlPoint)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryPel)
    return pfnFt2QueryPel(hps, pptlPoint);
  else
    return GpiQueryPel(hps, pptlPoint);
}

HBITMAP (*APIENTRY pfnFt2SetBitmap)(HPS hps, HBITMAP hbm);
HBITMAP APIENTRY Ft2SetBitmap(HPS hps, HBITMAP hbm)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetBitmap)
    return pfnFt2SetBitmap(hps, hbm);
  else
    return GpiSetBitmap(hps, hbm);
}

/** @defgroup ft2lib_lineprim   Line Primitive APIs
 * @{ */
LONG    (*APIENTRY pfnFt2Box)(HPS hps, LONG lControl, PPOINTL pptlPoint, LONG lHRound, LONG lVRound);
LONG    APIENTRY Ft2Box(HPS hps, LONG lControl, PPOINTL pptlPoint, LONG lHRound, LONG lVRound)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2Box)
    return pfnFt2Box(hps, lControl, pptlPoint, lHRound, lVRound);
  else
    return GpiBox(hps, lControl, pptlPoint, lHRound, lVRound);
}

BOOL    (*APIENTRY pfnFt2Move)(HPS hps, PPOINTL pptlPoint);
BOOL    APIENTRY Ft2Move(HPS hps, PPOINTL pptlPoint)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2Move)
    return pfnFt2Move(hps, pptlPoint);
  else
    return GpiMove(hps, pptlPoint);
}



/** @defgroup ft2lib_charprim   Character Primitive APIs
 * @{ */
BOOL    (*APIENTRY pfnFt2QueryCharStringPos)(HPS hps, ULONG flOptions, LONG lCount,
                                        PCH pchString, PLONG alXincrements, PPOINTL aptlPositions);
BOOL    APIENTRY Ft2QueryCharStringPos(HPS hps, ULONG flOptions, LONG lCount,
                                        PCH pchString, PLONG alXincrements, PPOINTL aptlPositions)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryCharStringPos)
    return pfnFt2QueryCharStringPos(hps, flOptions, lCount,pchString, alXincrements, aptlPositions);
  else
    return GpiQueryCharStringPos(hps, flOptions, lCount,pchString, alXincrements, aptlPositions);
}


BOOL    (*APIENTRY pfnFt2QueryTextBox)(HPS hps, LONG lCount1, PCH pchString,LONG lCount2, PPOINTL aptlPoints);
BOOL    APIENTRY Ft2QueryTextBox(HPS hps, LONG lCount1, PCH pchString,LONG lCount2, PPOINTL aptlPoints)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryTextBox)
    return pfnFt2QueryTextBox(hps, lCount1, pchString,lCount2, aptlPoints);
  else
    return GpiQueryTextBox(hps, lCount1, pchString,lCount2, aptlPoints);
}


BOOL    (*APIENTRY pfnFt2SetCharSet)(HPS hps, LONG llcid);
BOOL    APIENTRY Ft2SetCharSet(HPS hps, LONG llcid)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetCharSet)
    return pfnFt2SetCharSet(hps, llcid);
  else
    return GpiSetCharSet(hps, llcid);
}

LONG    (*APIENTRY pfnFt2QueryCharSet)(HPS hps);
LONG    APIENTRY Ft2QueryCharSet(HPS hps)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryCharSet)
    return pfnFt2QueryCharSet(hps);
  else
    return GpiQueryCharSet(hps);
}


BOOL    (*APIENTRY pfnFt2SetCharBox)(HPS hps, PSIZEF psizfxBox);
BOOL    APIENTRY Ft2SetCharBox(HPS hps, PSIZEF psizfxBox)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetCharBox)
    return pfnFt2SetCharBox(hps, psizfxBox);
  else
    return GpiSetCharBox(hps, psizfxBox);
}

BOOL    (*APIENTRY pfnFt2QueryCharBox)(HPS hps, PSIZEF psizfxSize);
BOOL    APIENTRY Ft2QueryCharBox(HPS hps, PSIZEF psizfxSize)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryCharBox)
    return pfnFt2QueryCharBox(hps, psizfxSize);
  else
    return GpiQueryCharBox(hps, psizfxSize);
}

BOOL    (*APIENTRY pfnFt2SetCharAngle)(HPS hps, PGRADIENTL pgradlAngle);
BOOL    APIENTRY Ft2SetCharAngle(HPS hps, PGRADIENTL pgradlAngle)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetCharAngle)
    return pfnFt2SetCharAngle(hps, pgradlAngle);
  else
    return GpiSetCharAngle(hps, pgradlAngle);
}

BOOL    (*APIENTRY pfnFt2QueryCharAngle)(HPS hps, PGRADIENTL pgradlAngle);
BOOL    APIENTRY Ft2QueryCharAngle(HPS hps, PGRADIENTL pgradlAngle)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryCharAngle)
    return pfnFt2QueryCharAngle(hps, pgradlAngle);
  else
    return GpiQueryCharAngle(hps, pgradlAngle);
}

BOOL    (*APIENTRY pfnFt2SetTextAlignment)(HPS hps, LONG lHoriz, LONG lVert);
BOOL    APIENTRY Ft2SetTextAlignment(HPS hps, LONG lHoriz, LONG lVert)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetTextAlignment)
    return pfnFt2SetTextAlignment(hps, lHoriz, lVert);
  else
    return GpiSetTextAlignment(hps, lHoriz, lVert);
}

BOOL    (*APIENTRY pfnFt2QueryTextAlignment)(HPS hps, PLONG plHoriz, PLONG plVert);
BOOL    APIENTRY Ft2QueryTextAlignment(HPS hps, PLONG plHoriz, PLONG plVert)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryTextAlignment)
    return pfnFt2QueryTextAlignment(hps, plHoriz, plVert);
  else
    return GpiQueryTextAlignment(hps, plHoriz, plVert);
}

BOOL    (*APIENTRY pfnFt2SetCharBreakExtra)(HPS hps, FIXED  BreakExtra);
BOOL    APIENTRY Ft2SetCharBreakExtra(HPS hps, FIXED  BreakExtra)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetCharBreakExtra)
    return pfnFt2SetCharBreakExtra(hps, BreakExtra);
  else
    return GpiSetCharBreakExtra(hps, BreakExtra);
}

BOOL    (*APIENTRY pfnFt2SetCharDirection)(HPS hps, LONG lDirection);
BOOL    APIENTRY Ft2SetCharDirection(HPS hps, LONG lDirection)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetCharDirection)
    return pfnFt2SetCharDirection(hps, lDirection);
  else
    return GpiSetCharDirection(hps, lDirection);
}

LONG    (*APIENTRY pfnFt2CharString)(HPS hps, LONG lCount, PCH pchString);
LONG    APIENTRY Ft2CharString(HPS hps, LONG lCount, PCH pchString)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CharString)
    return pfnFt2CharString(hps, lCount, pchString);
  else
    return GpiCharString(hps, lCount, pchString);
}


LONG    (*APIENTRY pfnFt2CharStringAt)(HPS hps, PPOINTL pptlPoint, LONG lCount,
                                 PCH pchString);
LONG    APIENTRY Ft2CharStringAt(HPS hps, PPOINTL pptlPoint, LONG lCount,
                                 PCH pchString)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CharStringAt)
    return pfnFt2CharStringAt(hps, pptlPoint, lCount, pchString);
  else
    return GpiCharStringAt(hps, pptlPoint, lCount, pchString);
}


LONG    (*APIENTRY pfnFt2CharStringPos)(HPS hps, PRECTL prclRect, ULONG flOptions,
                                  LONG lCount, PCH pchString, PLONG alAdx);
LONG    APIENTRY Ft2CharStringPos(HPS hps, PRECTL prclRect, ULONG flOptions,
                                  LONG lCount, PCH pchString, PLONG alAdx)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CharStringPos)
    return pfnFt2CharStringPos(hps, prclRect, flOptions, lCount, pchString, alAdx);
  else
    return GpiCharStringPos(hps, prclRect, flOptions, lCount, pchString, alAdx);
}

LONG    (*APIENTRY pfnFt2CharStringPosAt)(HPS hps, PPOINTL pptlStart, PRECTL prclRect,
                                    ULONG flOptions, LONG lCount, PCH pchString,
                                    PLONG alAdx);
LONG    APIENTRY Ft2CharStringPosAt(HPS hps, PPOINTL pptlStart, PRECTL prclRect,
                                    ULONG flOptions, LONG lCount, PCH pchString,
                                    PLONG alAdx)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CharStringPosAt)
    return pfnFt2CharStringPosAt(hps, pptlStart, prclRect, flOptions, lCount, pchString, alAdx);
  else
    return GpiCharStringPosAt(hps, pptlStart, prclRect, flOptions, lCount, pchString, alAdx);
}

BOOL    (*APIENTRY pfnFt2QueryCharStringPosAtA)(HPS hps, PPOINTL pptlStart, ULONG flOptions, LONG lCount,
                                          PCH pchString, PLONG alXincrements, PPOINTL aptlPositions);
BOOL    APIENTRY Ft2QueryCharStringPosAtA(HPS hps, PPOINTL pptlStart, ULONG flOptions, LONG lCount,
                                          PCH pchString, PLONG alXincrements, PPOINTL aptlPositions)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryCharStringPosAtA)
    return pfnFt2QueryCharStringPosAtA(hps, pptlStart, flOptions, lCount, pchString, alXincrements, aptlPositions);
  else
    return GpiQueryCharStringPosAt(hps, pptlStart, flOptions, lCount, pchString, alXincrements, aptlPositions);
}

BOOL    (*APIENTRY pfnFt2SetCp)(HPS hps, ULONG ulCodePage);
BOOL    APIENTRY Ft2SetCp(HPS hps, ULONG ulCodePage)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2SetCp)
    return pfnFt2SetCp(hps, ulCodePage);
  else
    return GpiSetCp(hps, ulCodePage);
}

ULONG   (*APIENTRY pfnFt2QueryCp)(HPS hps);
ULONG   APIENTRY Ft2QueryCp(HPS hps)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryCp)
    return pfnFt2QueryCp(hps);
  else
    return GpiQueryCp(hps);
}


/** @defgroup ft2lib_fontapi    Physical and Logical Font APIs
 * @{ */

LONG    (*APIENTRY pfnFt2CreateLogFont)(HPS hps, PSTR8 pName, LONG lLcid, PFATTRS pfatAttrs);
LONG    APIENTRY Ft2CreateLogFont(HPS hps, PSTR8 pName, LONG lLcid, PFATTRS pfatAttrs)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CreateLogFont)
    return pfnFt2CreateLogFont(hps, pName, lLcid, pfatAttrs);
  else
    return GpiCreateLogFont(hps, pName, lLcid, pfatAttrs);
}

BOOL    (*APIENTRY pfnFt2DeleteSetId)(HPS hps, LONG lLcid);
BOOL    APIENTRY Ft2DeleteSetId(HPS hps, LONG lLcid)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2DeleteSetId)
    return pfnFt2DeleteSetId(hps, lLcid);
  else
    return GpiDeleteSetId(hps, lLcid);
}

BOOL    (*APIENTRY pfnFt2QueryLogicalFont)(HPS PS, LONG lcid, PSTR8 name,
                                     PFATTRS attrs, LONG length);
BOOL    APIENTRY Ft2QueryLogicalFont(HPS PS, LONG lcid, PSTR8 name,
                                     PFATTRS attrs, LONG length)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryLogicalFont)
    return pfnFt2QueryLogicalFont(PS, lcid, name, attrs, length);
  else
    return GpiQueryLogicalFont(PS, lcid, name, attrs, length);
}

LONG    (*APIENTRY pfnFt2QueryFonts)(HPS hps, ULONG flOptions, PSZ pszFacename,
                                PLONG plReqFonts, LONG lMetricsLength, PFONTMETRICS afmMetrics);
LONG    APIENTRY Ft2QueryFonts(HPS hps, ULONG flOptions, PSZ pszFacename,
                                PLONG plReqFonts, LONG lMetricsLength, PFONTMETRICS afmMetrics)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryFonts)
    return pfnFt2QueryFonts(hps, flOptions, pszFacename, plReqFonts, lMetricsLength, afmMetrics);
  else
    return GpiQueryFonts(hps, flOptions, pszFacename, plReqFonts, lMetricsLength, afmMetrics);
}

BOOL    (*APIENTRY pfnFt2QueryFontMetrics)(HPS hps, LONG lMetricsLength, PFONTMETRICS pfmMetrics);
BOOL    APIENTRY Ft2QueryFontMetrics(HPS hps, LONG lMetricsLength, PFONTMETRICS pfmMetrics)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryFontMetrics)
    return pfnFt2QueryFontMetrics(hps, lMetricsLength, pfmMetrics);
  else
    return GpiQueryFontMetrics(hps, lMetricsLength, pfmMetrics);
}


LONG    (*APIENTRY pfnFt2QueryKerningPairs)(HPS hps, LONG lCount, PKERNINGPAIRS akrnprData);
LONG    APIENTRY Ft2QueryKerningPairs(HPS hps, LONG lCount, PKERNINGPAIRS akrnprData)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2QueryKerningPairs)
    return pfnFt2QueryKerningPairs(hps, lCount, akrnprData);
  else
    return GpiQueryKerningPairs(hps, lCount, akrnprData);
}

BOOL    (*APIENTRY pfnFt2LoadPublicFonts)(HAB hab, PSZ psz);
BOOL    APIENTRY Ft2LoadPublicFonts(HAB hab, PSZ psz)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2LoadPublicFonts)
    return pfnFt2LoadPublicFonts(hab, psz);
  else
    return GpiLoadPublicFonts(hab, psz);
}

BOOL    (*APIENTRY pfnFt2UnloadPublicFonts)(HAB hab, PSZ psz);
BOOL    APIENTRY Ft2UnloadPublicFonts(HAB hab, PSZ psz)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2UnloadPublicFonts)
    return pfnFt2UnloadPublicFonts(hab, psz);
  else
    return GpiUnloadPublicFonts(hab, psz);
}

BOOL    (*APIENTRY pfnFt2LoadFonts)(HAB hab, PSZ psz);
BOOL    APIENTRY Ft2LoadFonts(HAB hab, PSZ psz)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2LoadFonts)
    return pfnFt2LoadFonts(hab, psz);
  else
    return GpiLoadFonts(hab, psz);
}

BOOL    (*APIENTRY pfnFt2UnloadFonts)(HAB hab, PSZ psz);
BOOL    APIENTRY Ft2UnloadFonts(HAB hab, PSZ psz)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2UnloadFonts)
    return pfnFt2UnloadFonts(hab, psz);
  else
    return GpiUnloadFonts(hab, psz);
}

/** @defgroup ft2lib_palapi     Palette Manager APIs
 * @{ */
HPAL    (*APIENTRY pfnFt2CreatePalette)(HAB hab, ULONG flOptions, ULONG ulFormat, ULONG ulCount, PULONG aulTable);
HPAL    APIENTRY Ft2CreatePalette(HAB hab, ULONG flOptions, ULONG ulFormat, ULONG ulCount, PULONG aulTable)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CreatePalette)
    return pfnFt2CreatePalette(hab, flOptions, ulFormat, ulCount, aulTable);
  else
    return GpiCreatePalette(hab, flOptions, ulFormat, ulCount, aulTable);
}

BOOL    (*APIENTRY pfnFt2DeletePalette)(HPAL hpal);
BOOL    APIENTRY Ft2DeletePalette(HPAL hpal)
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2DeletePalette)
    return pfnFt2DeletePalette(hpal);
  else
    return GpiDeletePalette(hpal);
}

/** @} */


/** @defgroup ft2lib_pmwin      PMWIN Replacement APIs
 * @{ */
/*HPS     APIENTRY Ft2BeginPaint(HWND hwnd, HPS hps, PRECTL prclPaint);
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CreatePS)
    return pfnFt2Move(hps, pptlPoint);
  else
    return GpiMove(hps, pptlPoint);
}

BOOL    APIENTRY Ft2EndPaint(HPS hps);
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CreatePS)
    return pfnFt2Move(hps, pptlPoint);
  else
    return GpiMove(hps, pptlPoint);
}

HPS     APIENTRY Ft2GetPS(HWND hwnd);
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CreatePS)
    return pfnFt2Move(hps, pptlPoint);
  else
    return GpiMove(hps, pptlPoint);
}

BOOL    APIENTRY Ft2ReleasePS(HPS hps);
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CreatePS)
    return pfnFt2Move(hps, pptlPoint);
  else
    return GpiMove(hps, pptlPoint);
}

HPS     APIENTRY Ft2GetClipPS(HWND hwnd, HWND hwndClip, ULONG fl);
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CreatePS)
    return pfnFt2Move(hps, pptlPoint);
  else
    return GpiMove(hps, pptlPoint);
}

HPS     APIENTRY Ft2GetScreenPS(HWND hwndDesktop);
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CreatePS)
    return pfnFt2Move(hps, pptlPoint);
  else
    return GpiMove(hps, pptlPoint);
}

LONG    APIENTRY Ft2DrawTextA(HPS hps, LONG cchText, PCH lpchText, PRECTL prcl, LONG clrFore, LONG clrBack, ULONG flCmd);
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CreatePS)
    return pfnFt2Move(hps, pptlPoint);
  else
    return GpiMove(hps, pptlPoint);
}

LONG    APIENTRY Ft2DrawTabbedTextA(HPS hps, LONG cchText, LONG lTabWidth, PCH lpchText, PRECTL prcl, LONG clrFore, LONG clrBack, ULONG flCmd);
{
  if(!fFt2LibInit)
     Ft2LibInit();
  if(pfnFt2CreatePS)
    return pfnFt2Move(hps, pptlPoint);
  else
    return GpiMove(hps, pptlPoint);
}
  */
//BOOL    APIENTRY Ft2EnableFontEngine(BOOL fEnable);
/** @} */



APIRET Ft2LibInit(void)
{
 char pszUser[100];
 PSZ      ModuleName ="FT2LIB.DLL";
 ULONG ulID;
 UCHAR    LoadError[256] = "";          /* Area for Load failure information */
 HMODULE  ModuleHandle   = NULLHANDLE;  /* Module handle                     */
 PFN      ModuleAddr     = 0;           /* Pointer to a system function      */
 ULONG    ModuleType     = 0;           /* Module type                       */
 APIRET   rc             = NO_ERROR;    /* Return code                       */


   rc = DosLoadModule(LoadError,               /* Failure information buffer */
                      sizeof(LoadError),       /* Size of buffer             */
                      ModuleName,              /* Module to load             */
                      &ModuleHandle);          /* Module handle returned     */
   if (rc != NO_ERROR)
        return FALSE;

    pszFt2Error = malloc(256);
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2EnableFontEngine",&pfnFt2EnableFontEngine)!=NO_ERROR){sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2CreatePS"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2CreatePS",&pfnFt2CreatePS)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2CreatePS"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2DestroyPS",&pfnFt2DestroyPS)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2DestroyPS"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2Associate",&pfnFt2Associate)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2Associate"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2RestorePS",&pfnFt2RestorePS)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2RestorePS"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SavePS",&pfnFt2SavePS)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SavePS"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2Erase",&pfnFt2Erase)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2Erase"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetColor",&pfnFt2SetColor)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetColor"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryColor",&pfnFt2QueryColor)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryColor"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetAttrs",&pfnFt2SetAttrs)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetAttrs"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetBackColor",&pfnFt2SetBackColor)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetBackColor"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryBackColor",&pfnFt2QueryBackColor)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryBackColor"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetMix",&pfnFt2SetMix)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetMix"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetBackMix",&pfnFt2SetBackMix)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetBackMix"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetCurrentPosition",&pfnFt2SetCurrentPosition)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetCurrentPosition"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryCurrentPosition",&pfnFt2QueryCurrentPosition)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryCurrentPosition"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetPel",&pfnFt2SetPel)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetPel"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryPel",&pfnFt2QueryPel)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryPel"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetBitmap",&pfnFt2SetBitmap)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetBitmap"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2Box",&pfnFt2Box)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2Box"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2Move",&pfnFt2Move)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2Move"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryCharStringPos",&pfnFt2QueryCharStringPos)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryCharStringPos"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryTextBox",&pfnFt2QueryTextBox)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryTextBoxA"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetCharSet",&pfnFt2SetCharSet)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetCharSet"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryCharSet",&pfnFt2QueryCharSet)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryCharSet"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetCharBox",&pfnFt2SetCharBox)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetCharBox"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryCharBox",&pfnFt2QueryCharBox)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryCharBox"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetCharAngle",&pfnFt2SetCharAngle)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetCharAngle"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryCharAngle",&pfnFt2QueryCharAngle)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryCharAngle"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetTextAlignment",&pfnFt2SetTextAlignment)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetTextAlignment"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryTextAlignment",&pfnFt2QueryTextAlignment)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryTextAlignment"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetCharBreakExtra",&pfnFt2SetCharBreakExtra)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetCharBreakExtra"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetCharDirection",&pfnFt2SetCharDirection)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetCharDirection"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2CharString",&pfnFt2CharString)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2CharString"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2CharStringAt",&pfnFt2CharStringAt)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2CharStringAt"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2CharStringPos",&pfnFt2CharStringPos)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2CharStringPos"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2CharStringPosAt",&pfnFt2CharStringPosAt)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2CharStringPosAt"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryCharStringPosAtA",&pfnFt2QueryCharStringPosAtA)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryCharStringPosAtA"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2SetCp",&pfnFt2SetCp)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2SetCp"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryCp",&pfnFt2QueryCp)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryCp"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2CreateLogFont",&pfnFt2CreateLogFont)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2CreateLogFont"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2DeleteSetId",&pfnFt2DeleteSetId)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2DeleteSetId"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryLogicalFont",&pfnFt2QueryLogicalFont)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryLogicalFont"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryFonts",&pfnFt2QueryFonts)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryFonts"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryFontMetrics",&pfnFt2QueryFontMetrics)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryFontMetrics"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2QueryKerningPairs",&pfnFt2QueryKerningPairs)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2QueryKerningPairs"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2LoadPublicFonts",&pfnFt2LoadPublicFonts)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2LoadPublicFonts"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2UnloadPublicFonts",&pfnFt2UnloadPublicFonts)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2UnloadPublicFonts"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2LoadFonts",&pfnFt2LoadFonts)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2LoadFonts"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2UnloadFonts",&pfnFt2UnloadFonts)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2UnloadFonts"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2CreatePalette",&pfnFt2CreatePalette)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2CreatePalette"); return FALSE;}
   if(DosQueryProcAddr(ModuleHandle, 0,"Ft2DeletePalette",&pfnFt2DeletePalette)!=NO_ERROR){ sprintf(pszFt2Error,"DosQueryProcAddr failed for: %s","Ft2DeletePalette"); return FALSE;}
   fFt2LibInit=TRUE;
   return fFt2LibInit;
}

PSZ Ft2LibGetErrorMessage()
{
 return pszFt2Error;
}

