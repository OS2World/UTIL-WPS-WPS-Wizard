/* rexx -  System info gadget showing on the desktop background:

		-Physical memory
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
APPKEY="sysinfo"
MEMPOS="membarpos"
MEMCOLOR="membarcolor"
MEMBACKCOLOR="membarbackcolor"
MEMICONPOS="memiconpos"
DRIVEPOS="drivebarpos"
DRIVECOLOR="drivecolor"
DRIVEBACKCOLOR="drivebackcolor"
DRIVEICONPOS="driveiconpos"
GROUPPOS="grouppos"
GROUPBACKCOLOR="groupbackcolor"
GROUPBORDER="groupborder"
GROUPBORDERWIDTH="groupborderwidth"
UPTIMEPOS="uptimepos"
UPTIMECOLOR="uptimecolor"
UPTIMEFONT="uptimefont"
TIMEPOS="timepos"
TIMECOLOR="timecolor"
TIMEFONT="timefont"

childsLocked="YES"
drawBorder=0
NORMAL_BORDER=1
SUNKEN_BORDER=2
COLOR_BORDER=3

SELECT
WHEN ARG(1)="/GADGETSTARTED" THEN DO
	theObject=ARG(2)

	SIGNAL ON SYNTAX NAME errorHandler
	SIGNAL ON ERROR NAME errorHandler
	SIGNAL ON FAILURE NAME errorHandler

	INIFile=system.path.wpswizardini
	/* Default position after installation */
	defaultPos=system.screen.width-330||" 100 320 22"
	PARSE VAR defaultPos x y cx cy rest

	/* Default colors */
	backColor=X2D('8080ff') 
	foreColor=X2D('dddddd')

/* Get saved border info if any */
	ret=Sysini(INIFile, APPKEY, GROUPBORDER)
	IF  ret >< "ERROR:" THEN
		drawBorder=STRIP(ret)

	/* Create a group gadget */
	stem._x=x    /* x  */
	stem._y=y+30   /* y  */
	stem._cx=20   /* cx */
	stem._cy=20  /* cy */

	stem._x=x-30    /* x  */
	stem._y=y+30-100   /* y  */
	stem._cx=360  /* cx */
	stem._cy=100  /* cy */

	stem._type=GROUP_GADGET    /* Gadget type */
	stem._hwnd=ARG(3) /* hwnd */
	stem._flags=GSTYLE_POPUP /* + GSTYLE_NORESIZE */
	SELECT
		WHEN drawBorder=COLOR_BORDER THEN
			stem._flags=stem._flags + GSTYLE_COLORBORDER
		WHEN drawBorder=NORMAL_BORDER THEN
			stem._flags=stem._flags + GSTYLE_BORDER
		WHEN drawBorder=SUNKEN_BORDER THEN
			stem._flags=stem._flags + GSTYLE_SUNKENBORDER
		OTHERWISE
			NOP
	END
	stem.7="10.Helv" /* font */

/* Get saved position if any */
	ret=Sysini(INIFile, APPKEY, GROUPPOS)
	IF  ret >< "ERROR:" THEN DO
		PARSE VAR ret stem._x stem._y stem._cx stem._cy rest
	END

	/* Create the gadget on the desktop */
	ret=WIZCREATEGADGET("DESKTOP", "stem.", groupGadget)
	call groupGadget.text "System           info"

/* Get back color. This is the color of the border */
	ret=Sysini(INIFile, APPKEY, GROUPBACKCOLOR)
	IF  ret >< "ERROR:" THEN
		call groupGadget.backcolor ret
	ELSE
		call groupGadget.backcolor backcolor

	/* Create a memory bar gadget owned by the group gadget */
	stem._x=x                /* x  */
	stem._y=y                /* y  */
	stem._cx=cx              /* cx */
	stem._cy=cy              /* cy */
	stem._type=BAR_GADGET    /* Gadget type */
	stem._hwnd=ARG(3)        /* hwnd */
	stem._flags=0 + GSTYLE_COLORBORDER
	stem._font="14.Helv" /* font */

	/* Get saved position if any */

