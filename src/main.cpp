/*
   == Zumo Robot ==
  [ by Leona, Jakob und Marvin ]
*/
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Zumo32U4.h>

const char Starwars[] PROGMEM =
    "! O3 L4 ggge-8.b16 ge-8.b16g2";
// HARDWARE VARIABLES
Zumo32U4Motors motors;
Zumo32U4Buzzer buzzer;
Zumo32U4ProximitySensors proxSensors;

// SETUP BLUETOOTH SERIAL
const int TX_PIN = 13;
const int RX_PIN = 14;
SoftwareSerial BluetoothSerial(RX_PIN, TX_PIN);

// SPLIT A COMMAND STRING INTO 4 SUBSTRINGS (command word, argument 1-3)
void splitCommands(String command, String *cmd, String *arg0, String *arg1, String *arg2)
{
  command += " ";

  *cmd = command.substring(0, command.indexOf(" "));
  command = command.substring(command.indexOf(" ") + 1);

  *arg0 = command.substring(0, command.indexOf(" "));
  command = command.substring(command.indexOf(" ") + 1);

  *arg1 = command.substring(0, command.indexOf(" "));
  command = command.substring(command.indexOf(" ") + 1);

  *arg2 = command.substring(0, command.indexOf(" "));
  command = command.substring(command.indexOf(" ") + 1);
}

// RUNS THE CORRESPONDING ACTION TO THE GIVEN COMMAND
void commands(String command)
{

  String cmd, arg0, arg1, arg2;
  splitCommands(command, &cmd, &arg0, &arg1, &arg2);

  // MOVE THE ZUMO
  if (cmd == "m")
  {
    if (command.length() == 9)
    {
      int leftSpeed = arg0.toInt() - 300;
      int rightSpeed = arg1.toInt() - 300;

      motors.setSpeeds(leftSpeed, rightSpeed);
    }
  }
  // HONK A STAR WARS MELODY
  else if (cmd == "h")
  {
    if (command.length() == 1)
    {
      buzzer.stopPlaying();
      buzzer.playFromProgramSpace(Starwars);
    }
  }
}

// SENDS THE SENSOR DATA INTO THE BLUETOOTH CHANNEL
void sendSensorData(int frontLeft, int frontRight, int sideLeft, int sideRight)
{
  String sd = "sd ";
  sd += String(frontLeft) + " ";
  sd += String(frontRight) + " ";
  sd += String(sideLeft) + " ";
  sd += String(sideRight);
  BluetoothSerial.flush();
  BluetoothSerial.println(sd);
}

// ======================================================================================
//
// ======================================================================================

void setup()
{
  // START SERIAL (for debugging)
  Serial.begin(9600); // set the data rate for the Hardware Serial port
  Serial.write("Hardware Serial is working !");

  // START BLUETOOTH SERIAL
  BluetoothSerial.begin(9600); // set the data rate for the SoftwareSerial port
  BluetoothSerial.write("Software Serial and Bluetooth is working !");

  // INIT SENSORS
  proxSensors.initThreeSensors();
}

// USED FOR A DELAY LATER IN THE LOOP
unsigned long previousMillis = 0;
const long interval = 1000;

void loop()
{
  unsigned long currentMillis = millis();

  // ========== READ SENSORS ============================================================
  proxSensors.read();
  int frontLeft = proxSensors.countsFrontWithLeftLeds();
  int frontRight = proxSensors.countsFrontWithRightLeds();
  int sideLeft = proxSensors.countsLeftWithLeftLeds();
  int sideRight = proxSensors.countsRightWithRightLeds();
  // ====================================================================================
  //
  //
  //
  // ========== BLUETOOTH IN ============================================================

  const int datalength = 15;
  char btdata[datalength] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  // BLUETOOTH SIGNAL IS COMING IN
  if (BluetoothSerial.available() > 0)
  {
    // READ CHARS AND WRITE TO COMMAND STRING
    BluetoothSerial.flush();
    BluetoothSerial.readBytesUntil(';', btdata, datalength);
    String cmd = String(btdata);
    Serial.println(cmd);
    commands(cmd);
    delay(100);
  }

  // ====================================================================================
  //
  //
  //
  // ========== BLUETOOTH OUT ===========================================================

  // JUST SEND THE DATA EVERY 1000ms
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    sendSensorData(frontLeft, frontRight, sideLeft, sideRight);
    delay(100);
  }

  // ====================================================================================
  //
  //
  //
  //
  // ========== EMERGENCY MECHS =========================================================

  // STOP AND BACKUP IF SOMETHING IN FRONT
  if (frontLeft > 5 || frontRight > 5)
  {
    motors.setSpeeds(-100, -100);
    delay(200);
    motors.setSpeeds(0, 0);
  }
}
