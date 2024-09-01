#include <Wire.h>
#include "24s02ya__M24SR02.h"

nfcGadget nfc;

void setup() {
  Wire.begin();  //required for device check
  Serial.begin(9600);
  while (!Serial)
    ;  // Wait for Serial Monitor to open
  Serial.println("interfata NFC");

  nfc._verbose = true;
  nfc._cmds = true;

  nfc._sendGetI2cSession = true;
  Serial.println("Open ");

  char sel[] = "\x02\x00\xA4\x04\x00\x07\xD2\x76\x00\x00\x85\x01\x01\x00\x35\xC0";
  Serial.println(crcsum(sel, 14, 0x6363), HEX);
  memcpy(nfc._data, sel, 16);
  for (int i = 0; i < 16; i++)
    Serial.println(nfc._data[i] & 0xFF, HEX);
  nfc.sendCommand(14);

  nfc.receiveResponse(2 + 3);
  Serial.println("P3");

  char date[] = { 0x03, 0x00, 0xA4, 0x00, 0x0C, 0x02, 0xE1, 0x01, 0xD2, 0xAF };  //AIUCI
  memcpy(nfc._data, date, 10);
  for (int i = 0; i < 10; i++)
    Serial.println(nfc._data[i] & 0xFF, HEX);


  nfc.sendCommand(8);
  nfc.receiveResponse(6);

  Serial.println("Time to read ST file length");
  char date1[] = { 0x02, 0x00, 0xB0, 0x00, 0x00, 0x02, 0x6B, 0x7D };
  memcpy(nfc._data, date1, 8);
  for (int i = 0; i < 8; i++)
    Serial.println(nfc._data[i] & 0xFF, HEX);


  nfc.sendCommand(6);
  nfc.receiveResponse(8);

  Serial.println("Time to read ST file");
  char date2[] = { 0x03, 0x00, 0xB0, 0x00, 0x00, 0x0F, 0xA5, 0xA2 };
  memcpy(nfc._data, date2, 8);
  for (int i = 0; i < 8; i++)
    Serial.println(nfc._data[i] & 0xFF, HEX);


  nfc.sendCommand(6);
  nfc.receiveResponse(25);
}


void loop() {
}