/* Get saved position if any */
	ret=Sysini(INIFile, APPKEY, MEMPOS)
	IF  ret >< "ERROR:" THEN DO
		PARSE VAR ret stem._x stem._y stem._cx stem._cy rest
	END

	ulCur=0

	ret=WIZCREATEGADGET("groupGadget", "stem." , "membargadget")

	/* Get text color */
	ret=Sysini(INIFile, APPKEY, MEMCOLOR)
	IF  ret >< "ERROR:" THEN
		call groupGadget.membargadget.color ret
	ELSE
		call groupGadget.membargadget.color forecolor

	/* Get bar color */
	ret=Sysini(INIFile, APPKEY, MEMBACKCOLOR)
	IF  ret >< "ERROR:" THEN
		call groupGadget.membargadget.backcolor ret
	ELSE
		call groupGadget.membargadget.backcolor backcolor

	/* Set text */
	memText="Free physical memory: "||WizQueryFreeMem("physical")/1024||" / "||system.physicalmemory/1024
	ret=groupGadget.membargadget.text(memText)
	/* Set bar */
	call groupGadget.membargadget.barvalue WizQueryFreeMem("physical")/1024 , system.physicalmemory/1024

	stem._x=stem._x-25
	stem._cy=30               /* cy */
	stem._cx=30               /* cx */
	stem._type=ICON_GADGET    /* Gadget type */
	stem._hwnd=ARG(3)         /* hwnd */
	stem._flags=0
	stem._font="9.WarpSans"   /* font */

/* Get saved position if any */
	ret=Sysini(INIFile, APPKEY, MEMICONPOS)
	IF  ret >< "ERROR:" THEN DO
		PARSE VAR ret stem._x stem._y stem._cx stem._cy rest
	END

	ret=WIZCREATEGADGET("groupGadget", "stem.", "memiconGadget")
	call groupGadget.memicongadget.icon Scriptpath||"mem.ICO"

	/* Create a bootdrive bar gadget owned by the group gadget */
	stem._x=x    /* x  */
	stem._y=y-30    /* y  */
	stem._cx=cx   /* cx */
	stem._cy=cy   /* cy */
	stem._type=BAR_GADGET    /* Gadget type */
	stem._hwnd=ARG(3) /* hwnd */
	stem._flags=0 + GSTYLE_COLORBORDER
	stem._font="12.Helv" /* font */

/* Get saved position if any */
	ret=Sysini(INIFile, APPKEY, DRIVEPOS)
	IF  ret >< "ERROR:" THEN DO
		PARSE VAR ret stem._x stem._y stem._cx stem._cy rest
	END

	ret=WIZCREATEGADGET("groupGadget", "stem.", driveGadget)

	ret=Sysini(INIFile, APPKEY, DRIVECOLOR)
	IF  ret >< "ERROR:" THEN
		call groupGadget.drivegadget.color ret
	ELSE
		call groupGadget.drivegadget.color forecolor

	ret=Sysini(INIFile, APPKEY, DRIVEBACKCOLOR)
	IF  ret >< "ERROR:" THEN
		call groupGadget.drivegadget.backcolor ret
	ELSE
		call groupGadget.drivegadget.backcolor backcolor

/* Set text of drive bar */
	driveText=SysDriveInfo(system.path.bootdrive)
	PARSE VAR driveText drive free total driveName
	driveText="["drive"] "TRUNC(free/1024)" free"
	call groupGadget.drivegadget.text driveText
/* Set bar value */
	call groupGadget.driveGadget.barvalue free/1024 , total/1024


	stem._x=stem._x-25
	stem._cy=30               /* cy */
	stem._cx=30               /* cx */
	stem._type=ICON_GADGET    /* Gadget type */
	stem._hwnd=ARG(3)         /* hwnd */
	stem._flags=0
	stem._font="9.WarpSans"   /* font */

/* Get saved position if any */
	ret=Sysini(INIFile, APPKEY, DRIVEICONPOS)
	IF  ret >< "ERROR:" THEN DO
		PARSE VAR ret stem._x stem._y stem._cx stem._cy rest
	END

	ret=WIZCREATEGADGET("groupGadget", "stem.", "driveiconGadget")
	call groupGadget.driveicongadget.icon Scriptpath||"drive.ICO"


	/**** Create an uptime gadget owned by the group gadget ****/
	stem._x=x
	stem._y=stem._y-35        /* y  */
	stem._cy=35               /* cy */
	stem._cx=140              /* cx */
	stem._type=HTML_GADGET    /* Gadget type */
	stem._hwnd=ARG(3)         /* hwnd */
	stem._flags=0
	stem._font="9.WarpSans"   /* font */

/* Get saved position if any */
	ret=Sysini(INIFile, APPKEY, UPTIMEPOS)
	IF  ret >< "ERROR:" THEN DO
		PARSE VAR ret stem._x stem._y stem._cx stem._cy rest
	END

