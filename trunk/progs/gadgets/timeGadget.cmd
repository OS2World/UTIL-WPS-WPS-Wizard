/*
 rexx -  Time gadget showing the current data and time on the desktop
         background
*/

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/* Check if the script was directly started */
IF ARG() = 0 THEN DO
	/* Start the gadget */
	PARSE SOURCE . . theScript
	call SysSetObjectdata "<WP_DESKTOP>", "WIZLAUNCHGADGET="theScript	
	Exit(0)
END


NUMERIC DIGITS 15 /* We need this for calculating with hex numbers */

SELECT
WHEN ARG(1)="/GADGETSTARTED" THEN DO
	theObject=ARG(2)

	SIGNAL ON SYNTAX NAME errorHandler
	SIGNAL ON ERROR NAME errorHandler
	SIGNAL ON FAILURE NAME errorHandler

	/* Info for storing data in WPS-wizard ini file */
	INIFile=system.path.wpswizardini
	APPKEY="timegadget"
	TIMEPOS="timepos"
	TIMEFONT="timefont"

	defaultPos=system.screen.width-220||" 100 200 150"

	PARSE VAR defaultPos x y cx cy rest

/* Gadget create info */
	thestem._x=x                 /* x  */
	thestem._y=y                 /* y  */
	thestem._cx=cx               /* cx */
	thestem._cy=cy               /* cy */
	thestem._type=HTML_GADGET    /* Gadget type */
	thestem._hwnd=ARG(3)         /* hwnd */
	thestem._flags=GSTYLE_POPUP  /* We want a popup menu */
	thestem._font="12.Helv"      /* font */

/* Get saved position if any */
	ret=Sysini(INIFile, APPKEY, TIMEPOS)
	IF  ret >< "ERROR:" THEN DO
		PARSE VAR ret thestem._x thestem._y thestem._cx thestem._cy rest
	END
/* Get saved font if any */
	ret=Sysini(INIFile, APPKEY, TIMEFONT)
	IF  ret >< "ERROR:" THEN DO
		thestem._font=ret
	END

/* Create gadget on the desktop */
	rc=WizCreateGadget("DESKTOP", "thestem." , "timeGadget")

/* Set gadget text */
	gadgetText='<p><font color="#00dddddd">'||TIME()||'</font></p><p><font color="#00ff0000">'||DATE()||"</font></p>"
	call timeGadget.text gadgetText

/* Start a timer sending a message every 1000ms  */
	ret=WizStartTimer( ARG(3), 10, 1000)

/* Gadget message loop */
	DO FOREVER
		ret=WIZGETMESSAGE(ARG(3))
		if ret<>'' THEN 
			INTERPRET "call "ret
	END

	EXIT(0)
END
OTHERWISE
	/* We shouldn't end here... */
	Exit(0)
END

exit(0)


timegadget.onCommand:
/*
	ARG(1): hwnd of client area
	ARG(2): ID
	ARG(3): source (menu or button)
*/
	PARSE SOURCE . . theScript
	IF ARG(2)=1 THEN DO
		call SysIni iniFile, APPKEY, TIMEPOS,  timeGadget.position()
		call SysIni iniFile, APPKEY, TIMEFONT, timeGadget.font()
		rc=wizDestroyGadget("timegadget")
		exit(0)
	END
return


timegadget.onPopUp:
/*
	ARG(1): hwnd of client area
	ARG(2): x
	ARG(3): y
*/
	menu.0=1
	menu.1="Close Gadget"

	menu._x=ARG(2)
	menu._y=ARG(3)

	ret=WPSWizCallWinFunc("menuPopupMenu", ARG(1), 'menu.')
return

onTimer:
  newText='<p><font color="#00dddddd">'||TIME()||'</font></p><p><font color="#00ff0000">'||DATE()||"</font></p>"
  IF newText><gadgetText THEN DO
    gadgetText=newText
    call timeGadget.text gadgetText
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

