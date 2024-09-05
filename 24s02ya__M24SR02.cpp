#include "24s02ya__M24SR02.h"
#include <Wire.h>
#include "crc16.h"

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
  // Set adate2[0] based on _opt value, with default error handling
  switch (_opt) {
    case 1:
    case 2:
      adate2[0] = 0x02;
      break;
    case 3:
      adate2[0] = 0x03;
      break;
    default:
      Serial.println(F("_opt has been altered"));
      return nullptr; // Use nullptr instead of 0 for better type safety
  }

  // Check if fileLength exceeds the threshold to decide on longRead or direct read
  if (fileLength > 25) {
    longRead();
  } else {
    // Allocate or reallocate memory for _data and handle memory allocation
    char* newData = (char*)realloc(_data, 7 * sizeof(char));
    if (!newData) {
      Serial.println(F("Memory reallocation failed"));
      return nullptr;
    }
    _data = newData;

    // Set up adate2[5] for the number of bytes to read
    adate2[5] = fileLength;

    // Copy adate2 to _data and perform the command sequence
    memcpy(_data, adate2, 6);
    sendCommand(6);
    receiveResponse(fileLength + 5);
    interpretAnswer(fileLength + 5);
  }

  return _data; // Return the pointer to the data read
}


char* nfcGadget::longRead() {
  uint8_t temp = fileLength; // Copy the total file length to temp
  int offset = -20;          // Initialize offset (not used in the current code)
  adate2[5] = 20;            // Set initial read length to 20 bytes

  // Loop to read the file in chunks of 20 bytes
  while (temp > 20) {
    // Allocate memory for _data and check if the allocation was successful
    char* newData = (char*)realloc(_data, 7 * sizeof(char));
    if (newData == nullptr) {
      // Memory allocation failed, handle the error
      Serial.println(F("Memory allocation failed during chunk read."));
      free(_data); // Free previously allocated memory to avoid memory leaks
      return nullptr; // Return nullptr to indicate failure
    }
    _data = newData; // Update _data with the newly allocated memory

    // Prepare command data to read the next chunk of 20 bytes
    memcpy(_data, adate2, 6);
    sendCommand(6);                   // Send the command to read data
    receiveResponse(20 + 5);          // Receive the response (20 bytes of data + 5 bytes of status)
    interpretAnswer(20 + 5);          // Interpret the response status

    // Update the remaining file length and data pointer for the next chunk
    temp -= 20;                        // Decrease remaining length by 20 bytes
    longAdd(20);                       // Add the 20-byte chunk to the long data buffer
    adate2[4] += 20;                  // Update the offset in adate2 for the next read
  }

  // Handle the final chunk of data that is less than or equal to 20 bytes
  adate2[5] = temp;                   // Set the final read length to the remaining bytes
  char* finalData = (char*)realloc(_data, 7 * sizeof(char)); // Reallocate memory for the final chunk
  if (finalData == nullptr) {
    // Memory allocation failed, handle the error
    Serial.println(F("Memory allocation failed during final read."));
    free(_data); // Free previously allocated memory to avoid memory leaks
    return nullptr; // Return nullptr to indicate failure
  }
  _data = finalData; // Update _data with the newly allocated memory

  // Prepare and send the final command to read the remaining bytes
  memcpy(_data, adate2, 6);
  sendCommand(6);                   // Send the command to read the final chunk
  receiveResponse(temp + 5);        // Receive the final chunk response (remaining bytes + 5 bytes of status)
  longAdd(temp);                    // Add the remaining bytes to the long data buffer
  interpretAnswer(temp + 5);        // Interpret the final response status

  // Return the pointer to the data buffer
  return _data; // Return the pointer to the data buffer
}



