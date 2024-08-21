#include <Wire.h>
#define addrR 0x56
#define addrW 0xAD
#define zero  0x00
byte startSession=0x52;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial)
    ;  // Wait for Serial Monitor to open
  Serial.println("I2C Scanner");
}

void nopuri();

void loop() {
  byte error;

  Serial.println("=Bituiesc...=");

  Wire.beginTransmission(addrR);
  Wire.write(startSession);

  Wire.write(0x02);
  Wire.write(zero);
  Wire.write(0xB0);
  Wire.write(zero);
  Wire.write(zero);
  Wire.write(0x02);
  Wire.write(0x6B);
  Wire.write(0x7E);
  
  //Wire.write(0xE0);
  //Wire.write(0xB4);

  
  error = Wire.endTransmission();
  if(error==0)
    Serial.println("Succes");
  else
    Serial.println(error);

  Wire.beginTransmission(addrW);
  nopuri();
  Serial.println(Wire.available());
  error = Wire.endTransmission();
  if(error==0)
    Serial.println("Succes");
  else
    Serial.println("error");

  
  Serial.println("^");
  Serial.println("|Am terminat de bituit");
  delay(1000);  // Wait 5 seconds for the next scan
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