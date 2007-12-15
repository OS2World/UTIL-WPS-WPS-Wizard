:userdoc.

:docprof toc=1234.

:title. Einfhrung zu WPS-Wizard

:h1 res=001.Titelseite
:artwork align=center name='main.bmp'.
:p.
.ce WPS-Wizard 0.6.0 (c) 1996-2007 Chris Wohlgemuth
.ce Icons (c) Klaus Staedtler-Przyborski
:p.
:lines align=center.
:link reftype=launch
object='netscape.exe'
data='http://www.geocities.com/SiliconValley/Sector/5785/index.html'.
http&colon.//www.geocities.com/SiliconValley/Sector/5785/index.html:elink.

:link reftype=launch
object='netscape.exe'
data='http://www.os2world.com/cdwriting'.
http&colon.//www.os2world.com/cdwriting:elink.
:elines.

.*-----------------------------------------------------

:h1 .Lizenz
:p.
.im licence_ger.inc


.*-----------------------------------------------------
:h1  res=1002 x=left y=bottom width=40% height=100% group=1 .šberblick 
:link reftype=hd res=1001
auto dependent group=2.

:p.
WPS-Wizard ist eine frei konfigurierbare Erweiterung der WPS. Sie bietet:
:sl.
:li.:link reftype=hd res=1001.šber Drag and Drop konfigurierbare Meneintr„ge:elink. 
:li.:link reftype=hd res=1004.Die Anzeige der Dateigr”áe einer Datei direkt im Kontextmen:elink.
:li.:link reftype=hd res=1006.Entfernung des Formatieren-Mens bei allen Festplatten:elink.
:li.:link reftype=hd res=1010.REXX Interface zur WPS:elink.
:li.:link reftype=hd res=10200.Hinweise, wenn der Mauszeiger ber Objekten verweilt:elink.
:li.:link reftype=hd res=10230.Werkzeugleiste fr Ordner:elink.
:li.:link reftype=hd res=10220.Informationsbereich in Ordnern:elink.
:li.:link reftype=hd res=10240.Gadgets im Hintergrund der Arbeitsoberfl„che:elink.
:esl. 
:p.
Diese Features lassen sich ber ein zentrales
:link reftype=hd res=1007. Konfigurations-Notizbuch :elink.
ein- oder ausschalten.
:p.
šber die REXX-Schnittstelle sind unter anderem auch Ordner-Werkzeugleisten realisiert. Durch Anpassung
eines REXX-Skriptes lassen sich diese beliebig ver„ndern. N„heres dazu in den Kapiteln
:link reftype=hd res=1010.REXX Interface zur WPS:elink. und
:link reftype=hd res=7000.Default scripts:elink..



.*-----------------------------------------------------
:h2 res=1001 x=right y=bottom width=60% height=100% group=2.Erweitertes Kontextmen
:p.
Die Workplace Shell bietet die M”glichkeit Kontextmens zu erweitern. Dies passiert jedoch
nur fr jedes Objekt einzeln. An dieser Stelle kommt WPS-Wizard ins Spiel. Nach der 
Installation stehen zun„chst zwei Men-Ordner zur Verfgung, die Referenzen und Programm-Objekte
aufnehmen. Der Inhalt der Men-Ordner wird als Men-Eintr„ge in das Kontextmen aller Dateien bzw. 
Ordner eingeh„ngt.
:p.
:artwork align=center name='demo1.bmp'.
:p.
Darberhinaus k”nnen fr die verschiedenen Datei-Klassen (z.B. Icondateien, Textdateien usw.)
und Ordnerklassen (z.B. Laufwerke, Arbeitsoberfl„che) zus„tzliche Men-Ordner angelegt werden,
die Eintr„ge aufnehmen, die jeweils nur bei diesen Klassen erscheinen sollen.
:p.
N„heres findet sich unter 
:link reftype=hd res=20000.Men-Ordner.:elink.


.*-----------------------------------------------------
:h2 res=1004  x=right y=bottom width=60% height=100% group=2.Dateigr”áenanzeige
:p.
Ist diese Funktion aktiviert, so wird die Gr”áe einer Datei im Kontextmen angezeigt. Dies
vermeidet ein langwieriges ™ffnen des Einstellungen-Notizbuches mit anschlieáendem Durchklicken
zur entsprechenden Seite. So l„át sich rasch die Gr”áe zweier Dateien vergleichen oder absch„tzen,
ob eine Datei noch auf eine Diskette paát.
:p.
:artwork  align=center name='filesize.bmp'.
:p.
.*-----------------------------------------------------

