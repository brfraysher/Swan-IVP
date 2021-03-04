/************************************************************/
/*    NAME: Andrew Sucato                                   */
/*    ORGN: The University of Alabama                       */
/*    FILE: MotorController.cpp                             */
/*    DATE: 04/12/19                                        */
/************************************************************/

#include <iterator>
#include <chrono>
#include <thread>
#include <MBUtils.h>
#include <ACTable.h>
#include "MotorController.h"


//---------------------------------------------------------
// Constructor

MotorController::MotorController()
        : m_address("UNINITIALIZED"),
          m_baud(0),
          m_rudder(0),
          m_thrust(0),
          m_imu_active(0),
          m_gps_active(0)
          //m_leftMotorSpeed(90),
          //m_rightMotorSpeed(90)
{
  std::system("stty -F /dev/serial/by-id/usb-Arduino__www.arduino.cc__0042_5573631303835111D1B1-if00 -hupcl");
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

//---------------------------------------------------------
// Destructor

MotorController::~MotorController()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool MotorController::OnNewMail(MOOSMSG_LIST &NewMail)
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
    
    if (key == "DESIRED_RUDDER")
    {
      m_rudder = msg.GetDouble();
    }
    else if (key == "DESIRED_THRUST")
    {
      m_thrust = msg.GetDouble();
    }
    else if (key == "GPS_ACTIVE"){
      m_gps_active = int(msg.GetDouble());
    }
    else if (key == "IMU_ACTIVE"){
      m_imu_active = int(msg.GetDouble());
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

bool MotorController::OnConnectToServer()
{
  registerVariables();
  return (true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool MotorController::Iterate()
{
  AppCastingMOOSApp::Iterate();
  /*autonomy states:
    imu & gps active - 3, 
    only gps - 2, 
    only imu - 1, 
    neither - 0
  */
  m_autonomy_status = (m_imu_active && m_gps_active) ? 3 : (m_gps_active ? 2 : (m_imu_active ? 1 : 0));
  
  uint8_t rudder = static_cast<uint8_t >(m_rudder);
  uint8_t thrust = static_cast<uint8_t >(m_thrust);

  std::vector<uint8_t> data = {'K', rudder, thrust, m_autonomy_status, '\n'};
  //std::string data = "K"+std::to_string(rudder)+","+std::to_string(thrust)+","+std::to_string(m_autonomy_status)+"\n";
  const std::string arduinoPortWarning = "Arduino Port not open!!!";
  if (m_port.isOpen())
  {
    retractRunWarning(arduinoPortWarning);
    m_port.write(data);
    int serialTimeout = 0;
    while (m_port.available() > 1 && serialTimeout < 10)
    {
      if (m_port.read(sizeof(char)) == "K")
      {
        m_arduinoMsg = m_port.readline();
      }
       if (m_port.read(sizeof(char)) == "C")
      {
        m_compensation = m_port.readline();
      }
      serialTimeout++;
    }
    m_port.flushInput();
    
    Notify("ARDUINO_MSG", m_arduinoMsg);
  }
  else
  {
    reportRunWarning(arduinoPortWarning);
    return false;
  }
  

  
  AppCastingMOOSApp::PostReport();
  return (true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool MotorController::OnStartUp()
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
    if (param == "ADDRESS")
    {
      m_address = value;
      handled = true;
    }
    else if (param == "BAUD")
    {
      m_baud = std::stoi(value);
      handled = true;
    }
    
    if (!handled)
    {
      reportUnhandledConfigWarning(orig);
    }
    
  }
  
  if (m_address == "UNINITIALIZED")
  {
    reportConfigWarning("Serial port address not given");
  }
  if (m_baud == 0)
  {
    reportConfigWarning("Serial port baud rate not given");
  }
  
  initializeSerial();
  
  registerVariables();
  return (true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void MotorController::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("DESIRED_RUDDER");
  Register("DESIRED_THRUST");
  Register("IMU_ACTIVE");
  Register("GPS_ACTIVE");
}


//------------------------------------------------------------
// Procedure: buildReport()

bool MotorController::buildReport()
{
  ACTable actab(2);
  actab << "Param | Value";
  actab.addHeaderLines();
  actab << "Desired Rudder" << m_rudder;
  actab << "Desired Thrust" << m_thrust;
  actab << "" << "";
  actab << "GPS Active" << m_gps_active;
  actab << "IMU Active" << m_imu_active;
  actab << "" << "";
  actab << "Autonomy Status" << m_autonomy_status;
  actab << "" << "";
  actab << "Left Motor Speed" << m_leftMotorSpeed;
  actab << "Right Motor Speed" << m_rightMotorSpeed;
  actab << "" << "";
  actab << "Compensation" << m_compensation;
  actab << "Arduino message" << m_arduinoMsg;
 
  
  
  m_msgs << actab.getFormattedString();
  
  return (true);
}

bool MotorController::initializeSerial()
{
  try
  {
    m_port.setPort(m_address);
    m_port.setBaudrate(m_baud);
    m_port.open();
  }
  catch (const serial::IOException &e)
  {
    reportRunWarning(e.what());
    return false;
  }
  catch (const serial::SerialException &e)
  {
    reportRunWarning(e.what());
    return false;
  }
  return true;
}



