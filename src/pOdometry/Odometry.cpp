/************************************************************/
/*    NAME: Andrew Sucato                                              */
/*    ORGN:                                              */
/*    FILE: Odometry.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Odometry.h"

//---------------------------------------------------------
// Constructor

Odometry::Odometry()
{
  m_first_reading = false;
  m_current_x = 0;
  m_current_y = 0;
  m_previous_x = 0;
  m_previous_y = 0;
  m_total_distance = 0;
}

//---------------------------------------------------------
// Destructor

Odometry::~Odometry()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Odometry::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    std::string key = msg.GetKey();

    if (key == "NAV_X")
    {
      m_previous_x = m_current_x;
      m_current_x = msg.GetDouble();
    }
    else if (key == "NAV_Y")
    {
      m_previous_y = m_current_y;
      m_current_y = msg.GetDouble();
    }

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Odometry::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Odometry::Iterate()
{
  if (m_first_reading)
  {
    m_first_reading = false;
    return(true);
  }

  double x_delta, y_delta, delta;

  x_delta = std::abs(m_current_x - m_previous_x);
  y_delta = std::abs(m_current_y = m_previous_y);
  delta = std::sqrt(std::pow(x_delta, 2) + std::pow(y_delta, 2));

  m_total_distance += delta;

  Notify("ODOMETRY_DIST", m_total_distance);

  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Odometry::OnStartUp()
{
  std::list<std::string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    std::list<std::string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      std::string original_line = *p;
      std::string param = stripBlankEnds(toupper(biteString(*p, '=')));
      std::string value = stripBlankEnds(*p);
      
      if(param == "FOO") {
        //handled
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Odometry::RegisterVariables()
{
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
}

