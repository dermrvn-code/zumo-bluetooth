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

// SETUP BLUETOOTH SERIAL
int rx_pin = 14; // defining software serial pins (RX and TX) for arduino uno
int tx_pin = 13;
SoftwareSerial BluetoothSerial(rx_pin, tx_pin);

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

void cmd_m(String arg0, String arg1, String arg2)
{
  int leftSpeed = arg0.toInt();
  int rightSpeed = arg1.toInt();
}

void commands(String command)
{
  String cmd, arg0, arg1, arg2;
  splitCommands(command, &cmd, &arg0, &arg1, &arg2);

  if (cmd == "m")
  {
    cmd_m(arg0, arg1, arg2);
  }
}

void setup()
{
  // START SERIAL (for debugging)
  Serial.begin(9600); // set the data rate for the Hardware Serial port
  Serial.write("Hardware Serial is working !");

  // START BLUETOOTH SERIAL
  /*BluetoothSerial.begin(9600); // set the data rate for the SoftwareSerial port
  BluetoothSerial.write("Software Serial and Bluetooth is working !");*/
}

String cmd = "";

void loop()
{

  // BLUETOOTH SIGNAL IS COMING IN
  /*if (BluetoothSerial.available())
  {
    // READ CHARS AND WRITE TO COMMAND STRING
    char c = BluetoothSerial.read();
    cmd = cmd + c;
    delay(1);
  }
  else
  {
    // IF COMMAND IS FINISHED SENDING, PRINT COMMAND AND EXECUTE OVER COMMAND HANDLER
    if (cmd != "")
    {
      Serial.println(cmd);
      commands(cmd);
      cmd = "";
    }
  }

  // SEND SERIAL DATA TO BLUETOOTH (for debugging)
  if (Serial.available())
  {
    BluetoothSerial.write(Serial.read());
  }*/
}
