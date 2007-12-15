:userdoc.

:docprof.

:title. Hilfe fr Men-Ordner

:h1 res=001.WPS-Wizard
:p.
WPS-Wizard stellt eine neue Ordner-Klasse bereit, die Referenzen und 
Programm-Objekte aufnimmt. Es stehen standardm„áig je ein Men-Ordner fr Dateien und fr
Ordner zur Verfgung. Der Inhalt eines solchen Men-Ordners erscheint im Kontext-Men
einer Datei bzw. eines Ordners. Zus„tzlich kann fr jede abgeleitete Klasse
ein weiterer Ordner angelegt werden. So ist es z.B. m”glich, fr Icon-Dateien und fr
Programm-Dateien unterschiedliche zus„tzliche Men-Eintr„ge vorzusehen.
:p.
N„heres findet sich in der 
:link reftype=launch
object='view.exe'
data='menuwizz.inf'.
Einfhrung:elink.
zu WPS-Wizard.
.*

:h2 res=003.Klasse
:p.
Jedes Objekt (im allgemeinen durch ein Icon repr„sentiert) der Workplace Shell von OS/2 geh”rt
zu einer Klasse. Alle Objekte einer Klasse haben die gleichen Eigenschaften, z.B. die gleichen
Seiten im Einstellungen-Notizbuch. Programmierer haben die M”glichkeit, eine beliebige Klasse der
WPS als Grundlage fr eine neue Klasse zu nehmen. Diese neue abgeleitete Klasse erbt
s„mtliche Eigenschaften der Ursprungsklasse und kann zus„tzliche Eigenschaften einfhren.
Die Men-Ordner von WPS-Wizard sind z.B. von der Standard Ordnerklasse abgeleitet. Als neue
Eigenschaft akzeptieren sie jedoch ausschlieálich Referenzen und Programm-Objekte als Inhalt.
:p.
Darberhinaus ist es m”glich, Klassen der WPS durch eigene Klassen mit speziellen Eigenschaften
zu ersetzen. WPS-Wizard ersetzt die Ordner- und die Datendatei-Klasse durch eigene Klassen mit
der speziellen Eigenschaft, zus„tzliche Eintr„ge im Kontextmen anzuzeigen. Durch die Ersetzung
der Klasse haben automatisch alle Objekt dieser Klasse die neuen Eigenschaften.

:h2 res=100.WPS-Wizard-Optionen
:p.
Durch Ausw„hlen der entsprechenden Felder lassen sich die einzelnen Features von
WPS-Wizard ein- bzw. ausschalten.
:p.
Eine Beschreibung der Felder l„át sich durch Anklicken der folgenden Punkte anzeigen:
:sl.
:li.:link reftype=hd res=102.REXX-Skripte aktivieren:elink.
:li.:link reftype=hd res=105.Objekt-Info anzeigen:elink.
:li.:link reftype=hd res=101.Dateigr”áe anzeigen:elink.
:li.:link reftype=hd res=103.Kein Formatieren-Men:elink.
:li.:link reftype=hd res=104.Erweitertes Kontextmen:elink.
:esl.

:h3 res=102.REXX-Skripte aktivieren
:p.
WPS-Wizard l„át sich an vielen Stellen mit REXX-Skripten erweitern. So wird Ordnern zum Beispiel
ber ein Skript eine Werkzeugleiste hinzugefgt. Auch die Objekthinweise sind ber REXX-Skripte
realisiert.
:p.
šber dieses Feld lassen sich diese REXX-Erweiterungen global deaktivieren.
:nt.
Userskripte, die zu Ordnermens hinzugefgt wurden, sind hiervon nicht betroffen.
:ent.

:h3 res=105.Objekt-Info anzeigen
:p.
Hinweise, die nach l„ngerem Verweilen des Mauszeigers ber Objekten in einem Fenster erscheinen, lassen
sich ber diese Feld aktivieren.
:p.
:artwork  align=center name='objecthint.bmp'.
:p.
:nt.
Fr diese Objekthinweise muá die REXX-Untersttzung aktiviert sein.
:ent.

:h3 res=101.Dateigr”áe anzeigen
:p.
Ist dieses Feld aktiviert, so wird die Gr”áe einer Datei im Kontextmen angezeigt.
 

:h3 res=103.Kein Formatieren-Men
:p.
Durch Aktivieren dieses Feldes wird das Formatieren-Men aus dem Kontextmen von Festplatten
entfernt. Auf diese Weise ist ein versehentliches Formatieren nicht mehr m”glich.
Disketten-Laufwerke sind von dieser Einstellung nicht betroffen.

:h3 res=104.Erweitertes Kontextmen
:p.
šber dieses Feld lassen sich die zus„tzlichen Men-Eintr„ge, die WPS-Wizard mit Hilfe
der Men-Ordner zur Verfgung 
stellt, global ein- bzw. ausschalten. Soll lediglich das Kontextmen fr eine bestimmte
:link reftype=hd res=003.Klasse:elink. beeinfluát werden, so kann dieses ber das
Einstellungen-Notizbuch des zugeh”rigen Men-Ordners durchgefhrt werden.

:h1 res=201.Men-Ordner
.*Allgemeine Hilfe fr Men-Ordner
:p.
:artwork runin name='menu.bmp'.
:p.
Ein Men-Ordner nimmt Referenzen beliebiger Objekte und Programm-Objekte auf. Diese Erscheinen
als Eintr„ge im Kontextmen der :link reftype=hd res=003.Klasse:elink. der dieser Men-Ordner
zugeordnet ist. Jeder Klasse kann genau ein Men-Ordner zugeordnet werden. Zus„tzlich steht
je ein globaler Men-Ordner fr Datendateien und Ordner zur Verfgung.

:h2 res=002.Klassenauswahl
:p.
Um einer :link reftype=hd res=003.Klasse:elink. einen Men-Ordner zuzuordnen, reicht es,
ein Objekt dieser Klasse auf das freie
Feld fallen zu lassen. Daraufhin wird das zugeh”rige Icon angezeigt. Die Zuordnung kann
durch Drcken des Widerrufen-Knopfes oder durch das erneute Fallenlassen einer anderen
Klasse aufgehoben werden. Beim Schlieáen des Einstellungen-Notizbuches wird die gewnschte
Zuordnung dauerhaft gespeichert. Es ist je Klasse nur ein Men-Ordner m”glich (zus„tzlich
zum standardm„áig vorhandenen).

:note text='Anmerkung:'.Eine Zuordnung l„át sich nachtr„glich nicht „ndern. Soll eine Zuordnung
aufgehoben werden, so muá der Men-Ordner gel”scht werden.
:p.
Durch Anklicken der Checkbox :hp2.Men aktivieren:ehp2. lassen sich die zus„tzlichen
Meneintr„ge aktivieren bzw. deaktivieren.

:h1 res=202.WPS-Wizard Konfiguration
.*Allgemeine Hilfe fr Konfigurations-Notebook von WPS-Wizard
:p.
:artwork runin name='menuConfig.bmp'.
:p.
Dieses Objekt dient der Konfiguration von
:link reftype=hd res=001. WPS-Wizard:elink..
Nach einem Doppelklick erscheint das Einstellungen-Notizbuch, in dem die einzelnen Features
ein- bzw. ausgeschaltet werden.

:euserdoc.




