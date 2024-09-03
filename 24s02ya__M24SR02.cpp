#include "24s02ya__M24SR02.h"
#include <Wire.h>
#include <crc16.h>
#include "nopuri.h"

char asel[] = { 0x02, 0x00, 0xA4, 0x04, 0x00, 0x07, 0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01, 0x00, 0x35, 0xC0 };  //select app
char adate[] = { 0x03, 0x00, 0xA4, 0x00, 0x0C, 0x02, 0xE1, 0x01, 0xD2, 0xAF };                                     //select file
char adate1[] = { 0x02, 0x00, 0xB0, 0x00, 0x00, 0x02, 0x6B, 0x7D };                                                //read CC length
char adate2[] = { 0x03, 0x00, 0xB0, 0x00, 0x00, 0x17, 0xA5, 0xA2 };

void nfcGadget::selectNFCapp() {
  if (_verbose)
    Serial.println("Select NFC T4 application");

  memcpy(_data, asel, 14);
  sendCommand(14);
  receiveResponse(5);
  interpretAnswer(5);
}

bool nfcGadget::interpretAnswer(int expectedLength) {
  uint8_t Sw1 = _response[expectedLength - 4] & 0xFF;
  uint8_t Sw2 = _response[expectedLength - 3] & 0xFF;

  if (_verbose) {
    switch (Sw1) {
      case 0x62:
        if (Sw2 == 0x80)
          Serial.println(F("File overflow (Le error)"));
        else
          Serial.println(F("End of file or record reached before reading Le bytes"));
        break;
      case 0x90:
        Serial.println(F("Command completed successfully"));
        break;
      default:
        Serial.println(F("Unknown status word"));
        break;
    }
  }

  if (Sw1 == 0x90)
    return true;
  else
    return false;
}

void nfcGadget::selectFile(int opt) {
  _opt = opt;
  switch (opt) {
    case 1:
      {
        adate[6] = 0x00;
        adate[7] = 0x01;
      }
      break;

    case 2:
      {
        adate[6] = 0xE1;
        adate[7] = 0x01;
      }
      break;

    case 3:
      {
        adate[6] = 0xE1;
        adate[7] = 0x03;
      }
      break;
    default:
      Serial.println("UNKNOWN file !!!");
      break;
  }

  memcpy(_data, adate, 8);
  sendCommand(8);
  receiveResponse(5);
  interpretAnswer(5);
}

int nfcGadget::readFileLength() {
  if (_opt == 1)
    adate1[0] = 0x02;
  else if (_opt == 3)
    adate1[0] = 0x03;

  memcpy(_data, adate1, 6);
  sendCommand(6);
  receiveResponse(7);
  if (interpretAnswer(7)) {
    uint8_t MSB = _response[1] & 0xFF;
    uint8_t LSB = _response[2] & 0xFF;

    fileLength = ((MSB << 2) & 0xFF00) + LSB;
    return fileLength;
  }
}

char* nfcGadget::readFile() {
  if (_opt == 1)
    adate2[0] = 0x02;
  else if (_opt == 3)
    adate2[0] = 0x03;

  adate2[5] = fileLength;  //Le aka number of bytes to read

  memcpy(_data, adate2, 6);
  sendCommand(6);
  receiveResponse(fileLength + 5);
  interpretAnswer(fileLength + 5);



  return 0;
}

nfcGadget::nfcGadget() {
#ifdef RESET
  if (!this->deviceConnected())
    resetFunc();
#endif
}

nfcGadget::~nfcGadget() {
  if (_responseLength) {
    free(_response);
  }
}

bool nfcGadget::deviceConnected() {

  Wire.beginTransmission(0x2D);  // Start I2C transmission to a device with given address
  if (!Wire.endTransmission())
    Wire.beginTransmission(0x56);
  if (!Wire.endTransmission())
    return true;
  return false;
}

void nfcGadget::explainFile() {
  switch (_opt) {
    case CCfile:
      explainCC();
      break;
    case Systemfile:
      explainSystem();
      break;
    case NDEFfile:
      explainNDEF();
      break;
    default:
      Serial.println("opt has been altered !!!");
#ifdef RESET
      if (!this->deviceConnected())
        resetFunc();
#endif
      break;
  }
}

