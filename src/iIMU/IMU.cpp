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
{
  m_bno055.dev_addr = BNO055_I2C_ADDR1;
  m_bno055.bus_read = bus_read;
  m_bno055.bus_write = bus_write;
  m_bno055.delay_msec = delay_func;
  
  if ((m_bno055.fid = open("/dev/i2c-1", O_RDWR)) < 0)
  {
    reportRunWarning("Failed to open I2C Bus!!!");
  }
  
  if (ioctl(m_bno055.fid, I2C_SLAVE, m_bno055.dev_addr))
  {
    reportRunWarning("Failed to reach IMU on I2C Bus!!!");
  }
  
  bno055_init(&m_bno055);
  
  bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);
}

//---------------------------------------------------------
// Destructor

IMU::~IMU()
{
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
  
  if (bno055_convert_double_euler_hpr_deg(&m_euler))
  {
    reportRunWarning("Failed to get euler data");
  }
  else
  {
    retractRunWarning("Failed to get euler data");
    Notify("Heading", m_euler.h);
  }
  
  if (bno055_get_sys_calib_stat(&m_sysCalStatus))
  {
    reportRunWarning("Failed to get system calibration data");
  }
  else if (m_sysCalStatus == 0)
  {
    reportRunWarning("IMU not calibrated - lost absolute orientation");
    Notify("IMU_CALIB_STATUS", m_sysCalStatus);
  }
  else
  {
    retractRunWarning("Failed to get system calibration data");
    retractRunWarning("IMU not calibrated - lost absolute orientation");
    Notify("IMU_CALIB_STATUS", m_sysCalStatus);
  }
  
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
  m_msgs << "File:                                        \n";
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
