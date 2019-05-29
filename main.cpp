/**
 * High speed Serial Monitor 
 * --------------------------------------
 * This linux appliction opens a serial port and recoreds the time in miliseconds between each recived 
 * byte. This tool was orginally designed to validate the comunications of BACnet MSTP devices. 
 * 
 * Command line arguments
 *    serialMonitor <port = /dev/ttyS1> <baudrate = 38400>
 *    <port>     - The TTY serial port that this application will monitor. Default: /dev/ttyS1
 *    <baudrate> - The baud rate of the TTY serial port Default: 38400
 * 
 * Examples:
 *    serialMonitor /dev/ttyS1 38400
 *    serialMonitor /dev/ttyS2
 *    serialMonitor /dev/ttyS3 9600
 * 
 * 
 * Created by: Steven Smethurst 
 * Created on: May 22, 2019 
 * Last updated: May 22, 2019 
 *
 */

#include "CIBuildSettings.h"
#include "composeStream.h"
#include "linuxMSTPFunctions.h"
#include <fstream>
#include <iomanip> // std::setw
#include <iostream> // std::cout, std::ios
#include <stdio.h>
#include <string.h> // strlen

static const char* APPLICATION_VERSION = "0.0.0";
static const char* DEFAULT_SERIAL_PORT = "/dev/ttyS1";
static const uint32_t DEFAULT_SERIAL_BAUDRATE = 38400;
static const char* CONFIGURATION_LOG_FILE = "log.txt";

void printHelp()
{
    std::cout << std::endl;
    std::cout << "Command line arguments" << std::endl;
    std::cout << "   serialMonitor <port> <baudrate>" << std::endl;
    std::cout << "   <port>     - The TTY serial port that this application will monitor. Default: /dev/ttyS1" << std::endl;
    std::cout << "   <baudrate> - The baud rate of the TTY serial port Default: 38400" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "   serialMonitor /dev/ttyS1 38400" << std::endl;
    std::cout << "   serialMonitor /dev/ttyS2" << std::endl;
    std::cout << "   serialMonitor /dev/ttyS3 9600" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    std::cout << "FYI: High speed serail monitor. Version: " << APPLICATION_VERSION << "." << CI_PIPELINE_IID << std::endl;

    // Set up the output stream.
    ComposeStream outputStream;
    outputStream.linkStream(std::cout);

    // Open a log file
    std::ofstream outfile(CONFIGURATION_LOG_FILE, std::ios_base::app);
    outputStream.linkStream(outfile);

    // Check for command line parmeters
    uint32_t baudRate = DEFAULT_SERIAL_BAUDRATE;
    static const uint32_t MAX_COMPORT_STRING_LENGTH = 100;
    char comport[MAX_COMPORT_STRING_LENGTH];
    strncpy(comport, DEFAULT_SERIAL_PORT, MAX_COMPORT_STRING_LENGTH);

    // Check for help flag.
    for (uint32_t i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0) {
            printHelp();
            return EXIT_SUCCESS;
        }
    }

    // Extract the command line paramters.
    if (argc == 1) {
        outputStream << "FYI: No command line parameters set. Using defaults. " << std::endl;
    }
    if (argc >= 2) {
        strncpy(comport, argv[1], 100);
    }
    if (argc >= 3) {
        baudRate = atoi(argv[2]);
    }

    // Open the serial port
    outputStream
        << "FYI: Connecting to serial port. SerialPort=" << comport << ", BaudRate=" << baudRate << std::endl;
    if (!serialConnect(comport, baudRate)) {
        outputStream << "Error: Could not connect to the serial port" << std::endl;
        return EXIT_FAILURE;
    }
    outputStream << "FYI: Connected to serial port" << std::endl
                 << std::endl;

    const uint32_t PRINT_WIDTH_TIME = 12;
    const uint32_t PRINT_WIDTH_CHAR = 10;
    const uint32_t PRINT_WIDTH_HEX = 10;

    // Print the headers
    outputStream << "|" << std::setfill(' ') << std::setw(PRINT_WIDTH_TIME) << " Time (ms) ";
    outputStream << "|" << std::setfill(' ') << std::setw(PRINT_WIDTH_CHAR) << " CHAR ";
    outputStream << "|" << std::setfill(' ') << std::setw(PRINT_WIDTH_HEX) << " HEX ";
    outputStream << "|" << std::endl;
    outputStream << "* " << std::setfill('-') << std::setw(PRINT_WIDTH_TIME + 1);
    outputStream << " * " << std::setfill('-') << std::setw(PRINT_WIDTH_CHAR + 1);
    outputStream << " * " << std::setfill('-') << std::setw(PRINT_WIDTH_HEX + 1);
    outputStream << " * " << std::endl;

    mstp_TimerReset();
    uint32_t microsecond;
    uint8_t buffer;
    for (;;) {
        if (mstp_RecvByte(&buffer)) {
            microsecond = mstp_TimerDifference();
            mstp_TimerReset();

            outputStream << "|" << std::setfill(' ') << std::setw(PRINT_WIDTH_TIME - 1) << std::fixed << std::setprecision(3) << (float)microsecond / 1000;
            outputStream << " |" << std::setfill(' ') << std::setw(PRINT_WIDTH_CHAR - 1) << buffer;
            outputStream << " |" << std::setfill(' ') << std::setw(PRINT_WIDTH_HEX - 3) << " " << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << buffer;
            outputStream << " |" << std::endl;
        }
    }
}