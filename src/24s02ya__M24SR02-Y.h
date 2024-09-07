/*
  24s02ya__M24SR02.cpp - Library for interfacing with the 24s02ya variant of the M24SR02 NFC tag.
  Created by Andrei Valeriu Cosa, 05.09.2024
  Provides methods to send commands, interpret responses, and manage NDEF, System, and Capability Container files.
  Released into the public domain.
*/

#ifndef _24s02ya__M24SR02-Y
#define _24s02ya__M24SR02-Y

#include <Arduino.h>
#include "crc16.h"

// Command definitions for NFC communication
#define CMD_GETI2CSESSION 0x26  // Command to initiate I2C session
#define CMD_KILLRFSESSION 0x52  // Command to kill RF session

// File type definitions for selecting files in the NFC tag
#define NDEFfile 1      // NDEF file type
#define Systemfile 2    // System file type
#define CCfile 3        // Capability Container file type

// External string declarations
extern const char asel[];      // String for app selection
extern char adate[];           // String for date
extern char adate1[];          // String for alternative date format
extern char adate2[];          // String for another alternative date format

/**
 * Class for interfacing with an NFC tag using M24SR02.
 */
class nfcGadget {
public:
  
  // Destructor
  ~nfcGadget();

  // Constructor
  nfcGadget();

  /**
   * Check if the NFC device is connected.
   * @return true if connected, false otherwise.
   */
  static bool deviceConnected();

  /**
   * Sends a command to the NFC tag.
   * @param len Length of the command to send.
   */
  void sendCommand(unsigned len);

  /**
   * Receives a response from the NFC tag.
   * @param len Length of the response to receive.
   * @return The number of bytes received.
   */
  int receiveResponse(unsigned int len);

  /**
   * Selects the NFC application in the tag.
   */
  void selectNFCapp();

  /**
   * Selects a file from the NFC tag based on the option provided.
   * @param opt The option to select (NDEFfile, Systemfile, or CCfile).
   */
  void selectFile(int opt);

  /**
   * Interprets the response from the NFC tag.
   * @param expectedLength Expected length of the response.
   * @return true if the response is as expected, false otherwise.
   */
  bool interpretAnswer(int expectedLength);

  /**
   * Reads the length of a file from the NFC tag.
   * @return Length of the file.
   */
  int readFileLength();

  /**
   * Reads the contents of a file from the NFC tag.
   * @return Pointer to the file data.
   */
  char* readFile();

  /**
   * Reads large amounts of data from the NFC tag in multiple steps.
   * @return Pointer to the data read.
   */
  char* longRead();

  /**
   * Explains and prints the content of the selected file.
   */
  void explainFile();

  /**
   * Adds data to be sent in long-read commands.
   * @param temp Data to add.
   */
  void longAdd(int temp);

  // Public member variables
  boolean _verbose = false;         // Verbose mode flag for debugging
  boolean _cmds = false;            // Command mode flag for sending commands
  char* _data = nullptr;            // Data buffer for sending/receiving data
  uint8_t _deviceaddress = 0x56;    // I2C address of the NFC device
  boolean _sendGetI2cSession = true; // Flag to send the GetI2CSession command
  uint8_t _err;                     // Error code for command operations

  int fileLength = 0;               // Length of the file currently being read
  uint8_t _responseLength = 0x0;    // Length of the response from the NFC tag
  uint8_t* _response = nullptr;     // Buffer for storing the response
  uint8_t* _ndef = nullptr;         // Buffer for storing NDEF data

private:
  uint8_t _opt;                     // Option for file selection

  /**
   * Explains and interprets the Capability Container (CC) file.
   */
  void explainCC();

  /**
   * Explains and interprets the System file.
   */
  void explainSystem();

  /**
   * Explains and interprets the NDEF file.
   */
  void explainNDEF();

  /**
   * Handles and processes a URI payload from the NDEF file.
   * @param cursor Pointer to the payload data.
   * @param length Length of the payload data.
   */
  void handleURI(uint8_t* cursor, uint8_t length);

  /**
   * Handles and processes a TEXT payload from the NDEF file.
   * @param cursor Pointer to the payload data.
   * @param length Length of the payload data.
   */
  void handleTXT(uint8_t* cursor, uint8_t length);
};

#endif
