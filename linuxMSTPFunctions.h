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

// mstp_RecvByte
// Receives 1 byte from the UART, Non blocking
// Return
//   false - No bytes are avaliable on the UART.
//   true - A byte was recived.
bool mstp_RecvByte(uint8_t* buffer);

// mstp_SendByte
// Sends 1 or more bytes out of the UART, Non blocking
// Return
//   false - 1 or more of the bytes could not be sent.
//   true - all of the bytes were sent.
bool mstp_SendByte(uint8_t* buffer, uint32_t length);

// void mstp_Sleep(uint32_t ms);
// Used for single threaded systems. The MSTP thread is finnished with this loop. The MSTP stack
// will not need the thread again for {ms} time and the systems other proccess can use this time
// to do sensor readings, or other proccesses.
void mstp_Sleep(uint32_t ms);

// void mstp_TimerReset();
// Resets the timer back to zero.
void mstp_TimerReset();

// uint32_t mstp_TimerDifference();
// Returns the amount of microsecond (us) since the last time mstp_TimerReset() was called.
// This timer needs a resolution of 1 millisecond (ms). If the mstp_TimerDifference()
// function is called mulitple times without calling the mstp_TimerReset() then the value
// returned by mstp_TimerDifference() will incurment.
//
// For example: If 3 millisecond elapse then This function should return 3000 us
uint32_t mstp_TimerDifference();

// Driver specific
// ----------------------------------------------------------------------------
extern int serailFD;
extern struct timeval highspeedTimer;

bool serialConnect(const char* path, uint16_t baudRate = 38400);