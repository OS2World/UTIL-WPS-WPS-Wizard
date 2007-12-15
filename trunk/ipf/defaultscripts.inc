:h1 res=7000
x=left y=bottom width=50%.Default scripts
:p.
WPS-Wizard uses default scripts which handle the REXX callbacks. You may modify these
scripts or create new object scripts to add or remove functions. The scripts are plain
REXX files and you may find them in the :hp2.bin:ehp2. directory (or one of the :hp2.bin:ehp2. subdirectories)
 of your WPS-Wizard installation.
If you have problems using an object script (e.g. crashes) just rename it so it's not
invoked any longer.
:ul.
:li.:link reftype=hd res=7010.Folder script:elink.
:li.:link reftype=hd res=7020.Data file script:elink.
:li.:link reftype=hd res=7030.Object script:elink.
:eul.
:p.
The following skripts are invoked whenever a flyover hint must be created.

:ul.
:li.:link reftype=hd res=7040.Folder hint script:elink.
:li.:link reftype=hd res=7050.Data file hint script:elink.
:li.:link reftype=hd res=7060.Object hint script:elink.
:eul.
:p.These skripts are called when information should be shown in the folder information area.
:ul.
:li.:link reftype=hd res=7070.Folder info script:elink.
:li.:link reftype=hd res=7080.Data file info script:elink.
:li.:link reftype=hd res=7090.Object info script:elink.
:eul.

:h2 res=7010
x=50% width=50%
group=2
.Folder script
:p.
:hp2.folder.rx:ehp2.
:p.
This script is executed when folder callbacks are invoked. It adds the folder toolbar, creates the buttons
and handles the button events. Modify this script if you want to add menu items to folder objects or 
change the contents of the toolbar.


:h2 res=7020
x=50% width=50%
group=2
.Datafile script
:p.
:hp2.datafile.rx:ehp2.
:p.
This script is executed when data file callbacks are invoked. Modify it to change for example the menu of data files.

:h2 res=7030
x=50% width=50%
group=2
.Object script
:p.
:hp2.object.rx:ehp2.
:p.
This script is executed for abstract objects (e.g. program objects ) and if a callback isn't handled by the
:link reftype=hd res=7020.datafile.rx:elink. or :link reftype=hd res=7010.folder.rx:elink. scripts.


:h2 res=7040
x=50% width=50%
group=2
.Folder hint script
:p.
:hp2.bin\hint\folder.rx:ehp2.
:p.
Whenever a hint shall be shown by the WPS for folder objects this script is executed to build the
hint text.
:p.
The default implementation checks the object ID and looks in the hint database (:hp2.OBJHINT.INI:ehp2.)
for the hint text. If no entry is found some generic information about the folder is queried.

:h2 res=7050
x=50% width=50%
group=2
.Data file hint script
:p.
:hp2.bin\hint\datafile.rx:ehp2.
:p.
Whenever a hint shall be shown by the WPS for data file objects this script is executed to build the
hint text.
:p.
The default implementation checks the object ID and looks in the hint database (:hp2.OBJHINT.INI:ehp2.)
for the hint text. If no entry is found some generic information about the data file will be shown.

:h2 res=7060
x=50% width=50%
group=2
.Object hint script
:p.
:hp2.bin\hint\objecthint.rx:ehp2.
:p.
Whenever a hint shall be shown by the WPS for (abstract) objects this script is executed to build the
hint text.
:p.
The default implementation checks the object ID and looks in the hint database (:hp2.OBJHINT.INI:ehp2.)
for the hint text. If the object is a shadow the linked object will be found and the name of this object
will be shown as part of  the text.
If no entry is found some generic information about the object will be shown.


:h2 res=7070
x=50% width=50%
group=2
.Folder info script
:p.
:hp2.bin\fldrinfo\folder.rx:ehp2.
:p.
Whenever information should be shown in the folder information area by the WPS this script is executed to build the
text.
:p.
This skript is divided into several parts. 
:ul.
:li.When the skript is called with parameter :hp2./FOLDERINFO:ehp2.
information about the current open folder is queried. The default implementation checks the object ID
and looks in the hint database (:hp2.OBJHINT.INI:ehp2.) for the hint text.
After that some links are added to the text and the result is displayed in the top part of the information area.
Links created during this callback must have an ID below or equal 1000.

:li.When the skript is called with parameter :hp2./OBJECTINFO:ehp2. information about a currently selected folder should be
displayed. The returned text will be displayed in the lower part of the information area.
Links created during this callback must have an ID greater than 1000.

:li.When called with parameter :hp2./INFOWINDOWEVENT:ehp2. the user clicked one of the links in
the window. This link was provided when executing the :hp2./FOLDERINFO:ehp2. or the :hp2./OBJECTINFO:ehp2. event. 
Distinguish the links by using the link ID.
:eul.

:h2 res=7080
x=50% width=50%
group=2
.Datafile info script
:p.
:hp2.bin\fldrinfo\datafile.rx:ehp2.
:p.
This skript is called when datafiles are selected in a folder. Two parameters are possible.
:ul.
:li.:hp2./OBJECTINFO:ehp2. when a request for info about the selected file is made. The returned text will be displayed
in the lower part of the info area below the generic folder information.
Links created during this callback must have an ID greater than 1000.

:li.:hp2./INFOWINDOWEVENT:ehp2. when the user clicked a link created when this skript was called with the previous event.
:eul. 

:h2 res=7090
x=50% width=50%
group=2
.Object info script
:p.
:hp2.bin\fldrinfo\object.rx:ehp2.
:p.
This skript is called when an object is selected in a folder. It's invoked for abstract objects. Datafiles and folders
are handled by other skripts. Two parameters are possible.
:ul.
:li.:hp2./OBJECTINFO:ehp2. when a request for info about the selected object is made. The returned text will be displayed
in the lower part of the info area below the generic folder information.
Links created during this callback must have an ID greater than 1000.

:li.:hp2./INFOWINDOWEVENT:ehp2. when the user clicked a link created when this skript was called with the previous event.
:eul. 




