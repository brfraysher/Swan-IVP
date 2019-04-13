/************************************************************/
/*    NAME: Spencer Davis                                              */
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
   MotorController(const std::string& port);
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

 private: // Configuration variables

 private: // State variables
    serial::Serial m_port;

    double m_rudder;
    double m_thrust;
    double m_leftMotorSpeed;
    double m_rightMotorSpeed;
};

#endif