void nfcGadget::explainCC() {
  char* pointer = _response + 1;
  Serial.print(F("Number of bytes in CC file 0x"));
  int temp = (((pointer[0] << 2) & 0xff00) + *(++pointer));
  Serial.print(temp, HEX);
  Serial.print(" = ");
  Serial.println(temp, DEC);

  pointer++;
  Serial.print(F("Mapping version 0x"));
  Serial.print(*pointer, HEX);
  Serial.print(" = V");
  switch (*pointer & 0xFF) {
    case 0x10:
      Serial.println("1.0");
      break;
    case 0x20:
      Serial.println("2.0");
      break;
    default:
      Serial.println(" UNKNOWN");
      break;
  }

  pointer++;
  Serial.print(F("Maximum number of bytes that can be read 0x"));
  temp = (((pointer[0] << 2) & 0xff00) + (*(++pointer)) & 0xff);
  Serial.print(temp, HEX);
  Serial.print(" = ");
  Serial.println(temp, DEC);

  pointer++;
  Serial.print(F("Maximum number of bytes that can be written 0x"));
  temp = (((pointer[0] << 2) & 0xff00) + (*(++pointer)) & 0xff);
  Serial.print(temp, HEX);
  Serial.print(" = ");
  Serial.println(temp, DEC);

  Serial.println(F("NDEF file control TLV"));
  pointer++;
  Serial.print(F("T field 0x"));
  Serial.println(*pointer, HEX);

  pointer++;
  Serial.print(F("L field 0x"));
  Serial.println(*pointer, HEX);

  pointer++;
  Serial.print(F("FileID 0x"));
  temp = (((pointer[0] << 2) & 0xff00) + (*(++pointer)) & 0xff);
  Serial.println(temp, HEX);

  pointer++;
  Serial.print(F("Maximum NDEF file size in bytes 0x"));
  temp = (((pointer[0] << 2) & 0xff00) + (*(++pointer)) & 0xff);
  Serial.print(temp, HEX);
  Serial.print(" = ");
  Serial.println(temp, DEC);

  pointer++;
  Serial.print(F("Read acces 0x"));
  Serial.print(*pointer & 0xFF, HEX);

  switch (*pointer & 0xFF) {
    case 0x00:
      Serial.println(F(" (Read access without any security)"));
      break;
    case 0x80:
      Serial.println(F(" (Locked )"));
      break;
    case 0xFE:
      Serial.println(F(" (Read not authorized  )"));
      break;
    default:
      Serial.println(F(" (UNKNOWN)"));
      break;
  }

  pointer++;
  Serial.print(F("Write acces 0x"));
  Serial.print(*pointer & 0xFF, HEX);

  switch (*pointer & 0xFF) {
    case 0x00:
      Serial.println(F(" (Write access without any security)"));
      break;
    case 0x80:
      Serial.println(F(" (Locked )"));
      break;
    case 0xFF:
      Serial.println(F(" (Write not authorized)"));
      break;
    default:
      Serial.println(F(" (UNKNOWN)"));
      break;
  }
}

