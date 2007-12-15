:userdoc.

:docprof toc=1234.

:title. Introduction to WPS-Wizard

:h1 res=001.Start
:artwork align=center name='main.bmp'.
:p.
.ce WPS-Wizard 0.6.0 (c) 1996-2007 Chris Wohlgemuth
.ce Icons (c) Klaus Staedtler-Przyborski
:p.
:lines align=center.
:link reftype=launch
object='netscape.exe'
data='http://www.geocities.com/SiliconValley/Sector/5785/index.html'.
http&colon.//www.geocities.com/SiliconValley/Sector/5785/index.html:elink.

:link reftype=launch
object='netscape.exe'
data='http://www.os2world.com/cdwriting'.
http&colon.//www.os2world.com/cdwriting:elink.
:elines.

.*-----------------------------------------------------

:h1 .License
:p.
.im licence_eng.inc


.*-----------------------------------------------------
:h1  res=1002 x=left y=bottom width=40% height=100% group=1 .Overview 
:link reftype=hd res=1001
auto dependent group=2.

.*:h2 res=10020 hide.Overview Text
:p.
WPS-Wizard is a configurable extension for the WPS with the following features:
:sl.
:li.:link reftype=hd res=1001.Configure your menu using drag and drop:elink. 
:li.:link reftype=hd res=1004.Filesize of files in the context menu:elink.
:li.:link reftype=hd res=1006.Removing of the format menu for harddisks:elink.
:li.:link reftype=hd res=1010.REXX interface to the WPS:elink.
:li.:link reftype=hd res=10200.Object hints:elink.
:li.:link reftype=hd res=10230.Folder toolbars:elink.
:li.:link reftype=hd res=10220.Folder information window:elink.
:li.:link reftype=hd res=10240.Gadgets, living in the background of the desktop:elink.
:esl. 
:p.
Any of these features may be enabled or disabled in a central :link reftype=hd res=1007. configuration notebook :elink.
:p.
Using the REXX interface a toolbar is added to every folder which may be modified by changing a
REXX file. Read more about that in chapter
:link reftype=hd res=1010.REXX interface to the WPS:elink. and
:link reftype=hd res=7000.Default scripts:elink..



.*-----------------------------------------------------
:h2  res=1001 x=right y=bottom width=60% height=100% group=2.Extended context menu
:p.
You may extend the context menu of WPS objects by using the settings notebook of the object.
The new menu is tied to the particular object which isn't always desired. WPS-Wizard enables you
to change the context menus for every folders and file of your system at once. After installation
you find a menu folder for files and one for folders which may hold shadows or program
objects which show up as items in the menu of your objects. 
:p.
:artwork align=center name='demo1.bmp'.
:p.
A unique feature of WPS-Wizard is the ability to create additional menu folders for any file and
folder class on your system. So you may have special menu items for icon files or audio files and
different items for the desktop and your drives folder (which is a special folder class). 
:p.
Read more about menu folders
:link reftype=hd res=20000.here:elink..

.*-----------------------------------------------------

:h2 res=1004 x=right y=bottom width=60% height=100% group=2.Filesize
:p.
After activation you'll find the filesize of the particular file in the context menu. It's not
necessary to open the settings notebook and switch to the right page just to see if the file
will fit on your floppy disk.
:p.
:artwork  align=center name='filesize.bmp'.
:p.
.*-----------------------------------------------------

