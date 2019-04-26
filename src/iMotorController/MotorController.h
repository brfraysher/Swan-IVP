/************************************************************/
/*    NAME: Andrew Sucato                                             */
/*    ORGN: The University of Alabama                       */
/*    FILE: MotorController.h                                          */
/*    DATE: 04/12/19                                          */
/************************************************************/

#ifndef MotorController_HEADER
#define MotorController_HEADER

#include <serial/serial.h>
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class MotorController : public AppCastingMOOSApp
{
public:
    MotorController();
    ~MotorController();

protected: // Standard MOOSApp functions to overload
    bool OnNewMail(MOOSMSG_LIST &NewMail);
    bool Iterate();
    bool OnConnectToServer();
    bool OnStartUp();

protected: // Standard AppCastingMOOSApp function to overload
    bool buildReport();

protected:
    void registerVariables();
    bool initializeSerial();

private: // Configuration variables

private: // State variables
    serial::Serial m_port;
    std::string m_address;
    unsigned int m_baud;

    double m_rudder;
    double m_thrust;
    double m_maxPwmVal;
    double m_minPwmVal;
    double m_leftMotorSpeed;
    double m_rightMotorSpeed;
};

#endif
