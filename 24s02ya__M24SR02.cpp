#include "24s02ya__M24SR02.h"
#include <Wire.h>
#include <crc16.h>

void(* resetFunc) (void) = 0;


nfcGadget::nfcGadget()
{
  if(!this->deviceConnected())
    resetFunc();
}

nfcGadget::~nfcGadget() {
    if (_responseLength)
    {
        free(_response);
    }
}

bool nfcGadget::deviceConnected()
{
  
  Wire.beginTransmission(0x2D); // Start I2C transmission to a device with given address
  if(!Wire.endTransmission())
    Wire.beginTransmission(0x56);
      if(!Wire.endTransmission())
        return true;
  return false;
}

void nfcGadget::sendCommand(unsigned len) 
{
  uint8_t v;

  if (_sendGetI2cSession) 
  {
    Wire.beginTransmission(_deviceaddress); // transmit to device 0x2D
    Wire.write(byte(CMD_GETI2CSESSION)); // GetI2Csession
    _err = Wire.endTransmission();     // stop transmitting
    if (_verbose) {
      Serial.print(F("\r\nGetI2Csession: "));
      Serial.print(_err, HEX);
    }
    else
      delay(1);
  }
  
  if (_cmds)
      Serial.print(F("\r\n=> "));
  else
      delay(1);
  
  Wire.beginTransmission(_deviceaddress);
  
  for(int i=0; i < len; i++) {
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
  int chksum =  crcsum((unsigned char*) _data, len, 0x6363 );
  
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
  }
  else {
      delay(1);
  }

  if (_err != 0) {
    Serial.print(F("write err: "));
    Serial.print(_err, HEX);
  }
}

int nfcGadget::receiveResponse(unsigned int len) {
  int index=0;
  boolean WTX = false;
  boolean loop = false;
  if (_verbose) {
    Serial.print(F("\r\nreceiveResponse, len="));
    Serial.print(len, DEC);
    Serial.println();
  }
  if (_responseLength < len) {
    free(_response);
    if (_verbose) {
        Serial.print(F("\r\n_responseLength="));
        Serial.print(len, DEC);
    }
    _responseLength = len;
    _response = (byte*)malloc(_responseLength);
  }
  else
     delay(1);
  do {
    WTX = false;
    loop = false;
    Wire.requestFrom(_deviceaddress, len);
    if (_cmds)
        Serial.print(F("<= "));
    else
        delay(1);
    while ((Wire.available() && index < len && !WTX) ||
           (WTX && index < len-1)) {
      int c  = (Wire.read() & 0xff);
      if (_cmds) {
          if (c < 0x10)
              Serial.print(F("0"));
          Serial.print(c, HEX);
          Serial.print(F(" "));
      }
      else
         delay(1);
      if (c == 0xF2 && index == 0) {
         WTX = true;
      }
      if (index >= 1) {
          _response[index-1] = c;
      }
      index++;
    }
    if (WTX) {
       Serial.print(F("\r\nWTX"));
       delay(200 * _response[0]);
       //send WTX response
       //sendSBLOCK(0xF2);
       _data[0] = 0xF2;//WTX
       _data[1] = _response[0];

       sendCommand(/*_data,*/ 2/*, false*/);
       loop = true;
       index = 0;
    }
  }while(loop);

  Serial.println("");
  return index;
}

void nopuri()
{
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
  asm volatile ("nop");  // Insert NOP instruction here
}