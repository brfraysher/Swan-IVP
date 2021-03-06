/************************************************************/
/*    NAME: Spencer Davis                                              */
/*    ORGN: The University of Alabama                       */
/*    FILE: Localization.cpp                                        */
/*    DATE: 04/17/19                                          */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "Localization.h"


//---------------------------------------------------------
// Constructor

Localization::Localization()
{
}

//---------------------------------------------------------
// Destructor

Localization::~Localization()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Localization::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    std::string key    = msg.GetKey();

#if 0 // Keep these around just for template
    std::string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    std::string sval  = msg.GetString();
    std::string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
    //GPS Data Messages
    if(key == "GPS1_LAT")
      m_lat = msg.GetDouble();
    else if (key == "GPS1_LON")
      m_long = msg.GetDouble();
    else if (key == "GPS1_HEADING")
      m_gps_heading = msg.GetDouble();
    else if (key == "GPS1_SPEED")
      m_speed = msg.GetDouble();
    
    //GPS Status Messages
    else if (key == "GPS1_QUALITY")
      m_gps_quality = msg.GetDouble();
    else if (key == "GPS1_STATUS")
      m_gps_status = msg.GetString();
    
    //IMU Data Messages
    else if (key == "IMU_EULER_H")
      m_imu_heading = msg.GetDouble(); 
    
    //IMU Status Messages
    else if (key == "IMU_MAG_CALIB_STATUS")
      m_imu_mag_status = msg.GetDouble();
    else if (key == "IMU_GYR_CALIB_STATUS")
      m_imu_gyro_status = msg.GetDouble();

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }

   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Localization::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Localization::Iterate()
{
  AppCastingMOOSApp::Iterate();


  if (!m_geo_ok)
    reportRunWarning("GPS coordinate to local conversions not available");
  else
    retractRunWarning("GPS coordinate to local conversions not available");
  

  // Apply bias to heading
  m_imu_status = m_imu_gyro_status + m_imu_mag_status;
  m_imu_active = m_imu_mag_status!=0 && m_imu_gyro_status!=0 && m_imu_status > 2;
  m_gps_active = m_gps_status == "A" && m_gps_quality != 0;
  if(!m_imu_active && !m_gps_active){
    reportEvent("All sensors reporting bad status");
    Notify("MOOS_MANUAL_OVERIDE",true);
  }
  else{
    if(!m_imu_active || (m_gps_active && m_speed > 0.5)){
      m_heading = m_gps_heading + m_headingBias;
    }
    else{
      m_heading = m_imu_heading + m_headingBias; 
    }
    if (m_heading > 360)
    {
      m_heading -= 360;
    }
    else if (m_heading < 0)
    {
      m_heading += 360;
    }
    m_geodesy.LatLong2LocalUTM(m_lat, m_long, m_north, m_east);
    Notify("MOOS_MANUAL_OVERIDE",false);
    Notify("NAV_Y", m_north);
    Notify("NAV_X", m_east);

    Notify("NAV_HEADING", m_heading);
    Notify("NAV_SPEED", m_speed);
  }
  Notify("IMU_ACTIVE",m_imu_active);
  Notify("GPS_ACTIVE",m_gps_active);

  
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Localization::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    std::string orig  = *p;
    std::string line  = *p;
    std::string param = toupper(biteStringX(line, '='));
    std::string value = line;

    bool handled = false;
    if(param == "HEADING_BIAS") {
      handled = true;
      m_headingBias = std::stod(value);
    }
    else if(param == "BAR") {
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }

  // look for latitude, longitude global variables
  double latOrigin, longOrigin;
  if(!m_MissionReader.GetValue("LatOrigin", latOrigin))
  {
    MOOSTrace("pLocalization: LatOrigin not set in *.moos file.\n");
    m_geo_ok = false;
  }
  else if(!m_MissionReader.GetValue("LongOrigin", longOrigin))
  {
    MOOSTrace("pLocalization: LongOrigin not set in *.moos file\n");
    m_geo_ok = false;
  }
  else
  {
    m_geo_ok = true;
    // initialize m_geodesy
    if(!m_geodesy.Initialise(latOrigin, longOrigin))
    {
        MOOSTrace("pLocalization: Geodesy init failed.\n");
        m_geo_ok = false;
    }
  }


  registerVariables();
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void Localization::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("GPS1_LAT", 0);
  Register("GPS1_LON", 0);
  Register("IMU_EULER_H", 0);
  Register("GPS1_SPEED", 0);
  Register("GPS1_HEADING",0);
  Register("GPS1_QUALITY",0);
  Register("GPS1_STATUS",0);
  Register("IMU_MAG_CALIB_STATUS",0);
  Register("IMU_GYR_CALIB_STATUS",0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool Localization::buildReport()
{
  m_msgs << "============================================ \n";
  m_msgs << "File:    pLocalization                            \n";
  m_msgs << "============================================ \n";

  ACTable actab(2);
  actab << "Value | Param";
  actab.addHeaderLines();
  actab << "Latitude" << m_lat;
  actab << "Longitude" << m_long;
  actab << "North" << m_north;
  actab << "East" << m_east;
  actab << "Speed" << m_speed;
  actab << "Heading" << m_heading;
  actab << "" << "";
  actab << "IMU Active" << m_imu_active;
  actab << "IMU Heading" << m_imu_heading;
  actab << "GPS Active" << m_gps_active;
  actab << "GPS Heading" << m_gps_heading;
  m_msgs << actab.getFormattedString();

  return(true);
}