.*-----------------------------------------------------
:h2 res=1006 x=right y=bottom width=60% height=100% group=2.Entfernen des Formatieren-Mens
:p.
Die Workplace Shell bietet direkten Zugriff auf die Formatieren-Funktion von Laufwerken. Was
bei Disketten durchaus sinnvoll ist, fhrt andererseits zu der nicht zu untersch„tzenden Gefahr
versehentlich auch eine Festplatte in einen vollst„ndig entleerten Zustand zu berfhren. 
:p.
:artwork  align=left name='format1.bmp'.
:p.
Durch Entfernen des Formatieren-Mens bei Festplatten hat man einen Alptraum weniger:
:p.
:artwork align=right name='format2.bmp'.
:p.
:note text='Anmerkung:'.
:p.Diese Funktion erfllt ihren Zweck bei lokalen Festplatten. Da ich keinen Zugriff auf
Wechselplattenlaufwerke habe, kann ich nicht sagen, ob und wenn ja in welcher Art und Weise
deren Kontextmen beeinfluát wird. Falls das Handling von Wechselplatten verbessserungswrdig ist,
bitte eine Mail an mich.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=1010 x=right y=bottom width=60% height=100% group=2.REXX Interface zur WPS
:p.
WPS-Wizard bietet ein REXX Interface zur WPS mit dem interne WPS Funktionen genutzt werden
k”nnen. Zu diesem Zweck erh„lt jeder Ordner der WPS ein Skriptmenu, von dem aus REXX-Skripte
gestartet werden k”nnen. Ab version 0.3.0 stehen REXX-Callbacks zur Verfgung mit deren Hilfe
WPS-Methodenaufrufe beeinfluát werden k”nnen. So kann mit einem REXX-Skript z.B. die WPS-Methode
wpMenuItemSelected() oder wpModifyPopupMenu() behandelt werden. Auf diese Weise kann der gleiche
Effekt erreicht werden wie beim Unterklassieren mit Hilfe von C oder C++ und dem SOM-Compiler.
Es kann z.B. mit einem REXX-Skript kontrolliert werden, welche Objekte auf ein bestimmtes Objekt
geworfen werden drfen. Oder es ist m”glich, die Mens einzelner oder aller Objekte zu modifizieren.

:p.
N„heres dazu unter :link reftype=hd res=300.REXX interface:elink. (englisch).
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=10200 x=right y=bottom width=60% height=100% group=2.Objekt-Info anzeigen
:p.
Hinweise, die nach l„ngerem Verweilen des Mauszeigers ber Objekten in einem Fenster erscheinen, lassen
sich ber die Auswahlbox im Konfigurations-Notizbuch aktivieren.
:p.
:artwork  align=center name='objecthint.bmp'.
:p.
Mehr Informationen, wie der angezeigte Text ge„ndert werden kann, finden sich im Kapitel
:link reftype=hd res=1010.REXX Interface zur WPS:elink.. Diese Funktion wurde mit V0.4.0 eingefhrt.

:nt.
Fr diese Objekthinweise muá die REXX-Untersttzung aktiviert sein.
:ent.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=10230 x=right y=bottom width=60% height=100% group=2.Werkzeugleiste fr Ordner
:p.
Eine Werkzeugleiste wird standardm„áig jedem Ordner hinzugefgt. Diese Leiste ist mit Rexx frei programmierbar.
:p.
:artwork  align=center name='toolbar.bmp'.
:p.
Seit Version 0.4.1 k”nnen Objekte mit der Maus entfernt oder hinzu gefgt werden. Das heiát auch
Nicht-Programmierer k”nnen jetzt mit Leichtigkeit die Werkzeugleiste anpassen. Eine Anleitung findet sich im Kapitel
:link reftype=hd res=22000.Werkzeugleiste:elink..
:p.
Informationen, wie diese Leiste mit Rexx programmiert werden kann, finden sich im Kapitel
:link reftype=hd res=1010.REXX Interface zur WPS:elink.. Diese Funktion wurde mit V0.4.0 eingefhrt.

:nt.
REXX-Untersttzung in WPS-Wizard muá aktiviert sein, um die Werkzeugleiste nutzen zu k”nnen.
:ent.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=10220 x=right y=bottom width=60% height=100% group=2.Informationsbereich in Ordnern
:p.
Der Informationsbereich zeigt Informationen ber den Ordner und das jeweilig selektierte Objekt an.
Verweise sind m”glich, die beim Anklicken eine Aktion durchfhren, die sich z.B. auf das selektierte Objekt bezieht.

:artwork  align=center name='infoarea.bmp'.
:p.
Informationen, wie der angezeigte Text ge„ndert werden kann, finden sich im Kapitel
:link reftype=hd res=1010.REXX Interface zur WPS:elink.. Diese Funktion wurde mit V0.4.0 eingefhrt.

:nt.
REXX-Untersttzung in WPS-Wizard muá aktiviert sein, um den Informationsbereich nutzen zu k”nnen.
:ent.
.*-----------------------------------------------------

