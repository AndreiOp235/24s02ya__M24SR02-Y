#include "24s02ya__M24SR02.h"
#include <Wire.h>
#include <crc16.h>
//#include "nopuri.h"
void (*resetFunc)(void) = 0;
const char asel[] = { 0x02, 0x00, 0xA4, 0x04, 0x00, 0x07, 0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01, 0x00, 0x35, 0xC0 };  //select app
char adate[] = { 0x03, 0x00, 0xA4, 0x00, 0x0C, 0x02, 0xE1, 0x01, 0xD2, 0xAF };                                           //select file
char adate1[] = { 0x02, 0x00, 0xB0, 0x00, 0x00, 0x02, 0x6B, 0x7D };                                                      //read CC length
char adate2[] = { 0x03, 0x00, 0xB0, 0x00, 0x00, 0x17, 0xA5, 0xA2 };

void nfcGadget::selectNFCapp() {
  if (_verbose) {
    Serial.println("Select NFC T4 application");
  }

  // Resize buffer to ensure it can hold the required data
  _data = (char*)realloc(_data, 15 * sizeof(char));

  // Check for successful memory reallocation
  if (_data == nullptr) {
    Serial.println("Memory reallocation failed");
#ifdef RESET
    if (!this->deviceConnected())
      resetFunc();
#endif
#ifndef RESET
    return;
#endif
  }

  // Safely copy the data
  memcpy(_data, asel, 14);

  // Send command and receive response
  sendCommand(14);
  receiveResponse(5);
  interpretAnswer(5);
}


bool nfcGadget::interpretAnswer(int expectedLength) {
  // Extract status words from the response
  uint8_t Sw1 = _response[expectedLength - 4];
  uint8_t Sw2 = _response[expectedLength - 3];

  // Verbose output for debugging
  if (_verbose) {
    switch (Sw1) {
      case 0x62:
        Serial.println(Sw2 == 0x80 ? F("File overflow (Le error)") : F("End of file or record reached before reading Le bytes"));
        break;
      case 0x90:
        Serial.println(F("Command completed successfully"));
        break;
      default:
        Serial.println(F("Unknown status word"));
        break;
    }
  }

  // Return true if status word indicates success, false otherwise
  return Sw1 == 0x90;
}


void nfcGadget::selectFile(int opt) {
  _opt = opt;

  // Set adate values based on file type
  switch (opt) {
    case NDEFfile:
      adate[6] = 0x00;
      adate[7] = 0x01;
      break;

    case Systemfile:
      adate[6] = 0xE1;
      adate[7] = 0x01;
      break;

    case CCfile:
      adate[6] = 0xE1;
      adate[7] = 0x03;
      break;

    default:
      Serial.println(F("UNKNOWN file !!!"));
      return;  // Exit function early on unknown file
  }

  // Reallocate _data and check for success
  _data = (char*)realloc(_data, 9 * sizeof(char));

  if (_data == NULL) {
    Serial.println(F("Memory allocation failed!"));
    return;  // Exit function early on allocation failure
  }

  // Copy adate content to _data
  memcpy(_data, adate, 8);

  // Send command, receive response, and interpret answer
  sendCommand(8);
  receiveResponse(5);
  interpretAnswer(5);
}


int nfcGadget::readFileLength() {
  if (_opt == 1 || _opt == 2)
    adate1[0] = 0x02;
  else if (_opt == 3)
    adate1[0] = 0x03;
  // Reallocate memory for _data
  _data = (char*)realloc(_data, 7 * sizeof(char));
  if (_data == NULL) {
    Serial.println(F("Memory allocation failed!"));
    return -1;  // Return an error value if realloc fails
  }

  // Copy adate1 to _data
  memcpy(_data, adate1, 6);

  // Send command, receive response, and interpret answer
  sendCommand(6);
  receiveResponse(7);

  // Interpret response and calculate file length
  if (interpretAnswer(7)) {
    uint8_t MSB = _response[1] & 0xFF;
    uint8_t LSB = _response[2] & 0xFF;

    fileLength = ((MSB << 8) & 0xFF00) + LSB;
    return fileLength;
  }

  return -1;  // Return an error value if the answer is not interpreted successfully
}

char* nfcGadget::readFile() {
  if (_opt == 1)
    adate2[0] = 0x02;
  else if (_opt == 3)
    adate2[0] = 0x03;
  else if (_opt == 2)
    adate2[0] = 0x02;
  else {
    Serial.println("_opt has been altered");
    return 0;
  }

  if (fileLength > 25) {
    longRead();
  } else {
    adate2[5] = fileLength;  //Le aka number of bytes to read
    _data = (char*)realloc(_data, 7 * sizeof(char));
    memcpy(_data, adate2, 6);
    sendCommand(6);
    receiveResponse(fileLength + 5);
    interpretAnswer(fileLength + 5);
  }

  return 0;
}

