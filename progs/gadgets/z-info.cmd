/* rexx -  Default REXX script handling folder callbacks of
           WPS-Wizard */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

NUMERIC DIGITS 15 /* We need this for window handles */

SELECT
WHEN ARG(1)="/GADGETSTARTED" THEN DO

	theObject=ARG(2)

	zInfo=""
	global.pipename="\PIPE\ZMP3"
	gadgetText=""

	SIGNAL ON SYNTAX NAME errorHandler
	SIGNAL ON ERROR NAME errorHandler

	defaultPos=system.screen.width-220||" 100 200 150"

	/* Info for storing data in WPS-wizard ini file */
	INIFile=system.path.wpswizardini
	APPKEY="zinfogadget"
	ZINFOPOS="zinfopos"
	ZINFOFONT="zinfofont"

	PARSE VAR defaultPos x y cx cy rest

	stem._x=x    /* x  */
	stem._y=y    /* y  */
	stem._cx=cx   /* cx */
	stem._cy=cy   /* cy */
	stem._type=HTML_GADGET    /* Gadget type */
	stem._hwnd=ARG(3) /* hwnd */
	stem._flags=GSTYLE_POPUP
	stem._font="12.Helv" /* font */

/* Get saved position if any */
	ret=Sysini(INIFile, APPKEY, ZINFOPOS)
	IF  ret >< "ERROR:" THEN DO
		PARSE VAR ret stem._x stem._y stem._cx stem._cy rest
	END
/* Get saved font if any */
	ret=Sysini(INIFile, APPKEY, ZINFOFONT)
	IF  ret >< "ERROR:" THEN DO
		stem._font=ret
	END

	ret=WizStartTimer( ARG(3), 10, 500)

	rc=WizCreateGadget( "DESKTOP", "stem." , "zgadget")
	ret=zGadget.text(gadgetText)

       DO FOREVER
	ret=WIZGETMESSAGE( ARG(3))

	if ret<>'' THEN 
		INTERPRET "call "ret
	
	rt = stream(global.PipeName,"C",'OPEN')
	if rt="READY:" then DO
 	       rt=lineout(global.PipeName, '*rawinfo')

		ret=WIZGETMESSAGE(ARG(3))
		if ret<>'' THEN 
			INTERPRET "call "ret
		zinfo=readzpipe()
		rt = stream(global.PipeName,"C",'close')

		IF gadgetText<>zInfo THEN DO
			gadgetText=zInfo
			ret=zGadget.text(gadgetText)
		END
	END
	ELSE DO
		zinfo='<p><font color="#000000ff">z! </font><font color="#00dddddd">is not running.</font></p>'
		IF gadgetText<>zInfo THEN DO
			gadgetText=zInfo
			ret=zGadget.Text(gadgetText)
		END
	END

	END /* FOREVER */
	EXIT(0)
END
OTHERWISE
	/* Start the gadget */
	PARSE SOURCE . . theScript
	IF ARG(1)="" THEN
		call SysSetObjectData "<WP_DESKTOP>", "WIZLAUNCHGADGET="theScript
	Exit(0)
END

zGadget.onCommand:
/*
	ARG(1): hwnd of client area
	ARG(2): ID
	ARG(3): source (menu or button)
*/
	PARSE SOURCE . . theScript
	IF ARG(2)=1 THEN DO
		call SysIni iniFile, APPKEY, ZINFOPOS,  zGadget.position()
		call SysIni iniFile, APPKEY, ZINFOFONT, zGadget.font()
		rc=wizDestroyGadget("zgadget")
		exit(0)
	END
return


zGadget.onPopUp:
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
return

quit:
	PARSE SOURCE . . theScript
	call SysPutEa theScript, "GADGETPOS", zgadget.Position()
	rt = stream(global.PipeName,"C",'close')
exit(0)


errorHandler:
call beep 100, 500
ret=WPSWizGadgetFunc("cwDisplayRexxError", ret)
rt = stream(global.PipeName,"C",'close')
exit(0)


readzpipe:
	ret=LINEIN(global.PipeName)
	if LENGTH(ret)<256 THEN return ""

	fname=STRIP(TRANSLATE(LEFT(ret, 256), ' ', '00'x))
	ret=RIGHT(ret, LENGTH(ret)-256)
	songinfo=STRIP(TRANSLATE(LEFT(ret, 256), ' ', '00'x))
	ret=RIGHT(ret, LENGTH(ret)-256)
	track=STRIP(TRANSLATE(LEFT(ret, 128), ' ', '00'x))
	ret=RIGHT(ret, LENGTH(ret)-128)
	artist=STRIP(TRANSLATE(LEFT(ret, 128), ' ', '00'x))
	ret=RIGHT(ret, LENGTH(ret)-128)
	album=STRIP(TRANSLATE(LEFT(ret, 128), ' ', '00'x))
	ret=RIGHT(ret, LENGTH(ret)-128)
	comment=STRIP(TRANSLATE(LEFT(ret, 128), ' ', '00'x))
	ret=RIGHT(ret, LENGTH(ret)-128)
	year=STRIP(TRANSLATE(LEFT(ret, 6), ' ', '00'x))
	ret=RIGHT(ret, LENGTH(ret)-6)
	playtime=STRIP(TRANSLATE(LEFT(ret, 10), ' ', '00'x))
	ret=RIGHT(ret, LENGTH(ret)-10)
	timenow=STRIP(TRANSLATE(LEFT(ret, 10), ' ', '00'x))
	ret=RIGHT(ret, LENGTH(ret)-10)
	genre=STRIP(TRANSLATE(LEFT(ret, 26), ' ', '00'x))
	ret=RIGHT(ret, LENGTH(ret)-26)
	filesize=STRIP(TRANSLATE(LEFT(ret, 10), ' ', '00'x))

/*	SAY "File: "fname
	SAY "Songinfo: "songinfo
	SAY "Track: "track
	SAY "Artist: "artist
	SAY "Album: "album
	SAY timenow||" \ "playtime
*/
	zinfo='<p><font color="#000000ff">z! </font><font color="#00dddddd">is playing:</font></p><p>'

	if artist<>"" THEN
		zinfo=zinfo||'<font color="#00dddddd">'||artist||"</font><br>"

	if track<>"" THEN
	        zinfo=zinfo||'<font color="#00999999">'||track||"</font><br>"
	zinfo=zinfo||'<font color="#00dddddd">'||timenow||" \ "playtime||'</font></p>'

return zinfo
