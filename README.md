# 24s02ya__M24SR02-Y_Library
 Arduino library written in an attempt to revive a NFC module salvaged from a Candy washing machine

## Overview

The **24s02ya__M24SR02-Y Library** is an Arduino library designed to interact with the M24SR02-Y NFC/RFID device. This library provides functions to communicate with the device, read and write NFC data, and manage various NFC files.

## Features

- **Select NFC Application**: Choose the NFC application for communication.
- **Select and Read Files**: Access different files (CC file, System file, NDEF file) on the NFC device.
- **Interpret and Explain Data**: Decode and explain NFC data, including payload and file structure.
- **Verbose and Command Modes**: Options for detailed output and command visibility.

## Installation

1. **Download the Library**: Clone or download the repository from GitHub.

   ```bash
   git clone https://github.com/your-username/24s02ya__M24SR02-Y_Library.git
   ```

2. **Install via Arduino Library Manager**:
   - Open the Arduino IDE.
   - Go to **Sketch** > **Include Library** > **Add .ZIP Library**.
   - Select the downloaded `.zip` file of the library and click **Open**.

3. **Manual Installation**:
   - Extract the downloaded repository.
   - Copy the extracted folder into the Arduino `libraries` directory. This is usually located in `Documents/Arduino/libraries`.

## Usage

Here's a basic example demonstrating how to use the `24s02ya__M24SR02-Y_Library` to interact with the M24SR02-Y device.

### Example: Reading NFC Files


## Functions

- **`nfcGadget()`**: Constructor to initialize the NFC object.
- **`void selectNFCapp()`**: Select the NFC application (optional).
- **`void selectFile(int opt)`**: Select a specific file on the NFC device (`CCfile`, `Systemfile`, `NDEFfile`).
- **`int readFileLength()`**: Returns the length of the selected file.
- **`char* readFile()`**: Reads the content of the selected file.
- **`void explainFile()`**: Decodes and prints information about the file content.
- **`void explainNDEF()`**: Detailed explanation of NDEF files.

## Configuration

- **Verbose Mode**: Set `nfc._verbose` to `true` to enable detailed output.
- **Command Mode**: Set `nfc._cmds` to `true` to view command inputs.
- **I2C Session**: Control whether to send an I2C session request with `nfc._sendGetI2cSession`.

## License

This library is released into the public domain under the GNU General Public License v2.0 Feel free to use, modify, and distribute it.
Credit goes to  https://github.com/rena2019 for his library that was a good starting place for the basic I2C comunication with the IC. All the subquentle wrappers are written by me.
Also this includes a crc16 lib found on some random archieve that I cannot find the author of

## Support

For questions or issues, please open an issue
