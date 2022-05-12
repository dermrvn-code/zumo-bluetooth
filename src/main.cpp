/*
  ______________________________________________________
  |                                                     |
  |   ||======||   ||=====||   ||======||   ||=====||   |
  |   ||     //    ||     ||   ||     //    ||     ||   |
  |   ||====||     ||     ||   ||====||     ||     ||   |
  |   ||     \\    ||     ||   ||     \\    ||     ||   |
  |   ||======||   ||=====||   ||======||   ||=====||   |
  |_____________________________________________________|
   == Zumo Robot by 2.25 ==
  [ Leona, Jakob und Marvin ]

*/
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Zumo32U4.h>

// HARDWARE VARIABLES
Zumo32U4Motors motors;
Zumo32U4Buzzer buzzer;
Zumo32U4ProximitySensors proxSensors;

// SETUP BLUETOOTH SERIAL
const int RX_PIN = 14;
const int TX_PIN = 13;
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

// SPLITS A COMMAND STRING AND RUNS THE CORRESPONDING ACTION
void commands(String command)
{

  String cmd, arg0, arg1, arg2;
  splitCommands(command, &cmd, &arg0, &arg1, &arg2);

  if (cmd == "m")
  {
    if (command.length() == 9)
    {
      int leftSpeed = arg0.toInt() - 300;
      int rightSpeed = arg1.toInt() - 300;

      motors.setSpeeds(leftSpeed, rightSpeed);
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

void loop()
{
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

  const int datalength = 25;
  char btdata[datalength];
  String cmd = "";

  // BLUETOOTH SIGNAL IS COMING IN
  if (BluetoothSerial.available() > 0)
  {
    // READ CHARS AND WRITE TO COMMAND STRING
    BluetoothSerial.readBytesUntil(';', btdata, datalength);
    cmd = String(btdata);
    Serial.println(cmd);
    commands(cmd);
    BluetoothSerial.flush();
    delay(100);
  }

  // ====================================================================================
  //
  //
  //
  // ========== BLUETOOTH OUT ===========================================================

  // sendSensorData(frontLeft, frontRight, sideLeft, sideRight);

  // ====================================================================================
  //
  //
  //
  //
  // ========== EMERGENCY MECHS =========================================================

  // STOP AND BACKUP IF SOMETHING IN FRONT
  if (frontLeft > 5 || frontRight > 5)
  {
    motors.setSpeeds(-50, -50);
    delay(500);
    motors.setSpeeds(0, 0);
  }

  delay(50);
}
