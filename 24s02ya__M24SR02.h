#ifndef _24s02ya__M24SR02
#define _24s02ya__M24SR02

#include <Arduino.h>
#include <crc16.h>

#define CMD_GETI2CSESSION 0x26
#define CMD_KILLRFSESSION 0x52

void nopuri();

char asel[] = "\x02\x00\xA4\x04\x00\x07\xD2\x76\x00\x00\x85\x01\x01\x00\x35\xC0";
char adate[] = { 0x03, 0x00, 0xA4, 0x00, 0x0C, 0x02, 0xE1, 0x01, 0xD2, 0xAF };  //AIUCI
char adate1[] = { 0x02, 0x00, 0xB0, 0x00, 0x00, 0x02, 0x6B, 0x7D };
char adate2[] = { 0x03, 0x00, 0xB0, 0x00, 0x00, 0x0F, 0xA5, 0xA2 };

class nfcGadget {
public:
  ~nfcGadget();
  nfcGadget();
  static bool deviceConnected();
  void sendCommand(unsigned len);
  int receiveResponse(unsigned int len);
  void selectNFCapp();

  boolean _verbose;
  boolean _cmds;
  char _data[100];
  uint8_t _deviceaddress = 0x56;
  boolean _sendGetI2cSession = true;
  uint8_t _err;
  uint8_t _blockNo = 0;

  uint8_t _responseLength = 0x0;
  uint8_t* _response = 0;
};

#endif