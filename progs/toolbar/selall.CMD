/* Select all objects in the folder */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

ret=SysSetObjectData(WPSWizCallFunc("cwQueryRealName", ARG(2), 1), "MENUITEMSELECTED=114")

exit