.*-----------------------------------------------------
:h2 res=1006 x=right y=bottom width=60% height=100% group=2.Remove the format menu
:p.
The WPS gives you direct access to the format command for any drive of the system. This may be useful
for floppy disk drives but on the other hand it's possible to erase your boot drive by accident.
:p.
:artwork  align=left name='format1.bmp'.
:p.
By disabling the format menu your harddisks are more safe:
:p.
:artwork align=right name='format2.bmp'.
:p.
:note text='Note:'.
:p.
This feature was only tested with local drives. I don't have any remote drives connected
to my system so I can't say anything about network drives etc. If there're any problems
drop me a note.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=1010 x=right y=bottom width=60% height=100% group=2.REXX interface to the WPS
:p.
WPS-Wizard provides a REXX interface to use the internal functions of the WPS from
REXX. Every folder gets a script menu item to execute REXX scripts. With V0.3.0 REXX callbacks
are available which may be used to intercept the processing of selected WPS methods like
wpModifyPopupMenu() or wpMenuItemSelected(). With these callbacks you may create the same effect
like with creating a child class of a class using C or C++ and the SOM compiler. As an example
it's possible to control which object may be dropped on another object using a REXX script.
Or you may modify the menus of all or only some selected objects.

:p.
Get more information in the section :link reftype=hd res=300.REXX interface:elink..
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=10200 x=right y=bottom width=60% height=100% group=2.Show object info
:p.
Object hints are shown in a small window if the mouse pointer isn't moved when over objects.
Use the check box in the configuration notebook to enable this feature.
:p.
:artwork  align=center name='objecthint.bmp'.
:p.
See :link reftype=hd res=1010.REXX interface to the WPS:elink.
for more information about changing the displayed information. This feature is new with V0.4.0.

:nt.
REXX support of WPS-Wizard must be enabled to use object hints.
:ent.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=10230 x=right y=bottom width=60% height=100% group=2.Folder toolbars
:p.
A toolbar is added to every folder. This toolbar may be changed using the REXX programming language. Starting
with version 0.4.1 the toolbar may be adapted without explicit programming using the mouse. Read the
:link reftype=hd res=22000.toolbar:elink. chapter for more.
:p.
:artwork  align=center name='toolbar.bmp'.
:p.
See :link reftype=hd res=1010.REXX interface to the WPS:elink.
for more information about changing the toolbar programatically. This feature is new with V0.4.0.

:nt.
REXX support of WPS-Wizard must be enabled to use folder toolbars.
:ent.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=10220 x=right y=bottom width=60% height=100% group=2.Folder information window
:p.
The folder information area shows an informational text about the currently open folder and the selected
object in the folder. Links may be created to start actions related to the selected object or folder.

:artwork  align=center name='infoarea.bmp'.
:p.
See :link reftype=hd res=1010.REXX interface to the WPS:elink.
for more information about changing the displayed information. This feature is new with V0.4.0.

:nt.
REXX support of WPS-Wizard must be enabled to use the information area.
:ent.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=10240 x=right y=bottom width=60% height=100% group=2.Gadgets, living in the background of the desktop
:p.
Gadgets are REXX scripts running hidden and displaying their output in the background of the desktop.
This may be for example information about free memory available in the system. The normal usage of the
desktop is not affected by these gadgets, so you may put objects even where the gadget draws the graphics.
:p.
:artwork  align=center name='gadget.bmp'.
:p.
Gadgets are programmed in REXX and use the programming interface provided by WPS-wizard.
See :link reftype=hd res=1010.REXX interface to the WPS:elink.
for more information. This feature is new with V0.5.0.

.*-----------------------------------------------------


.*-----------------------------------------------------
:h1  res=3000 x=left y=bottom width=100% height=100% group=1.Installation
:p.
Install WPS-Wizard by running the install skript. It will register some classes and replace your
folder, file and disk class with enhanced versions.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h1 res=1007 x=left y=bottom width=40% height=100% group=1.Configuration notebook
:link reftype=hd res=10072
auto dependent group=2.

:p.
You may disable or enable any feature of WPS-Wizard in the configuration notebook.
:p.
The settings have effect on any object of your system.
As an additional feature it's possible to disable/enable the :link reftype=hd res=1001.extended context menu:elink.
for individual classes. For doing this use the settings notebook of the particular menu folder.
:sl.

:li.
:link reftype=hd res=102
vpx=right
vpy=bottom
vpcx=60%
vpcy=50%.Enable REXX scripts:elink.

