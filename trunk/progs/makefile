# Makefile for helper progs
#
# Use klibc and gcc<> EMX
#

include ./../make.inc

GCC_CC          = gcc
GCCFLAGS	=	-Zomf -Zmt -O2 -Zcrtdll


all: copyname.exe repclass.exe wpsreset.exe


copyname.exe:	copyname.c copyname.def
	$(GCC_CC) $(GCCFLAGS)  $< copyname.def

repclass.exe:	repclass.c
	$(GCC_CC)  $(GCCFLAGS) $< 

wpsreset.exe:	wpsreset.c
	$(GCC_CC)  $(GCCFLAGS) $< 

clean:
	rm *.exe

cleaner:
	rm *.*~