.*-----------------------------------------------------
:h2 res=10240 x=right y=bottom width=60% height=100% group=2.Gadgets im Hintergrund der Arbeitsoberfl„che
:p.
Gadgets sind REXX-Skripte, die im Verborgenen arbeiten und auf dem Hintergrund der Arbeitsoberfl„che
z.B. Systeminformationen wie den verfgbaren freien Speicher anzeigen. Die normale Funktion der
Arbeitsoberfl„che wird dabei nicht beeinfluát, so ist es etwa m”glich Objekte auch direkt auf einem
Gadget abzulegen (genauer auf dem Bereich mit der Gadget-Ausgabe). 
:p.
:artwork  align=center name='gadget.bmp'.
:p.
Gadgets werden in Rexx programmiert. N„heres findet sich unter
:link reftype=hd res=1010.REXX Interface zur WPS:elink.. Diese Funktion wurde mit V0.5.0 eingefhrt.

.*-----------------------------------------------------

.***************************

:h1  res=3000 x=left y=bottom width=100% height=100% group=1.Installation
:p.
Die Installation von WPS-Wizard wird durch Aufruf des Installationsprogramms durchgefhrt.


.*----------------------- +++ h1 +++ ------------------------------
:h1 res=1007 x=left y=bottom width=40% height=100% group=1.Konfigurations-Notizbuch
:link reftype=hd res=10072
auto dependent group=2.

:p.
šber das Konfigurations-Notizbuch im System-Ordner lassen sich alle Funktionen von WPS-Wizard unabh„ngig
voneinander ein- bzw. ausschalten.
:p.
Diese Einstellungen wirken global. Fr :link reftype=hd res=1001.Kontextmen-Erweiterungen :elink.
besteht allerdings die
M”glichkeit, die Erweiterung fr einzelne Datei- oder Ordner-Klassen einzeln zu aktivieren
oder zu deaktivieren. Dieses geschieht ber das Einstellungen-Notizbuch des entsprechenden
Men-Ordners.
:sl.

:li.
:link reftype=hd res=102
vpx=right
vpy=bottom
vpcx=60%
vpcy=50%.REXX-Skripte aktivieren:elink.

:li.
:link reftype=hd res=105
vpx=right
vpy=bottom
vpcx=60%
vpcy=50%.Objekt-Info anzeigen:elink.


:li.
:link reftype=hd res=101
vpx=right
vpy=bottom
vpcx=60%
vpcy=50%.Dateigr”áe anzeigen:elink.

:li.:link reftype=hd res=103
vpx=right
vpy=bottom
vpcx=60%
vpcy=50%.Kein Formatieren-Men:elink.

:li.:link reftype=hd res=104
vpx=right
vpy=bottom
vpcx=60%
vpcy=50%.Erweitertes Kontextmen:elink.
:esl.

.*-----------------------------------------------------
:h2 hide res=10072 x=right y=bottom width=60% height=100% group=2.Konfigurations-Notizbuch Bild
:artwork  align=center name='configuration.bmp'.
:p.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=102 group=3.REXX-Skripte aktivieren
:p.
WPS-Wizard l„át sich an vielen Stellen mit REXX-Skripten erweitern. So wird Ordnern zum Beispiel
ber ein Skript eine Werkzeugleiste hinzugefgt. Auch die Objekthinweise sind ber REXX-Skripte
realisiert.
:p.
šber dieses Feld lassen sich diese REXX-Erweiterungen global deaktivieren.
:nt.
Userskripte, die zu Ordnermens hinzugefgt wurden, sind hiervon nicht betroffen.
:ent.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=105 group=3.Objekt-Info anzeigen
:p.
Hinweise, die nach l„ngerem Verweilen des Mauszeigers ber Objekten in einem Fenster erscheinen, lassen
sich ber die Auswahlbox im Konfigurations-Notizbuch aktivieren.
:p.
:artwork  align=center name='objecthint.bmp'.
:p.
:nt.
Fr diese Objekthinweise muá die REXX-Untersttzung aktiviert sein.
:ent.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=101 group=3.Dateigr”áe anzeigen
:p.
Ist dieses Feld aktiviert, so wird die Gr”áe einer Datei im Kontextmen angezeigt.

:h2 res=103 group=2.Kein Formatieren-Men
:p.
Durch Aktivieren dieses Feldes wird das Formatieren-Men aus dem Kontextmen von Festplatten
entfernt. Auf diese Weise ist ein versehentliches Formatieren nicht mehr m”glich.
Disketten-Laufwerke sind von dieser Einstellung nicht betroffen.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=104 group=3.Erweitertes Kontextmen
:p.
šber dieses Feld lassen sich die zus„tzlichen Men-Eintr„ge, die Menu-Wizard mit Hilfe
der Men-Ordner zur Verfgung 
stellt, global ein- bzw. ausschalten. Soll lediglich das Kontextmen fr eine bestimmte
:link reftype=hd res=003.Klasse:elink. beeinfluát werden, so kann dieses ber das
Einstellungen-Notizbuch des zugeh”rigen Men-Ordners durchgefhrt werden.
.*-----------------------------------------------------


