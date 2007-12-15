.*************** winQueryContainerHWND ***************************************
:h3 res=2010
width=30%
.winQueryContainerHWND
:link reftype=hd res=2011
auto dependent group=2.
:p.
:link reftype=hd res=2011.Syntax:elink.
.br
:link reftype=hd res=2013 dependent.Returns:elink.
.br
:link reftype=hd res=2014 dependent.Remarks:elink.
.*.br
.*:link reftype=hd res=315.Usage:elink.
.*.br
.*:link reftype=hd res=416 dependent.Example:elink.

:h3 res=2011
x=30%
width=70%
hide
group=2
.winQueryContainerHWND - Syntax
:xmp.
/* This function returns the container handle from
    the frame handle of a folder */

frameHandle   /* Handle of the folder frame  */
:p.
containerHandle=WPSWizCallWinFunc("winQueryContainerHWND", 
                    frameHandle);
:exmp.


:h3 res=2013
x=30%
width=70% height=35%
group=3
hide
.winQueryContainerHWND - Return value
:p.
:hp2.containerHandle:ehp2. numeric
:p.
Handle to the container of a folder.


:h3 res=2014
x=30%
width=70%
hide
group=2
.winQueryContainerHWND - Remarks
:p.
The frame handle of the media folder is given to the skript as a parameter
when started.



