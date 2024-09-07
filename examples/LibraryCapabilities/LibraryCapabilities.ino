/*
  NFC_Example.ino - Example code for interacting with an NFC tag using the M24SR02 NFC IC.
  This code demonstrates how to read the Capability Container (CC), System, and NDEF files from an NFC tag.
  
  Created by Andrei Valeriu Cosa, 05.09.2024
  Based on the NFC communication library "24s02ya__M24SR02-Y".
  Released into the public domain.

  This example shows how to:
  - Initialize I2C communication and NFC tag interaction.
  - Select and read different types of files from an NFC tag:
    - CC file: Capability Container file, defines memory layout.
    - System file: Provides information about the system or device.
    - NDEF file: Stores the actual NFC message (e.g., text, URI).
  - Display the content of each file in a readable format using the `explainFile()` function.
*/

#include <Wire.h>                 // Include the Wire library for I2C communication
#include "24s02ya__M24SR02-Y.h"      // Include the custom NFC library

nfcGadget nfc;  // Create an instance of the nfcGadget class to interact with the NFC tag

void setup() {
  Wire.begin();              // Initialize I2C communication, required for NFC tag communication
  Serial.begin(9600);        // Start the Serial communication at 9600 baud rate for monitoring
  
  // Wait for the Serial Monitor to open (important for some boards like Leonardo or Due)
  while (!Serial)
    ;  
  Serial.println("");        // Print a blank line for spacing
  Serial.println("NFC Interface Initialized");

  // Configure NFC library flags
  nfc._verbose = false;         // Set to true to print detailed outputs for debugging
  nfc._cmds = false;            // Set to true to print raw command data in hexadecimal format
  nfc._sendGetI2cSession = true;// Enable the I2C session request before each communication
  
  // **Step 1: Read and explain the Capability Container (CC) file**
  Serial.println("Opening NFC Application");
  nfc.selectNFCapp();           // Select the NFC application (optional, but good practice)

  Serial.println("Reading CC file");
  nfc.selectFile(CCfile);       // Select the Capability Container (CC) file on the NFC tag
  int j = nfc.readFileLength(); // Read the size of the CC file payload (must be called before reading data)
  char *c = nfc.readFile();     // Read the actual data of the CC file
  nfc.explainFile();            // Print the contents of the CC file in a readable format
  Serial.println("");

  // **Step 2: Read and explain the System file**
  nfc.selectNFCapp();           // Select the NFC application again (optional)

  Serial.println("Reading System file");
  nfc.selectFile(Systemfile);   // Select the System file, which contains system-specific data
  j = nfc.readFileLength();     // Read the size of the System file payload
  c = nfc.readFile();           // Read the actual data of the System file
  nfc.explainFile();            // Print the contents of the System file in a readable format
  Serial.println("");

  // **Step 3: Read and explain the NDEF (NFC Data Exchange Format) file**
  nfc.selectNFCapp();           // Select the NFC application again (optional)

  Serial.println("Reading NDEF file");
  nfc.selectFile(NDEFfile);     // Select the NDEF file, which stores the actual NFC message (e.g., URI, text)
  j = nfc.readFileLength();     // Read the size of the NDEF file payload
  c = nfc.readFile();           // Read the actual data of the NDEF file
  nfc.explainFile();            // Print the contents of the NDEF file in a readable format
  Serial.println("");
}

void loop() {
  // The loop is empty as the example performs its operations in the setup function
}
