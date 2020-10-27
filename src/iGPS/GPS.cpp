/************************************************************/
/*    NAME: Andrew Sucato                                              */
/*    ORGN: MIT                                             */
/*    FILE: GPS.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "GPS.h"
#include "nmeaparse/nmea.h"

using namespace std;

//---------------------------------------------------------
// Constructor

GPS::GPS()
{
}

//---------------------------------------------------------
// Destructor

GPS::~GPS()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GPS::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);
  
  MOOSMSG_LIST::iterator p;
  for (p = NewMail.begin();
       p != NewMail.end();
       p++)
  {
    CMOOSMsg &msg = *p;
    string key = msg.GetKey();

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
      cout << "great!";
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

bool GPS::OnConnectToServer()
{
  registerVariables();
  return (true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool GPS::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  
  if (m_comPort->isOpen())
  {
    std::string nmeaSentence;
    m_comPort->readline(nmeaSentence);
    m_comPort->flushInput();
    try
    {
      m_parser->readLine(nmeaSentence);
    }
    catch (nmea::NMEAParseError &e)
    {
    
    }
  }
  
  if (!m_gps->fix.locked())
  {
    reportRunWarning("GPS1 lock lost");
  }
  
  retractRunWarning("GPS1 lock lost");
  Notify("GPS1_LOCKED", m_gps->fix.locked());
  Notify("GPS1_LAT", m_gps->fix.latitude);
  Notify("GPS1_LON", m_gps->fix.longitude);
  Notify("GPS1_SPEED", m_gps->fix.speed);
  Notify("GPS1_HEADING", m_gps->fix.travelAngle);
  Notify("GPS1_QUALITY", m_gps->fix.quality);
  Notify("GPS1_STATUS", m_gps->fix.status);
  Notify("GPS1_ALT", m_gps->fix.altitude);
  Notify("GPS1_TRACKING_SATS", (double) m_gps->fix.trackingSatellites);
  Notify("GPS1_VISIBLE_SATS", (double) m_gps->fix.visibleSatellites);
  Notify("GPS1_HORIZONTAL_ACCURACY", m_gps->fix.horizontalAccuracy());
  Notify("GPS1_VERTICAL_ACCURACY", m_gps->fix.verticalAccuracy());
  
  
  AppCastingMOOSApp::PostReport();
  return (true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GPS::OnStartUp()
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
    string orig = *p;
    string line = *p;
    string param = toupper(biteStringX(line, '='));
    string value = line;
    
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
  std::string port = "/dev/ttyS1";
  const uint32_t baud = 38400;
  
  m_comPort = new serial::Serial(port, baud);
  m_parser = new nmea::NMEAParser;
  m_gps = new nmea::GPSService(*m_parser);
  
  registerVariables();
  return (true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void GPS::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("LAT", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool GPS::buildReport()
{
  m_msgs << "============================================ \n";
  m_msgs << "File:   iGPS                                 \n";
  m_msgs << "============================================ \n";
  
  ACTable actab(2);
  actab << "Param | Value";
  actab.addHeaderLines();
  actab << "Locked" << m_gps->fix.locked();
  actab << "Status" << m_gps->fix.status;
  actab << "Quality" << m_gps->fix.quality;
  actab << "Tracking Sats" << m_gps->fix.trackingSatellites;
  actab << "Visible Sats" << m_gps->fix.visibleSatellites;
  actab << "Current Latitude" << m_gps->fix.latitude;
  actab << "Current Longitude" << m_gps->fix.longitude;
  actab << "Speed" << m_gps->fix.speed;
  actab << "GPS Heading" << m_gps->fix.travelAngle;
  actab << "Vertical Accuracy" << m_gps->fix.verticalAccuracy();
  actab << "Horizontal Accuracy" << m_gps->fix.horizontalAccuracy();
  m_msgs << actab.getFormattedString();
  
  return (true);
}