:li.
:link reftype=hd res=105
vpx=right
vpy=bottom
vpcx=60%
vpcy=50%.Show object info:elink.

:li.
:link reftype=hd res=10220
vpx=right
vpy=bottom
vpcx=60%
vpcy=50%.Information area in folders:elink.

:li.
:link reftype=hd res=101
vpx=right
vpy=bottom
vpcx=60%
vpcy=50%.Show filesize:elink.

:li.:link reftype=hd res=103
vpx=right
vpy=bottom
vpcx=60%
vpcy=50%.Remove format menuitem:elink.

:li.:link reftype=hd res=104
vpx=right
vpy=bottom
vpcx=60%
vpcy=50%.Extended context menu:elink.
:esl.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 hide res=10072  x=right y=bottom width=60% height=100% group=2.Picture of configuration notebook
:artwork  align=center name='configuration.bmp'.
:p.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=102 group=3.Enable REXX scripts
:p.
WPS-Wizard may be extended by adding REXX scripts. For example the folder toolbar is added by using
a script and object hints are also shown by small REXX scripts. Use the check box to disable all REXX scripts
used by WPS-Wizard.
:nt.
Scripts added to the folder menu bar are not affected by this setting.
:ent.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=105 group=3.Show object info
:p.
Object hints are shown in a small window if the mouse pointer isn't moved when over objects.
Use the check box in the configuration notebook to enable this feature.
:p.
:artwork  align=center name='objecthint.bmp'.
:p.
:nt.
REXX support of WPS-Wizard must be enabled to use object hints.
:ent.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=101 group=3.Show filesize
:p.
Select this item to show the filesize in the context menu of a file object
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=103 group=3.Remove format menuitem
:p.
After selecting this item the format menuitem will be removed from the menu of your harddisks to
prevent formating by accident. The context menu of a floppy disk drive isn't affected.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=104 group=3.Extended context menu
:p.
Use the checkbox to disable or enable the aditional menuitems inserted by WPS-Wizard for all
objects at once.
If you want to disable the new items for a particular :link reftype=hd res=003.class:elink. go
to the settings notebook of the menu folder created for the class.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h1 res=20000 x=left y=bottom width=50% height=100% group=1 clear.Menu folder (configure context menus)
:p.
After installation you find two menu  folders in the WPS-Wizard folder,
:hp2.Menu for folders:ehp2. and :hp2.MEnu for files:ehp2..
Put shadows and program objects into these folders to insert them into the
context menu. It's possible to customize the menuitems to meet your needs and
create menu folders for individual object classes. 

:sl.
:li.:link reftype=hd res=2001.Building the menu:elink. 
:li.:link reftype=hd res=2002.Creating submenus:elink.
:li.:link reftype=hd res=2003.Enable the menu:elink.
:li.:link reftype=hd res=2004.Create a menu for an individual class:elink.
:li.:link reftype=hd res=2005.Settings for the program objects:elink.
:esl.
.*-----------------------------------------------------
 

.*-----------------------------------------------------
:h2  res=2001 x=right y=bottom width=50% height=100% group=2.Building the menu
:p.
You build a menu by using drag and drop. Just put the object you want to see in the menu
into the menu folder. The object isn't moved but a shadow will be created.
To put a program object into the folder hold down the Shift-key. The settings
of the program object may be changed to meet your particular needs. Read
:link reftype=hd res=2005.Settings for program objects:elink. for more.

:note text='Note:'.
There's no limit for the number of menu items.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=2002 x=right y=bottom width=50% height=100% group=2.Creating submenus
:p.
Create a submenu by dragging the menu folder template into the folder.
.br
You may create as many submenus as desired.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=2003 x=right y=bottom width=50% height=100% group=2.Enable the menu
:p.
You may enable or disable any individual menu folder. You find a checkbox
:hp1.Enable menu:ehp1. on the class selection page of the settings notebook.
.br
By using the central :link reftype=hd res=1007.configuration notebook:elink. you may
disable the new menu items for the whole system. 

