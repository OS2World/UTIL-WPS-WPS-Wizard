/*
 rexx -  Month gadget showing the current month on the desktop background
*/

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/* Check if the script was directly started */
IF ARG() = 0 THEN DO
    /* Start the gadget */
    PARSE SOURCE . . theScript
    IF ARG(1)="" THEN
        CALL SysSetObjectdata "<WP_DESKTOP>", "WIZLAUNCHGADGET="theScript
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
        APPKEY="monthgadget"
        MONTHPOS="monthpos"
        MONTHFONT="monthfont"
        MONTHCOLOR="monthcolor"

        defaultPos=system.screen.width-220||" 100 220 220"
        PARSE SOURCE . . theScript

    /* Use defaults in the beginning */
        PARSE VAR defaultPos x y cx cy rest

        /* Gadget create info */
        thestem._x=x                    /* x  */
        thestem._y=y                    /* y  */
        thestem._cx=cx                  /* cx */
        thestem._cy=cy                  /* cy */
        thestem._type=HTML_GADGET       /* Gadget type */
        thestem._hwnd=ARG(3)            /* hwnd */
        thestem._flags=GSTYLE_POPUP     /* We want a popup menu */
        thestem._font="8.Courier"       /* font - must be monospaced */

        line1 = COPIES('Í',21)
        line2 = COPIES('Ä',21)
        col1 = '<font color="#cc0000">' /* Sunday */
        col2 = '<font color="#0000cc">' /* today */
        mon_str = 'January February March April May Jun July August September October November December'

        /* Get saved position if any */
        ret=Sysini(INIFile, APPKEY, MONTHPOS)
        IF  ret <> "ERROR:" THEN DO
            PARSE VAR ret thestem._x thestem._y thestem._cx thestem._cy rest
        END
        /* Get saved font if any */
        ret=Sysini(INIFile, APPKEY, MONTHFONT)
        IF  ret <> "ERROR:" THEN DO
            thestem._font=ret
        END

        /* Create gadget on the desktop */
        rc=WizCreateGadget("DESKTOP", "thestem." , "monthGadget")

        /* Get saved color if any */
        ret=Sysini(INIFile, APPKEY, MONTHCOLOR)
        IF  ret <> "ERROR:" THEN DO
            call monthGadget.Color ret
        END

        /* Set gadget text - create calendar for current month */
        CALL monthCurrent

        /* Start a timer sending a message every 1000ms  */
        ret=WPSWizCallWinFunc("winStartTimer", ARG(3), 10, 1000)

        /* Gadget message loop */
        DO FOREVER
            ret=WIZGETMESSAGE(ARG(3))
            IF ret<>'' THEN
                INTERPRET "call "ret
        END

        EXIT(0)
    END
    OTHERWISE
        /* We shouldn't end here... */
        Exit(0)
END
exit(0)


monthgadget.onCommand:
/*
    ARG(1): hwnd of client area
    ARG(2): ID
    ARG(3): source (menu or button)
*/
SELECT
    WHEN ARG(2) = 1 THEN DO
        IF mm > 1 THEN DO
            mm = mm-1
            mm_days = WORD(mon_days,mm)
            base_1st = base_1st-mm_days
            dow_1st = base_1st//7+1
            day_dif = dow_1st-1
            CALL ShowMe
        END
    END
    WHEN ARG(2) = 2 THEN CALL monthCurrent
    WHEN ARG(2) = 3 THEN DO
        IF mm < 12 THEN DO
            mm_dold = mm_days
            mm = mm+1
            mm_days = WORD(mon_days,mm)
            base_1st = base_1st+mm_dold
            dow_1st = base_1st//7+1
            day_dif = dow_1st-1
            CALL ShowMe
        END
    END
    WHEN ARG(2) = 5 THEN DO
        call SysIni iniFile, APPKEY, MONTHPOS,  monthGadget.position()
        call SysIni iniFile, APPKEY, MONTHFONT, monthGadget.font()
        call SysIni iniFile, APPKEY, MONTHCOLOR, monthGadget.Color()
        rc=wizDestroyGadget("monthgadget")
        exit(0)
    END
    OTHERWISE NOP
END

return


monthgadget.onPopUp:
/*
    ARG(1): hwnd of client area
    ARG(2): x
    ARG(3): y
*/
    menu.0=5
    menu.1="Previous"
    menu.2="Current"
    menu.3="Next"
    menu.4="-"
    menu.5="Close Gadget"

    menu._x=ARG(2)
    menu._y=ARG(3)

    ret=WPSWizCallWinFunc("menuPopupMenu", ARG(1), 'menu.')

return

onTimer:

IF TIME('S')//60 = 0 THEN DO
    PARSE VALUE DATE('S') WITH dy +4 dm +2 dd
    SELECT
    	WHEN dy = yy THEN CALL ShowMe
    	OTHERWISE CALL monthCurrent
    END
END

RETURN

monthCurrent:

PARSE VALUE DATE('S') WITH yy +4 mm +2 dd
mm = mm/1
dd = dd/1
SELECT
	WHEN yy//400 = 0 THEN mon_days = '31 29 31 30 31 30 31 31 30 31 30 31'
    WHEN yy//100 = 0 THEN mon_days = '31 28 31 30 31 30 31 31 30 31 30 31'
    WHEN yy//4 = 0 THEN mon_days = '31 29 31 30 31 30 31 31 30 31 30 31'
    OTHERWISE mon_days = '31 28 31 30 31 30 31 31 30 31 30 31'
END
mm_days = WORD(mon_days,mm)
base_now = DATE('B')+1
base_1st = base_now-dd
dow_1st = base_1st//7+1
day_dif = dow_1st-1
CALL ShowMe

RETURN

ShowMe:

myText='<h1>'||WORD(mon_str,mm)||' '||yy||'</h1><br>'||line1||'<br>'||' Mo Tu We Th Fr Sa '||col1||'Su</font>'||'<br>'||line2||'<br>'
DO a = 1 TO mm_days+day_dif
    d = a-day_dif
    SELECT
        WHEN d < 1 THEN myText = myText||COPIES(' ',3)
        WHEN d = dd THEN myText = myText||col2||RIGHT(d,3)||'</font>'
        WHEN a//7 = 0 THEN myText = myText||col1||RIGHT(d,3)||'</font>'
        OTHERWISE myText = myText||RIGHT(d,3)
    END
    IF a//7 = 0 THEN DO
        myText = myText||'<br>'
    END
END
SELECT
    WHEN RIGHT(myText,4) = '<br>' THEN myText = myText||line1
    OTHERWISE myText = myText||'<br>'||line1
END
CALL monthGadget.text myText

RETURN

quit:
exit(0)

errorHandler:
    PARSE SOURCE . . theScript

    ret=WPSWizGadgetFunc("cwDisplayRexxError", "")
    ret=WPSWizGadgetFunc("cwDisplayRexxError", theScript||": ")
    ret=WPSWizGadgetFunc("cwDisplayRexxError", "Error in line "||SIGL)

exit(0)

