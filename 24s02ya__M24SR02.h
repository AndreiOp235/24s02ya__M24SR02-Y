#ifndef _24s02ya__M24SR02
#define _24s02ya__M24SR02

#include <Arduino.h>
#include <crc16.h>

#define CMD_GETI2CSESSION 0x26
#define CMD_KILLRFSESSION 0x52

void nopuri();

#define NDEFfile 1
#define Systemfile 2
#define CCfile 3 

extern char asel[];
extern char adate[];
extern char adate1[];
extern char adate2[];

class nfcGadget {
public:
  ~nfcGadget();
  nfcGadget();
  static bool deviceConnected();
  void sendCommand(unsigned len);
  int receiveResponse(unsigned int len);
  void selectNFCapp();
  void selectFile(int opt);

  bool interpretAnswer(int expectedLentgh);
  int readFileLength();
  char* readFile();
  char* longRead();
  void explainFile();

  void longAdd(int temp);

  boolean _verbose;
  boolean _cmds;
  char _data[100];
  uint8_t _deviceaddress = 0x56;
  boolean _sendGetI2cSession = true;
  uint8_t _err;

  int fileLength=0;
  uint8_t _responseLength = 0x0;
  uint8_t* _response = 0;
  uint8_t* _ndef = 0;

  private:
  uint8_t _opt;
  void explainCC();
  void explainSystem();
  void explainNDEF();
};

#endif