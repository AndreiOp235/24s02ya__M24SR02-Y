#include <Wire.h>
#include "24s02ya__M24SR02.h"

void setup() {
  Wire.begin(); //required for device check
  Serial.begin(9600);
  while (!Serial)
    ;  // Wait for Serial Monitor to open
  Serial.println("I2C Scanner");
  nfcGadget nfc;
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning...");

  // Scan all possible addresses from 1 to 127
  for (address = 1; address < 128; address++) {
    Wire.beginTransmission(address); // Start I2C transmission to a device with given address
    error = Wire.endTransmission(); // End the I2C transmission and capture the return value

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0"); // Add a leading zero for single-digit addresses
      }
      Serial.println(address, HEX); // Print the address in hexadecimal
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }

  delay(5000); // Wait 5 seconds before scanning again
}