:h1 res=6500.WPS-wizards WPS classes
:p.
WPS-wizard consists of several WPS classes in two DLLs. The first DLL contains replacements for
:hp2.WPObject:ehp2., :hp2.WPDataFile:ehp2., :hp2.WPFolder:ehp2. and :hp2.WPDisk:ehp2..
:ul.
:li.CWObject
:li.CWDataFile
:li.CWFolder
:li.CWDisk
:eul.
:p.
These objects insert the items into the popup menu and the menubar of folders by calling
methods of a specialized class in the second DLL.
:p.
The second DLL contains all the Rexx calling code needed and classes designed to
be called for starting objects or executing Rexx scripts. These classes also implement
the user interface for configuring by providing the :hp2.Menu folder:ehp2. and the
:hp2.WPS-wizard settings:ehp2. notebook.

:ul.
:li.CWWizzSettings
:li.CWMenuFolder
:li.CWLaunchArea
:eul.
:p.
CWMenuFolder may be used by other classes if custom menus are needed. The class
provides methods to insert object items into a menu based on the contents of a Menu Folder.
Other methods start/execute these inserted objects. As a real life example the Audio/Data-CD-Creator
package and the CW-Multimedia classes use this API to provide scripting ability.
:p.
A CWLaunchArea may be used by other classes if a generic launchpad is needed. Creating
such an area is done just by creating a CWLaunchArea folder and calling a single method on
this folder. All the drag and drop handling and object starting is entirely done by the specialized
CWLaunchArea class and doesn't has to be reimplemented in other classes.
:p.
If you want to use the classes in your own project send me a mail. As soon as time permits the
API will be documented in this book.
