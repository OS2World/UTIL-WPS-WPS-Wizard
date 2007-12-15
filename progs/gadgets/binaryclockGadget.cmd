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
    APPKEY="binaryclockgadget"
    TIMEPOS="binaryclockpos"
    TIMEFONT="binaryclockfont"

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
    thestem._font="9.WarpSans Bold"      /* font */

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
    rc=WizCreateGadget("DESKTOP", "thestem." , "binaryclockGadget")

/* Set gadget text */
    PARSE VALUE TIME() WITH hh ':' mm ':' ss
    CALL binaryclockGadget.text RIGHT(X2B(D2X(hh/1)),8,'0')||'<br>'||RIGHT(X2B(D2X(mm/1)),8,'0')||'<br>'||RIGHT(X2B(D2X(ss/1)),8,'0')

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


binaryclockGadget.onCommand:
/*
    ARG(1): hwnd of client area
    ARG(2): ID
    ARG(3): source (menu or button)
*/
    PARSE SOURCE . . theScript
    SELECT
        WHEN ARG(2)=1 THEN DO
            IF RxMessageBox("There are 10 groups of people - those who understand binary and those who don't.","Binary Clock",,"INFORMATION") = 1 THEN DO
            END
        END
        WHEN ARG(2)=3 THEN DO
            call SysIni iniFile, APPKEY, TIMEPOS,  binaryclockGadget.position()
            call SysIni iniFile, APPKEY, TIMEFONT, binaryclockGadget.font()
            rc=wizDestroyGadget("binaryclockGadget")
            exit(0)
        END
        OTHERWISE NOP
    END
return


binaryclockGadget.onPopUp:
/*
    ARG(1): hwnd of client area
    ARG(2): x
    ARG(3): y
*/
    menu.0=3
    menu.1="What's this?"
    menu.2="-"
    menu.3="Close Gadget"

    menu._x=ARG(2)
    menu._y=ARG(3)

    ret=WPSWizCallWinFunc("menuPopupMenu", ARG(1), 'menu.')
return

onTimer:

PARSE VALUE TIME() WITH hh ':' mm ':' ss
CALL binaryclockGadget.text RIGHT(X2B(D2X(hh/1)),8,'0')||'<br>'||RIGHT(X2B(D2X(mm/1)),8,'0')||'<br>'||RIGHT(X2B(D2X(ss/1)),8,'0')

RETURN

quit:
exit(0)

errorHandler:
    PARSE SOURCE . . theScript

    ret=WPSWizGadgetFunc("cwDisplayRexxError", "")
    ret=WPSWizGadgetFunc("cwDisplayRexxError", theScript||": ")
    ret=WPSWizGadgetFunc("cwDisplayRexxError", "Error in line "||SIGL)


exit(0)