.*--------------------- +++ h1 +++ --------------------------------
:h1 res=20000 x=left y=bottom width=50% height=100% group=1 clear.Men-Ordner (Kontextmen konfigurieren) 
:p.
Nach der Installation von WPS-Wizard finden sich im WPS-Wizard Installationsordner
zwei neue Ordner :hp2.Kontextmen fr Ordner:ehp2. und :hp2.Kontextmen fr Datendatei:ehp2..
Diese Men-Ordner nehmen Referenzen und Programmobjekt auf, die anschlieáend in den jeweiligen
Kontext-Mens erscheinen.
Die Men-Eintr„ge lassen sich in weiten Grenzen konfigurieren. Darberhinaus stehen erweiterte
M”glichkeiten wie die Erstellung von speziellen Mens fr einzelne Klassen zur Verfgung.
:sl.
:li.:link reftype=hd res=2001.Konfiguration des Mens:elink. 
:li.:link reftype=hd res=2002.Erstellung von Untermens:elink.
:li.:link reftype=hd res=2003.Aktivierung der Men-Erweiterung:elink.
:li.:link reftype=hd res=2004.Erstellung von Mens fr einzelne Klassen:elink.
:li.:link reftype=hd res=2005.Einstellungen fr Programmobjekte:elink.
:esl.
.*-----------------------------------------------------


.*----------------------------------------------------- 
:h2  res=2001 x=right y=bottom width=50% height=100% group=2.Konfiguration des Mens
:p.
Die Konfiguration eines Kontextmens geschieht ber Drag and Drop. Das Objekt, welches im
Men erscheinen soll, wird einfach in den entsprechenden Men-Ordner gezogen. Es wird
automatisch eine Referenz erstellt. Daneben ist es m”glich, auch Programmobjekte in einen
Men-Ordner zu legen. Hierfr ist beim Verschieben die Shift-Taste zu drcken. Diese
Programmobjekte k”nnen spezielle Einstellungen erhalten, die ihr Startverhalten bestimmen.
N„heres unter :link reftype=hd res=2005.Einstellungen fr Programmobjekte:elink..

:note text='Anmerkung:'.
Die Anzahl von Meneintr„gen ist unbegrenzt.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=2002 x=right y=bottom width=50% height=100% group=2.Erstellung von Untermens
:p.
Ein Untermen l„át sich erstellen, indem eine Men-Schablone aus dem Schablonenordner
in den Men-Ordner gezogen wird.
.br
Es sind beliebig tief verschachtelte Mens m”glich.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=2003 x=right y=bottom width=50% height=100% group=2.Aktivierung der Men-Erweiterung
:p.
Jeder einzelne Men-Ordner l„át sich ein- und ausschalten. Die erste Seite des 
Einstellungen-Notizbuches bietet hierfr eine Checkbox :hp1.Men aktivieren:ehp1. an.
.br
šber das :link reftype=hd res=1007.Konfigurations-Notizbuch:elink. von WPS-Wizard lassen sich 
darberhinaus alle Men-Ordner global aktivieren bzw. deaktivieren.
:artwork  align=center name='classchoose.bmp'.  
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=2004 x=right y=bottom width=50% height=100% group=2.
Erstellung von Mens fr einzelne Klassen
:p.
Neben den beiden bei der Installation eingerichteten Men-Ordner fr Dateien und Ordner lassen
sich weitere Men-Ordner fr einzelne Datei- und Ordner-Klassen einrichten. Auf diese
Weise ist es m”glich spezielle Kontextmens fr z.B. Icon-Dateien und Programm-Dateien zu
erstellen. Der Inhalt dieser Ordner wird zus„tzlich in das Men eingeh„ngt.
:p.
Ein neuer Men-Ordner wird durch Ziehen einer Men-Schablone z.B. in den
System-Konfigurationsordner erstellt. Die erste Seite des Einstellungen-Notizbuches
bietet ein Feld, auf den ein Objekt fallengelassen werden kann. Der :link reftype=hd res=003.
Klasse:elink., der dieses
Objekt angeh”rt, wird damit dieser Ordner zugeordnet. In dem Feld wird das Icon dieser
Klasse angezeigt. Der Name erscheint darunter.

:note text='Anmerkung:'.Eine Zuordnung l„át sich nachtr„glich nicht „ndern. Soll eine Zuordnung
aufgehoben werden, so muá der Men-Ordner gel”scht werden.  
:p.
:artwork  align=center name='classchoose.bmp'.  
:p.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=2005 x=right y=bottom width=50% height=100% group=2.Einstellungen fr Programmobjekte
:p.
Einem Programmobjekt kann der Name und der Pfad des Objektes bergeben werden, dessen 
Kontextmen gew„hlt wurde.
.br
Dies geschieht, indem auf der Programmseite unter :hp1.Parameter:ehp1. :hp9.%*:ehp9. eingetragen
wird.
Dieser spezielle Parameter darf an beliebiger Stelle des Parameter-Strings auftauchen und
wird bei Aufruf des Programmobjekts durch den Pfad und gegebenenfalls Dateinamen des Objekts
ersetzt.
.br
Auch das Arbeitsverzeichnis kann angepaát werden. Wird unter :hp1.Arbeitsverzeichnis:ehp1.
nichts eingetragen (auch kein Leerzeichen), so wird beim Programmstart der Pfad des Objekts
verwendet.
.*-----------------------------------------------------


