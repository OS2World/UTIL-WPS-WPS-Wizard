

  Note: If you don't like this package don't use it. You get it for
        free in the end.



PNG-Icons for WPS-wizard 02.Sep.2007
------------------------------------

History
-------

V1.1rc3 (02.09.2007)

-Images set using a setup string weren't saved.
-Appending to a NOMPtrArray was broken
-Not all strings in the CLASS-NAME.INI were terminated by zero.
-More images defined in CLASS-NAME.INI
-Data files with attached own images had the normal icon during drag and drop
-Basic PNG iconeditor
-Removed obsolete methods in CWThemeMgr
-Internal cleanups
-When dropping an object without custom icon use the class icon instead
-Using bigger stack when opening folders or showing background pages. This
 solves a general WPS problem with stack sizes and thus is a stability improvement. 
 It is not related to the theme classes or WPS wizard but a workaround for a system
 problem.
-New CairoGadget class.
-And much more...

V1.1rc2 (05.08.2007)

-wrong readme file

V1.1rc (04.08.2007)

-Fixed WPS crashes because of missing stack space
-Specify icons by object ID
-Drag and drop icon changing
-Icons for way more classes and objects

V1.0 (06.Apr.2007)

-First public version


Introduction
------------

This addon for WPS-wizard gives you fancy PNG icons for the
WPS. It uses Cairo for rendering and allows most of the Cairo
magic when drawing icons like e.g. translucency.

Note that this is a preview which lacks quite some stuff. It
works for me, that's all.


Installation
------------

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!                                             !!!
!!! Make sure you have a recent INI file backup !!!
!!!                                             !!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


You need the cairo runtime libs (cairo-1.4.6-os2-bin.exe):

  ftp://ftp.netlabs.org/incoming/cairo-1.4.6-os2-bin.exe
       (will be moved to ftp.netlabs.org/pub/cairo eventually)

  http://os2site.com/sw/dev/cairo/

and libc0.6.1:

  ftp://ftp.netlabs.org/pub/libc/


!!! Make sure WPS-wizard 0.5.2 is installed and working properly !!!
!!! Make sure the runtime libs mentioned are installed           !!!


Copy the contents of this folder into the WPS-wizard (0.5.2)
installation directory. Make sure the *.rx files in ..\bin\theme
end up in ...\bin\theme of your WPS-wizard installation.

Start iconinstall.cmd and restart the WPS. You should have a new
iconsize menu item in every folder view menu. Test it if it works.


Hint
----

To enable new icons for the templates and the drives folder open
the settings and reset the icons on the icon page.


Customization
-------------

Add icons for more  classes to the INI file in the 64x64 subdir.
Be aware that class replacement may cover some base classes. If this
is the case add an entry for the replacement class. See CWFolder/WPFolder
for an example.


Known issues
------------

- When using shadows the color of normal and shadow object texts may be
  the same on some systems.

- Sometimes objects lose their PNG image and only display the normal OS/2
  icon. Opening the settings may help.

- Scrolling in folders is slow.

- Very long icon titles are not completely drawn with emphasis but only
  the left part.

- Browsing for new icons is slow for the first time after startup.

- Undo for icons when using the settings doesn't work.

- Template background is drawn as a normal OS/2 template most of the time.

- No tutorial for icon creation yet.

- No extensive documentation (well nobody reads it anyway...).


Copyright
---------

See the files in the 64x64 subdir for the image copyrights. The images
come with another copyright than the actual code of this WPS-wizard
addon. The images are only included for your convenience.

There's not warranty whatsoever. Use this code or leave it. If it burns
your house it's all your fault.


The code is (c) Cinc

-> #netlabs irc.aroa.ch