void nfcGadget::longAdd(int temp) {
  // Check if the _ndef pointer is null (i.e., not allocated yet)
  if (!_ndef) {
    // Allocate memory for _ndef and check if allocation was successful
    _ndef = malloc(temp * sizeof(uint8_t));
    if (!_ndef) {
      Serial.println(F("Insufficient memory !!!"));
      // Optional: If the RESET macro is defined and device is not connected, reset the device
#ifdef RESET
      if (!this->deviceConnected()) {
        resetFunc();
      }
#endif
      return; // Exit function if memory allocation failed
    } else {
      // If memory allocation was successful, copy data from _response to _ndef
      memcpy(_ndef, (_response + 1), temp);
    }
  } else {
    // Allocate new memory for _ndef with additional space and check if allocation was successful
    uint8_t* newNdef = (uint8_t*)realloc(_ndef, (adate2[4] + temp) * sizeof(uint8_t));
    if (!newNdef) {
      Serial.println(F("Memory reallocation failed !!!"));
      // Optional: If the RESET macro is defined and device is not connected, reset the device
#ifdef RESET
      if (!this->deviceConnected()) {
        resetFunc();
      }
#endif
      return; // Exit function if memory reallocation failed
    }
    // Update _ndef with the newly allocated memory
    _ndef = newNdef;

    // Copy data from _response to the newly allocated space in _ndef
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
  // Set a pointer to the start of the response data, skipping the initial byte
  char* pointer = _response + 1;
  
  // Extract and print the number of bytes in the CC file
  Serial.print(F("Number of bytes in CC file 0x"));
  int temp = (((pointer[0] << 8) & 0xff00) + *(++pointer)); // Combine high and low bytes
  Serial.print(temp, HEX); // Print in hexadecimal format
  Serial.print(" = ");
  Serial.println(temp, DEC); // Print in decimal format

  // Move to the next byte and extract the mapping version
  pointer++;
  Serial.print(F("Mapping version 0x"));
  Serial.print(*pointer, HEX);
  Serial.print(" = V");
  
  // Print the mapping version based on the value
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

  // Move to the next byte and extract the maximum number of bytes that can be read
  pointer++;
  Serial.print(F("Maximum number of bytes that can be read 0x"));
  temp = (((pointer[0] << 8) & 0xff00) + (*(++pointer)) & 0xff); // Combine high and low bytes
  Serial.print(temp, HEX);
  Serial.print(" = ");
  Serial.println(temp, DEC); // Print in decimal format

  // Move to the next byte and extract the maximum number of bytes that can be written
  pointer++;
  Serial.print(F("Maximum number of bytes that can be written 0x"));
  temp = (((pointer[0] << 8) & 0xff00) + (*(++pointer)) & 0xff); // Combine high and low bytes
  Serial.print(temp, HEX);
  Serial.print(" = ");
  Serial.println(temp, DEC); // Print in decimal format

  // Move to the next byte and extract the NDEF file control TLV
  Serial.println(F("NDEF file control TLV"));
  pointer++;
  
  // Extract and print the T field from the NDEF file control TLV
  Serial.print(F("T field 0x"));
  Serial.println(*pointer, HEX);

  // Move to the next byte and extract the L field from the NDEF file control TLV
  pointer++;
  Serial.print(F("L field 0x"));
  Serial.println(*pointer, HEX);

  // Move to the next byte and extract the FileID
  pointer++;
  Serial.print(F("FileID 0x"));
  temp = (((pointer[0] << 8) & 0xff00) + (*(++pointer)) & 0xff); // Combine high and low bytes
  Serial.println(temp, HEX);

  // Move to the next byte and extract the maximum NDEF file size
  pointer++;
  Serial.print(F("Maximum NDEF file size in bytes 0x"));
  temp = (((pointer[0] << 8) & 0xff00) + (*(++pointer)) & 0xff); // Combine high and low bytes
  Serial.print(temp, HEX);
  Serial.print(" = ");
  Serial.println(temp, DEC); // Print in decimal format

  // Move to the next byte and extract the read access permissions
  pointer++;
  Serial.print(F("Read access 0x"));
  Serial.print(*pointer & 0xFF, HEX);
  
  // Print the read access permissions based on the value
  switch (*pointer & 0xFF) {
    case 0x00:
      Serial.println(F(" (Read access without any security)"));
      break;
    case 0x80:
      Serial.println(F(" (Locked)"));
      break;
    case 0xFE:
      Serial.println(F(" (Read not authorized)"));
      break;
    default:
      Serial.println(F(" (UNKNOWN)"));
      break;
  }

  // Move to the next byte and extract the write access permissions
  pointer++;
  Serial.print(F("Write access 0x"));
  Serial.print(*pointer & 0xFF, HEX);
  
  // Print the write access permissions based on the value
  switch (*pointer & 0xFF) {
    case 0x00:
      Serial.println(F(" (Write access without any security)"));
      break;
    case 0x80:
      Serial.println(F(" (Locked)"));
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
  // Initialize cursor to point to the start of the NDEF data
  uint8_t* cursor = _ndef;
  
  // Flags to identify specific fields in the NDEF message
  bool MB = 0, ME = 0, SR = 0, IL = 0;

  // Print the NDEF data in hexadecimal format
  for (int i = 0; i < fileLength; i++) {
    Serial.print(_ndef[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Extract and check the length header (MSB and LSB)
  uint8_t MSB = (_ndef[0] << 8) & 0x00FF;
  uint8_t LSB = _ndef[1] & 0xFF;
  
  // Verify if the length header matches the file length
  if ((MSB + LSB) == fileLength) {
    Serial.println(F("Length header is correct !"));
  } else {
    Serial.println(F("Length header is not correct ! EXIT"));
    return;  // Exit if the length header is incorrect
  }

  // Move cursor to the first NDEF field
  cursor += 2;

  // Check and report Message Begin (MB) flag
  if (cursor[0] & 0b10000000) {
    Serial.println(F("Message Begin is set !"));
    MB = true;
  } else {
    Serial.println(F("Message Begin is not set !"));
  }

  // Check and report Message End (ME) flag
  if (cursor[0] & 0b01000000) {
    Serial.println(F("Message End is set !"));
    ME = true;
  } else {
    Serial.println(F("Message End is not set !"));
  }

  // Check and report Chunk Flag (CF) status
  if (cursor[0] & 0b00100000) {
    Serial.println(F("Chunk Flag is set (payload isn't complete) !"));
  } else {
    Serial.println(F("Chunk Flag is not set !"));
  }

  // Check and report Short Record (SR) flag
  if (cursor[0] & 0b00010000) {
    Serial.println(F("Short Record is set !"));
    SR = true;
  } else {
    Serial.println(F("Short Record is not set !"));
  }

  // Check and report ID Length (IL) presence
  if (cursor[0] & 0b00001000) {  
    Serial.println(F("ID Length present is set !"));
    IL = 1;
  } else {
    Serial.println(F("ID Length present is not set !"));
  }

  // Print Type Name Format (TNF) value and its description
  Serial.print(F("Type Name Format is 0x"));
  Serial.print(cursor[0] & 0b00000111, HEX);

  // Interpret the Type Name Format value
  switch (cursor[0] & 0b00000111) {
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

  // Move to the next byte for the Type Length
  cursor++;
  Serial.print("Type has a length of 0x");
  Serial.println(*cursor, HEX);

  // Extract and calculate the payload length
  uint8_t payloadL = *cursor;
  int payloadSize = 0;

  for (int i = 0; i < payloadL; i++) {
    cursor++;
    payloadSize = ((payloadSize << 8) + (*cursor));
  }

  // Print the payload size
  Serial.print("Payload is ");
  Serial.print(payloadSize, DEC);
  Serial.println(" Bytes long");
  cursor++;

  // Check the payload type and handle accordingly
  if (*cursor == 0x54) {
    Serial.println(F(" The payload is a TEXT"));
    handleTXT(cursor, payloadSize);
  } else if (*cursor == 0x55) {
    Serial.println(F("The payload is a URI"));
    handleURI(cursor, payloadSize);
  }
  
  // Move cursor past the current payload
  cursor += payloadSize + 1;

  // If Message End (ME) flag is not set, process additional records
  if (!ME) {
    while ((int(cursor - _ndef)) < fileLength) {
      cursor++;
      payloadL = *cursor;
      payloadSize = 0;

      // Calculate payload size for subsequent records
      for (int i = 0; i < payloadL; i++) {
        cursor++;
        payloadSize = ((payloadSize << 8) + (*cursor));
      }
      Serial.print("Payload is ");
      Serial.print(payloadSize, DEC);
      Serial.println(" Bytes long");

      // Handle subsequent payloads based on type
      if (cursor[-2] == 0x54) {
        Serial.println(F("The payload is a TEXT"));
        handleTXT(cursor, payloadSize);
      } else if (cursor[-2] == 0x55) {
        Serial.println(F("The payload is a URI"));
        handleURI(cursor, payloadSize);
      }

      Serial.println(" ");
      cursor += payloadSize; // Move cursor past the current payload
    }
  }
}


void nfcGadget::handleTXT(uint8_t* cursor, uint8_t lungime) {
  cursor++;

  // Print encoding type
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
  Serial.println();  // New line after URI
}


void nfcGadget::sendCommand(unsigned len) {
  uint8_t v;

  // If the I2C session needs to be established, initiate the transmission
  if (_sendGetI2cSession) {
    // Begin I2C transmission to the device with address _deviceaddress
    Wire.beginTransmission(_deviceaddress);
    // Send the command to get the I2C session
    Wire.write(byte(CMD_GETI2CSESSION));
    // End the transmission and store the error status
    _err = Wire.endTransmission();
    /*
    Uncomment if verbose debugging is enabled:
    if (_verbose) {
      Serial.print(F("\nGetI2Csession: "));
      Serial.print(_err, HEX);
    } else
      delay(1);
    */
  }

  // If debugging commands (_cmds) is enabled, print a start message
  if (_cmds) {
    Serial.print(F("\r\n=> "));
  } else {
    delay(1);  // Short delay for non-debug mode
  }

  // Begin I2C transmission to the device again for sending commands
  Wire.beginTransmission(_deviceaddress);

  // Iterate over the data to be sent
  for (int i = 0; i < len; i++) {
    // Extract the byte to send
    v = (_data[i] & 0xff);
    if (_cmds) {
      // Print the byte in hexadecimal format, padded with leading zero if necessary
      if (v < 0x10) {
        Serial.print(F("0"));
      }
      Serial.print(v, HEX);
    } else {
      delay(5);  // Short delay for non-debug mode
    }
    // Send the byte over I2C
    Wire.write(byte(v & 0xff));
    if (_cmds) {
      Serial.print(F(" "));
    } else {
      delay(1);  // Short delay for non-debug mode
    }
  }

  // Calculate the CRC checksum for the I2C frame
  int chksum = crcsum((unsigned char*)_data, len, 0x6363);

  // Send the checksum as the last bytes of the command
  v = chksum & 0xff;
  if (_cmds) {
    // Print the low byte of the checksum in hexadecimal format
    if (v < 0x10) {
      Serial.print(F("0"));
    }
    Serial.print(v, HEX);
  } else {
    delay(1);  // Short delay for non-debug mode
  }
  Wire.write(byte(v & 0xff));

  // Send the high byte of the checksum
  v = (chksum >> 8) & 0xff;
  if (_cmds) {
    // Print the high byte of the checksum in hexadecimal format
    if (v < 0x10) {
      Serial.print(F("0"));
    }
    Serial.print(v, HEX);
  } else {
    delay(1);  // Short delay for non-debug mode
  }
  Wire.write(byte(v & 0xff));

  // End the I2C transmission
  _err = Wire.endTransmission();
  if (_cmds) {
    Serial.print(F("\r\n"));
  } else {
    delay(1);  // Short delay for non-debug mode
  }

  // Print an error message if the transmission failed
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