void nfcGadget::explainSystem() {
  char* pointer = _response + 1;
  Serial.print(F("Length system file 0x"));
  int temp = (((pointer[0] << 2) & 0xff00) + *(++pointer));
  Serial.print(temp, HEX);
  Serial.print(" = ");
  Serial.println(temp, DEC);

  pointer++;
  Serial.print(F("I2C protection 0x"));
  Serial.print(*pointer, HEX);
  switch (*pointer & 0xFF) {
    case 0x00:
      Serial.println(F(": the I²C host has the SuperUser right access without sending the I²C password"));
      break;
    case 0x01:
      Serial.println(F(": the I²C host has the SuperUser right access after sending the I²C password"));
      break;
    default:
      Serial.println(F(" UNKNOWN rigths"));
      break;
  }

  pointer++;
  Serial.print(F("I2C Watchdog 0x"));
  Serial.print(*pointer, HEX);
  if (*pointer == 0) {
    Serial.println(F(": watchdog disabled"))
      :
  } else {
    Serial.print(" - watchdog waits for ms: ")
      Serial.print((*pointer) & 0x03);
  }

  pointer++;
  Serial.print(F("GPO Status 0x"));
  Serial.println(*pointer & 0xFF, HEX);
  Serial.println(F("Given a RF session:"));
  uint8_t MSB = (*pointer) & 0b01110000;

  switch (MSB) {
    case 0b000:  // 0 in binary
      Serial.println(F("High impedance"));
      break;
    case 0b001:  // 1 in binary
      Serial.println(F("Session opened"));
      break;
    case 0b010:  // 2 in binary
      Serial.println(F("WIP"));
      break;
    case 0b011:  // 3 in binary
      Serial.println(F("MIP"));
      break;
    case 0b100:  // 4 in binary
      Serial.println(F("Interrupt"));
      break;
    case 0b101:  // 5 in binary
      Serial.println(F("State control"));
      break;
    case 0b110:  // 6 in binary
      Serial.println(F("RF busy"));
      break;
    case 0b111:  // 7 in binary
      Serial.println(F("RFU"));
      break;
    default:
      Serial.println(F("Unknown state"));
      break;
  }

  Serial.println(F("Given a I2C session:"));
  uint8_t LSB = (*pointer) & 0b00000111;

  switch (LSB) {
    case 0b000:  // 0 in binary
      Serial.println(F("High impedance"));
      break;
    case 0b001:  // 1 in binary
      Serial.println(F("Session opened"));
      break;
    case 0b010:  // 2 in binary
      Serial.println(F("WIP"));
      break;
    case 0b011:  // 3 in binary
      Serial.println(F("MIP"));
      break;
    case 0b100:  // 4 in binary
      Serial.println(F("Interrupt"));
      break;
    case 0b101:  // 5 in binary
      Serial.println(F("State control"));
      break;
    case 0b110:  // 6 in binary
      Serial.println(F("RF busy"));
      break;
    case 0b111:  // 7 in binary
      Serial.println(F("RFU"));
      break;
    default:
      Serial.println(F("Unknown state"));
      break;
  }

  pointer++;
  Serial.print(F("ST reserved 0x"));
  Serial.println(*pointer, HEX);

  pointer++;
  Serial.print(F("Maximum number of bytes that can be read 0x"));
  temp = (((pointer[0] << 2) & 0xff00) + (*(++pointer)) & 0xff);
  Serial.print(temp, HEX);
  Serial.print(" = ");
  Serial.println(temp, DEC);

  pointer++;
  Serial.print(F("Maximum number of bytes that can be written 0x"));
  temp = (((pointer[0] << 2) & 0xff00) + (*(++pointer)) & 0xff);
  Serial.print(temp, HEX);
  Serial.print(" = ");
  Serial.println(temp, DEC);



  pointer++;
  Serial.print(F("L field 0x"));
  Serial.println(*pointer, HEX);

  pointer++;
  Serial.print(F("FileID 0x"));
  temp = (((pointer[0] << 2) & 0xff00) + (*(++pointer)) & 0xff);
  Serial.println(temp, HEX);

  pointer++;
  Serial.print(F("Maximum NDEF file size in bytes 0x"));
  temp = (((pointer[0] << 2) & 0xff00) + (*(++pointer)) & 0xff);
  Serial.print(temp, HEX);
  Serial.print(" = ");
  Serial.println(temp, DEC);

  pointer++;
  Serial.print(F("Read acces 0x"));
  Serial.print(*pointer & 0xFF, HEX);

  switch (*pointer & 0xFF) {
    case 0x00:
      Serial.println(F(" (Read access without any security)"));
      break;
    case 0x80:
      Serial.println(F(" (Locked )"));
      break;
    case 0xFE:
      Serial.println(F(" (Read not authorized  )"));
      break;
    default:
      Serial.println(F(" (UNKNOWN)"));
      break;
  }

  pointer++;
  Serial.print(F("Write acces 0x"));
  Serial.print(*pointer & 0xFF, HEX);

  switch (*pointer & 0xFF) {
    case 0x00:
      Serial.println(F(" (Write access without any security)"));
      break;
    case 0x80:
      Serial.println(F(" (Locked )"));
      break;
    case 0xFF:
      Serial.println(F(" (Write not authorized)"));
      break;
    default:
      Serial.println(F(" (UNKNOWN)"));
      break;
  }
}

