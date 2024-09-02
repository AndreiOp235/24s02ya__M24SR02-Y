#include <Wire.h>
#include "24s02ya__M24SR02.h"

nfcGadget nfc;


//Serial.println(crcsum(sel, 14, 0x6363), HEX); serial checker

void setup() {
  Wire.begin();  //required for device check
  Serial.begin(9600);
  while (!Serial)
    ;  // Wait for Serial Monitor to open
  Serial.println("");
  Serial.println("interfata NFC");

  nfc._verbose = true;
  nfc._cmds = true;
  nfc._sendGetI2cSession = true;
  Serial.println("Open ");
  nfc.selectNFCapp(); //not mandatory

  Serial.println("CC file ");
  nfc.selectFile(CCfile);
  Serial.print(nfc.readFileLength());

  nfc.readFile();
  nfc.explainFile();

  /*
  Serial.println("Time to read ST file length");

  memcpy(nfc._data, adate1, 8);
  for (int i = 0; i < 8; i++)
    Serial.println(nfc._data[i] & 0xFF, HEX);


  nfc.sendCommand(6);
  nfc.receiveResponse(8);

  Serial.println("Time to read ST file");

  memcpy(nfc._data, adate2, 8);
  for (int i = 0; i < 8; i++)
    Serial.println(nfc._data[i] & 0xFF, HEX);


  nfc.sendCommand(6);
  nfc.receiveResponse(25);
  */
}


void loop() {
}
