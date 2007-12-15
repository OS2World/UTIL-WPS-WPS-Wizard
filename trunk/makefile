# Makefile fr WPS-Wizard
#
# (C) C. Wohlgemuth 1996-2007
#
#
# Make sure to change make.inc according to your setup.
#

include	./make.inc

DISTNAME	=	wpswizard-$(VERSION)
SOURCEDISTNAME	= wpswizard-src-$(VERSION)

ICONDISTNAME	=	wpswizard-icon-addon-v1.1rc3


CPPFLAGS     = -O -Zomf -Zcrtdll -s
 
all:	menufldr.dll wpswizz.dll programs helpStuff bubblepad

rxtest.exe:	rxtest.c
	gcc $(CPPFLAGS) rxtest.c rxtest.def
#-lrexx

menufldr.dll:
	@echo ''
	@echo '[32;1;m********** Compiling Menufolder... ***********[0;m'
	-@cd menufolder/c && make

wpswizz.dll:
	@echo ''
	@echo '[32;1;m********** Compiling wpswizz.dll... ***********[0;m'
	-@cd newclasses && make

programs:
	@echo ''
	@echo '[32;1;m********** Compiling helper programs... ***********[0;m'
	-@cd progs &&  make
#	-@cd progs && make

helpStuff:
	@echo ''
	@echo '[32;1;m********** Compiling help files... ***********[0;m'
	-@cd ipf && make

bubblepad:
ifdef	BUBBLEPADSOURCE
	@echo ''
	@echo '[32;1;m********** Compiling BubblePad... ***********[0;m'
	-@cd $(BUBBLEPADSOURCE)/source && make
endif

clean:
	-@cd ./NewClasses && make clean
	-@cd ./progs && make clean
	-@cd ./menufolder/c && make clean
	-@cd ./ipf && make clean

cleaner:
	-@cd NewClasses && make cleaner
	-@cd progs && make cleaner
	-@cd menufolder/c && make cleaner
	-@cd ipf && make cleaner
	-@cd helper && rm *.*~
	-@cd dist_ger && rm -r *
	-@cd dist_eng && rm -r *
	
german:
	@echo ""
	@echo '[35;1;m******************************************************[0;m'
	@echo '[35;1;m********** Creating German distribution... ***********[0;m'
	-@cd dist_ger && rm -r *
	mkdir dist_ger\bin
	mkdir dist_ger\bin\hint
	mkdir dist_ger\bin\fldrinfo
	mkdir dist_ger\bin\toolbar
	mkdir dist_ger\bin\gadgets
	mkdir dist_ger\bin\theme
	mkdir dist_ger\help
	mkdir dist_ger\res
