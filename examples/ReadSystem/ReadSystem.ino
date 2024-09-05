/*
This example demonstrates how to interact with an NFC (Near Field Communication) tag, specifically focusing on reading the System file stored in the NFC tag. The System file usually contains device-specific information such as memory size, configuration details, and more. This example shows how to:

Initialize communication with the NFC tag via I2C.
Select and read the System file from the NFC tag.
Print the contents of the System file to the Serial Monitor for further inspection.
This example can be adapted to handle other types of files on the NFC tag or to extend functionality for writing data to the tag.
*/

#include <Wire.h>
#include "24s02ya__M24SR02.h"

nfcGadget nfc;

void setup() {
  Wire.begin();  //required for device check
  Serial.begin(9600);
  while (!Serial)
    ;  // Wait for Serial Monitor to open
  Serial.println("");
  Serial.println("interfata NFC");


  nfc._verbose = false; //enable to see various outputs
  nfc._cmds = false; //enable to see various inputs
  nfc._sendGetI2cSession = true;
  Serial.println("Open ");
  nfc.selectNFCapp(); //not mandatory

  Serial.println("Systemfile");
  nfc.selectFile(Systemfile);
  int j=nfc.readFileLength(); //size of payload, it must be called !!!
  char *c=nfc.readFile(); //read data
  nfc.explainFile();
  Serial.println("");
}


void loop() {
}
