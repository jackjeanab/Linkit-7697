/*
 * Generated using BlocklyDuino:
 *
 * https://github.com/MediaTek-Labs/BlocklyDuino-for-LinkIt
 *
 * Date: Sat, 05 Jan 2019 07:15:07 GMT
 */

#include <Servo.h>

int left;

int posX;

int right;

Servo __myservo2;

void setup()
{
  Serial.begin(115200);

  __myservo2.attach(2);
  posX = 90;
  __myservo2.write(posX);
  delay(1000);
}


void loop()
{
  left = analogRead(A0);
  right = analogRead(A1);
  Serial.println(String() + "left:" + left);
  Serial.println(String() + "right:" + right);
  if (left - right > 100 && posX < 180) {
    posX = posX + 1;

  }
  if (left - right < 100 && posX > 0) {
    posX = posX - 1;

  }
  __myservo2.write(posX);
  delay(10);
}