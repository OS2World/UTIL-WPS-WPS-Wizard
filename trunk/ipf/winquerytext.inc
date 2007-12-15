.*************** winQueryText ***************************************
:h3 res=2240
width=30%
.winQueryText
:link reftype=hd res=2241
auto dependent group=2.
:p.
:link reftype=hd res=2241.Syntax:elink.
.br
:link reftype=hd res=2245.Parameters:elink.
.br
:link reftype=hd res=2243 dependent.Returns:elink.
.*.br
.*:link reftype=hd res=2204 dependent.Remarks:elink.
.br
:link reftype=hd res=2246 dependent.Example:elink.

:h3 res=2241
x=30%
width=70%
hide
group=2
.winQueryText - Syntax
:xmp.
/*****************************************/
/* This function queries the window text */
/*****************************************/

HWND  hwndWindow      /* Handle of the window  */
PSZ   :link reftype=hd res=2243 dependent.wndText:elink.         /* Window text */
:p.
wndText=WPSWizCallWinFunc("winQueryText", hwndWindow);
:exmp.

:h3 res=2245
x=30%
width=70%
group=3
hide
.winQueryText - Parameters
:p.
:parml tsize=10.
:pt.:hp2.hwndWindow:ehp2. (numeric) - input
:pd.Handle to the window. This is the Presentation Manager HWND.
:pt.:hp2.wndText:ehp2. (string) - returns
:pd.Window text.
:eparml.

:h3 res=2243
x=30%
width=70% height=35%
group=3
hide
.winQueryText - Return value
:p.
:hp2.wndText:ehp2. (string)
.br
Window text.


:h3 res=2246
x=30%
width=70%
hide
group=2
.winQueryText - Example
:xmp.
    /* 
    In this example a user selection of toolbar
    items is handled.
    */

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

/* Defines for window classes */
WC_BUTTON=ffff0003
WC_STATIC=ffff0005
WC_ENTRYFIELD=ffff0006
WC_MLE=ffff000A

NUMERIC DIGITS 15 /* We need this for caculating with hex numbers.
                     Default is 9. */

SELECT
    .
    .
    .
WHEN ARG(1)="/TOOLBAREVENT" THEN DO
    /* An event from the folder tool bar */

    /* This callback is called with only 3 parameters!
 
    ARG(1): The callback indentifier. In this case /TOOLBAREVENT
    ARG(2): The object pointer
    ARG(3): A composite argument

    */

    PARSE VALUE ARG(3) WITH cmd id hwnd hwndFrame
    /* cmd: the event. Currently only COMMAND 
       id:  id of the window (button) which caused the event
           hwnd: window handle of window 'id'
           hwndFrame: frame handle of the folder with this toolbar
    */
    if cmd="COMMAND" THEN DO
    SELECT
    WHEN id=1 THEN /* Open parent */
        ret=SysSetObjectData(WPSWizCallFunc("cwQueryRealName", ARG(2), 1), "MENUITEMSELECTED=714")
    WHEN id=2 THEN /* Open properties */
        ret=SysSetObjectData(WPSWizCallFunc("cwQueryRealName", ARG(2), 1), "MENUITEMSELECTED=112")
    WHEN id=3 THEN /* Change to details */
        ret=SysSetObjectData(WPSWizCallFunc("cwQueryRealName", ARG(2), 1), "MENUITEMSELECTED=718")
    WHEN id=4 THEN /* Change to icon */
        ret=SysSetObjectData(WPSWizCallFunc("cwQueryRealName", ARG(2), 1), "MENUITEMSELECTED=716")
    WHEN id=5 THEN /* Open system configuration */
        ret=SysSetObjectData("<WP_CONFIG>", "OPEN=DEFAULT")
    WHEN id=6 THEN /* Open a command line window */
        ret=SysSetObjectData("<WP_OS2WIN>", "OPEN=DEFAULT")
    WHEN id=50 THEN DO /* Find button clicked */
        /* get handle of entry field */
        hwndEntry=WPSWizCallWinFunc("winWindowFromID", hwnd , idEntry)
        /* Get text from entry field */
        searchText=WPSWizCallWinFunc("winQueryText", hwndEntry )
            if searchText="" THEN DO
            call rxMessagebox "Insert a word to search for." , "Information" 
            EXIT(0)
        END 
        /* Open the topic */
        ADDRESS CMD 'help 'searchText		
    END
    OTHERWISE
        EXIT(0)
    END
    END /* if COMMAND */
    EXIT(0)
END
WHEN ARG(1)="/OPENFOLDER" THEN DO   /* The folder just opened */
    .
    .
    /* Create tool bar and tool bar buttons here */
    .
    .
    EXIT(0)
END
OTHERWISE
    /* Always return ARG(3) for unknown methods */
    return ARG(3)
END
:exmp.




