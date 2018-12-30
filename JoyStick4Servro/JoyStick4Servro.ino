/*
  This example configures LinkIt 7697 to control two servo motor
  Author: Jack Jean
*/
#include <LRemote.h>
#include <Servo.h>

LRemoteJoyStick stick1;

LRemoteLabel label1;

Servo myservo1;
Servo myservo2;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  Serial.println("Start configuring remote");

    // Setup the Remote Control's UI canvas
  LRemote.setName("JackJean");
  
  // We use a protrail since it is easir to contorl joysticks
  LRemote.setOrientation(RC_PORTRAIT);
  LRemote.setGrid(4, 5);

  // Add stick
  stick1.setPos(0, 1);
  stick1.setSize(4, 4);
  stick1.setColor(RC_ORANGE);
  LRemote.addControl(stick1);

  
    // Add a simple text label
  label1.setText("(0, 0)");
  label1.setPos(0, 0);
  label1.setSize(4, 1);
  label1.setColor(RC_GREY);
  LRemote.addControl(label1);

  


  // Start broadcasting our remote contoller
  // This method implicitly initialized underlying BLE subsystem
  // to create a BLE peripheral, and then
  // start advertisement on it.
  LRemote.begin();
  Serial.println("begin() returned");
  // creat two servo motor
  myservo1.attach(2);
  myservo2.attach(3);
}


void loop() {
  // check if we are connect by some 
  // BLE central device, e.g. an mobile app
  if(!LRemote.connected()) {
    Serial.println("waiting for connection");
    delay(1000);
  } else {
    // The interval between button down/up
    // can be very short - e.g. a quick tap
    // on the screen.
    // We could lose some event if we
    // delay something like 100ms.
    delay(15);
  }
  
  // Process the incoming BLE write request
  // and translate them to control events
  LRemote.process();

  if(stick1.isValueChanged()){
    LRemoteDirection d = stick1.getValue();
    // d.x and d.y are the value from the Joystick component:
    // d.x : -100 ~ 100, where 0 is center, -100 is leftmost, and 100 is rightmost.
    // d.y : -100 ~ 100, where 0 is center, -100 is bottommost, and 100 is topmost.

    // you can print d directly.
    Serial.println(d);

    // in this example, we simply pass the value of d.x/d.y
    // back to the LinkIt Remote app.
    label1.updateText(d.toString());
    myservo1.write(map(d.x,-100,100,0,180));
    myservo2.write(map(d.y,-100,100,0,100));
  } 
  
  
}