:artwork  align=center name='classchoose.bmp'.  
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=2004 x=right y=bottom width=50% height=100% group=2.
Create a menu for an individual class
:p.
After installation you find menu folders for files and normal folders. In addition you may
create menu folders for particular file and folder classes. By doing so you may have for
example a special additional menu for bitmap files or your drives folder. The menu items
of these additional menu folders will be added to the ones already inserted using the
standard menu folders created during installation. 
:p.
Create a new menu folder by dragging the menu folder template to a place of your choice.
Drop an object of the :link reftype=hd res=003.class:elink. you want to associate with that
folder on the class selection area. After dropping you'll see the icon of the class and the
class name.

:note text='Note:'.It's not possible to change the association with a particular object class after
closing the notebook. You have to delete the menu folder and create a new one.
:p.
:artwork  align=center name='classchoose.bmp'.  
:p.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=2005 x=right y=bottom width=50% height=100% group=2.Settings for program objects
:p.
The name and path of the object which menuitem was selected may be given to a program object
for processing.
.br
Put :hp9.%*:ehp9. into the :hp1.Parameters:ehp1. field of the program object so WPS-Wizard 
will replace it when launching the object with the path and name. 
You may put this special parameter anywhere in the parameter list of the object. 
.br
You may change the working directory, too. If you don't enter a directory in the
:hp1.Working directory:ehp1. field (and no char even not a space) the path of the
object which menuitem was selected will be inserted.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h1 res=22000 x=left y=bottom width=40% height=100% group=1 clear.Toolbar
:link reftype=hd res=22001
auto dependent group=2.

:p.
The folder toolbar can be programmed using Rexx. For example it's possible to create various
launch areas on a toolbar or add entry fields etc.
:p.
The default toolbar can be changed even without programming just by dropping objects on it.

:sl.
:li.:link reftype=hd res=22001.Change folder action buttons:elink. 
:li.:link reftype=hd res=22002.Launch area:elink.
:li.:link reftype=hd res=22003.Removing objects:elink.
.*:li.:link reftype=hd res=22004.Erstellung von MenÅs fÅr einzelne Klassen:elink.
.*:li.:link reftype=hd res=22005.Einstellungen fÅr Programmobjekte:elink.
:esl.
.*-----------------------------------------------------


.*----------------------------------------------------- 
:h2  res=22001 x=right y=bottom width=60% height=100% group=2.Change folder action buttons
:p.
When clicking a button on the left of the toolbar a folder action is executed.
This means the folder is changed in some way or the contents (dependent on the type
of object clicked). For example one may open the parent folder, refresh the folder contents
and more.
:p.
After installation there're a couple of predefined action objects which may be dragged to
the launch area on the left. For certain needs it's possible to create new action objects.
These objects are Rexx scripts which are executed after clicking.
:p.
:artwork  align=center name='tbconfigure.bmp'.
:p.
To remove an object from the launch area right klick on it and select the menu item :hp2.Delete:ehp2..
Only the object on the toolbar will be removed not the object pointed to.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22002 x=right y=bottom width=60% height=100% group=2.Launch area
:p.
On the right of the toolbar you find a launch area. Drop any object there to have it right at hand.
This area can be used like the launchpad coming with OS/2.
:p.
To remove an object from the launch area right klick on it and select the menu item :hp2.Delete:ehp2..
Only the object on the toolbar will be removed not the object pointed to.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22003 x=right y=bottom width=60% height=100% group=2.Removing objects
:p.
To remove an object from the launch area right klick on it and select the menu item :hp2.Delete:ehp2..
Only the object on the toolbar will be removed not the object pointed to.

.*-----------------------------------------------------