.*---------------------- +++ h1 +++ -------------------------------
:h1 res=22000 x=left y=bottom width=40% height=100% group=1 clear.Werkzeugleiste 
:link reftype=hd res=22001
auto dependent group=2.

:p.
Die Werkzeugleiste fr Ordner ist mit Rexx frei programmierbar. So lassen sich beliebige
Startbereiche (Launchpad) fr Objekte anlegen, Eingabefelder programmieren usw.
:p.
Die Standardleiste l„át sich jedoch auch ohne Programmierkenntnisse ver„ndern. Dabei
werden Objekte einfach mit der Maus auf die Leiste gezogen.

:sl.
:li.:link reftype=hd res=22001.Ordner-Aktionen ver„ndern:elink. 
:li.:link reftype=hd res=22002.Allgemeiner Startbereich:elink.
:li.:link reftype=hd res=22003.Objekte entfernen:elink.
.*:li.:link reftype=hd res=22004.Erstellung von Mens fr einzelne Klassen:elink.
.*:li.:link reftype=hd res=22005.Einstellungen fr Programmobjekte:elink.
:esl.
.*---------------------------------------------------------


.*----------------------------------------------------- 
:h2  res=22001 x=right y=bottom width=60% height=100% group=2.Ordner-Aktionen ver„ndern
:p.
Ordner-Aktionen werden durch Anklicken eines Knopfes im linken Bereich der Werkzeugleiste
ausgel”st. Sie wirken auf den jeweiligen Ordner oder auch (je nach Aktionsobjekt) auf dessen Inhalt.
So ist es m”glich, den Stammordner zu ”ffnen, den Inhalt zu aktualisieren und vieles mehr.
:p.
Die Standardinstallation kommt mit einer Reihe von Aktionsobjekten, die einfach auf den Startbereich
gezogen werden. Es ist m”glich fr spezielle Anwendungen weitere Aktionsobjekte zu erstellen.
Ein derartiges Objekt besteht aus einem Rexx-Skript, welches beim Anklicken gestartet wird.
 
:p.
:artwork  align=center name='tbconfigure.bmp'.
:p.
Um ein abgelegtes Objekt zu entfernen, ist im Kontextmen des Objekts der Menpunkt :hp2.L”schen:ehp2.
auszuw„hlen. Es wird lediglich das Objekt vom Startbereich gel”scht, jedoch nicht das Original.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22002 x=right y=bottom width=60% height=100% group=2.Allgemeiner Startbereich
:p.
Im rechten Bereich der Werkzeugleiste findet sich ein allgemeiner Startbereich. Hier lassen
sich beliebige Objekte ablegen. Dieser Bereich verh„lt sich wie die Klickstartleiste von OS/2. 
:p.
Um ein abgelegtes Objekt zu entfernen, ist im Kontextmen des Objekts der Menpunkt :hp2.L”schen:ehp2.
auszuw„hlen. Es wird lediglich das Objekt vom Startbereich gel”scht, jedoch nicht das Original.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22003 x=right y=bottom width=60% height=100% group=2.Objekte entfernen
:p.
Um ein abgelegtes Objekt zu entfernen, ist im Kontextmen des Objekts der Menpunkt :hp2.L”schen:ehp2.
auszuw„hlen. Es wird lediglich das Objekt vom Startbereich gel”scht, jedoch nicht das Original.
.*-----------------------------------------------------


.*---------------------- +++ h1 +++ -------------------------------
:h1 res=22500 x=left y=bottom width=40% height=100% group=1 clear.Gadgets 
:link reftype=hd res=22501
auto dependent group=2.


:p.
Gadgets sind REXX-Skripte die im Hintergrund abgearbeitet werden und ihre Ausgabe auf dem
Hintergrund der Arbeitsoberfl„che zeichnen.
:nt.
Gadgets werden nur ausgefhrt, wenn die WPS l„uft.
:ent.

:sl.
:li.:link reftype=hd res=22501.Gadgets starten:elink. 
:li.:link reftype=hd res=22502.Aktivierung eines Gadgets:elink.
:li.:link reftype=hd res=22503.Gadgets beenden:elink.
:li.:link reftype=hd res=22504.Anpassung eines Gadgets:elink.
:esl.
:p.
Die folgenden Gadgets werden z.Z: standardm„áig installiert:

:sl.
:li.:link reftype=hd res=22510.System-Information:elink. 
:li.:link reftype=hd res=22511.Laufwerksinformation:elink.
:li.:link reftype=hd res=22512.z!-info:elink.
:li.:link reftype=hd res=22513.Zeitanzeige:elink.
:li.:link reftype=hd res=22514.Kalender:elink. (:link reftype=hd res=22599.*:elink.)
:li.:link reftype=hd res=22515.Datei-šberwachung:elink. (:link reftype=hd res=22599.*:elink.)
:li.:link reftype=hd res=22516.Bin„re Uhr:elink. (:link reftype=hd res=22599.*:elink.)
:esl.
:p.
Wie neue Gadgets erstellt werden k”nnen, wird im Kapitel :link reftype=hd res=30000.Gadget programming:elink. beschrieben. 

