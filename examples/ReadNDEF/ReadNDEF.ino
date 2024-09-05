/*
This example demonstrates how to interact with an NFC (Near Field Communication) tag using the nfcGadget class. Specifically, it shows how to:

Initialize the NFC communication over I2C.
Select and read the NDEF (NFC Data Exchange Format) file from the NFC tag.
Print the contents of the NDEF file to the Serial Monitor, which typically contains useful data such as text or URLs.
This basic structure can be expanded to handle more complex NFC operations, such as writing to the NFC tag or handling other file types.
*/

#include <Wire.h>             // Include the Wire library for I2C communication
#include "24s02ya__M24SR02.h" // Include the custom NFC library

nfcGadget nfc;  // Create an instance of the nfcGadget class to interface with the NFC tag

void setup() {
  Wire.begin();               // Initialize I2C communication
  Serial.begin(9600);         // Start the serial communication at 9600 baud rate
  
  // Wait for the Serial Monitor to open (important for some boards like Leonardo or Due)
  while (!Serial)
    ;  
  Serial.println("");         // Print a blank line for spacing
  Serial.println("interfata NFC"); // Indicate that the NFC interface is being initialized

  // Configure flags for debugging and session management
  nfc._verbose = false;         // Set to true to print detailed outputs during operations
  nfc._cmds = false;            // Set to true to print raw command data in hexadecimal format
  nfc._sendGetI2cSession = true;// Enable the I2C session request before each communication

  // Attempt to establish communication with the NFC app on the NFC tag (optional)
  Serial.println("Open ");
  nfc.selectNFCapp();           // Select the NFC application on the M24SR02 (optional, not mandatory)

  // Select the System file on the NFC tag, which contains system information like memory and configuration
  Serial.println("Systemfile");
  nfc.selectFile(Systemfile);   // Choose the System file to be read

  // Retrieve the length of the data in the selected System file
  int j = nfc.readFileLength(); // Read the size of the System file payload, necessary before reading the file

  // Read the actual content of the System file
  char *c = nfc.readFile();     // Fetch the System file data from the NFC tag

  // Explain the content of the System file, providing details like memory configuration or status
  nfc.explainFile();            // Print the content of the System file in a readable format

  Serial.println("");           // Print a blank line for spacing
}

void loop() {
  // Empty loop, no continuous operations in this example
}
