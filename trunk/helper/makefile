# Makefile fÅr NewFile.dll

include	../make.inc

CC           = icc
LIB_DIR      = $d:/emx/lib
LDLIBS       = -Zomf -Zsys -los2me -ldebug 

CFLAGS       = /Ge- /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /ss+ /Q+ /C+
CPPFLAGS     = /Ge- /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /Q+
# $(VACDEBUG)

DEFS         = 
OBJECTS	= 	newfile.obj
MOREINC	=	..\include -I..\menufolder\include -I..\newclasses\c -I..\newclasses\include -I..\newclasses\inc_class -I $(COMMONSRC)/include

MENUOBJ		=	wpswizzsettings.obj \
			menufolder.obj
HELPEROBJ	=	helper.obj \
			except.obj \
			cell.obj

#			htmlwnd.obj folderinfownd.obj 

all:	$(HELPEROBJ)
	cd ./.. && make

launchpad.obj:		launchpad.cpp
	icc $(CPPFLAGS) /C+ -I$(MOREINC) /Fo$@ $<

helper.obj:	helper.cpp
	 icc -I$(MOREINC) $(CFLAGS) $<

except.obj:	except.c
	 icc -I$(MOREINC) -I../include $(CFLAGS) $<

cell.obj:	cell.c
	 icc -I$(MOREINC) -I../include $(CFLAGS) $<

htmlwnd.obj:	htmlwnd.c
	 icc -I$(MOREINC) -I../include $(CFLAGS) $<

folderinfownd.obj:	folderinfownd.c ..\include\folderinfownd.h
	 icc -I$(MOREINC) -I..\include $(CFLAGS) $<