.*---------------------------------------------------------


.*-----------------------------------------------------
:h2  res=22501 x=right y=bottom width=60% height=100% group=2.Gadgets starten
:p.
Gadgets werden einfach durch Doppelklick gestartet. Eine andere M”glichkeit ist
der Aufruf des REXX-Skriptes ber die Kommandozeile, wie jedes andere Skript auch.
Zu beachten ist, daá die WPS gestartet sein muá, damit Gadgets funktionieren.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22502 x=right y=bottom width=60% height=100% group=2.Aktivierung eines Gadgets
:p.
Ein Gadget ist im Normalzustand nicht erreichbar. Es taucht nicht in der Fensterliste auf
und hat kein sichtbares Fenster. Um mit ihm zu interagieren muá es zun„chst aktiviert
werden. Dies geschieht indem beim Klicken des Gadgets auf der Arbeitsoberfl„che die STRG-Taste
gedrckt gehalten wird. Es erscheint daraufhin das Gadgetfenster.
:p.
Umgekehrt wird ein Gadget wieder deaktiviert, d.h. in den Hintergrund zurck geschickt, indem
wiederum bei gedrckter STRG-Taste auf das Gadget geklickt wird.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22503 x=right y=bottom width=60% height=100% group=2.Gadgets beenden
:p.
Zum Beenden eines Gadgets wird es zun„chst :link reftype=hd res=22502.aktiviert:elink. und anschlieáend
aus dem Kontextmen der Punkt :hp2.Beenden:ehp2. bzw. :hp2.Close:ehp2. ausgew„hlt.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22504 x=right y=bottom width=60% height=100% group=2.Anpassung eines Gadgets
:p.
Bei vielen Gadgets lassen sich Farbe, Zeichensatz oder Gr”áe anpassen. Nach dem :link reftype=hd res=22502.Aktivieren:elink.
die gewnschte Farbe von der Farbpalette auf das Gadget ziehen. Gleiches gilt fr den
Zeichensatz. Bei Gadgets, die aus mehreren Objekten bestehen (z.B. Systeminformation oder
Laufwerksinformation), kann es erforderlich sein, die Einzelobjekte zun„chst freizuschalten.
Dies wird durch den Aufruf des entsprechenden Menpunkts durchgefhrt. 
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22510 x=right y=bottom width=60% height=100% group=2.Gadget: System-Information
:p.
Diese Gadget zeigt einige Systemwerte an:

:ul.
:li.Verfgbarer physikalischer Speicher
:li.Platz auf dem Boot-Laufwerk
:li.Bisherige Laufzeit des Systems (uptime)
:li.Aktuelle Zeit und Datum
:eul.
:p.
:artwork  align=center name='sysinfo.bmp'.

.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22511 x=right y=bottom width=60% height=100% group=2.Gadget: Laufwerksinformation
:p.
Dieses Gadget zeigt den freien Speicherplatz auf allen Festplatten des Systems an.
:p.
:artwork  align=center name='gadget.bmp'.

.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22512 x=right y=bottom width=60% height=100% group=2.Gadget: z!-Info
:p.
Anzeige des Titels, der gerade von z! gespielt wird.
:p.
:artwork  align=center name='zinfo.bmp'.

.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22513 x=right y=bottom width=60% height=100% group=2.Gadget: Zeitanzeige
:p.
Anzeige der aktuellen Systemzeit und des Datums.
:p.
:artwork  align=center name='timegadget.bmp'.

.*-----------------------------------------------------



.*-----------------------------------------------------
:h2  res=22514 x=right y=bottom width=60% height=100% group=2.Gadget: Kalender
:p.
Anzeige des aktuellen Monats mit hervorgehobenem Datum. šber das Men k”nnen andere
Monate ausgew„hlt werden.
:p.
:artwork  align=center name='calendargadget.bmp'.
:p.
Diese Gadget wurde von Goran Ivankovic zur Verfgung gestellt.
:p.
Weitere ntzliche Programme von G. Ivankovic finden sich hier:
:p.
http&colon.//www.os2world.com/goran/
:p.

.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22515 x=right y=bottom width=60% height=100% group=2.Gadget: file monitor
:p.
šberwachung von Dateien auf Ver„nderungen. Es kann eine Liste mit Dateien bergeben
werden, die regelm„áig berprft werden sollen. Das Men hat eine Hilfefunktion fr
n„here Erkl„rungen.
:p.
:artwork  align=center name='filemonitorgadget.bmp'.
:p.
Diese Gadget wurde von Goran Ivankovic zur Verfgung gestellt.
:p.
Weitere ntzliche Programme von G. Ivankovic finden sich hier:
:p.
http&colon.//www.os2world.com/goran/
:p.

