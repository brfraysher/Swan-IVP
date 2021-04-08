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
  
  void initIMU();
  bool checkPOST();
  void checkConnected();
  void checkStatus();
  void sendStatus();
  void readCalibrationStatus();
  void readEuler();
  void readQuaternion();
  void readSystemCalibration();
  void logMagnetometerOffset();
  u8 writeSystemCalibration();
  void calibrateIMU(u32 max_timeout);

private: // Configuration variables
  bool m_open;
  bool m_savedCal;
  bool m_save_calib;
  double m_headingBias;
  bool m_log_mag;
  u8 m_status;
  u8 m_systemCalibration[22];
  u8 m_magCalibration[6];
  u8 m_sysCalStatus;
  u8 m_accelCalStatus;
  u8 m_gyroCalStatus;
  u8 m_magCalStatus;
  u8 m_lastErrorCode;
  u8 m_calib_test;
  u8 m_imu_axis;
  std::string m_i2cAddr;
  bno055_t m_bno055;
  bno055_euler_double_t m_euler;
  bno055_quaternion_t m_quaternion;
private: // State variables
};

#endif
