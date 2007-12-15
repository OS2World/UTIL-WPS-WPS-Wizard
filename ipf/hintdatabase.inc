:h1 res=5500
x=right y=bottom
width=100%
height=100%
.Extending the hint database
:p.
The hint database (OBJHINT.INI located in the HELP directory) is built from an
input textfile using the script :hp2.buildhintini.CMD:ehp2. (located in the BIN directory).
If you want to add your own hints create an appropriate input text, put OBJHINT.INI into the
directory with the script and add the new information to the database by invoking the script:

:xmp.

     buildhintini.CMD <drive>\<path>\hintdata.txt

:exmp.
:p.
The data already in the INI file will be updated.
:p.
The format of this file is the following:
:xmp.

# Format of the input text file:
#
# Comments start with '#'.
#
# The first word is the WPS object ID. After one space the object hint text follows.
# Line breaks are not allowed in this text. Use "0ax" (without qoutation marks) to
# insert breaks 
#
# <WPS_ID> Object hint text.


# Objects of the Audio/Data-CD-Creator package
[1]
<CWCREATOR_SETTINGS> Use this object to 0ax change global settings 0ax e.g. path to CDRecord/2 or FIFO size.

[1]
<CDR_TOOLS> CD-RW blanking, session closing 0ax and other tools.

[...]
:exmp.
:p.
You may send in your additions so they may be included in the next release of the distribution. Be
sure to send the textfile NOT the OBJHINT.INI!







