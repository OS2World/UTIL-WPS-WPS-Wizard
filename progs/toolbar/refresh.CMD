/* Refresh folder */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

ret=SysSetObjectData(WPSWizCallFunc("cwQueryRealName", ARG(2), 1), "MENUITEMSELECTED=503")

exit
