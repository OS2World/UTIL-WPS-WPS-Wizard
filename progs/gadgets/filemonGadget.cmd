/* File monitor gadget showing the selected files and monitor changes on the desktop background */

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
    PARSE SOURCE . . theScript
    lstfile = SUBSTR(theScript,1,LASTPOS('.',theScript))||'txt'
    INIFile=system.path.wpswizardini
    APPKEY="filemongadget"
    APPPOS="filepos"
    APPFONT="filefont"

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
    thestem._font="9.WarpSans"   /* font */

    col1 = '<font color="#00cc00">'     /* no changes */
    col2 = '<font color="#cc0000">'     /* file changed */
    col3 = '<font color="#cccc00">'     /* file not found */

/* Get saved position if any */
    ret=Sysini(INIFile, APPKEY, APPPOS)
    IF  ret <> "ERROR:" THEN DO
        PARSE VAR ret thestem._x thestem._y thestem._cx thestem._cy rest
    END
/* Get saved font if any */
    ret=Sysini(INIFile, APPKEY, APPFONT)
    IF  ret <> "ERROR:" THEN DO
        thestem._font=ret
    END

/* Create gadget on the desktop */
    rc=WizCreateGadget("DESKTOP", "thestem." , "filemonGadget")

/* Set gadget text */
    CALL ReloadMe

/* Start a timer sending a message every 20 sec  */
    ret=WPSWizCallWinFunc("winStartTimer", ARG(3), 10, 20000)

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


filemonGadget.onCommand:
/*
    ARG(1): hwnd of client area
    ARG(2): ID
    ARG(3): source (menu or button)
*/
    PARSE SOURCE . . theScript
    IF ARG(2)=1 THEN CALL ReloadMe
    IF ARG(2)=2 THEN DO
        hlptxt =    'Colors:'||'0d0a'x||'Green - no changes'||'0d0a'x||'Red - file was changed'||'0d0a'x||'Yellow - file not found'||'0d0a0d0a'x||,
                    'To add new or remove existing files you want to monitor, edit file list in filemonGadget.txt'
        IF RxMessageBox(hlptxt,'File Monitor',,'INFORMATION') = 1 THEN DO
            DROP hlptxt
        END
    END
    IF ARG(2)=4 THEN DO
        call SysIni iniFile, APPKEY, APPPOS,  filemonGadget.position()
        call SysIni iniFile, APPKEY, APPFONT, filemonGadget.font()
        rc=wizDestroyGadget("filemonGadget")
        exit(0)
    END
return


filemonGadget.onPopUp:
/*
    ARG(1): hwnd of client area
    ARG(2): x
    ARG(3): y
*/
    menu.0=4
    menu.1="Reload list"
    menu.2="Help"
    menu.3="-"
    menu.4="Close Gadget"

    menu._x=ARG(2)
    menu._y=ARG(3)

    ret=WPSWizCallWinFunc("menuPopupMenu", ARG(1), 'menu.')
return

onTimer:

IF STREAM(lstfile,'C','QUERY EXISTS') <> '' THEN DO
    IF STREAM(lstfile,'C','QUERY DATETIME') <> lsttime THEN DO
        CALL ReloadMe
    END
END
CALL ShowMe

RETURN

ReloadMe:

files.0 = 0
f = 0
IF STREAM(lstfile,'C','QUERY EXISTS') <> '' THEN DO
    dummy = STREAM(lstfile,'C','OPEN READ')
    DO WHILE LINES(lstfile)
        lstline = STRIP(LINEIN(lstfile))
        IF lstline <> '' THEN DO
            f = f+1
            files.f = lstline
        END
    END
    dummy = STREAM(lstfile,'C','CLOSE')
    lsttime = STREAM(lstfile,'C','QUERY DATETIME')
    files.0 = f
END
IF files.0 = 0 THEN DO
    bootdrv = TRANSLATE(system.path.bootdrive)
    files.1 = bootdrv||'\CONFIG.SYS'
    files.2 = bootdrv||'\AUTOEXEC.BAT'
    files.0 = 2
END
gadgetText = ''
DO f = 1 TO files.0
    SELECT
        WHEN STREAM(files.f,'C','QUERY EXISTS') = '' THEN times.f = '?'
        OTHERWISE times.f = STREAM(files.f,'C','QUERY DATETIME')
    END
END
CALL ShowMe

RETURN

ShowMe:

gadgetText = ''
DO f = 1 TO files.0
    SELECT
        WHEN STREAM(files.f,'C','QUERY EXISTS') = '' THEN gadgetText = gadgetText||col3||files.f||'</font><br>'
        WHEN STREAM(files.f,'C','QUERY DATETIME') = times.f THEN gadgetText = gadgetText||col1||files.f||'</font><br>'
        OTHERWISE gadgetText = gadgetText||col2||files.f||'<br>'
    END
END
CALL filemonGadget.text gadgetText

RETURN

quit:
exit(0)

errorHandler:
    PARSE SOURCE . . theScript

    ret=WPSWizGadgetFunc("cwDisplayRexxError", "")
    ret=WPSWizGadgetFunc("cwDisplayRexxError", theScript||": ")
    ret=WPSWizGadgetFunc("cwDisplayRexxError", "Error in line "||SIGL)


exit(0)