void nfcGadget::explainNDEF() {
  asm volatile("nop");
}


void nfcGadget::sendCommand(unsigned len) {
  uint8_t v;

  if (_sendGetI2cSession) {
    Wire.beginTransmission(_deviceaddress);  // transmit to device 0x2D
    Wire.write(byte(CMD_GETI2CSESSION));     // GetI2Csession
    _err = Wire.endTransmission();           // stop transmitting
    if (_verbose) {
      Serial.print(F("\nGetI2Csession: "));
      Serial.print(_err, HEX);
    } else
      delay(1);
  }

  if (_cmds)
    Serial.print(F("\r\n=> "));
  else
    delay(1);

  Wire.beginTransmission(_deviceaddress);

  for (int i = 0; i < len; i++) {
    v = (_data[i] & 0xff);
    if (_cmds) {
      if (v < 0x10)
        Serial.print(F("0"));
      Serial.print(v, HEX);
    } else
      delay(5);
    Wire.write(byte(v & 0xff));
    if (_cmds)
      Serial.print(F(" "));
    else
      delay(1);
  }

  //5.5 CRC of the I2C and RF frame ISO/IEC 13239. The initial register content shall be 0x6363
  int chksum = crcsum((unsigned char*)_data, len, 0x6363);

  v = chksum & 0xff;
  if (_cmds) {
    if (v < 0x10) {
      Serial.print(F("0"));
    }
    Serial.print(v, HEX);
  } else
    delay(1);

  Wire.write(byte(v & 0xff));

  //EOD field
  v = (chksum >> 8) & 0xff;
  if (_cmds) {
    if (v < 0x10) {
      Serial.print(F("0"));
    }
    Serial.print(v, HEX);
  } else
    delay(1);
  Wire.write(byte(v & 0xff));

  _err = Wire.endTransmission();
  if (_cmds) {
    Serial.print(F("\r\n"));
  } else {
    delay(1);
  }

  if (_err != 0) {
    Serial.print(F("write err: "));
    Serial.print(_err, HEX);
  }
}

int nfcGadget::receiveResponse(unsigned int len) {
  int index = 0;
  boolean WTX = false;
  boolean loop = false;
  if (_verbose) {
    Serial.print(F("\r\nreceiveResponse, len="));
    Serial.print(len, DEC);
    Serial.println();
  }
  if (_responseLength < len) {
    free(_response);
    _responseLength = len;
    _response = (byte*)malloc(_responseLength);
  } else
    delay(1);
  do {
    WTX = false;
    loop = false;
    Wire.requestFrom(_deviceaddress, len);
    if (_cmds)
      Serial.print(F("<= "));
    else
      delay(1);
    while ((Wire.available() && index < len && !WTX) || (WTX && index < len - 1)) {
      int c = (Wire.read() & 0xff);
      if (_cmds) {
        if (c < 0x10)
          Serial.print(F("0"));
        Serial.print(c, HEX);
        Serial.print(F(" "));
      } else
        delay(1);
      if (c == 0xF2 && index == 0) {
        WTX = true;
      }

      _response[index] = c;
      index++;
    }
    if (WTX) {
      Serial.print(F("\r\nWTX"));
      delay(200 * _response[0]);
      //send WTX response
      //sendSBLOCK(0xF2);
      _data[0] = 0xF2;  //WTX
      _data[1] = _response[0];

      sendCommand(/*_data,*/ 2 /*, false*/);
      loop = true;
      index = 0;
    }
  } while (loop);

  Serial.println("");
  return index;
}
