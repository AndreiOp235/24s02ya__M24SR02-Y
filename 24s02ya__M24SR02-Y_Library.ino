#include <Wire.h>
#include "24s02ya__M24SR02.h"

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial)
    ;  // Wait for Serial Monitor to open
  Serial.println("I2C Scanner");
  nfcGadget nfc;
}

void nopuri();

void loop() 
{

}