/*
 * Linux MSTP functions 
 * ----------------------------------------------------------------------------
 * These functions are used by the CAS BACnet MSTP stack via callbacks to 
 * interface with the OS level resources. This file can be updated to work with 
 * your custom hardare or OS. 
 * 
 * Created by: Steven Smethurst 
 * Created on: May 23, 2019 
 * Last updated: May 23, 2019 
 *
 */
#include <stdbool.h> // Bool
#include <stdint.h> // uint8_t, uint32_t

// CAS BACnet Stack MSTP callback functions
// ----------------------------------------------------------------------------
bool mstp_RecvByte(uint8_t* buffer);
bool mstp_SendByte(uint8_t* buffer, uint32_t length);
void mstp_Sleep(uint32_t ms);
void mstp_TimerReset();
uint32_t mstp_TimerDifference();

// Driver specific
// ----------------------------------------------------------------------------
extern int serailFD;
extern struct timeval highspeedTimer;

bool serialConnect(const char* path, uint16_t baudRate = 38400);