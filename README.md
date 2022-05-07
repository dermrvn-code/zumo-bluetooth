# Zumo 32U4 Bluetooth

Dieses Projekt integriert die Steuerung über Bluetooth. Der Code in diesem Projekt steht in direkter Verbindung zu einer dafür erstellten **Unity App** ([*Zumo-Bluetooth-App*](https://github.com/dermrvn-code/zumo-bluetooth-app)).
Da der Pololu 32U4 ohne Bluetooth Antenne kommt, muss diese seperat angebaut werden. Details dazu gibt es unter [Aufbau und Verkabelung](#aufbau-und-verkabelung).
Die App integriert die Möglichkeit die Motoren zu steuern und die Sensordaten des Zumos auszulesen.
Natürlich kann dieser Code beliebig um weitere Funktionen erweitert werden.

## Inhaltsverzeichnis
1. [Aufbau und Verkabelung](#aufbau-und-verkabelung)
2. [Installation](#installation)
4. [Codeübersicht](#codeübersicht)
5. [Code erweitern](#code-erweitern)
6. [Quellen](#quellen)

## Aufbau und Verkabelung
![Verkabelung](https://raw.githubusercontent.com/dermrvn-code/zumo-bluetooth/master/circuit.png)<br><sub>*Verschaltung von Zumo, Levelshifter, und Bluetoothmodul*</sub>

Der Code definiert die Pins 13 und 14 als RX bzw. TX Pins. Diese Pins sind zu erreichen, wenn man das Display des Zumos absteckt.

![Pinout des Zumos](https://a.pololu-files.com/picture/0J6286.1200.jpg?177f62fab7018b9ff5f2898d27f91d61)<br><sub>*Das Pinout der Oberseite des Zumos*</sub>

Hier sieht man das Pinout des Zumos. Für dieses Projekt ist nur der obere Teil relevant, wo normalerweise das Display eingesteckt ist und die Pins 13 und 14 liegen.

Da wir mehr Funktionen des Zumos nutzen wollen, nutzen wir nur die Pins des Displays. Da wir aber auch eine 3.3V Quelle benötigen, aber nur eine 5V Quelle bei unseren Pins vorhanden haben, müssen wir über zwei Widerstände die 3.3V selbst erzeugen.

**Wichtig:** In der Schaltung werden der TX bzw. RX Pin des Zumos jeweils mit dem Kontrahenten am Bluetooth Modul verbunden (RX -> TX; TX -> RX).

Alle weiteren Verbindungen gehen aus der Schematik hervor.

## Installation

## Codeübersicht

## Code erweitern

## Quellen
