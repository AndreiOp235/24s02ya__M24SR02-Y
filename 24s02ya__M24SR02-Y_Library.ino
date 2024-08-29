#include <Wire.h>
#include "24s02ya__M24SR02.h"

void setup() {
  Wire.begin(); //required for device check
  Serial.begin(9600);
  while (!Serial)
    ;  // Wait for Serial Monitor to open
  Serial.println("I2C Scanner");
}

byte v;

void loop() {
  nfcGadget nfc;
  if(nfc.deviceConnected())
    Serial.println("reset");

  byte frame[]= {0x02, 0x00, 0xA4, 0x04, 0x00, 0x07, 0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01,  0x35, 0xC0};
  byte raspuns[20];
  Wire.beginTransmission(0x56);
  Wire.write(0x52);

  for(int i=0; i<15;i++)
  {
    v = (frame[i] & 0xff);
    delay(5);
    Wire.write(byte(v & 0xff));
    delay(1);
  }
  //receiveResponse(2 + 3);

  Wire.endTransmission(); // End transmission to send data

    int index = 0;
    boolean loop = false;

    do {
        loop = false;
        Wire.requestFrom(0x56, 5);
            delay(1);

        while (Wire.available() && index < 5) {
            raspuns[index] = (Wire.read() & 0xff);

                delay(1);
            index++;
        }
    } while (loop);

  Serial.print(index);


  delay(5000); // Wait 5 seconds before scanning again
}
