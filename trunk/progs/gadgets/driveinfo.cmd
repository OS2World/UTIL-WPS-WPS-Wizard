/* Gadget -  Drive info gadget showing on the desktop background:

		-Size and free space on all fixed drives

  Feel free to use and extend this gadget script. The code is placed into the
  public domain.

  But: !!! The icons are copyrighted. Don't redistribute them without permission !!!

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
/*INIFile=FILESPEC("d", theScript)||FILESPEC("p", theScript)||"sysinfo.ini" */

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


	/* Get number of drives starting with c: */
	allDrives=WizQueryDrives("c:", "fixed")

	numDrives=WORDS(allDrives)

	INIFile=system.path.wpswizardini
	APPKEY="driveinfo"
	
	DRIVEPOS="drivebarpos"
	DRIVECOLOR="drivecolor"
	DRIVEBACKCOLOR="drivebackcolor"
	GROUPPOS="grouppos"
	GROUPBORDER="groupborder"
	GROUPBORDERWIDTH="groupborderwidth"

	/* Default border style for bars */
	DEFAULT_BORDER=GSTYLE_COLORBORDER

	/* Default position after installation */
	defaultPos="100 100 220 22"
	PARSE VAR defaultPos x y cx cy rest

	/* Default colors */
	backColor=X2D('8080ff') 
	foreColor=X2D('dddddd')
	/* Color for bootdrive */
	bootbackColor=X2D('bf005f') 

/* Get saved border info if any */
	ret=Sysini(INIFile, APPKEY, GROUPBORDER)
	IF  ret >< "ERROR:" THEN DO
		drawBorder=STRIP(ret)
	END
	
/* Create a group gadget */
	stem._x=x-30    /* x  */
	stem._y=y       /* y  */
	stem._cx=260            /* cx */
	stem._cy=(cy+5)*numDrives+5    /* cy */

	stem._type=GROUP_GADGET    /* Gadget type */
	stem._hwnd=ARG(3) /* hwnd */
	stem._flags=GSTYLE_POPUP/* + GSTYLE_NORESIZE */
	IF drawBorder=1 THEN
			stem._flags=stem._flags + GSTYLE_BORDER
	stem.7="10.Helv" /* font */

/* Get saved position if any */
	ret=Sysini(INIFile, APPKEY, GROUPPOS)
	IF  ret >< "ERROR:" THEN DO
		PARSE VAR ret stem._x stem._y stem._cx stem._cy rest
	END

	/* Create the gadget on the desktop */
	ret=WIZCREATEGADGET("DESKTOP", "stem.", groupGadget)
	call groupGadget.text "Drive           info"

    
/* Now create the drive gadgets */
	yTemp=stem._cy+y-cy-5
DO a=1 TO numDrives
	/* Create a memory bar gadget owned by the group gadget */
	stem._x=x                /* x  */
	stem._y=yTemp-(a-1)*(5+cy)       /* y  */
	stem._cx=cx              /* cx */
	stem._cy=cy              /* cy */
	stem._type=BAR_GADGET    /* Gadget type */
	stem._hwnd=ARG(3)        /* hwnd */
	stem._flags=0 + DEFAULT_BORDER
	stem._font="14.Helv" /* font */

	/* Get saved position if any */

