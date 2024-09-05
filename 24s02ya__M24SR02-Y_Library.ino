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


  nfc._verbose = false;
  nfc._cmds = false;
  nfc._sendGetI2cSession = true;
  Serial.println("Open ");
  nfc.selectNFCapp(); //not mandatory

  Serial.println("CC file");
  nfc.selectFile(CCfile);
  int j=nfc.readFileLength();

  nfc.readFile();
  nfc.explainFile();
  Serial.println("");

    nfc.selectNFCapp(); //not mandatory

  Serial.println("ST file");
  nfc.selectFile(Systemfile);
  j=nfc.readFileLength();

  nfc.readFile();
  nfc.explainFile();
  Serial.println("");

    nfc.selectNFCapp(); //not mandatory

  Serial.println("NDEF file");
  nfc.selectFile(NDEFfile);
  j=nfc.readFileLength();

  nfc.readFile();
  nfc.explainFile();
  Serial.println("");

}


void loop() {
}
