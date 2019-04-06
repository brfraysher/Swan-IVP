//
// Created by ansucato on 4/4/19.
//

#include "Garmin19x.h"


Garmin19x::Garmin19x(const std::string& port)
{
    uint32_t baud = 38400;

    m_gps.setPort(port);
    m_gps.setBaudrate(baud);

}

Garmin19x::~Garmin19x()
{

}

