//
// Created by ansucato on 4/4/19.
//

#ifndef IVP_EXTEND_GARMIN19X_H
#define IVP_EXTEND_GARMIN19X_H


#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <serial/serial.h>

class Garmin19x
{
public:
    Garmin19x(const std::string& port);
    ~Garmin19x();

private:
    serial::Serial m_gps;
};


#endif //IVP_EXTEND_GARMIN19X_H
