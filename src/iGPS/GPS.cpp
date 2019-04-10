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
    : m_current_lat(0),
      m_current_lon(0)
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
    for (p = NewMail.begin(); p != NewMail.end(); p++)
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
            cout << "great!";

        else if (key != "APPCAST_REQ") // handled by AppCastingMOOSApp
            reportRunWarning("Unhandled Mail: " + key);
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
        std::cout << nmeaSentence << std::endl;
        try
        {
            m_parser->readLine(nmeaSentence);
        }
        catch (nmea::NMEAParseError& e)
        {

        }

    }

//    if (!m_gps->fix.locked())
//    {
//        reportRunWarning("GPS1 lock lost");
//    }
//    else
//    {
//        retractRunWarning("GPS1 lock lost");
        Notify("GPS1_LAT", m_gps->fix.latitude);
        Notify("GPS1_LON", m_gps->fix.longitude);
    Notify("GPS1_LAT", 12);
    Notify("GPS1_LON", 13);

//        m_msgs << "Lat: " << m_gps->fix.latitude << std::endl;
//        m_msgs << "Lon: " << m_gps->fix.longitude << std::endl;
//    }


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
        reportConfigWarning("No config block found for " + GetAppName());

    STRING_LIST::iterator p;
    for (p = sParams.begin(); p != sParams.end(); p++)
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
            reportUnhandledConfigWarning(orig);

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
    m_msgs << "File:                                        \n";
    m_msgs << "============================================ \n";

    ACTable actab(4);
    actab << "Alpha | Bravo | Charlie | Delta";
    actab.addHeaderLines();
    actab << "one" << "two" << "three" << "four";
    m_msgs << actab.getFormattedString();

    return (true);
}




