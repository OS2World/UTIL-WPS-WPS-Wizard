:userdoc.

:docprof.

:title. Help for menu folder

:h1 res=001.WPS-Wizard
:p.

WPS-Wizard gives you a new folder class which may hold shadows and program objects.
After installation there're menu folders for files and folders. The contents of
each menu folder shows up in the context menu of the object the folder belongs to.
In addition you may create menu folders for any folder and datafile class of your system.
For example you may create an additional menu folder for iconfiles which contents will
only be shown when requesting the context menu of the iconfile.

:p.
You may find more about this feature in the
:link reftype=launch
object='view.exe'
data='menuwizz.inf'.
introduction to WPS-Wizard:elink..
.*

:h2 res=003.Class
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

:h2 res=100.Options
:p.
Use this page to disable or enable the features of WPS-Wizard.
:p.
Klick for further information:
:sl.
:li.:link reftype=hd res=102.Enable REXX scripts:elink.
:li.:link reftype=hd res=105.Show object info:elink.
:li.:link reftype=hd res=101.Show filesize:elink.
:li.:link reftype=hd res=103.Remove format menuitem:elink.
:li.:link reftype=hd res=104.Extended context menu:elink.
:esl.

:h3 res=102.Enable REXX scripts
:p.
WPS-Wizard may be extended by adding REXX scripts. For example the folder toolbar is added by using
a script and object hints are also shown by small REXX scripts. Use the check box to disable all REXX scripts
used by WPS-Wizard.
:nt.
Scripts added to the folder menu bar are not affected by this setting.
:ent.

:h3 res=105.Show object info
:p.
Object hints are shown in a small window if the mouse pointer isn't moved when over objects.
Use the check box to enable this feature.
:p.
:artwork  align=center name='objecthint.bmp'.
:p.
:nt.
REXX support of WPS-Wizard must be enabled to use object hints.
:ent.

:h3 res=101.Show filesize
:p.
Select this item to show the filesize in the context menu of a file object

:h3 res=103.Remove format menuitem
:p.
After selecting this item the format menuitem will be removed from the menu of your harddisks to
prevent formating by accident. The context menu of a floppy disk drive isn't affected.

:h3 res=104.Extended context menu
:p.
Use the checkbox to disable or enable the aditional menuitems inserted by WPS-Wizard for all
objects at once.
If you want to disable the new items for a particular :link reftype=hd res=003.class:elink. go
to the settings notebook of the menu folder created for the class.


:h1 res=201.Menu folder
.*Allgemeine Hilfe fÅr MenÅ-Ordner
:p.
:artwork runin name='menu.bmp'.
:p.
The menu folder holds shadows of objects or program objects configured to start your preferred
program. All these objects will show up in the context menu of the :link reftype=hd res=003.class:elink.
the menu folder belongs to. For every class in the system there may be one associated menu folder.
The contents of the global menu folders (created upon installation) will be inserted in addition
to these associated folders.

:h2 res=002.Class selection
:p.
To associate a menu folder with a particular :link reftype=hd res=003.class:elink. drop
an object of the class on the class selection area. The standard icon of the class will be shown
and the name written below the area. Press the cancel button or drop another object to change the
association while the notebook is opened. After closing the selection will be saved. It's
only possible to have one menu folder for every given class. 

:note text='Note:'.You can't change an association after closing the notebook. If you want to do
so you have to delete the menu folder.

:p.
Click the checkbox :hp2.Enable menu:ehp2. to use the context menu items contained in the menu folder.

:h1 res=202.WPS-Wizard configuration
.*Allgemeine Hilfe fÅr Konfigurations-Notebook von WPS-Wizard
:p.
:artwork runin name='menuConfig.bmp'.
:p.
Use this object to configure
:link reftype=hd res=001. WPS-Wizard:elink..
Doubleclick to open the settings notebook where you may enable or disable the features.

:euserdoc.




