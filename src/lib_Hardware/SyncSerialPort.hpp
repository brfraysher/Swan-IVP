//
// Created by ansucato on 1/27/20.
//

#ifndef KAYAK_SYNCSERIALPORT_HPP
#define KAYAK_SYNCSERIALPORT_HPP

#include <string>
#include <mutex>
#include <queue>
#include <termios.h>

namespace uwal
{
    namespace Hardware
    {
        class SyncSerialPort
        {
            public:
            SyncSerialPort(const std::string file, speed_t baud_rate);

            ~SyncSerialPort();

            bool writeBinary(std::vector<std::uint8_t> data);
            bool readBinary(std::vector<std::uint8_t>& readData, std::size_t size, unsigned int timeout_ms);

            bool writeString(const std::string stringData, bool nullTerminate = true);
            bool readString(std::string& stringData, const char delimiter, unsigned int timeout_ms);

            void flush();

            void lockPort();
            void unlockPort();

            std::unique_lock<std::recursive_mutex> getScopedLock();

            private:
            std::recursive_mutex _mutex;
            int fd;
        };
    }
}


#endif //KAYAK_SYNCSERIALPORT_HPP