char* nfcGadget::longRead() {
  uint8_t temp = fileLength;
  int offset = -20;
  adate2[5] = 20;

  while (temp > 20) {
    _data = (char*)realloc(_data, 7 * sizeof(char));
    memcpy(_data, adate2, 6);
    sendCommand(6);
    receiveResponse(20 + 5);
    interpretAnswer(20 + 5);

    temp -= 20;
    longAdd(20);
    adate2[4] += 20;
  }
  adate2[5] = temp;
  _data = (char*)realloc(_data, 7 * sizeof(char));
  memcpy(_data, adate2, 6);
  sendCommand(6);
  receiveResponse(temp + 5);
  longAdd(temp);
  interpretAnswer(temp + 5);


  return 0;
}

void nfcGadget::longAdd(int temp) {
  if (!_ndef) {
    _ndef = malloc(temp * sizeof(uint8_t));
    if (!_ndef) {
      Serial.println("Insuficient memory !!!");
#ifdef RESET
      if (!this->deviceConnected())
        resetFunc();
#endif
    } else {
      memcpy(_ndef, (_response + 1), temp);
    }
  } else {
    realloc(_ndef, (adate2[4] + temp) * sizeof(uint8_t));
    memcpy((_ndef + adate2[4]), (_response + 1), temp);
  }
}


nfcGadget::nfcGadget() {

  _data = malloc(2 * sizeof(char));  //added to make sure it`s never blank
#ifdef RESET
  if (!this->deviceConnected())  //hard reset of uC
    resetFunc();
#endif
}

nfcGadget::~nfcGadget() {
  if (_response) {  // Check if _response is not null
    free(_response);
    _response = nullptr;  // Set to nullptr to avoid dangling pointer
  }

  if (_ndef) {  // If _ndef was dynamically allocated, free it
    free(_ndef);
    _ndef = nullptr;  // Prevent dangling pointer
  }

  if (_data) {  // If _ndef was dynamically allocated, free it
    free(_data);
    _data = nullptr;  // Prevent dangling pointer
  }
}


bool nfcGadget::deviceConnected() {

  Wire.beginTransmission(0x2D);  // Start I2C transmission to a device with given address
  if (!Wire.endTransmission())
    Wire.beginTransmission(0x56);  // send a init byte
  if (!Wire.endTransmission())
    return true;
  return false;  // Device is not connected if there was an error
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
      if (!deviceConnected()) {
        resetFunc();
      }
#endif
      break;
  }
}


