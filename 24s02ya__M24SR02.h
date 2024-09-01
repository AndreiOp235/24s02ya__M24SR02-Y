#ifndef _24s02ya__M24SR02
#define _24s02ya__M24SR02

#include <Arduino.h>
#include <crc16.h>

#define CMD_GETI2CSESSION 0x26
#define CMD_KILLRFSESSION 0x52

void nopuri();

class nfcGadget {
public:
  ~nfcGadget();
  nfcGadget();
  static bool deviceConnected();
  void sendCommand(unsigned len);
  int receiveResponse(unsigned int len);

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