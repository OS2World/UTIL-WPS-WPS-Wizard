/* rexx -  Boot drive info gadget showing on the desktop background:

		-Bootdrive size

*/

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

IF ARG() = 0 THEN DO
	/* Start the gadget */
	PARSE SOURCE . . theScript
	IF ARG(1)="" THEN
		call SysSetObjectdata "<WP_DESKTOP>", "WIZLAUNCHGADGET="theScript	
	Exit(0)
END


NUMERIC DIGITS 15 /* We need this for calculating with hex numbers */

ulCur=0

PARSE SOURCE . . theScript
scriptPath=FILESPEC("d", theScript)||FILESPEC("p", theScript)

/* Info for storing data in private ini file */
APPKEY="bootdriveGadget"
MEMPOS="pos"


drawBorder=0
NORMAL_BORDER=1
SUNKEN_BORDER=2
COLOR_BORDER=3

/* The Cairo commands */

CAIROCMD1="<ctxt>save</><lwidth>0.9</><linecap>round</><colora>0.3 0.3 0.3 0.3</><move>0.12 0.55</><line>9.57 0.55</><stroke></><colora>0 0.4 0.75 0.4</><move>0.05 0.5</><line>9.5 0.5</><stroke></><lwidth>0.01</><colora>0.4 0.9 0.9 0.6</><rect>1.1 0.2 "

cairoBarValue=8.5
maxBarValue=8.5

CAIROCMD2=" 0.6</><fill></><lwidth>0.05</><colora>0.9 1 1 1</><rect>1.1 0.2 8.5 0.6</><stroke></><colora>1 1 1 1</><font>Arial 0 1</><fontsize>0.4</><move>1.25 0.62</><textkey>Text</><ctxt>restore</><ctxt>save</><transl>0.01 -0.05</><scale>0.016 0.016</>"

CAIROCMD2=CAIROCMD2||"<imgkey>OverlayImage # 1</><ctxt>restore</>"

SELECT
WHEN ARG(1)="/GADGETSTARTED" THEN DO
	theObject=ARG(2)

	SIGNAL ON SYNTAX NAME errorHandler
	SIGNAL ON ERROR NAME errorHandler
	SIGNAL ON FAILURE NAME errorHandler

	INIFILE=sysIni('USER' , 'IconTheme', 'IconFolder')
	INIFILE=LEFT(INIFILE, LENGTH(INIFILE)-1)
	if RIGHT(INIFILE, 1)<>'\' THEN INIFILE=INIFILE||"\"
	THEMEINI=INIFILE||"wpswiz-theme.ini"
	INIFile=system.path.wpswizardini

	/* Default position after installation */
	defaultPos=system.screen.width-530||" 100 400 40"
	PARSE VAR defaultPos x y cx cy rest

	stem._x=x
    stem._y=y
	stem._cy=cy               /* cy */
	stem._cx=cx               /* cx */
	stem._type=0              /* Gadget type */
	stem._hwnd=ARG(3)         /* hwnd */
    stem._flags=GSTYLE_POPUP  /* We want to have a popup menu */
	stem._font="9.WarpSans"   /* font */
    stem._alpha=100;

	stem._classdll=system.path.wpswizard||"\CWTHEME.DLL"
	stem._gadgetname="CWCairoGadget"

/* Get saved position if any */
	ret=Sysini(INIFile, APPKEY, MEMPOS)

	IF  ret >< "ERROR:" THEN DO
		PARSE VAR ret stem._x stem._y stem._cx stem._cy rest
	END

	ret=WIZCREATEGADGET("DESKTOP", "stem.", "bootdriveGadget")
    if ret==0 THEN DO
       call RxMessageBox "The boot drive gadget can't be created. Make sure the theme addon for WPS wizard is installed correctly.", "Problem"
       Exit(1)
    END

    call bootdrivegadget.image  "gadgetBootDrive"

    call onTimer

    call bootdrivegadget.refresh

	ret=WizStartTimer(ARG(3), 10, 2500)

	/* Message loop for the gadget */
	DO FOREVER
		ret=WIZGETMESSAGE(ARG(3))
		if ret<>'' THEN 
			INTERPRET "call "ret
	END

	EXIT(0)
END

OTHERWISE
	Exit(0)
END

exit(0)


/************* Command handling ***************************/

bootdriveGadget.onPopUp:
/*
	ARG(1): hwnd of client area
	ARG(2): x
	ARG(3): y
*/

	menu.0=1
	menu.1="~Close Gadget"

	menu._x=ARG(2)
	menu._y=ARG(3)

	ret=WPSWizCallWinFunc("menuPopupMenu", ARG(1), 'menu.')
return

bootdriveGadget.onCommand:
/*
	ARG(1): hwnd of client area
	ARG(2): ID
	ARG(3): source (menu or button)
*/
	PARSE SOURCE . . theScript

	SELECT
	WHEN ARG(2)=1 THEN DO
		/* Gadget closes. Save configuration. */
		call SysIni iniFile, APPKEY, MEMPOS, bootdriveGadget.position()
		ret=WizDestroyGadget("bootdriveGadget") 
		exit(0)
	END
	OTHERWISE
		NOP
	END
return


onTimer:
	ulCur=WizQueryFreeMem("physical")/1024

/* Set text of drive bar */
	driveText=SysDriveInfo(system.path.bootdrive)
	PARSE VAR driveText drive free total driveName
	newText="["drive"] "TRUNC(free/1024)" free / "
    newText=newText||TRUNC(total/1024)" total"'00'x

	if newText><memText then DO
		memText=newText
        call SysIni THEMEINI, "gadgetBootDrive", "Text", memText
        cairoBarValue=maxBarValue*(free/total)
        call SysIni THEMEINI, "gadgetBootDrive", "CairoCommands", CAIROCMD1||cairoBarValue||CAIROCMD2
        call bootdrivegadget.image  "gadgetBootDrive"
        call bootdrivegadget.refresh
 /*		call groupGadget.membargadget.barvalue ulCur , system.physicalmemory/1024  */
	END

return

quit:
	exit(0)

errorHandler:
	PARSE SOURCE . . theScript

	ret=WPSWizGadgetFunc("cwDisplayRexxError", "")
	ret=WPSWizGadgetFunc("cwDisplayRexxError", theScript||": ")
	ret=WPSWizGadgetFunc("cwDisplayRexxError", "Error in line "||SIGL)	
exit(0)

