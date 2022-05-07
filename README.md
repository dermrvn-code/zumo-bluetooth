# Zumo 32U4 Bluetooth

Dieses Projekt integriert die Steuerung über Bluetooth. Der Code in diesem Projekt steht in direkter Verbindung zu einer dafür erstellten **Unity App** ([*Zumo-Bluetooth-App*](https://github.com/dermrvn-code/zumo-bluetooth-app)).<br>
Da der Pololu 32U4 ohne Bluetooth Antenne kommt, muss diese seperat angebaut werden. Details dazu gibt es unter [Aufbau und Verkabelung](#aufbau-und-verkabelung).
Die App integriert die Möglichkeit die Motoren zu steuern und die Sensordaten des Zumos auszulesen.
Natürlich kann dieser Code beliebig um weitere Funktionen erweitert werden.

## Inhaltsverzeichnis
1. [Aufbau und Verkabelung](#aufbau-und-verkabelung)
2. [Installation](#installation)
4. [Codeübersicht](#codeübersicht)
5. [Code erweitern](#code-erweitern)

## Aufbau und Verkabelung
![Verkabelung](https://raw.githubusercontent.com/dermrvn-code/zumo-bluetooth/master/circuit.png)<br><sub>*Verschaltung von Zumo, Levelshifter, und Bluetoothmodul*</sub>
<br>
Der Code definiert die Pins 13 und 14 als RX bzw. TX Pins. Diese Pins sind zu erreichen, wenn man das Display des Zumos absteckt.

![Pinout des Zumos](https://a.pololu-files.com/picture/0J6286.1200.jpg?177f62fab7018b9ff5f2898d27f91d61)<br><sub>*Das Pinout der Oberseite des Zumos*</sub>
<br>
Hier sieht man das Pinout des Zumos. Für dieses Projekt ist nur der obere Teil relevant, wo normalerweise das Display eingesteckt ist und die Pins 13 und 14 liegen.
<br>
Da wir mehr Funktionen des Zumos nutzen wollen, nutzen wir nur die Pins des Displays. Da wir aber auch eine 3.3V Quelle benötigen, aber nur eine 5V Quelle bei unseren Pins vorhanden haben, müssen wir über zwei Widerstände die 3.3V selbst erzeugen.
<br>
**Wichtig:** In der Schaltung werden der TX bzw. RX Pin des Zumos jeweils mit dem Kontrahenten am Bluetooth Modul verbunden (RX -> TX; TX -> RX).
<br>
Alle weiteren Verbindungen gehen aus der Schematik hervor.

## Installation
Die Ordnerstruktur dieses Projektes wurde durch PlatformIO in VS Code generiert. Diese Struktur ist jedoch nur relevant, wenn man Arduino Code über VS Code bearbeiten und hochladen möchte.<br>
 [Hier ein Überblick dazu](https://devdrik.de/arduino-in-vs-code/)<br>
 Für die Arduino IDE reicht jedoch lediglich die *zumo.ino* Datei im [Arduino Ordner](/arduino).
 <br>
 Zum einbinden der Zumo Bibliothek gebe ich hier eine Dokumentation von Pololu selbst an: [Einbindung der Bibliothek](https://www.pololu.com/docs/0J63/all#5.2)
 <br>
 In beiden Fällen muss dann nur noch der Code nur auf den Zumo hochgeladen werden. Der Serielle Monitor sollte auch Infos über die Bluetooth Daten ausgeben.

## Codeübersicht
Im Folgenden gebe ich eine grobe Übersicht über den Code und seine allgemeine Funktionsweise. Ich werde nicht jedes Statement erklären, sondern nur die Wichtigsten Blöcke im Überblick.
### Kopf
<br>
*Nur für die, die PIO nutzen:* Da wir in C++ schreiben und nicht in dem Arduino Format, werden zunächst die benötigten Bibliotheken eingebunden.
<br>
*Für alle:* Zum Start werden die Motoren und die Sensoren in Variablen definiert. Ebenfalls definieren wir die Pins für RX und TX und Erstellen mit diesen Pins ein SoftwareSerial welches wir für die Bluetooth Kommunikation verwenden werden.
<br><br>
Auf die dann definierten Funktionen gehe ich zum Schluss ein. Zunächst beschäftigen wir uns mit der [`void setup()`](#setup) und der [`void loop()`](#loop).
<br>
### setup()
In der Setup setzen wir zunächst den normalen Seriellen Monitor mit 9600Baud. Auf der selben Rate starten wir dann den Bluetooth Serial.
<br>
Als letzte Aktion in der Setup initialisieren wir noch die Sensoren.

### loop()
Vor der Loop Funktion definieren wir eine Konstante für eine Char-Array Größe, welche unsere einkommenden Bluetooth Befehle speichert. Wir erstellen ebenfalls einen leeren String der unser Kommando an Zumo hält.
<br>
Zu Beginn des Loops lesen wir alle vier Sensoren aus und speichern diese.
<br>
Danach kümmern wir uns um einkommende Bluetooth Signale. Solange wir über Bluetooth Zeichen verfügbar haben und keinen fertigen Befehl gespeichert haben, werden alle Zeichen bis zum ; in unser char array eingelesen. Danach wird unser Befehl gespeichert und über den Else Block verarbeitet.
<br>
In diesem Block wird die [`commands()`](#commands()) Funktion aufgerufen, welche den Befehl filtert und die verbundene Aktion ausführt. Danach wird noch der Befehlsstring und das char array geleert.
<br>


## Code erweitern

