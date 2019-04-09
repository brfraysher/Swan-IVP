/************************************************************/
/*    NAME: Andrew Sucato                                              */
/*    ORGN: MIT                                             */
/*    FILE: GPS.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef GPS_HEADER
#define GPS_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <serial/serial.h>

class GPS : public AppCastingMOOSApp
{
public:
    GPS();
    ~GPS();

protected: // Standard MOOSApp functions to overload
    bool OnNewMail(MOOSMSG_LIST &NewMail);

    bool Iterate();

    bool OnConnectToServer();

    bool OnStartUp();

protected: // Standard AppCastingMOOSApp function to overload
    bool buildReport();

protected:
    void registerVariables();

private: // Configuration variables

private: // State variables
    double m_current_lat;
    double m_current_long;
    serial::Serial m_gps;
};

#endif 