.*---------------------- +++ h1 +++ -------------------------------
:h1 res=22500 x=left y=bottom width=40% height=100% group=1 clear.Gadgets 
:link reftype=hd res=22501
auto dependent group=2.

:p.
Gadgets are REXX scripts running hidden and drawing their output on the background of the desktop.
:nt.
The WPS must be running in order to use gadgets.
:ent.

:sl.
:li.:link reftype=hd res=22501.Start a gadget:elink. 
:li.:link reftype=hd res=22502.Activating a gadget:elink.
:li.:link reftype=hd res=22503.Terminating gadgets:elink.
:li.:link reftype=hd res=22504.Customizing gadgets:elink.
:esl.
:p.
The following gadgets are installed by default:
:sl.
:li.:link reftype=hd res=22510.System information:elink. 
:li.:link reftype=hd res=22511.Drive information:elink.
:li.:link reftype=hd res=22512.z!-info:elink.
:li.:link reftype=hd res=22513.Time:elink.
:li.:link reftype=hd res=22514.Calendar:elink. (:link reftype=hd res=22599.*:elink.)
:li.:link reftype=hd res=22515.File monitor:elink. (:link reftype=hd res=22599.*:elink.)
:li.:link reftype=hd res=22516.Binary clock:elink. (:link reftype=hd res=22599.*:elink.)
:esl.

:p.
The chapter :link reftype=hd res=30000.Gadget programming:elink. describes how to create new gadgets. 

.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22501 x=right y=bottom width=60% height=100% group=2.Start a gadget
:p.
Gadgets are started like every other object by double clicking them. You may also start
the gadget script using the command line. Be aware that the WPS must be up and running to
use gadgets.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22502 x=right y=bottom width=60% height=100% group=2.Activating a gadget
:p.
Normally a gadget doesn't respond to the mouse, isn't visible in the window list and
doesn't have a visible window. To interact with it one has to activate it. This is done
by holding down the CTRL key while clicking on it.
:p.
For deactivation of gadgets, this means sending them back to the background, again
hold down the CTRL key and click on the gadget. 
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22503 x=right y=bottom width=60% height=100% group=2.Terminating gadgets
:p.
To terminate a gadget first :link reftype=hd res=22502.activate:elink. it. Afterwards
select :hp2.Close:ehp2. from the context menu.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22504 x=right y=bottom width=60% height=100% group=2.Customizing gadgets
:p.
Gadgets may allow to change colors, fonts or the size. After :link reftype=hd res=22502.activation:elink.
use the color and font palettes to change the appearance. USe the mouse to change the size. Gadgets built from several parts
(e.g. system information or drive info) may need unlocking of the child gadgets before changing
them. To do so use the appropriate menu item from the context menu.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22510 x=right y=bottom width=60% height=100% group=2.Gadget: system information
:p.
This gadget shows the following system values
:ul.
:li.Physical memory
:li.Space on the boot drive
:li.System uptime
:li.Current time and date
:eul.
:p.
:artwork  align=center name='sysinfo.bmp'.

.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22511 x=right y=bottom width=60% height=100% group=2.Gadget: drive information
:p.
This gadget shows the free space on all fixed drives of the system.
:p.
:artwork  align=center name='gadget.bmp'.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22512 x=right y=bottom width=60% height=100% group=2.Gadget: z!-info
:p.
Display the title currently played by z!.
:p.
:artwork  align=center name='zinfo.bmp'.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22513 x=right y=bottom width=60% height=100% group=2.Gadget: time
:p.
Show the current time and date.
:p.
:artwork  align=center name='timegadget.bmp'.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22514 x=right y=bottom width=60% height=100% group=2.Gadget: calendar
:p.
The current month is displayed with the current day highlighted. Use the menu to
change the month.
:p.
:artwork  align=center name='calendargadget.bmp'.
:p.
This gadget was contributed by Goran Ivankovic.
:p.
Visit the homepage for more nice stuff:
:p.
http&colon.//www.os2world.com/goran/
:p.

