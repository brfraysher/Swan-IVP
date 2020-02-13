//
// Created by ansucato on 1/27/20.
//

#include "SyncSerialPort.hpp"
#include <thread>
#include <cstdlib>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

using namespace uwal::Hardware;


SyncSerialPort::SyncSerialPort(const std::string file, speed_t baudRate)
{
    fd = open(file.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
        throw std::runtime_error("[SyncSerialPort] Failed to open serial port");

    struct termios tty;

    if (tcgetattr(fd, &tty) < 0)
        throw std::runtime_error("[SyncSerialPort] Failed to configure serial port");

    cfsetospeed(&tty, (speed_t)baudRate);
    cfsetispeed(&tty, (speed_t)baudRate);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
        throw std::runtime_error("[SyncSerialPort] Failed to configure serial port");
}

SyncSerialPort::~SyncSerialPort()
{

}

bool SyncSerialPort::writeBinary(std::vector<std::uint8_t> data)
{
    return write(fd, data.data(), data.size());
}

bool SyncSerialPort::readBinary(std::vector<std::uint8_t>& returnData, std::size_t size, unsigned int timeout_ms)
{
    std::vector<uint8_t> buff(size);
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::milliseconds(timeout_ms);

    while (std::chrono::steady_clock::now() < end)
    {
        std::size_t bytes_avail = 0;
        ioctl(fd, FIONREAD, &bytes_avail);
        if (bytes_avail >= size)
        {
            std::size_t readnum = read(fd, buff.data(), size);
            if (readnum == size)
            {
                returnData = buff;
                return true;
            }
            else
                return false;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }

    return false;
}

bool SyncSerialPort::writeString(const std::string stringData, bool nullTerminate)
{
    std::vector<uint8_t> data(stringData.begin(), stringData.end());

    if (nullTerminate)
        data.push_back(0);  // append null terminator

    return writeBinary(data);
}

bool SyncSerialPort::readString(std::string& , const char , unsigned int )
{
    return false;
}

void SyncSerialPort::flush()
{
    ioctl(fd, TCFLSH, 2);
}

void SyncSerialPort::lockPort()
{
    _mutex.lock();
}

void SyncSerialPort::unlockPort()
{
    _mutex.unlock();
}

std::unique_lock<std::recursive_mutex> SyncSerialPort::getScopedLock()
{
    return std::unique_lock<std::recursive_mutex> (_mutex);
}