void nfcGadget::explainCC() {
  char* pointer = _response + 1;
  Serial.print(F("Number of bytes in CC file 0x"));
  int temp = (((pointer[0] << 8) & 0xff00) + *(++pointer));
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
  temp = (((pointer[0] << 8) & 0xff00) + (*(++pointer)) & 0xff);
  Serial.print(temp, HEX);
  Serial.print(" = ");
  Serial.println(temp, DEC);

  pointer++;
  Serial.print(F("Maximum number of bytes that can be written 0x"));
  temp = (((pointer[0] << 8) & 0xff00) + (*(++pointer)) & 0xff);
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
  temp = (((pointer[0] << 8) & 0xff00) + (*(++pointer)) & 0xff);
  Serial.println(temp, HEX);

  pointer++;
  Serial.print(F("Maximum NDEF file size in bytes 0x"));
  temp = (((pointer[0] << 8) & 0xff00) + (*(++pointer)) & 0xff);
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

// Helper macro for printing hex and decimal values
#define PRINT_HEX_DEC(label, value) \
  Serial.print(F(label)); \
  Serial.print(F("0x")); \
  Serial.print(value, HEX); \
  Serial.print(F(" = ")); \
  Serial.println(value, DEC)

  // Length system file
  int temp = (((pointer[0] << 8) & 0xff00) + *(++pointer));
  PRINT_HEX_DEC("Length system file: ", temp);

  // I2C protection
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

  // I2C Watchdog
  pointer++;
  Serial.print(F("I2C Watchdog 0x"));
  Serial.print(*pointer, HEX);
  if (*pointer == 0) {
    Serial.println(F(": watchdog disabled"));
  } else {
    Serial.print(F(" - watchdog waits for ms: "));
    Serial.print(((*pointer) & 0x03) * 30);
  }

  // GPO Status
  pointer++;
  Serial.print(F("GPO Status 0x"));
  Serial.println(*pointer & 0xFF, HEX);
  Serial.println(F("Given a RF session:"));
  uint8_t MSB = ((*pointer) & 0b01110000) >> 4;

  Serial.print(F("RF Session: "));
  switch (MSB) {
    case 0b000: Serial.println(F("High impedance")); break;
    case 0b001: Serial.println(F("Session opened")); break;
    case 0b010: Serial.println(F("WIP")); break;
    case 0b011: Serial.println(F("MIP")); break;
    case 0b100: Serial.println(F("Interrupt")); break;
    case 0b101: Serial.println(F("State control")); break;
    case 0b110: Serial.println(F("RF busy")); break;
    case 0b111: Serial.println(F("RFU")); break;
    default: Serial.println(F("Unknown state")); break;
  }

  Serial.println(F("Given a I2C session:"));
  uint8_t LSB = (*pointer) & 0b00000111;

  Serial.print(F("I2C Session: "));
  switch (LSB) {
    case 0b000: Serial.println(F("High impedance")); break;
    case 0b001: Serial.println(F("Session opened")); break;
    case 0b010: Serial.println(F("WIP")); break;
    case 0b011: Serial.println(F("MIP")); break;
    case 0b100: Serial.println(F("Interrupt")); break;
    case 0b101: Serial.println(F("State control")); break;
    case 0b110: Serial.println(F("RF busy")); break;
    case 0b111: Serial.println(F("RFU")); break;
    default: Serial.println(F("Unknown state")); break;
  }

  // ST reserved
  pointer++;
  Serial.print(F("ST reserved 0x"));
  Serial.println(*pointer, HEX);

  // RF enable
  pointer++;
  Serial.print(F("RF enable 0x"));
  Serial.println(*pointer, HEX);
  if (*pointer & 0b10000000) {
    Serial.println(F(" the RF field is on"));
  } else {
    Serial.println(F(" the RF field is ooff"));
  }
  if (*pointer & 0b00001000) {
    Serial.println(F(" the RF disable pad is at high state"));
  } else {
    Serial.println(F(" the RF disable pad is at low state"));
  }
  if (*pointer & 0b00000001) {
    Serial.println(F(" the M24SR02-Y decodes the command received from the RF interface"));
  } else {
    Serial.println(F(" the M24SR02-Y does not decode the command received from the RF interface"));
  }

  // NDEF file number (RFU)
  pointer++;
  Serial.print(F("NDEF file number (RFU) 0x"));
  Serial.println(*pointer, HEX);

  //UID
  Serial.print(F("UID 0x"));
  for (int i = 0; i < 7; i++) {
    pointer++;
    Serial.print((*pointer) & 0xff, HEX);
    Serial.print(" ");
  }

  Serial.println("");

  // Memory size
  pointer++;
  temp = (((pointer[0] << 8) & 0xff00) + *(++pointer)) & 0xff;
  PRINT_HEX_DEC("Memory size in bytes: ", temp);

    // Product code
  pointer++;
  Serial.print(F("Product code 0x"));
  Serial.println((*pointer) & 0xff, HEX);
}

