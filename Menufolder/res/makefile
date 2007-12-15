# Makefile

MOREINC	=	..\include
OBJDIR		=	../o

all: $(OBJDIR)/menufolder.res $(OBJDIR)/menufolder001.res

$(OBJDIR)/menufolder.res:	menufolder.rc
	 rc -n -r -i $(MOREINC) menufolder.rc $(OBJDIR)/menufolder.res

$(OBJDIR)/menufolder001.res:	menufolder001.rc
	 rc -n -r -i $(MOREINC) menufolder001.rc $(OBJDIR)/menufolder001.res