/* Get saved font if any */
	ret=Sysini(INIFile, APPKEY, UPTIMEFONT)
	IF  ret >< "ERROR:" THEN DO
		stem._font=ret
	END

	ret=WIZCREATEGADGET("groupGadget", "stem.", "uptimeGadget")

	ret=Sysini(INIFile, APPKEY, UPTIMECOLOR)
	IF  ret >< "ERROR:" THEN
		call groupGadget.uptimegadget.color ret
	ELSE
		call groupGadget.uptimegadget.color forecolor

	uptime=WIZQUERYUPTIME('s')
	days=STRIP(WORD(uptime, 1))
	IF days<>0 THEN 
		uptimetext="<p>Uptime: "WORD(uptime, 1) "days, "WORD(uptime,2)" h</p>"
	ELSE
		uptimetext="<p>Uptime: "WORD(uptime, 2)" h</p>"

	call groupGadget.uptimegadget.text uptimeText


	/**** Create an uptime gadget owned by the group gadget ****/
	stem._x=x+142
	stem._cy=35               /* cy */
	stem._cx=180              /* cx */
	stem._type=HTML_GADGET    /* Gadget type */
	stem._hwnd=ARG(3)         /* hwnd */
	stem._flags=0
	stem._font="9.WarpSans"   /* font */

/* Get saved position if any */
	ret=Sysini(INIFile, APPKEY, TIMEPOS)
	IF  ret >< "ERROR:" THEN DO
		PARSE VAR ret stem._x stem._y stem._cx stem._cy rest
	END

/* Get saved font if any */
	ret=Sysini(INIFile, APPKEY, TIMEFONT)
	IF  ret >< "ERROR:" THEN DO
		stem._font=ret
	END

	ret=WIZCREATEGADGET("groupGadget", "stem.", "timeGadget")

	ret=Sysini(INIFile, APPKEY, TIMECOLOR)
	IF  ret >< "ERROR:" THEN
		call groupGadget.timegadget.color ret
	ELSE
		call groupGadget.timegadget.color forecolor

	timetext="<p>"TIME() ", "DATE()"</p>"

	call groupGadget.timegadget.text timeText


/* Lock all childs */
	call groupgadget.membargadget.style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE
	call groupgadget.drivegadget.style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE 
	call groupgadget.uptimegadget.style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE 
	call groupgadget.timegadget.style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE 
	call groupgadget.driveicongadget.style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE 
	call groupgadget.memicongadget.style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE 

	ret=WizStartTimer(ARG(3), 10, 1000)

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

groupGadget.onPopUp:
/*
	ARG(1): hwnd of client area
	ARG(2): x
	ARG(3): y
*/

	menu.0=8
	menu.1="~Close Gadget"
	menu.2="-"
	IF childsLocked="YES" THEN
		menu.3="~Unlock childs"
	ELSE
		menu.3="~Lock childs"

	menu.4="-" /* Separator */
	menu.5="~No Border"
	menu.6="~Border"
	menu.7="~Sunken Border"
	menu.8="~Color Border"

	
	menu._x=ARG(2)
	menu._y=ARG(3)

	ret=WPSWizCallWinFunc("menuPopupMenu", ARG(1), 'menu.')

	SELECT
	WHEN drawBorder=NORMAL_BORDER THEN
		ret=WPSWizCallWinFunc("menuCheckItem", ret, 6, 0, 1)
	WHEN drawBorder=SUNKEN_BORDER THEN
		ret=WPSWizCallWinFunc("menuCheckItem", ret, 7, 0, 1)
	WHEN drawBorder=COLOR_BORDER THEN
		ret=WPSWizCallWinFunc("menuCheckItem", ret, 8, 0, 1)
	OTHERWISE
		ret=WPSWizCallWinFunc("menuCheckItem", ret, 5, 0, 1)
	END


return