void nfcGadget::explainNDEF() {
  uint8_t* cursor = _ndef;
  bool MB = 0, ME = 0, SR = 0, IL = 0;
  for (int i = 0; i < fileLength; i++) {
    Serial.print(_ndef[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  uint8_t MSB = (_ndef[0] << 8) & 0x00FF;
  uint8_t LSB = _ndef[1] & 0xFF;
  if ((MSB + LSB) == fileLength) {
    Serial.println(F("Length header is correct !"));
  } else {
    Serial.println(F("Length header is not correct ! EXIT"));
    return;  // Insert NOP instruction here
  }
  cursor++;
  cursor++;

  if (cursor[0] & 0b10000000) {
    Serial.println(F("Message Begin is set !"));
    MB = true;
  } else {
    Serial.println(F("Message Begin is not set !"));
  }

  if (cursor[0] & 0b01000000) {
    Serial.println(F("Message End is set !"));
    ME = true;
  } else {
    Serial.println(F("Message End is not set !"));
  }

  if (cursor[0] & 0b00100000) {
    Serial.println(F("Chunk Flag is set (payload isn't complete) !"));
  } else {
    Serial.println(F("Chunk Flag is not set !"));
  }

  if (cursor[0] & 0b00010000) {
    Serial.println(F("Short Record is set !"));
    SR = true;
  } else {
    Serial.println(F("Short Record is not set !"));
  }

  if (cursor[0] & 0b00001000) {  // Fixed typo in the bit mask (it should be 0b00001000)
    Serial.println(F("ID Length present is set !"));
    IL = 1;
  } else {
    Serial.println(F("ID Length present is not set !"));
  }

  Serial.print(F("Type Name Format is 0x"));
  Serial.print(cursor[0] & 0b00000111, HEX);  // Fixed typo in the bit mask (it should be 0b00000111)

  switch (cursor[0] & 0b00000111)  // Fixed typo in the bit mask
  {
    case 0x00:
      Serial.println(F(" Empty"));
      break;
    case 0x01:
      Serial.println(F(" NFC Forum well-known type [NFC RTD]"));
      break;
    case 0x02:
      Serial.println(F(" Media-type [RFC 2046]"));
      break;
    case 0x03:
      Serial.println(F(" Absolute URI [RFC 3986]"));
      break;
    case 0x04:
      Serial.println(F(" NFC Forum external type [NFC RTD]"));
      break;
    case 0x05:
      Serial.println(F(" Unknown"));
      break;
    case 0x06:
      Serial.println(F(" Unchanged"));
      break;
    case 0x07:
      Serial.println(F(" Reserved"));
      break;
  }


  cursor++;
  Serial.print("Type has a length of 0x");
  Serial.println(*cursor, HEX);

  uint8_t payloadL = *cursor;
  int payloadSize = 0;

  for (int i = 0; i < payloadL; i++) {
    cursor++;
    payloadSize = ((payloadSize << 8) + (*cursor));
  }

  Serial.print("Payload is ");
  Serial.print(payloadSize, DEC);
  Serial.println(" Bytes long");
  cursor++;


  if (*cursor == 0x54) {
    Serial.println(F(" The payload is a TEXT"));
    handleTXT(cursor, payloadSize);
  } else if (*cursor == 0x55) {
    Serial.println(F("The payload is a URI"));
    handleURI(cursor, payloadSize);
  }
  cursor += payloadSize + 1;

  if (!ME) {
    /*
    Serial.println("DEMON");
    Serial.println(int(cursor - _ndef));
    */

    while ((int(cursor - _ndef)) < fileLength) {
      cursor++;
      payloadL = *cursor;
      payloadSize = 0;

      for (int i = 0; i < payloadL; i++) {
        cursor++;
        payloadSize = ((payloadSize << 8) + (*cursor));
      }
      Serial.print("Payload is ");
      Serial.print(payloadSize, DEC);
      Serial.println(" Bytes long");

      Serial.print(cursor[-2], HEX);

      if (cursor[-2] == 0x54) {
        Serial.println(F(" The payload is a TEXT"));
        handleTXT(cursor, payloadSize);
      } else if (cursor[-2] == 0x55) {
        Serial.println(F("The payload is a URI"));
        handleURI(cursor, payloadSize);
      }

      cursor += payloadSize;

      Serial.println("DEMON");
      Serial.println(uint8_t(cursor - _ndef));

      Serial.println("HELP");
      Serial.println(*cursor, HEX);
    }
  }
}

void nfcGadget::handleTXT(uint8_t* cursor, uint8_t lungime) {
  cursor++;
  if (*cursor == 0x01) {
    Serial.println(F("utf-8 encoding"));
  } else {
    Serial.print(F("unknown"));
  }


  for (int i = 1; i < lungime - 1; i++) {
    Serial.print(char(cursor[i]));
  }

  Serial.println("");
}

void nfcGadget::handleURI(uint8_t* cursor, uint8_t lungime) {
  cursor++;
  // Array of URI prefixes
  const char* uriPrefixes[] = {
    "",              // 0x00 - No prefix
    "http://www.",   // 0x01
    "https://www.",  // 0x02
    "http://",       // 0x03
    "https://"       // 0x04
  };

  // Get the prefix index from the cursor
  uint8_t prefixIndex = *cursor;
  
  // Check for valid prefix index
  if (prefixIndex >= 0x01 && prefixIndex <= 0x04) {
    Serial.print(uriPrefixes[prefixIndex]);
  }
  
  // Print the rest of the URI
  Serial.write(cursor + 1, lungime - 1);
  Serial.println(); // New line after URI
}


void nfcGadget::sendCommand(unsigned len) {
  uint8_t v;

  if (_sendGetI2cSession) {
    Wire.beginTransmission(_deviceaddress);  // transmit to device 0x2D
    Wire.write(byte(CMD_GETI2CSESSION));     // GetI2Csession
    _err = Wire.endTransmission();           // stop transmitting
    /*
    if (_verbose) {
      Serial.print(F("\nGetI2Csession: "));
      Serial.print(_err, HEX);
    } else
      delay(1);*/
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
