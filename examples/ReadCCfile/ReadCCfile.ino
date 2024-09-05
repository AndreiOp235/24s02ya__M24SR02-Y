/*
  This example demonstrates how to use the `nfcGadget` class to communicate with an M24SR02 NFC chip over I2C. 
  The purpose of this code is to initialize communication with the NFC device, select a specific file (Capability 
  Container or CC file), read the file's content, and provide a human-readable explanation of the file's structure.

  The CC file is a standard file in NFC tags that contains information about the tag's structure, including 
  the size and format of the NDEF (NFC Data Exchange Format) message. Reading this file is a crucial step 
  in interacting with NFC devices, especially when handling NDEF messages like URLs, text, or other data formats.
  
  To see additional outputs or raw command inputs, you can set the `_verbose` and `_cmds` flags to `true` 
  respectively.

  This example is a starting point for more advanced NFC applications, such as reading or writing NDEF 
  messages or managing multiple files on the NFC chip.
*/

#include <Wire.h>             // Include the Wire library for I2C communication
#include "24s02ya__M24SR02.h" // Include the custom NFC library

nfcGadget nfc;                // Create an instance of the nfcGadget class

void setup() {
  Wire.begin();               // Initialize the I2C communication
  Serial.begin(9600);         // Start the serial communication at 9600 baud rate
  
  // Wait for the Serial Monitor to open (only for boards like Leonardo)
  while (!Serial)
    ;  
  Serial.println("");         // Print a blank line for spacing
  Serial.println("interfata NFC"); // Print a message to indicate NFC interface is ready

  // Set various flags for debugging and session management
  nfc._verbose = false;         // Set to true to see detailed outputs for debugging
  nfc._cmds = false;            // Set to true to see command inputs in hexadecimal format
  nfc._sendGetI2cSession = true;// Enable sending the GetI2CSession command before each communication

  // Select the NFC application (this step is optional but recommended)
  Serial.println("Open ");
  nfc.selectNFCapp();           // Open communication with the NFC app on the M24SR02

  // Select the Capability Container (CC) file to read its contents
  Serial.println("CC file");
  nfc.selectFile(CCfile);       // Select the CC file (defines the structure of the NDEF message)

  // Read the length of the data in the selected file
  int j = nfc.readFileLength(); // Obtain the size of the payload (important to call this first)

  // Read the actual file content from the NFC tag
  char *c = nfc.readFile();     // Read the data from the selected file

  // Interpret and explain the content of the file (in this case, the CC file)
  nfc.explainFile();            // Print the details of the file content in a human-readable format

  Serial.println("");           // Print a blank line for spacing
}

void loop() {
  // No continuous operations in the loop for this example
}
