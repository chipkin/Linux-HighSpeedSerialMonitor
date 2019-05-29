# Linux High Speed Serial Monitor

A Linux serial port application that shows the time between received bytes in milliseconds and nanoseconds.

This tool was orginally designed to test and validate the callback functions for the CAS BACnet MSTP Stack on custom hardware. We have found the tool to be useful for detecting interbyte timeouts in serial protocol communications.

## Command line arguments

```txt
serialMonitor <port> <baudrate>
   <port>     - The TTY serial port that this application will monitor. Default: /dev/ttyS1
   <baudrate> - The baud rate of the TTY serial port. Default: 38400

Examples:
   serialMonitor /dev/ttyS1 38400
   serialMonitor /dev/ttyS2
   serialMonitor /dev/ttyS3 9600
```

## Example output

Below is an example output from the serialMonitor application

```txt

./serialMonitor /dev/ttyS5 38400
FYI: High speed serail monitor. Version: 0.0.0.1
FYI: Connecting to serial port. SerialPort=/dev/ttyS5, BaudRate=38400
FYI: Connected to serial port

|  Time (ms) |     CHAR |      HEX |
* ---------- * -------- * -------- *
|   3172.003 |        0 |       00 |
|      0.489 |        a |       0a |
|     10.226 |        b |       0b |
|     10.379 |        c |       0c |
|     10.926 |        d |       0d |
|     20.361 |        e |       0e |
|     30.979 |        f |       0f |
|     41.015 |        g |       0g |
|     50.169 |        h |       0h |
```

## Building

A included [makefile](https://github.com/chipkin/Linux-HighSpeedSerialMonitor/blob/master/makefile) can be built with [GCC](https://gcc.gnu.org/)

```bash
make all
```

This project also auto built using [Gitlab CI](https://docs.gitlab.com/ee/ci/) on every commit.

## Binary

A compiled version of the serial monitor is included in this repo. [serialMonitor](https://github.com/chipkin/Linux-HighSpeedSerialMonitor/blob/master/serialMonitor)

## Change log

### 2019 May 23

- 0.0.2 Added CI build number. Updated make file to include "all", and "Clean" steps. Added Gitlab CI auto builds.
- 0.0.1 Initial version