.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22515 x=right y=bottom width=60% height=100% group=2.Gadget: file monitor
:p.
This gadget monitors a set of files for changes. You may provide a list of files to
be monitored. Use the menu for further explanations.

:p.
:artwork  align=center name='filemonitorgadget.bmp'.
:p.
This gadget was contributed by Goran Ivankovic.
:p.
Visit the homepage for more nice stuff:
:p.
http&colon.//www.os2world.com/goran/
:p.

.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22516 x=right y=bottom width=60% height=100% group=2.Gadget: binary clock
:p.
A binary clock for your desktop. If you don't know what to do with it it's not
for you...
:p.
:artwork  align=center name='binaryclockgadget.bmp'.
:p.
This gadget was contributed by Goran Ivankovic.
:p.
Visit the homepage for more nice stuff:
:p.
http&colon.//www.os2world.com/goran/
:p.

.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22599 x=right y=bottom width=60% height=40% group=3 hide.(*)
:p.
This gadget was contributed by Goran Ivankovic.
:p.
Visit the homepage for more nice stuff:
:p.
http&colon.//www.os2world.com/goran/
:p.


.*****************************************


.*------------------- +++ h1 +++ ----------------------------------
:h1.Some explanations of OS/2isms
:p.

This chapter explains some terms used within this manual.

:h2 group=2.Workplace Shell
:p.
The Workplace Shell is the graphical user interface of OS/2. It's highly configurable and
the advanced concept allows
the seemless addition of features. Programs or better classes may extend the classses already
present on your system for example the folder class with new functionality. These new features
are not a kind of add on (or a kludgy hack) but become part of the WPS. 

:h2 res=003 group=2.Class
:p.
Every object of the Workplace Shell belongs to a particular class. The objects of a class
all have the same features  for example they share the same pages in the settings notebook. 
Programmers may take a class and build a new one using the chosen class as the base. The
new class has all the characteristics of the base class in addition to the features added
by the programmer. For example the menu folders of WPS-Wizard use the normal folder class
as a base and thus behave almost like normal folders in respect to the settings notebook or
the ability to show the contents in different ways. The difference is it accepts only
shadows and program objects. 
:p.
Building a new extended class is one way to add features to the WPS. Another way is to replace a
class by another one. WPS-Wizard replaces the common file and folder classes with new versions
which have an extended context menu. By doing so all folders and files on the system share
the new feature set. 

:h1.Known problems
:p.

:sl.
:li.Currently there's no way to change the order of menu items. They show up in the order they were created.
:li.If you drop a shadow on the class selection page of a menu folder it will use the shadow as the chosen
class not the object the shadow points to. 
:esl.
:p.
(All bugs you may find are meant as features &colon.-) )


:h1.History

:p.
V0.5.2 (06.Apr.2007):
:sl.
:ul compact.
:li.New CWObject method 'wizQueryRexxScript()'.
:li.Fixed a possible memory leak with folder info scripts.
:li.Sometimes carriage returns in object names truncated flyover help for launchpad buttons.
:li.Iconsize may be increased. Esp. useful for folders with image files.
:li.Check for broken SET HOME= statement in config.sys
:li.Fixed crash when 'cwCreateLaunchArea' was called with a WPFolder instead of CWLaunchArea object.
:li.Improved REXX error reporting.
:li.Image data is deleted when scrolled out of sight in a container. This prevents memory exhausting when a
 folder contains several hundred or thousand image files. CWMM-classes V2.9.0 or above must be installed.
:li.You may TAB from the folder toolbar to the container. Tabbing to the toolbar or between toolbars isn't yet implemented.
:li.ONTIMER in gadgets was called without the timer ID
:li.Changed installation script to always create new toolbar folders ('replace' instead of 'update')
:li.Added check for timer ID value in winStartTimer() and winStopTimer()
:li.Folder information area is using the Innotek font engine for text drawing.
:li.Added support for icon theming in the WPS. You may use PNG icons now. You need a separate icon addon.
:eul.
:esl.


