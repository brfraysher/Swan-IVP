//
// Created by ansucato on 4/4/19.
//

#include "Garmin19x.h"
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <iostream>

Garmin19x::Garmin19x(const char* port)
{
    open_port(port);

}

Garmin19x::~Garmin19x()
{
}

int Garmin19x::open_port(const char* port)
{
    int fd; /* File descriptor for the port */


    fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
        /*
         * Could not open the port.
         */

        perror("open_port: Unable to open port - ");
    }
    else
    {
        struct termios options;

        tcgetattr(fd, &options);

        cfsetispeed(&options, B38400);
        cfsetospeed(&options, B38400);

        options.c_cflag |= (CLOCAL | CREAD);

        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;

        tcsetattr(fd, TCSANOW, &options);

        std::cout << "Port opened!" << std::endl;
        char read_buffer[100];
        int bytes_read = 0;
        bytes_read = read(fd,&read_buffer,100);
        std::cout <<read_buffer << std::endl;
    }

    return (fd);
}