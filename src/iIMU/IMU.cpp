/************************************************************/
/*    NAME: Andrew Sucato                                              */
/*    ORGN: The University of Alabama                       */
/*    FILE: IMU.cpp                                        */
/*    DATE: 10/03/19                                          */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "IMU.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

extern "C" {
#include "bno055.h"
}


//---------------------------------------------------------
// Constructor

IMU::IMU()
        : m_open(false),
        m_sysCalStatus(0),
        m_status(1),
        m_lastErrorCode(0),
        m_i2cAddr("/dev/i2c-1"),
        m_euler({0}),
        m_quaternion({0})
        
{
  m_bno055.dev_addr = BNO055_I2C_ADDR1;
  m_bno055.bus_read = bus_read;
  m_bno055.bus_write = bus_write;
  m_bno055.delay_msec = delay_func;
  
  initIMU();
  
}

//---------------------------------------------------------
// Destructor

IMU::~IMU()
{
}


void IMU::initIMU()
{
  if ((m_bno055.fid = open(m_i2cAddr.c_str(), O_RDWR)) < 0)
  {
    reportRunWarning("Failed to open I2C Bus!!!");
    return;
  }
  
  if (ioctl(m_bno055.fid, I2C_SLAVE, m_bno055.dev_addr))
  {
    reportEvent("Failed to reach IMU on I2C Bus!!!");
    return;
  }
  
  
  if (bno055_init(&m_bno055) || m_bno055.chip_id != 0xA0)
  {
    reportEvent("Failed to initialize IMU!");
    return;
  }
  
  if (bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF))
  {
    reportEvent("Failed to set IMU operation mode!");
    return;
  }
  
  if (!checkPOST())
  {
    reportEvent("Failed POST!");
    return;
  }
  
  m_open = true;
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool IMU::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);
  
  MOOSMSG_LIST::iterator p;
  for (p = NewMail.begin();
       p != NewMail.end();
       p++)
  {
    CMOOSMsg &msg = *p;
    std::string key = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString();
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
    
    if (key == "FOO")
    {
      std::cout << "great!";
    }
    
    else if (key != "APPCAST_REQ")
    { // handled by AppCastingMOOSApp
      reportRunWarning("Unhandled Mail: " + key);
    }
  }
  
  return (true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool IMU::OnConnectToServer()
{
  registerVariables();
  return (true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool IMU::Iterate()
{
  AppCastingMOOSApp::Iterate();
  
  checkConnected();
  //checkStatus();
  
  readCalibration();
  readEuler();
  readQuaternion();
  
  AppCastingMOOSApp::PostReport();
  return (true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool IMU::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();
  
  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if (!m_MissionReader.GetConfiguration(GetAppName(), sParams))
  {
    reportConfigWarning("No config block found for " + GetAppName());
  }
  
  STRING_LIST::iterator p;
  for (p = sParams.begin();
       p != sParams.end();
       p++)
  {
    std::string orig = *p;
    std::string line = *p;
    std::string param = toupper(biteStringX(line, '='));
    std::string value = line;
    
    bool handled = false;
    if (param == "FOO")
    {
      handled = true;
    }
    else if (param == "BAR")
    {
      handled = true;
    }
    
    if (!handled)
    {
      reportUnhandledConfigWarning(orig);
    }
    
  }
  
  registerVariables();
  return (true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void IMU::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool IMU::buildReport()
{
  m_msgs << "============================================ \n";
  m_msgs << "File:   iIMU                                 \n";
  m_msgs << "============================================ \n";
  
  ACTable actab(2);
  actab << "Param | Value";
  actab.addHeaderLines();
  actab << "I2C ADDR" << m_bno055.dev_addr;
  actab << "Chip ID" << m_bno055.chip_id;
  actab << "Calib Status" << m_sysCalStatus;
  actab << "Heading" << m_euler.h;
  m_msgs << actab.getFormattedString();
  
  return (true);
}

void IMU::checkConnected()
{
  if (!m_open)
  {
    reportEvent("IMU not connected! Retrying...");
    initIMU();
    return;
  }
}

void IMU::checkStatus()
{
  if (bno055_get_sys_stat_code(&m_status))
  {
    reportEvent("Error reading status!");
    m_open = false;
  }
  
  if (m_status == 1) // status = 1 indicates error
  {
    bno055_get_sys_error_code(&m_lastErrorCode);
    std::string msg = "IMU Error code reported: " + std::to_string((unsigned int)m_lastErrorCode);
    reportEvent(msg);
  }
}

void IMU::readCalibration()
{
  if (bno055_get_sys_calib_stat(&m_sysCalStatus))
  {
    reportEvent("Failed to get system calibration data");
    m_open = false;
    return;
  }
  
  if (m_sysCalStatus == 0)
  {
    reportEvent("IMU not calibrated - lost absolute orientation");
  }
  
  Notify("IMU_CALIB_STATUS", m_sysCalStatus);
}

void IMU::readEuler()
{
  if (bno055_convert_double_euler_hpr_deg(&m_euler))
  {
    reportEvent("Failed to get euler data");
    m_open = false;
    return;
  }
  
  Notify("IMU_EULER_P", m_euler.p);
  Notify("IMU_EULER_R", m_euler.r);
  Notify("IMU_EULER_H", m_euler.h);
}

void IMU::readQuaternion()
{
  if (bno055_read_quaternion_wxyz(&m_quaternion))
  {
    reportEvent("Failed to get Quaternion");
    m_open = false;
    return;
  }
  
  Notify("IMU_QUATERNION_X", m_quaternion.x);
  Notify("IMU_QUATERNION_Y", m_quaternion.y);
  Notify("IMU_QUATERNION_Z", m_quaternion.z);
  Notify("IMU_QUATERNION_W", m_quaternion.w);
}

bool IMU::checkPOST()
{
  int comrslt = 0;
  unsigned char stMCU = 0, stAcc = 0, stGyr = 0, stMag = 0;
  
  comrslt += bno055_get_selftest_mcu(&stMCU);
  comrslt += bno055_get_selftest_accel(&stAcc);
  comrslt += bno055_get_selftest_gyro(&stGyr);
  comrslt += bno055_get_selftest_mag(&stMag);
  
  if (comrslt)
  {
    reportEvent("Error reading POST");
    m_open = false;
    return false;
  }
  
  if (stMCU == 0)
  {
    reportEvent("MCU self test failed!");
  }
  
  if (stAcc == 0)
  {
    reportEvent("Accelerometer self test failed!");
  }
  
  if (stGyr == 0)
  {
    reportEvent("Gyroscope self test failed!");
  }
  
  if (stMag == 0)
  {
    reportEvent("Magnetometer self test failed!");
  }
}