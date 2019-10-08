/************************************************************/
/*    NAME: Andrew Sucato                                              */
/*    ORGN: The University of Alabama                       */
/*    FILE: IMU.h                                          */
/*    DATE: 10/03/19                                          */
/************************************************************/

#ifndef IMU_HEADER
#define IMU_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

extern "C" {
#include "bno055.h"
}

class IMU : public AppCastingMOOSApp
{
public:
  IMU();
  
  ~IMU();

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
  bno055_t m_bno055;
  bno055_euler_double_t m_euler;
  bno055_quaternion_t m_quaternion;
  unsigned char m_sysCalStatus;
private: // State variables
};

#endif
