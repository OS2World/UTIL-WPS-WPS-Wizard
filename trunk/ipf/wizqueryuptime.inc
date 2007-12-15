.**************** WizQueryUptime **************************************
:h3 res=31110
width=30%
.WizQueryUptime
:link reftype=hd res=31111
auto dependent group=2.
:p.
:link reftype=hd res=31111.Syntax:elink.
.br
:link reftype=hd res=31112 dependent.Parameters:elink.
.br
:link reftype=hd res=31113 dependent.Returns:elink.
.br
:link reftype=hd res=31114 dependent.Remarks:elink.
.br
:link reftype=hd res=31116 dependent.Example:elink.

:h3 res=31111
x=30%
width=70%
height=100%
hide
group=2
.WizQueryUptime - Syntax
:xmp.
/* This function gets the uptime of the system */

method       /* String specifying the method to be used for getting the uptime */
format       /* Format of the returned data */
:p.
rc=WizQueryUptime(method [, format]);
:exmp.


:h3 res=31112
x=30%
width=70%
height=100%
hide
group=2
.WizQueryUptime - Parameters
:p.
:parml tsize=10.
:pt.:hp2.method:ehp2. (string)
:pd.A string specifying how to query the uptime.
:parml tsize=3.
:pt."swap"
:pd.Use the age of the swap file to calculate the uptime.
:pt."timer"
:pd.Use the timer counter value to calculate the uptime.
:eparml.
:pt.:hp2.format:ehp2. (string)
:pd.String to specify the format of the returned data.
:parml tsize=3.
:pt."s"
:pd.Return the uptime in seconds. If this parameter is omitted the uptime is returned as a string
in the following format&colon.
:p.
:hp2.days hours&colon.minutes&colon.seconds:ehp2.
:eparml.
:eparml.

:h3 res=31113
x=30%
width=70% height=35%
group=3
hide
.WizQueryUptime - Return value
:p.
:hp2.rc:ehp2. REXX-string
:p.
Uptime in seconds or as a preformated string.


:h3 res=31114
x=30%
width=70%
height=100%
hide
group=2
.WizQueryUptime - Remarks
:p.
When using the timer counter the maximum uptime reported is about 50 days. The counter used
will wrap after this amount of time. This limitation doesn't hold true for getting the uptime
from the swap file creation date and time. When using the swap file on a notebook
using hibernation the hibernation time will also taken into account when calculating the
uptime.

:h3 res=31116
x=30%
width=70%
height=100%
hide
group=2
.WizQueryUptime - Example
:p.

:xmp.
/* Query the uptime of this system using the swap file */

ret=WizQueryUptime("swap")
:exmp.