/* Get saved position if any */
	ret=Sysini(INIFile, APPKEY, DRIVEPOS||a)
	IF  ret >< "ERROR:" THEN DO
		PARSE VAR ret stem._x stem._y stem._cx stem._cy rest
	END

	ret=WIZCREATEGADGET("groupGadget", "stem." , "drive"||a)

	/* Get text color */
	ret=Sysini(INIFile, APPKEY, DRIVECOLOR||a)
	IF  ret >< "ERROR:" THEN
		INTERPRET "call groupGadget.drive"||a".color ret"
	ELSE
		INTERPRET "call groupGadget.drive"||a".color forecolor"

	/* Set text */
    driveText.a=SysDriveInfo(WORD(allDrives, a))
	PARSE VAR driveText.a drive free total driveName
    IF driveText.a \="" THEN
        driveText.a="["drive"] "TRUNC(free/1024)" free"
    ELSE DO
        driveText.a="???"
        free=1024000
        total=102400000
    END
      
    INTERPRET "ret=groupGadget.drive"||a".text(driveText.a)"
	/* Set bar */
	INTERPRET "call groupGadget.drive"||a".barvalue free/1024 , total/1024"

	/* Get bar color */
	ret=Sysini(INIFile, APPKEY, DRIVEBACKCOLOR||a)
	IF  ret >< "ERROR:" THEN
		INTERPRET "call groupGadget.drive"||a".backcolor ret"
	ELSE DO
                IF TRANSLATE(system.path.bootdrive)=TRANSLATE(drive) THEN
    	           INTERPRET "call groupGadget.drive"||a".backcolor bootbackcolor"
                ELSE
    	           INTERPRET "call groupGadget.drive"||a".backcolor backcolor"
        END

/* Create an icon for the drive */
    stem._x=stem._x-25
	stem._cy=30               /* cy */
	stem._cx=30               /* cx */
	stem._type=ICON_GADGET    /* Gadget type */
	stem._hwnd=ARG(3)         /* hwnd */
	stem._flags=0
	stem._font="9.WarpSans"   /* font */

	ret=WIZCREATEGADGET("groupGadget", "stem.", "driveicon"||a)

	INTERPRET "call groupGadget.driveicon"a".icon Scriptpath||'drive.ICO'"

END /* DO */


/* Lock all childs */
DO a=1 TO numDrives
    INTERPRET "call groupgadget.drive"a".style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE"
    INTERPRET "call groupgadget.driveicon"a".style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE"
END

/* Start a timer */
	ret=WizStartTimer( ARG(3), 10, 2500)

	/* Message loop for the gadget */
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
        
        DO a=1 TO numdrives
            INTERPRET "ret=groupGadget.drive"a".position()"
            call SysIni iniFile, APPKEY, DRIVEPOS||a, ret
            INTERPRET "ret=groupGadget.drive"a".color()"
            call SysIni iniFile, APPKEY, DRIVECOLOR||a, ret
            INTERPRET "ret=groupGadget.drive"a".backcolor()"
            call SysIni iniFile, APPKEY, DRIVEBACKCOLOR||a, ret
            INTERPRET "ret=WizDestroyGadget('groupGadget.drive"a"')"
            INTERPRET "ret=WizDestroyGadget('groupGadget.driveicon"a"')"
        END

        call SysIni iniFile, APPKEY, GROUPPOS,  groupGadget.position()
	ret=Sysini(INIFile, APPKEY, GROUPBORDER, drawBorder)
	ret=WizDestroyGadget("groupGadget")
	exit(0)
	END
	WHEN ARG(2)=3 THEN DO
		IF childsLocked="YES" THEN DO
			/* Childs are now unlocked */
			childsLocked="NO"
            DO a=1 TO numDrives
                INTERPRET "call groupgadget.drive"a".style 0, GSTYLE_NOACTIVATE"
                INTERPRET "call groupgadget.driveicon"a".style 0, GSTYLE_NOACTIVATE"
            END
		END
		ELSE DO
			childsLocked="YES"
            DO a=1 TO numDrives
                INTERPRET "call groupgadget.drive"a".style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE"
                INTERPRET "call groupgadget.driveicon"a".style GSTYLE_NOACTIVATE, GSTYLE_NOACTIVATE"
            END
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

DO a=1 TO numDrives
    newText.a=SysDriveInfo(WORD(allDrives, a))
	PARSE VAR newText.a drive free total driveName

    IF newText.a \="" THEN
        newText.a="["drive"] "TRUNC(free/1024)" free"
    ELSE DO
        newText.a="???"
        free=1024000
        total=102400000
    END

    if newText.a >< driveText.a THEN DO
	driveText.a=newText.a
        INTERPRET "ret=groupGadget.drive"||a".text(driveText.a)"
        /* Set bar */
        INTERPRET "call groupGadget.drive"||a".barvalue free/1024 , total/1024"
    END
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