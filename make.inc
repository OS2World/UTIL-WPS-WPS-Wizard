#define the next makro if compiling a release rather than a beta
#This will remove the build string from the about dialog
export WPSWIZRELEASE	=	YES


VERSION		=	0_6_0
VERSION_STRING  =	0.6.0
AUTHOR_STRING	=	(C) Chris Wohlgemuth 1996-

# Put version in environment for REXX skripts
export	WPSWIZSVERSION	=	$(VERSION_STRING)
export	WPSWIZSAUTHOR	=	$(AUTHOR_STRING)

#The base dir for additional functions for the MM helper programs
export COMMONSRC	=	H:/cwmm/common_functions


# Where to find BubblePad. If empty bubblepad will not be added.
export BUBBLEPADSOURCE=
#	H:/BUBBLEPAD107

#The path to the gcc ENV setup command file
GCCENV	=	L:\usr\bin\gccenv.cmd

#The path to the SOM includes
SOMINC	=	f:/ibmcpp/include/som