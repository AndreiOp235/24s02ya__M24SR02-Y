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

  Serial.println("NDEF file");
  nfc.selectFile(NDEFfile);
  int j=nfc.readFileLength();

  nfc.readFile();
  nfc.explainFile();
  Serial.println("");

  

}


void loop() {
}