.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22516 x=right y=bottom width=60% height=100% group=2.Gadget: Bin„re Uhr
:p.
Eine bin„re Uhr fr die Arbeitsoberfl„che. Sollte dieses Gadget R„tsel aufgeben, so
ist es wahrscheinlich nicht fr Sie gedacht...
:p.
:artwork  align=center name='binaryclockgadget.bmp'.
:p.
Diese Gadget wurde von Goran Ivankovic zur Verfgung gestellt.
:p.
Weitere ntzliche Programme von G. Ivankovic finden sich hier:
:p.
http&colon.//www.os2world.com/goran/
:p.

.*-----------------------------------------------------


.*-----------------------------------------------------
:h2  res=22599 x=right y=bottom width=60% height=40% group=3 hide.(*)
:p.
Dieses Gadget wurde von Goran Ivankovic zur Verfgung gestellt.
:p.
Weitere ntzliche Programme von G. Ivankovic finden sich hier:
:p.
http&colon.//www.os2world.com/goran/




.*****************************************


.*-------------------- +++ h1 +++ ---------------------------------
:h1.Begriffserkl„rungen
:p.
In diesem Kapitel werden einige h„ufig wiederkehrende Begriffe bezglich der Workplace Shell
von OS/2 erl„utert.


.*-----------------------------------------------------
:h2 group=2.Workplace Shell
:p.
Die Workplace Shell ist die graphische Benutzeroberfl„che von OS/2.
Sie ist derart leistungsf„hig und konfigurierbar, daá selbst der erfahrene Benutzer
stets neue Eigenschaften entdeckt. Durch ihr durchgehend objektorientiertes Design lassen sich
Programme schreiben,die die schon vorhandenen F„higkeiten erweitern und sich 
dabei nahtlos in die Oberfl„che einfgen. Diese Programme sind kein Aufsatz auf die Oberfl„che
sondern werden ein Teil von ihr.
.*-----------------------------------------------------


.*-----------------------------------------------------
:h2 res=003 group=2.Klasse
:p.
Jedes Objekt (im allgemeinen durch ein Icon repr„sentiert) der Workplace Shell von OS/2 geh”rt
zu einer Klasse. Alle Objekte einer Klasse haben die gleichen Eigenschaften, z.B. die gleichen
Seiten im Einstellungen-Notizbuch. Programmierer haben die M”glichkeit, eine beliebige Klasse der
WPS als Grundlage fr eine neue Klasse zu nehmen. Diese neue abgeleitete Klasse erbt
s„mtliche Eigenschaften der Ursprungsklasse und kann zus„tzliche Eigenschaften einfhren.
Die Men-Ordner von WPS-Wizard sind z.B. von der Standard Ordnerklasse abgeleitet. Als neue
Eigenschaft akzeptieren sie jedoch ausschlieálich Referenzen und Programm-Objecte als Inhalt.
:p.
Darberhinaus ist es m”glich, Klassen der WPS durch eigene Klassen mit speziellen Eigenschaften
zu ersetzen. WPS-Wizard ersetzt die Ordner- und die Datendatei-Klasse durch eigene Klassen mit
der speziellen Eigenschaft, zus„tzliche Eintr„ge im Kontextmen anzuzeigen. Durch die Ersetzung
der Klasse haben automatisch alle Objekt dieser Klasse die neuen Eigenschaften.
.*-----------------------------------------------------


.*****************************************


.*-------------------- +++ h1 +++ ---------------------------------
:h1.Fehler und Einschr„nkungen
:p.
Folgende Einschr„nkungen mssen in Kauf genommen werden:
:sl.
:li.Die Reihenfolge der Men-Eintr„ge l„át sich zur Zeit nicht festlegen.
:li.Werden bei der Auswahl einer Klasse fr einen Men-Ordner Referenzen auf dem Auswahlfeld
fallengelassen, so wird dieser Ordner der Referenz zugeordnet, nicht jedoch dem referenzierten
Objekt. 
:esl.
:p.
:p.
Fehler:
:p.
(Eventuelle Bugs sind selbstverst„ndlich als Feature zu begreifen &colon.-) )
:p.
:hp8.Zur Zeit sind mir keine Fehler bekannt, was jedoch nicht heiát, daá WPS-Wizard fehlerfrei ist.
Wie bei jeder Software ist mit Fehlfunktionen zu rechnen. Daher ist durch regelm„áiges Backup aller
Daten Vorsorge zu treffen!:ehp8.
:p.


.*-------------------- +++ h1 +++ ---------------------------------
:h1.Versionen

:p.
V0.5.2 (06.Apr.2007):
:sl.
:ul compact.
:li.New CWObject method 'wizQueryRexxScript()'.
:li.Fixed a possible memory leak with folder info scripts.
:li.Sometimes carriage returns in object names truncated flyover help for launchpad buttons.
:li.Iconsize may be increased. Esp. useful for folders with image files.
:li.Check for broken SET HOME= statement in config.sys
:li.Fixed crash when 'cwCreateLaunchArea' was called with a WPFolder instead of CWLaunchArea object.
:li.Improved REXX error reporting.
:li.Image data is deleted when scrolled out of sight in a container. This prevents memory exhausting when a
 folder contains several hundred or thousand image files. CWMM-classes V2.9.0 or above must be installed.
