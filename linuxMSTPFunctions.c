#include "linuxMSTPFunctions.h"
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <sys/time.h> // gettimeofday()
#include <termios.h> // termios, tcgetattr(), tcsetattr()
#include <unistd.h> // read(), write()

int serailFD;
struct timeval highspeedTimer;

bool serialConnect(const char* path, uint16_t baudRate)
{
    struct termios portSettings;

    serailFD = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serailFD == -1) {
        std::cerr << "Error: Could not open port. path=" << path << ", Error: " << strerror(errno) << std::endl;
        return false;
    }

    if (!isatty(serailFD)) {
        std::cerr << "Error: File descripter is not pointing at a TTY device. path=" << path << std::endl;
        return false;
    }

    // Set the port options
    tcgetattr(serailFD, &portSettings); /* save current port settings */
    switch (baudRate) {
        case 38400:
            if (cfsetispeed(&portSettings, B38400) < 0 || cfsetospeed(&portSettings, B38400) < 0) {
                std::cerr << "Error: Could not set the Baud rate. path=" << path << ", baudRate: " << baudRate << std::endl;
                return false;
            }
            break;

        default:
            std::cerr << "Error: Unknown baud rate. path=" << path << ", baudRate: " << baudRate << std::endl;
            return false;
    }

    //
    // Input flags - Turn off input processing
    //
    // convert break to null byte, no CR to NL translation,
    // no NL to CR translation, don't mark parity errors or breaks
    // no input parity check, don't strip high bit off,
    // no XON/XOFF software flow control
    //
    portSettings.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP);
    // No software flow control
    portSettings.c_iflag &= ~(IXON | IXOFF | IXANY);

    //
    // Output flags - Turn off output processing
    //
    // no CR to NL translation, no NL to CR-NL translation,
    // no NL to CR translation, no column 0 CR suppression,
    // no Ctrl-D suppression, no fill characters, no case mapping,
    // no local output processing
    //
    // config.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
    //                     ONOCR | ONOEOT| OFILL | OLCUC | OPOST);
    portSettings.c_oflag = 0;

    //
    // No line processing
    //
    // echo off, echo newline off, canonical mode off,
    // extended input processing off, signal chars off
    //
    portSettings.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

    //
    // Turn off character processing
    //
    // clear current char size mask, no parity checking,
    // no output processing, force 8 bit input
    //
    portSettings.c_cflag &= ~(CSIZE | PARENB);
    portSettings.c_cflag |= CS8;

    //
    // One input byte is enough to return from read()
    // Inter-character timer off
    //
    portSettings.c_cc[VMIN] = 1;
    portSettings.c_cc[VTIME] = 0;

    // Enable reading
    // portSettings.c_cflag |= (CLOCAL | CREAD);

    if (tcsetattr(serailFD, TCSAFLUSH, &portSettings) < 0) {
        std::cerr << "Error: Could not set the port settings. path=" << path << ", baudRate: " << baudRate << std::endl;
        return false;
    }
    /*
    if (tcflush(serailFD, TCSAFLUSH, &portSettings) <= 0) {
        std::cerr << "Error: Could not inital flush the serial port. path=" << path << ", baudRate: " << baudRate << std::endl;
        return false;
    }
    */

    return true;
}

bool mstp_RecvByte(uint8_t* buffer)
{
    if (serailFD == -1) {
        std::cerr << "Error: serial port has not been connected yet. " << std::endl;
        return false;
    }

    if (read(serailFD, buffer, 1) != 1) {
        return false;
    }

    return true;
}

bool mstp_SendByte(uint8_t* buffer, uint32_t length)
{
    if (serailFD == -1) {
        std::cerr << "Error: serial port has not been connected yet. " << std::endl;
        return false;
    }

    if (write(serailFD, buffer, length) != length) {
        return false;
    }
    return true;
}

void mstp_Sleep(uint32_t ms)
{
    // 1000 nano  seconds (nsec) == 1 micro seconds
    // 1000 micro seconds (us)  == 1 mili  seconds
    // 1000 mili  seconds (ms)  == 1 seconds
    //    1       second  (sec) == 1,000,000 micro seconds (us)

    uint32_t usec = ms * 1000;
    static struct timespec delay, rem;
    delay.tv_sec = usec / 1000000;

    delay.tv_nsec = (usec - (delay.tv_sec * 1000000)) * 1000;
    int ret = -1;
    while (ret != 0) {
        ret = nanosleep(&delay, &rem);
    }
}

void mstp_TimerReset()
{
    gettimeofday(&highspeedTimer, NULL);
}

uint32_t mstp_TimerDifference()
{
    struct timeval end;
    gettimeofday(&end, NULL);

    static long seconds, useconds;
    seconds = end.tv_sec - highspeedTimer.tv_sec;
    useconds = end.tv_usec - highspeedTimer.tv_usec;
    // 1000000 microseconds in a second
    // Return the time in microsecond (us)
    return ((seconds)*1000000 + useconds);
}
