# Zumo 32U4 Bluetooth
Dieses Projekt integriert die Steuerung über Bluetooth. Der Code in diesem Projekt steht in direkter Verbindung zu einer dafür erstellten **Unity App** ([*Zumo-Bluetooth-App*](https://github.com/dermrvn-code/zumo-bluetooth-app)).
  
Da der Pololu 32U4 ohne Bluetooth Antenne kommt, muss diese seperat angebaut werden. Details dazu gibt es unter [Aufbau und Verkabelung](#aufbau-und-verkabelung).
  
Die App integriert die Möglichkeit die Motoren zu steuern und die Sensordaten des Zumos auszulesen.
Natürlich kann dieser Code beliebig um weitere Funktionen erweitert werden.
  
## Inhaltsverzeichnis
1. [Aufbau und Verkabelung](#aufbau-und-verkabelung)
2. [Installation](#installation)
3. [Codeübersicht](#codeübersicht)
4. [Code erweitern](#code-erweitern)
  
## Aufbau und Verkabelung
![Verkabelung](https://raw.githubusercontent.com/dermrvn-code/zumo-bluetooth/master/circuit.png)
  
<sub>*Verschaltung von Zumo, Levelshifter, und Bluetoothmodul*</sub>
  
Der Code definiert die Pins 13 und 14 als RX bzw. TX Pins. Diese Pins sind zu erreichen, wenn man das Display des Zumos absteckt.

![Pinout des Zumos](https://a.pololu-files.com/picture/0J6286.1200.jpg?177f62fab7018b9ff5f2898d27f91d61)
  
<sub>*Das Pinout der Oberseite des Zumos*</sub>
  
Hier sieht man das Pinout des Zumos. Für dieses Projekt ist nur der obere Teil relevant, wo normalerweise das Display eingesteckt ist und die Pins 13 und 14 liegen.
  
Da wir mehr Funktionen des Zumos nutzen wollen, nutzen wir nur die Pins des Displays. Da wir aber auch eine 3.3V Quelle benötigen, aber nur eine 5V Quelle bei unseren Pins vorhanden haben, müssen wir über zwei Widerstände die 3.3V selbst erzeugen.
  
**Wichtig:** In der Schaltung werden der TX bzw. RX Pin des Zumos jeweils mit dem Kontrahenten am Bluetooth Modul verbunden (RX -> TX; TX -> RX).
  
Alle weiteren Verbindungen gehen aus der Schematik hervor.
  
## Installation
Die Ordnerstruktur dieses Projektes wurde durch PlatformIO in VS Code generiert. Diese Struktur ist jedoch nur relevant, wenn man Arduino Code über VS Code bearbeiten und hochladen möchte.
  
 [Hier ein Überblick dazu](https://devdrik.de/arduino-in-vs-code/)
   
 Für die Arduino IDE reicht jedoch lediglich die *zumo.ino* Datei im [Arduino Ordner](/arduino).
  
 Zum einbinden der Zumo Bibliothek gebe ich hier eine Dokumentation von Pololu selbst an: [Einbindung der Bibliothek](https://www.pololu.com/docs/0J63/all#5.2)
  
 In beiden Fällen muss dann nur noch der Code nur auf den Zumo hochgeladen werden. Der Serielle Monitor sollte auch Infos über die Bluetooth Daten ausgeben.
  
## Codeübersicht
Im Folgenden gebe ich eine grobe Übersicht über den Code und seine allgemeine Funktionsweise. Ich werde nicht jedes Statement erklären, sondern nur die Wichtigsten Blöcke im Überblick.
  
### Kopf des Codes
*Nur für die, die PIO nutzen:* Da wir in C++ schreiben und nicht in dem Arduino Format, werden zunächst die benötigten Bibliotheken eingebunden.
  
*Für alle:* Zum Start werden die Motoren und die Sensoren in Variablen definiert. Ebenfalls definieren wir die Pins für RX und TX und Erstellen mit diesen Pins ein SoftwareSerial welches wir für die Bluetooth Kommunikation verwenden werden.
  
Auf die dann definierten Funktionen gehe ich zum Schluss ein. Zunächst beschäftigen wir uns mit der [`void setup()`](#setup) und der [`void loop()`](#loop).
  
### setup()
In der Setup setzen wir zunächst den normalen Seriellen Monitor mit 9600Baud. Auf der selben Rate starten wir dann den Bluetooth Serial.
   
Als letzte Aktion in der Setup initialisieren wir noch die Sensoren.
  
### loop()
Vor der Loop Funktion definieren wir eine Konstante für eine Char-Array Größe, welche unsere einkommenden Bluetooth Befehle speichert. Wir erstellen ebenfalls einen leeren String der unser Kommando an Zumo hält.
  
Zu Beginn des Loops lesen wir alle vier Sensoren aus und speichern diese.
  
Danach kümmern wir uns um einkommende Bluetooth Signale. Solange wir über Bluetooth Zeichen verfügbar haben und keinen fertigen Befehl gespeichert haben, werden alle Zeichen bis zum ; in unser char array eingelesen. Danach wird unser Befehl gespeichert und über den Else Block verarbeitet.
  
In diesem Block wird die [`commands()`](#commands) Funktion aufgerufen, welche den Befehl filtert und die verbundene Aktion ausführt. Danach wird noch der Befehlsstring und das char array geleert.
  
Bei ausgehenden Bluetooth Daten wird nur die [`readSensorData()`](#readsensordata) Funktion ausgeführt, auf welche ich in Kürze eingehen werde.
  
Der letzte Block der loop beeinhaltet einen "Sicherheitsblock". Dieser ist dafür zustänidg, dass der Zumo stoppt, wenn die Sensoren nach vorne eine Blockade erkennen. Erkennen sie dieses, geht der Zumo ein Stück zurück und stoppt.
  
Nun gehe ich auf die vor der [`setup()`](#setup) definierten Funktionen ein.
  
### splitCommands()
`void  splitCommands(String  command, String  *cmd, String  *arg0, String  *arg1, String  *arg2)`
Diese Funktion macht nichts anderes, als einen `String command` zu nehmen und diesen in vier Strings zu teilen. 
  
Dies macht diese Funktion, indem sie den Hauptstring an den Leerzeichen teilt. Der erste Teil wird dann über einen Pointer dem `String *cmd` übergeben. Der zweite, dritte und vierte Teil wird, ebenfalls über Pointer, den Strings der Argumente `arg0, arg1, arg2` übergeben.
  
Da C++ bzw. Arduino keine einfache split Methode zur Verfügung stellt, habe ich mich für diese Methode des Teilens entschieden.  
<sub>*Verbesserungsvorschläge sind gerne gesehen*</sub>
  
### commands()
`void  commands(String  command)`
Diese Funktion bearbeitet die Befehle, welche per Bluetooth hineinkommen und führt je nach Befehl die Aktion durch.
  
Dafür werden zunächst die vier Strings angelegt, die in die [`splitCommands()`](#splitcommands) Funktion gefüttert werden.
  
Einer dieser Strings (`cmd`) enhält den Befehl, welcher auszuführen ist. Dieser String wird dann in einer if-Abfrage überprüft. Da der Zumo bisher nur einen *move* Befehl annimmt (Keyword *m*), wird nur nach diesem gesucht. Danach wird zur Aussortierung von fehlerhaften Befehlen überprüft, ob der ursprüngliche Befehlsstring die richtige Länge hat und nicht zu wenig oder zu viele Zeichen gesendet hat.
  
Wenn dies erfüllt ist, werden die Argumente in Zahlen umgewandelt und die Werte werden zurück ins Negative umgewandelt.
  
Zur Vereinfachung schickt die App keine negativen werden. Der Wert 300 entspricht einer 0. Dementsprechend ist die 0 = -300 und die 600 = 300. Diese Umwandelung wird gemeinsam mit der Konvertierung zur Zahl durchgeführt.
  
Diese Integer werden nun einfach an den Motor des Zumos gesendet.
  
### sendSensorData()
`void  sendSensorData(int  frontLeft, int  frontRight, int  sideLeft, int  sideRight)`
Diese Funktion sendet die als Parameter angegebenen SensorDaten über Bluetooth an die App.
  
Da die App die Sensordaten über das Keyword *sd*(sendData) empfängt, wird dieses an den Anfang gesetzt. Danach werden die Sensordaten angehängt, sodass der Befehl nach dem Schema `sd <frontLeft> <frontRight <sideLeft> <sideRight>` aufgebaut ist.
  
Dieser String wird nun einfach über den Bluetooth Serial gesendet.
  
## Code erweitern
Im Folgenden werde ich erklären, wie der Code um mehr Funktionen erweitert werden kann.
  