:p.
V0.5.1 (12.Mar.2004):
:sl.
:ul compact.
:li.Added IMAGE_GADGET
:li.New context menu for folder action buttons
:li.New menu function menuEnableItem()
:li.More german translations of hints
:li.Size of infowindow wasn't always saved
:li.Bar showing the free drive space for the Icons in drives folder 
:li.The image contained in an image file is drawn as the icon (if the cw-multimedia classes are installed). 
:li.Format menu is properly removed for USB floppy drives now.
:li.Some more icons by Klaus Staedtler
:li.Gadget variable <system.path.wpswizardhints> pointed to INI file not hint database.
:li.The INI file WPSWIZZ.INI is now created in the users home directory if defined.
:eul.
:esl.

:p.
V0.5.0 (02.Jan.2004):
:sl.
:ul compact.
:li.Added Gadgets (small scripts running in the background and drawing their
output on the desktop).
:li.Added a script tutorial section to the documentation.
:li.Some minor fixes to the install and uninstall scripts.
:eul.
:esl.

:p.
V0.4.1 (21.Nov.2003):
:sl.
:ul compact.
:li.Info area text formatting rewritten for better tag and font handling. It may be necessary to adjust your info scripts.
:li.Better font handling for the info area
:li.When REXX/info area are disabled the corresponding menuitems are removed.
:li.Fixed a couple of minor memory leaks
:li.New generic WPS class implementing a general purpose launch area.
:li.Launch area for toolbar. Drop any object there and use it like the LaunchPad.
:li.Toolbar buttons may be configured using drag and drop.
:li.Better error reporting for Rexx scripts.
:li.New function 'winShowWindow()'
:li.Fixed broken transparency with SDD and SNAP
:eul.
:esl.

:p.
V0.4.0 (21.Jun.2003)&colon.
:sl.
:ul compact.
:li.Corrected some typos in the manual
:li.Fixed some memory leaks
:li.Better script handling. Different kind of scripts have their own subdir.
:li.New callback for wpModifyMenu
:li.New function winQueryFolderObjects()
:li.Info area in folders.
:li.Toolbar may be disabled independently for any folder.
:li.Some code cleanups
:eul.
:esl.

:p.
V0.3.0 (10.Jan.2003)&colon.
:sl.
:ul compact.
:li.Saving of scrollbar position in detailsview not always worked.
:li.Files copied from CD are not read only
:li.Submenus in user skripts menu are possible
:li.Added support for wpSetType(), wpQueryFldrSortClass(), wpQueryFldrDetailsClass(),
wpSetFldrDetailsClass(), wpSetFldrSortClass()
:li.Added WPS call back interface for selected methods for use with REXX scripts
:li.Added menu functions and drag functions to REXX interface
:li.Added object hints (fly over help)
:eul.
:esl.

:p.
V0.2.0 (8.Mar.2002)&colon.
:sl.
:ul compact.
:li.Saving of splitbar position for folder details view.
:li.Saving of scrollbar position for details view.
:li.REXX interface to the WPS added.
:li.Templates checkbox was lost.
:li.Fixed some minor memory leaks.
:eul.
:esl.

:p.
V0.1.2 (13.May.2001)&colon.
:sl.
:ul compact.
:li.Parameters of program objects were sometimes messed up.
:li.The tilde wasn't properly removed from titles.
:eul.
:esl.
:p.
V0.1.0 (05.May.2001)&colon.
:sl.
:ul compact.
:li.First public version.

:eul.
:esl.


.***************************************

.im rexxinterface.inc

.im gadget.inc

.im programminghowto.inc

.im defaultscripts.inc

.im hiddenfeatures.inc

.im setupstrings.inc

.im hintdatabase.inc

.im wpsclasses.inc

:euserdoc.





