

#ifndef PMWP_INCLUDED

#define  SETTINGS_PAGE_NUMBERS     0x01

typedef struct _PAGEINFO
{
  ULONG   cb;
  HWND    hwndPage;
  PFNWP   pfnwp;
  ULONG   resid;
  PVOID   pCreateParams;
  USHORT  dlgid;
  USHORT  usPageStyleFlags;
  USHORT  usPageInsertFlags;
  USHORT  usSettingsFlags;
  PSZ     pszName;
  USHORT  idDefaultHelpPanel;
  USHORT  usReserved2;
  PSZ     pszHelpLibraryName;
  PUSHORT pHelpSubtable;   
  HMODULE hmodHelpSubtable;
  ULONG   ulPageInsertId;
} PAGEINFO;

typedef PAGEINFO *PPAGEINFO;

#endif

int wpwizzQueryNumFuncs(int  iVersion); /* Returns num funcs or zero */

BOOL wpwizzLoad(void * structFunctions, int iNumFuncs);
BOOL wpwizzUnload(HINI hIni);


void wpwizzRefreshSettings(HINI hIni, ULONG ulReserved);
BOOL wpwizzSave();
BOOL wpwizzRestore();

BOOL wpwizzCheckVersion(int  iVersion);
BOOL wpwizzAddSettingsPages( int iNumPage, PAGEINFO * pageInfo, HWND hwndNotebook);


typedef BOOL (APIENTRY FN_2)(int iNumPage, PAGEINFO * pageInfo, HWND hwndNotebook);
typedef FN_2 * PFN_2 ;

#define calcVersion(ma, mi, b) ((ma<<16)+(mi<<8)+b)

typedef int (APIENTRY FN_1)(int iVersion);
typedef FN_1 * PFN_1 ;

#define INITDLLS_MSGPARAM            0
#define FREEDLLS_MSGPARAM          1







