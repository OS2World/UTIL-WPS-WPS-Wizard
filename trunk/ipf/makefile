# Makefile fÅr WPS-Wizard Hilfedateien


all: menuwizz.inf menufold.hlp menuwizz001.inf menufold001.hlp

menufold.hlp:	menufold.ipf
	ipfc menufold.ipf

menufold001.hlp:	menufold001.ipf
	ipfc menufold001.ipf

menuwizz.inf:	menuwizz.ipf
	ipfc -i menuwizz.ipf

menuwizz001.inf:	menuwizz001.ipf
	ipfc -i menuwizz001.ipf

clean:
	-rm *.inf
	-rm *.hlp

cleaner:
	-rm *.*~

