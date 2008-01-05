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

#define SETTINGS_CLASSNAME         "CWWizzSettings"
#define HELPFILENAME "menufold.hlp"

#define ID_OPTIONPAGENAME       1003
#define ID_CONFIGURATIONCLASSNAME  2000







