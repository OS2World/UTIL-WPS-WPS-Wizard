# Makefile fÅr NewFile.dll

include ../../make.inc

CC           = icc
#CFLAGS       =  /Gm+ /G4 /Gl  /O+ /Oc+ /Ol /ss+

#LDFLAGS =	-Ge- /Gm+ -G4 /ss -Gl /O+ -Oc+ -Ol-  /Q+ /Tn /Ti
LDFLAGS =	-Ge- /Gm+ -G4 /ss -Gl /O+ -Oc+ -Ol-  /Q+
#CFLAGS       =   -Ge- /Gm+ -G4 /ss -Gl /O+ -Oc+ -Ol- /Q+ /Tn /Ti /C
CFLAGS       =   -Ge- /Gm+ -G4 /ss -Gl /O+ -Oc+ -Ol- /Q+ /C

LIBS =		rexx.lib
DEFS         = 
OBJECTS	= 
# Include file of WPS class
CLASSINC	=	../include
MOREINC	=	../include -I ../../include -I $(COMMONSRC)/include
MOREINC2	=	../../include
MOREINC3	=	../../NewClasses/c
OBJDIR		=	../o
RESDIR		=	../res
MENUOBJ	=  	$(OBJDIR)/menufolder.obj \
		$(OBJDIR)/wpswizzsettings.obj \
		$(OBJDIR)/except.obj \
		$(OBJDIR)/helper.obj \
		$(OBJDIR)/menufldrrexx.obj \
		$(OBJDIR)/rxcallwpsfunc.obj \
		$(OBJDIR)/rxcallwpsclassfunc.obj \
		$(OBJDIR)/rxcallcwwpsfunc.obj \
		$(OBJDIR)/fnwpwizbuttonproc.obj \
		$(OBJDIR)/rxcallpmfunc.obj \
		$(OBJDIR)/progbars.obj \
		$(OBJDIR)/cwlauncharea.obj

LIBOBJECTS	=	$(OBJDIR)/menu_funcs.obj \
			$(OBJDIR)/sys_funcs.obj \
			$(OBJDIR)/som_wps_funcs.obj

HELPEROBJ	= 

OBJECTS		=	$(MENUOBJ) $(LIBOBJECTS) $(HELPEROBJ)

all:	$(OBJDIR)/menufldr.dll

$(OBJDIR)/menufldr.dll:  $(OBJECTS) $(RESDIR)/menufolder.res $(RESDIR)/menufolder001.res
	cmd.exe /C create_menufldr_def.CMD $(OBJDIR)/menufolder.def
	$(CC) $(LDFLAGS) /L /Fm$(basename $@)  /Fe$@ $(OBJECTS) $(LIBS) $(OBJDIR)/menufolder.def
	-cp -f $@ $(OBJDIR)/menufldr001.dll
	rc -n $(OBJDIR)/menufolder.res $@
	rc -n $(OBJDIR)/menufolder001.res $(OBJDIR)/menufldr001.dll
	@cd ../o && MAPSYM $(notdir $(basename $@))

$(RESDIR)/menufolder.res:	$(RESDIR)/menufolder.rc
	@cd ../res && make

$(RESDIR)/menufolder001.res:	$(RESDIR)/menufolder001.rc
	@cd ../res && make

###################################
#
#    Rules for compiling
#
###################################

### WPS objects ###
$(OBJDIR)/%.obj:	%.cpp $(CLASSINC)/%.hh
	$(CC) -I $(MOREINC) $(CFLAGS)  /Fo$@ $<



### additional objects for class ###
$(OBJDIR)/%.obj:	%.cpp
	$(CC) -I $(MOREINC) $(CFLAGS)  /Fo$@ $<



### Lib files ###
$(OBJDIR)/%.obj:	$(COMMONSRC)/%.c $(COMMONSRC)/include/%.h
	$(CC) -I $(MOREINC) $(CFLAGS)  /Fo$@ $<

$(OBJDIR)/%.obj:	$(COMMONSRC)/%.c $(COMMONSRC)/include/%_funcs.h
	$(CC) -I $(MOREINC) $(CFLAGS)  /Fo$@ $<



$(OBJDIR)/except.obj:		../../helper/except.c ../../include\except.h
	$(CC) -I $(MOREINC2) $(CFLAGS)  /Fo$@ $<

$(OBJDIR)/helper.obj:		../../helper/helper.cpp
	$(CC) -I $(MOREINC2) $(CFLAGS)  /Fo$@ $<

$(OBJDIR)/debug.obj:		../../helper/debug.c  ../../include\debug.h
	$(CC) -I $(MOREINC2) $(CFLAGS)  /Fo$@ $<


$(HELPEROBJ):	../../helper/helper.cpp
	$(CC) -I $(MOREINC2) -I $(MOREINC) $(CFLAGS)  /Fo$@ $<


clean:
	-cd ../o && rm *

cleaner:
	-cd ../include && rm *.*~
	-rm *.*~
	-cd ../res && rm *.*~