:li.You may TAB from the folder toolbar to the container. Tabbing to the toolbar or between toolbars isn't yet implemented.
:li.ONTIMER in gadgets was called without the timer ID
:li.Changed installation script to always create new toolbar folders ('replace' instead of 'update')
:li.Added check for timer ID value in winStartTimer() and winStopTimer()
:li.Folder information area is using the Innotek font engine for text drawing.
:li.Added support for icon theming in the WPS. You may use PNG icons now. You need a separate icon addon.
:eul.
:esl.


:p.
V0.5.1 (12.Mar.2004):
:sl.
:ul compact.
:li.Added IMAGE_GADGET
:li.New context menu for folder action buttons
:li.New menu function menuEnableItem()
:li.More german translations of hints
:li.Size of infowindow wasn't always saved
:li.Bar showing the free drive space for the Icons in drives folder 
:li.The image contained in an image file is drawn as the icon (if the cw-multimedia classes are installed). 
:li.Format menu is properly removed for USB floppy drives now.
:li.Some more icons by Klaus Staedtler
:li.Gadget variable <system.path.wpswizardhints> pointed to INI file not hint database.
:li.The INI file WPSWIZZ.INI is now created in the users home directory if defined.
:eul.
:esl.

:p.
V0.5.0 (02.Jan.2004):
:sl.
:ul compact.
:li.Added Gadgets (small scripts running in the background and drawing their
output on the desktop).
:li.Added a script tutorial section to the documentation.
:li.Some minor fixes to the install and uninstall scripts.
:eul.
:esl.

:p.
V0.4.1 (21.Nov.2003):
:sl.
:ul compact.
:li.Info area text formatting rewritten for better tag and font handling. It may be necessary to adjust your info scripts.
:li.Better font handling for the info area
:li.When REXX/info area are disabled the corresponding menuitems are removed.
:li.Fixed a couple of minor memory leaks
:li.New generic WPS class implementing a general purpose launch area.
:li.Launch area for toolbar. Drop any object there and use it like the LaunchPad.
:li.Toolbar buttons may be configured using drag and drop.
:li.Better error reporting for Rexx scripts.
:li.New function 'winShowWindow()'
:li.Fixed broken transparency with SDD and SNAP
:eul.
:esl.

:p.
V0.4.0 (21.Jun.2003)&colon.
:sl.
:ul compact.
:li.Corrected some typos in the manual
:li.Fixed some memory leaks
:li.Better script handling. Different kind of scripts have their own subdir.
:li.New callback for wpModifyMenu
:li.New function winQueryFolderObjects()
:li.Info area in folders.
:li.Toolbar may be disabled independently for any folder.
:li.Some code cleanups
:eul.
:esl.

:p.
V0.3.0 (10.Jan.2003)&colon.
:sl.
:ul compact.
:li.Speicherung der Scrollbarposition von Ordnern in Detailansicht war nicht immer m”glich.
:li.L”schen des 'Nur Lesen'-Attributs fr Dateien kopiert von CD.
:li.Das Userscripts-Men darf jetzt Unterordner enthalten.
:li.Untersttzung fr wpSetType(), wpQueryFldrSortClass(), wpQueryFldrDetailsClass(),
 wpSetFldrDetailsClass(), wpSetFldrSortClass() hinzugefgt.
:li.Men- und Drag'n Drop-Funktionen zur REXX-Schnittstelle hinzugefgt.
:li.WPS-Callback-Schnittstelle fr REXX-Prozeduren hinzugefgt.
:li.Objekt-Hilfe (Flyover help) hinzugefgt.
:eul.
:esl.

:p.
V0.2.0 (8.M„rz.2002)&colon.
:sl.
:ul compact.
:li.Position des Splitbars in der Detailansicht von Ordnern wird nun gespeichert.
:li.Scrollbarposition von Ordnern in der Detailansicht wird nun gespeichert.
:li.REXX interface zur WPS hinzugefgt.
:li.Schablonen Checkbox war nicht in den Einstellungen.
:li.Einige kleine Speicherlecks beseitigt.
:eul.
:esl.

:p.
V0.1.2 (13.Mai.2001)&colon.
:sl.
:ul compact.
:li.Parameter von Programmobjekten wurden nicht richtig verarbeitet.
:li.Das Entfernen der Tilde aus Titeltexten war fehlerhaft.
:eul.
:esl.
:p.
V0.1.0 (05.Mai.2001)&colon.
:sl.
:ul compact.
:li.Erste ”ffentliche Version.

:eul.
:esl.

.***************************************

.im rexxinterface.inc

.im gadget.inc

.im programminghowto.inc

.im defaultscripts.inc

.im hiddenfeatures.inc

.im setupstrings.inc

.im hintdatabase.inc

.im wpsclasses.inc

:euserdoc.