groupGadget.onCommand:
/*
	ARG(1): hwnd of client area
	ARG(2): ID
	ARG(3): source (menu or button)
*/
	PARSE SOURCE . . theScript

	SELECT
	WHEN ARG(2)=1 THEN DO
		/* Gadget closes. Save configuration. */
		call SysIni iniFile, APPKEY, MEMPOS,  groupGadget.membarGadget.position()
		call SysIni iniFile, APPKEY, MEMCOLOR,  groupGadget.membarGadget.color()
		call SysIni iniFile, APPKEY, MEMBACKCOLOR,  groupGadget.membarGadget.backcolor()
		call SysIni iniFile, APPKEY, MEMICONPOS,  groupGadget.memiconGadget.position()
		call SysIni iniFile, APPKEY, DRIVEPOS,  groupGadget.driveGadget.position()
		call SysIni iniFile, APPKEY, DRIVECOLOR,  groupGadget.driveGadget.color()
		call SysIni iniFile, APPKEY, DRIVEBACKCOLOR,  groupGadget.driveGadget.backcolor()
		call SysIni iniFile, APPKEY, DRIVEICONPOS,  groupGadget.driveIconGadget.position()
		call SysIni iniFile, APPKEY, UPTIMEPOS,  groupGadget.uptimeGadget.position()
		call SysIni iniFile, APPKEY, UPTIMECOLOR,  groupGadget.uptimeGadget.color()
		call SysIni iniFile, APPKEY, UPTIMEFONT, groupgadget.uptimeGadget.font()
		call SysIni iniFile, APPKEY, TIMEPOS,  groupGadget.timeGadget.position()
		call SysIni iniFile, APPKEY, TIMECOLOR,  groupGadget.timeGadget.color()
		call SysIni iniFile, APPKEY, TIMEFONT, groupgadget.timeGadget.font()
		call SysIni iniFile, APPKEY, GROUPPOS,  groupGadget.position()
		ret=Sysini(INIFile, APPKEY, GROUPBORDER, drawBorder)
		call SysIni iniFile, APPKEY, GROUPBACKCOLOR,  groupGadget.Backcolor()

		ret=WizDestroyGadget("groupGadget.membarGadget") 
		ret=WizDestroyGadget("groupGadget.driveGadget") 
		ret=WizDestroyGadget("groupGadget.memiconGadget") 
		ret=WizDestroyGadget("groupGadget.driveiconGadget") 
		ret=WizDestroyGadget("groupGadget.uptimeGadget")
		ret=WizDestroyGadget("groupGadget.timeGadget")

		ret=WizDestroyGadget("groupGadget")
		exit(0)
	END
	WHEN ARG(2)=3 THEN DO
		IF childsLocked="YES" THEN DO
			/* Childs are now unlocked */
			childsLocked="NO"
			call groupgadget.membargadget.style 0, GSTYLE_NOACTIVATE 
			call groupgadget.drivegadget.style 0, GSTYLE_NOACTIVATE 
			call groupgadget.uptimegadget.style 0, GSTYLE_NOACTIVATE
			call groupgadget.timegadget.style 0, GSTYLE_NOACTIVATE 
			call groupgadget.driveicongadget.style 0, GSTYLE_NOACTIVATE 
			call groupgadget.memicongadget.style 0, GSTYLE_NOACTIVATE 
		END
		ELSE DO
			childsLocked="YES"
			call groupgadget.membargadget.style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE
			call groupgadget.drivegadget.style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE 
			call groupgadget.uptimegadget.style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE 
			call groupgadget.timegadget.style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE 
			call groupgadget.driveicongadget.style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE 
			call groupgadget.memicongadget.style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE 
		END
	END
	WHEN ARG(2)=5 THEN DO
		drawBorder=0
		call groupgadget.style 0, GSTYLE_SUNKENBORDER + GSTYLE_BORDER + GSTYLE_COLORBORDER
	END
	WHEN ARG(2)=6 THEN DO
		/* User wants a border */
		drawBorder=1
		call groupgadget.style GSTYLE_BORDER, GSTYLE_SUNKENBORDER + GSTYLE_BORDER + GSTYLE_COLORBORDER
	END
	WHEN ARG(2)=7 THEN DO
		/* Sunken border */
		drawBorder=2
		call groupgadget.style GSTYLE_SUNKENBORDER, GSTYLE_SUNKENBORDER + GSTYLE_BORDER + GSTYLE_COLORBORDER
	END
	WHEN ARG(2)=8 THEN DO
		/* Color border */
		drawBorder=3
		call groupgadget.style GSTYLE_COLORBORDER, GSTYLE_SUNKENBORDER + GSTYLE_BORDER + GSTYLE_COLORBORDER
	END
	OTHERWISE
		NOP
	END
return


onTimer:
	ulCur=WizQueryFreeMem("physical")/1024

	/* Update the memory bar */
	newText="Free physical memory: "||ulCur||" / "||system.physicalmemory/1024
	if newText><memText then DO
		memText=newText
		call groupGadget.membargadget.text memText
		call groupGadget.membargadget.barvalue ulCur , system.physicalmemory/1024
	END

	/* Update the drive bar */
	newText=SysDriveInfo(system.path.bootdrive)
	PARSE VAR newText drive free total driveName
	newText="["drive"] "TRUNC(free/1024)" free"
	if newText >< driveText THEN DO
		driveText=newText
		call groupGadget.drivegadget.text driveText
		call groupGadget.driveGadget.barvalue free/1024 , total/1024
	END

	uptime=WIZQUERYUPTIME('s')
	days=STRIP(WORD(uptime, 1))
	IF days<>0 THEN 
		newtext="<p>Uptime: "WORD(uptime, 1) "days, "WORD(uptime,2)" h</p>"
	ELSE
		newtext="<p>Uptime: "WORD(uptime, 2)" h</p>"

	if newText >< uptimeText THEN DO
		uptimetext=newtext
		call groupGadget.uptimegadget.text uptimeText
	END

	/* Update time gadget */
	newtext="<p>"TIME() ", "DATE()"</p>"
	if newText >< timeText THEN DO
		timetext=newtext
		call groupGadget.timegadget.text timeText
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