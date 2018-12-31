/*
  This example configures LinkIt 7697 using MCS gamepad channel
  to control two servo motor
  Author: Jack Jean
  Date: 12/31/2018
*/

#include <LWiFi.h>
#include <WiFiClient.h>
#include "MCS.h"
#include <Servo.h>

// Assign AP ssid / password here
#define _SSID "ssid"
#define _KEY "password"

// Assign device id / key of your test device
MCSDevice mcs("device id", "device key");

// Assign channel id
// The test device should have a channel id "control_gamepad".
// the first channel should be "Controller" - "GamePad"
MCSControllerGamePad gamepad("channel id");

int X_axis = 0;
int Y_axis = 0;
const int X_origin = 0;
const int Y_origin = 0;
const int X_mid = 90;
const int Y_mid = 50;
const int X_end = 180;
const int Y_end = 100;
const int X_step = 5;
const int Y_step = 5;

Servo myservoX;
Servo myservoY;

void setup()
{
  // setup Serial output at 9600
  Serial.begin(9600);

  // setup Wifi connection
  while (WL_CONNECTED != WiFi.status())
  {
    Serial.print("WiFi.begin(");
    Serial.print(_SSID);
    Serial.print(",");
    Serial.print(_KEY);
    Serial.println(")...");
    WiFi.begin(_SSID, _KEY);
  }
  Serial.println("WiFi connected !!");

  // setup MCS connection
  mcs.addChannel(gamepad);

  while (!mcs.connected())
  {
    Serial.println("MCS.connect()...");
    mcs.connect();
  }
  Serial.println("MCS connected !!");

  while (!gamepad.valid())
  {
    Serial.println("initialize gamepad default value...");
    gamepad.value();
    // Note: At this moment we can "read" the values
    // of the gamepad - but the value is meaningless.
    //
    // The MCS server returns that "last button pressed"
    // in this cause - even if the user is not pressing any button
    // at this moment.
    //
    // We read the values here simply to make the following
    // process() -> if(gamepad.updated()) check working.
  }

  X_axis = X_mid;
  Y_axis = Y_mid;

  myservoX.attach(2);
  myservoY.attach(3);
  myservoX.write(X_axis);
  myservoY.write(Y_axis);
}

void loop()
{
  // Note that each process consumes 1 command from MCS server.
  // The 100 millisecond timeout assumes that the server
  // won't send command rapidly.
  mcs.process(100);

  // updated flag will be cleared in process(), user must check it after process() call.
  if (gamepad.updated())
  {
    Serial.print("Gamepad event arrived, new value = ");
    Serial.println(gamepad.value());
    String temp = MCSValueToString(gamepad.value());
    if (temp == "A|pressed")
    {
      X_axis = X_mid;
      Y_axis = Y_mid;
      myservoX.write(X_axis);
      myservoY.write(Y_axis);
    }
    if (temp == "B|pressed")
    {
      X_axis = X_origin;
      Y_axis = Y_origin;
      myservoX.write(X_axis);
      myservoY.write(Y_axis);
    }
    if (temp == "right|pressed" && X_axis <= X_end)
    {
      X_axis = X_axis + X_step;
      myservoX.write(X_axis);
    }
    if (temp == "left|pressed" && X_axis >= X_origin)
    {
      X_axis = X_axis - X_step;
      myservoX.write(X_axis);
    }
    if (temp == "up|pressed" && Y_axis <= Y_end)
    {
      Y_axis = Y_axis + Y_step;
      myservoY.write(Y_axis);
    }
    if (temp == "down|pressed" && Y_axis >= Y_origin)
    {
      Y_axis = Y_axis - Y_step;
      myservoY.write(Y_axis);
    }
  }

  // check if need to re-connect
  while (!mcs.connected())
  {
    Serial.println("re-connect to MCS...");
    mcs.connect();
    if (mcs.connected())
      Serial.println("MCS connected !!");
  }
}
