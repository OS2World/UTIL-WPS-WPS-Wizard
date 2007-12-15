.*********** winFileDlg *************
:h3 res=2050
width=30%
.winFileDlg
:link reftype=hd res=2051
auto dependent group=2.
:p.
:link reftype=hd res=2051.Syntax:elink.
.br
:link reftype=hd res=2053 dependent.Returns:elink.
.br
:link reftype=hd res=2054 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=2051
x=30%
width=70%
hide
group=2
.winFileDlg - Syntax
:xmp.
/* Display a file dialog. It may be an 'open'- or
   a 'Save as' dialog */

flag          /* 0: open dialog, 1: SaveAs dialog */
title         /* Title of the file dialog         */
fileMask      /* The file mask for example *.exe  */

:p.
fileName=WPSWizCallWinFunc("winFileDlg", 
                    flag, title, fileMask);
:exmp.


:h3 res=2053
x=30%
width=70% height=35%
group=3
hide
.winFileDlg - Return value
:p.
:hp2.fileName:ehp2. string
:p.
Selected file name
:p.
In case of error :hp2.ERROR&colon.:ehp2. is returned.


:h3 res=2054
x=30%
width=70%
hide
group=2
.winFileDlg - Remarks
:p.
The frame handle of the media folder is given to the skript as a parameter
when started.

