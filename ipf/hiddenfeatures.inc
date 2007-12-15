:h1 res=6000
x=left y=bottom width=50%.Hidden features
:p.
Change some features of WPS-Wizard by setting values in :hp2.WPSIWIZZ.INI:ehp2.. This INI file
may be found in your home directory or in the installation directory of WPS-Wizard. The home
directory is specified in the config.sys file with the statement :hp2.SET HOME=...:ehp2.

:ul.
:li.:link reftype=hd res=6010.Disable hint shadows:elink.
:li.:link reftype=hd res=6020.Disable hint transparency:elink.
:li.:link reftype=hd res=6030.Color of fill bar for drive objects:elink.
:eul.

:h2 res=6010
x=50% width=50%
group=2
.Disable hint shadows
:p.
Add the following key to WPSWIZZ.INI to disable the shadows of object hints.
:p.
:hp2.FlyOver->bDisableFlyOverShadow->1:ehp2.


:h2 res=6020
x=50% width=50%
group=2
.Disable hint transparency
:p.
Add the following key to WPSWIZZ.INI to disable the transparency of object hints.
:p.
:hp2.FlyOver->bDisableFlyOverTransparency->1:ehp2.

:h2 res=6030
x=50% width=50%
group=2
.Color of fill bar for drive objects
:p.
Add the following key to WPSWIZZ.INI to set the color of the fill bars for drive objects.
The color is specified as RGB. You have to restart the WPS so the change take effect.
:p.
:hp2.DriveBarClr->r-><value>:ehp2.:p.
:hp2.DriveBarClr->g-><value>:ehp2.:p.
:hp2.DriveBarClr->b-><value>:ehp2.:p.
:p.
Value may be 0...255.
