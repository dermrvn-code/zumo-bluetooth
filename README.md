[![wakatime](https://wakatime.com/badge/github/dermrvn-code/bobo-bluetooth.svg)](https://wakatime.com/badge/github/dermrvn-code/bobo-bluetooth)

# Zumo 32U4 Bluetooth

Dieses Projekt integriert die Steuerung über Bluetooth. Der Code in diesem Projekt steht in direkter Verbindung zu einer dafür erstellten **Unity App** ([*Zumo-Bluetooth-App*](https://github.com/dermrvn-code/zumo-bluetooth-app)).

Da der Pololu 32U4 ohne Bluetooth-Antenne kommt, muss diese separat angebaut werden. Details dazu gibt es unter [Aufbau und Verkabelung](#aufbau-und-verkabelung).

Die App integriert die Möglichkeit die Motoren zu steuern und die Sensordaten des Zumos auszulesen.

Natürlich kann dieser Code beliebig um weitere Funktionen erweitert werden.

## Inhaltsverzeichnis

1. [Aufbau und Verkabelung](#aufbau-und-verkabelung)

2. [Installation](#installation)

3. [Codeübersicht](#codeübersicht)

4. [Code erweitern](#code-erweitern)

5. [Schlusswort](#schlusswort)

## Aufbau und Verkabelung

![Verkabelung](https://raw.githubusercontent.com/dermrvn-code/zumo-bluetooth/master/circuit.png)
  
<sub>*Verschaltung von Zumo, Levelshifter, und Bluetoothmodul*</sub>

Der Code definiert die Pins 13 und 14 als RX bzw. TX Pins. Diese Pins sind zu erreichen, wenn man das Display des Zumos absteckt.

  

![Pinout des Zumos](https://a.pololu-files.com/picture/0J6286.1200.jpg?177f62fab7018b9ff5f2898d27f91d61)
  
<sub>*Das Pinout der Oberseite des Zumos*</sub>

Hier sieht man das Pinout des Zumos. Für dieses Projekt ist nur der obere Teil relevant, wo normalerweise das Display eingesteckt ist und die Pins 13 und 14 liegen.

Da wir möglichst viele Funktionen des Zumos nutzen wollen, nutzen wir nur die Pins des Displays. Da wir aber auch eine 3.3V Quelle benötigen, aber nur eine 5V Quelle bei unseren freigelegte Pins vorhanden ist, müssen wir über zwei Widerstände die 3.3V selbst herunterregeln.

**Wichtig:** In der Schaltung werden der TX bzw. RX Pin des Zumos jeweils mit dem Kontrahenten am Bluetooth Modul verbunden (RX -> TX; TX -> RX).

Alle weiteren Verbindungen gehen aus der Schematik hervor.

## Installation

Die Ordnerstruktur dieses Projektes wurde durch PlatformIO in VS Code generiert. Diese Struktur ist jedoch nur relevant, wenn man den Arduino Code über VS Code bearbeiten und hochladen möchte.

[Hier ein Überblick dazu](https://devdrik.de/arduino-in-vs-code/)

Für die Arduino IDE reicht jedoch lediglich die *zumo.ino* Datei im [Arduino Ordner](/arduino).

Zur Einbindung der Zumo Bibliothek gebe ich hier eine Dokumentation von Pololu selbst an: [Einbindung der Bibliothek](https://www.pololu.com/docs/0J63/all#5.2)

In beiden Fällen muss dann nur noch der Code nur auf den Zumo hochgeladen werden. Der Zumo gibt Infos über die Bluetooth Daten auf dem seriellen Monitor aus.

## Codeübersicht

Im Folgenden gebe ich eine grobe Übersicht über den Code und seine allgemeine Funktionsweise. Ich werde nicht jedes Statement erklären, sondern nur die Wichtigsten Blöcke im Überblick.

### Kopf des Codes

*Nur für die, die PIO nutzen:* Da wir in C++ schreiben und nicht in dem Arduino Format, werden zunächst die benötigten Bibliotheken eingebunden.

*Für alle:* Zum Start wird der Anfang einer Star Wars Melodie in Form von Noten im Porgrammspeicher gespeichert. Danach werden die Motoren, die Sensoren und der Buzzer in Variablen definiert. Ebenfalls definieren wir die Pins für RX und TX und erstellen mit diesen Pins ein SoftwareSerial welches wir für die Bluetooth Kommunikation verwenden werden.

Auf die dann definierten Funktionen gehe ich zum Schluss ein. Zunächst beschäftigen wir uns mit der [`void setup()`](#setup) und der [`void loop()`](#loop).

### setup()

In der Setup setzen wir zunächst den normalen Seriellen Monitor mit 9600Baud. Auf der selben Rate starten wir dann den Bluetooth Serial.

Als letzte Aktion in der Setup initialisieren wir noch die Sensoren.

### loop()

Vor der Loop Funktion definieren wir zwei longs, die für ein Delay später im Code benötigt werden. Einer der longs hält die alte ms Sekunden Zahl und der andere eine Intervalangabe ebenfalls in ms. 

Zu Beginn des Loops erstellen wir einen long und speichern dort die aktuellen Millisekunden. Daraufhin lesen wir alle vier Sensoren aus und speichern diese.

Danach kümmern wir uns um einkommende Bluetooth Signale. 
Dafür erstellen wir ein char-Array gefüllt mit Nullen mit einer vordefinierten Größe, welche wir ebenfalls als int speichern.
Solange wir über Bluetooth Zeichen verfügbar haben, werden alle Zeichen bis zum ; in unser char array eingelesen. Danach wird unser Befehl als String gespeichert, über den seriellen Monitor ausgegeben und über den [`commands()`](#commands) Funktion verarbeitet. Diese Funktion filtert den Befehl und führt die verbundene Aktion aus.

Bevor die Sensor Daten über [`readSensorData()`](#readsensordata) gesendet werden, wird überprüft, ob das letzte Senden der Daten schon länger als unser vorher gesetztes Intervall her ist.

Der letzte Block der loop beeinhaltet einen "Sicherheitsblock". Dieser ist dafür zuständig, dass der Zumo stoppt, wenn die Sensoren nach vorne eine Blockade erkennen. Erkennen er diese, geht der Zumo ein Stück zurück und stoppt.

Nun gehe ich auf die vor der [`setup()`](#setup) definierten Funktionen ein.

### splitCommands()

`void splitCommands(String command, String *cmd, String *arg0, String *arg1, String *arg2)`

Diese Funktion macht nichts anderes, als einen `String command` zu nehmen und diesen in vier Strings zu teilen.

Dies macht diese Funktion, indem sie den Hauptstring an den Leerzeichen teilt. Der erste Teil wird dann über einen Pointer dem `String *cmd` übergeben. Der zweite, dritte und vierte Teil wird, ebenfalls über Pointer, den Strings der Argumente `arg0, arg1, arg2` übergeben.

Da C++ bzw. Arduino keine einfache split Methode zur Verfügung stellt, habe ich mich für diese Methode des Teilens entschieden.

<sub>*Verbesserungsvorschläge sind gerne gesehen*</sub>

### commands()

`void commands(String command)`

Diese Funktion bearbeitet die Befehle, welche per Bluetooth hineinkommen und führt je nach Befehl die Aktion durch.

Dafür werden zunächst die vier Strings angelegt, die in die [`splitCommands()`](#splitcommands) Funktion gefüttert werden.

Einer dieser Strings (`cmd`) enhält den Befehl, welcher auszuführen ist. Dieser String wird dann in einer if-Abfrage überprüft. Da der Zumo bisher nur den *move* und den *honk* Befehl annimmt (Keyword *m* und Keyword *h*), wird nur nach diesem gesucht. Dabei wird zur Aussortierung von fehlerhaften Befehlen überprüft, ob der ursprüngliche Befehlsstring die richtige Länge hat und nicht zu wenig oder zu viele Zeichen gesendet hat.

Wenn dies erfüllt ist, werden in dem mover-Block Argumente in Zahlen umgewandelt und die Werte werden zurück ins Negative umgewandelt.
Bei dem honk-Block wird dann die vorher definierte StarWars Melodie gespielt.

Zur Vereinfachung schickt die App keine negativen Werte im move-Block. Der Wert 300 entspricht einer 0. Dementsprechend ist die 0 = -300 und die 600 = 300. Diese Umwandelung wird gemeinsam mit der Konvertierung zur Zahl durchgeführt.

Diese Integer werden nun einfach an den Motor des Zumos gesendet.

### sendSensorData()

`void sendSensorData(int frontLeft, int frontRight, int sideLeft, int sideRight)`

Diese Funktion sendet die als Parameter angegebenen SensorDaten über Bluetooth an die App.

Da die App die Sensordaten über das Keyword *sd*(sendData) empfängt, wird dieses an den Anfang gesetzt. Danach werden die Sensordaten angehängt, sodass der Befehl nach dem Schema `sd <frontLeft> <frontRight <sideLeft> <sideRight>` aufgebaut ist.

Dieser String wird nun einfach über den Bluetooth Serial gesendet.

## Code erweitern

Im Folgenden werde ich erklären, wie der Code um mehr Funktionen erweitert werden kann.

Die aktuellen Befehle des Zumos sehen wie folgt aus:

**Eingehend**

`m <leftSpeed> <rightSpeed>`

Motordaten zwischen 0-600

`h`
  
 Keine Argumente  

**Ausgehend**

`sd <frontLeft> <frontRight> <sideLeft> <sideRight>`

Sensordaten zwischen 0-6

Nun kann man sich natürlich weitere Befehle ausdenken.

Als Beispiel für einen einkommenden Befehl könnte man nun den Buzzer des Zumos über direkte Töne ansteuern.

Dafür würde man sich einen Befehl ausdenken.

Nehmen wir nun einfach mal folgenden Aufbau:

`b <note> <duration> <volume>`

Diese Syntax orientiert sich an den drei Parametern, die die [`playNote()`](https://pololu.github.io/pololu-buzzer-arduino/class_pololu_buzzer.html#a989d410dd6cdb7abfa136c3734040fb5) Funktion des Buzzers nimmt.

  

Dieser Befehl muss nun natürlich auch in der App integriert werden. Darauf gehe ich jedoch in der Dokumentation der [**Zumo-Bluetooth-App**](https://github.com/dermrvn-code/zumo-bluetooth-app) ein. Hier beschränken wir uns nun erstmal nur auf den Zumo Code.

Nun wäre der erste Schritt in die [`commands()`](#commands) Funktion zu gehen und eine weitere Abfrage nach dem `if (cmd == "m")` zu machen.

In unserem Falle wäre dies `else if(cmd == "b")`.

Nun kann man je nach Einheitlichkeit des Befehls noch Abfragen, ob der Befehl der konventionellen Länge entspricht. Da ich aber hier nicht definieren werde, in welchem Format z.B. die Note in dem Befehl geschickt wird, gebe ich hier keinen Wert an. Diese Abfrage kann also weggelassen oder durch andere bessere Sicherungen ersetzt werden.

Nun müssen unsere drei Befehlsargumente nur noch in die von [`playNote()`](https://pololu.github.io/pololu-buzzer-arduino/class_pololu_buzzer.html#a989d410dd6cdb7abfa136c3734040fb5) geforderten Parameter konvertiert und dann in die Funktion eingegeben werden.

Da diese Konvertierung abhängig von dem gewählten Format der Note, der Dauer und der Lautstärke ist, gebe ich hierfür kein Beispiel.

Zu beachten bei eingehenden Befehlen ist jedoch, dass man neben dem Befehlswort mit dem aktuellen Code nur drei Argumente zur Verfügung hat. Dies kann man natürlich erweitern, jedoch werde ich darauf hier nicht eingehen.

Nun kann man natürlich ähnlich auch ausgehende Befehle erstellen.

Beim Zumo könnte man nun die Liniensensoren auslesen und sie an die App schicken.

Hierfür denkt man sich nun wieder einen Befehl aus.

`sld <left> <middle> <right>`

Nach diesem Schema kann man sich nun einfach eine Funktion schreiben, die der [`sendSensorData`](#sendsensordata) Funktion ähnelt. Nur muss man hier den Befehl mit `sld` (*sendLineData*) starten und dann die Liniendaten anhängen. Dieses Kommando sendet man dann einfach in den Bluetooth Serial.

Diese Funktion wird dann einfach nach der [`sendSensorData`](#sendsensordata) Funktion aufgerufen und fertig ist die Ausgabe der Liniensensordaten.

Natürlich muss die App dann auch um die entsprechenden Funktionen erweitert werden. Darauf gehe ich aber, wie bereits erwähnt, in der Dokumentation der [**Zumo-Bluetooth-App**](https://github.com/dermrvn-code/zumo-bluetooth-app) ein.

Mit dieser Grundidee lässt sich dieser Code (und der der App) nun beliebig erweitern. Natürlich lässt sich dieser Code auch so weit umgestalten, dass nicht unbedingt ein Zumo, sondern irgendein Arduino darüber gesteuert werden kann.

  

## Schlusswort

Vorschläge und Verbesserungen sind gerne gesehen. Der Code ist nicht perfekt, aber tut was er soll. Wenn jemand Fehler entdeckt oder eine bessere Lösung für Dinge hat, bin ich offen dafür, diese in das Projekt zu integrieren.

Ich hoffe der Code kann gut genutzt werden oder zumindest als Inspiration für jemanden dienen.