#	make all
	cmd /C copy install\install.cmd+install\nls\install.ger dist_ger\install.cmd
	cmd /C copy install\uninstal.cmd+install\nls\uninstal.ger dist_ger\uninstal.cmd
	cp newclasses/o/wpswizz.dll dist_ger/
	cp newclasses/o/mmtheme.dll dist_ger/
	cp newclasses/o/mmtheme.sym dist_ger/
	@cp newclasses/o/wpswizz.sym dist_ger/
	@cp menufolder/o/menufldr.dll dist_ger/
	@cp menufolder/o/menufldr.sym dist_ger/
	-@cd dist_ger && lxlite *.DLL 
	cp progs/*.exe dist_ger/bin
	cp progs/zip*.cmd dist_ger/bin
	cp progs/example*.cmd dist_ger/bin
	cp progs/*.rx dist_ger/bin
	cp progs/scripts dist_ger/bin
	cp progs/hint/*.rx dist_ger/bin/hint
	cp progs/fldrinfo/*.rx dist_ger/bin/fldrinfo
	cp progs/toolbar/*.cmd dist_ger/bin/toolbar
	cp progs/gadgets/*.cmd dist_ger/bin/gadgets
	-cp progs/theme/*.* dist_ger/bin/theme
	cp ./res/mem.ico dist_ger/bin/gadgets
	cp ./res/drive.ico dist_ger/bin/gadgets
	cmd /C copy progs\createfd.cmd+progs\nls\createfd.ger dist_ger\bin\createfd.cmd
	cmd /C copy progs\createdf.cmd+progs\nls\createdf.ger dist_ger\bin\createdf.cmd
#	-cp newclasses/res/*.INI dist_ger/bin
	cp ipf/menufold.hlp dist_ger/HELP
	cp ipf/menuwizz.inf dist_ger/HELP
	cp NewClasses/o/OBJHINTS.INI dist_ger/HELP
	cp NewClasses/res/buildhintini.CMD dist_ger/bin
	cp res/*.* dist_ger/res
	cmd /C copy file_id\FILE_ID.DIZ+file_id\FILE_ID.ger dist_ger\FILE_ID_DIZ
#	-cp COPYING dist_ger/
	cp file_id/Readme.1st dist_ger/
	cp file_id/Changes dist_ger/
ifdef	BUBBLEPADSOURCE
	@echo '[33;1;m********** Adding BubblePad... ***********[0;m'
	@mkdir dist_ger\BubblePad
	@cp $(BUBBLEPADSOURCE)/Readme dist_ger/BubblePad
	@cp -r $(BUBBLEPADSOURCE)/Deutsch dist_ger/BubblePad
	@echo '   BubblePad added.[0;m'
endif
	@cd dist_ger && zip -r $(DISTNAME)-ger.zip * 1>NUL
	@echo '[32;1;m********** German distribution done ***********[0;m'

english:
	@echo '[35;1;m********** Creating english distribution... ***********[0;m'
	@echo ""
	-@cd dist_eng && rm -r *
	@mkdir dist_eng\bin
	@mkdir dist_eng\bin\hint
	@mkdir dist_eng\bin\fldrinfo
	@mkdir dist_eng\bin\toolbar
	@mkdir dist_eng\bin\gadgets
	@mkdir dist_eng\bin\theme
	@mkdir dist_eng\help
	@mkdir dist_eng\res
	@make all
	@cmd /C copy install\install.cmd+install\nls\install.eng dist_eng\install.cmd
	@cmd /C copy install\uninstal.cmd+install\nls\uninstal.eng dist_eng\uninstal.cmd
	@cp newclasses/o/wpswizz001.dll dist_eng/wpswizz.dll
	@cp newclasses/o/wpswizz.sym dist_eng/
	@cp menufolder/o/menufldr001.dll dist_eng/menufldr.dll
	@cp menufolder/o/menufldr.sym dist_eng/
	cp newclasses/o/mmtheme.dll dist_eng/
	cp newclasses/o/mmtheme.sym dist_eng/
	-cd dist_eng && lxlite *.DLL
	-cp progs/*.exe dist_eng/bin
	-cp progs/zip*.cmd dist_eng/bin
	-cp progs/example*.cmd dist_eng/bin
	-cp progs/*.rx dist_eng/bin
	-cp progs/scripts dist_eng/bin
	-cp progs/hint/*.rx dist_eng/bin/hint
	-cp progs/fldrinfo/*.rx dist_eng/bin/fldrinfo
	-cp progs/toolbar/*.cmd dist_eng/bin/toolbar
	-cp progs/gadgets/*.cmd dist_eng/bin/gadgets
	-cp progs/theme/*.* dist_eng/bin/theme
	-cp ./res/mem.ico dist_eng/bin/gadgets
	-cp ./res/drive.ico dist_eng/bin/gadgets
	-cmd /C copy progs\createfd.cmd+progs\nls\createfd.eng dist_eng\bin\createfd.cmd
	-cmd /C copy progs\createdf.cmd+progs\nls\createdf.eng dist_eng\bin\createdf.cmd
	-cp ipf/menufold001.hlp dist_eng/HELP/menufold.hlp
	-cp ipf/menuwizz001.inf dist_eng/HELP/menuwizz.inf
	-cp NewClasses/o/OBJHINTS.eng dist_eng/HELP/OBJHINTS.INI
	-cp NewClasses/res/buildhintini.CMD dist_eng/bin
	-cp res/*.* dist_eng/res
#	-cp newclasses/res/*.INI dist_eng/bin
	-cmd /C copy file_id\FILE_ID.DIZ+file_id\FILE_ID.eng dist_eng\FILE_ID_DIZ
#	-cp COPYING dist_eng/
	-cp file_id/Readme.1st dist_eng/
	-cp file_id/Changes dist_eng/
ifdef	BUBBLEPADSOURCE
	@echo '[33;1;m********** Adding BubblePad... ***********[0;m'
	-@mkdir dist_eng\BubblePad
	@cp $(BUBBLEPADSOURCE)/Readme dist_eng/BubblePad
	@cp -r $(BUBBLEPADSOURCE)/English dist_eng/BubblePad
	@echo '[33;1;m   BubblePad added to distribution.[0;m'
endif
	-@cd dist_eng && zip -r $(DISTNAME)-eng.zip *

distribution:
	make german
	make english

sourcedistribution:
	@echo "Creating source package..."
	-rm  $(SOURCEDISTNAME).zip
	make clean
	make cleaner
	zip -r $(SOURCEDISTNAME).zip *

icontheme:
	-@cd dist_eng && rm -r *
	mkdir dist_eng/pngicons
	mkdir dist_eng/pngicons/bin
	mkdir dist_eng/pngicons/bin/theme
	cp newclasses/gcc_o/cwtheme.dll dist_eng/pngicons
	cp newclasses/gcc_o/cwtheme.sym dist_eng/pngicons
	cp newclasses/o/mmtheme.dll dist_eng/pngicons
	cp newclasses/o/mmtheme.sym dist_eng/pngicons
	cp install/iconinstall.cmd dist_eng/pngicons
	cp install/iconuninstall.cmd dist_eng/pngicons
	cp File_id/Readme-Icons.txt dist_eng/pngicons
	cp desktop-blue.jpg dist_eng/pngicons
	cp -r 64x64 dist_eng/pngicons
	cp -r progs/theme/*.rx dist_eng/pngicons/bin/theme
	cp -r progs/theme/*.cmd dist_eng/pngicons/bin/theme
	-cd dist_eng/pngicons && zip -r $(ICONDISTNAME).zip *

help:
	-@echo "make help         : This page"
	-@echo "make              : Compile only"
	-@echo "make english      : Create english LV (in dist_eng)"
	-@echo "make german       : Create german LV (in dist_ger)"
	-@echo "make distribution : Create german and english LV"
	-@echo "make sourcedistribution : Create the source distribution"
	-@echo ""
	-@echo "This version of WPS-Wizard is "$(DISTNAME)

