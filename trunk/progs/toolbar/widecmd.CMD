/* Open a command window  */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

dirName=WPSWizCallFunc("cwQueryRealName", ARG(2), 1)

oldDir=DIRECTORY(WPSWizCallFunc("cwQueryRealName", ARG(2), 1))

ADDRESS CMD 'start "Command line: 'WPSWizCallFunc("cwQueryRealName", ARG(2), 0)'" /F mode 100,66'

call DIRECTORY oldDir
exit
