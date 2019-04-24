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
    for (p = NewMail.begin(); p != NewMail.end(); p++)
    {
        CMOOSMsg &msg = *p;
        std::string key = msg.GetKey();

#if 0 // Keep these around just for template
        std::string comm  = msg.GetCommunity();
        double dval  = msg.GetDouble();
        std::string sval  = msg.GetString();
        std::string msrc  = msg.GetSource();
        double mtime = msg.GetTime();
        bool   mdbl  = msg.IsDouble();
        bool   mstr  = msg.IsString();
#endif

        if (key == "GPS1_LAT")
            m_lat = msg.GetDouble();
        else if (key == "GPS1_LONG")
            m_long = msg.GetDouble();
        else if (key == "GPS1_HEADING")
            m_heading = msg.GetDouble();
        else if (key == "GPS1_SPEED")
            m_speed = msg.GetDouble();

        else if (key != "APPCAST_REQ") // handled by AppCastingMOOSApp
            reportRunWarning("Unhandled Mail: " + key);
    }

    return (true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Localization::OnConnectToServer()
{
    registerVariables();
    return (true);
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

    double north;
    double east;

    m_geodesy.LatLong2LocalUTM(m_lat, m_long, north, east);
    Notify("NAV_Y", north);
    Notify("NAV_X", east);

    Notify("NAV_HEADING", m_heading);
    Notify("NAV_SPEED", m_speed);

    m_lastX = north;
    m_lastY = east;

    AppCastingMOOSApp::PostReport();
    return (true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Localization::OnStartUp()
{
    AppCastingMOOSApp::OnStartUp();

    STRING_LIST sParams;
    m_MissionReader.EnableVerbatimQuoting(false);
    if (!m_MissionReader.GetConfiguration(GetAppName(), sParams))
        reportConfigWarning("No config block found for " + GetAppName());

    STRING_LIST::iterator p;
    for (p = sParams.begin(); p != sParams.end(); p++)
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
            reportUnhandledConfigWarning(orig);

    }

    // look for latitude, longitude global variables
    double latOrigin, longOrigin;
    if (!m_MissionReader.GetValue("LatOrigin", latOrigin))
    {
        reportConfigWarning("pLocalization: LatOrigin not set in *.moos file.\n");
        m_geo_ok = false;
    }
    else if (!m_MissionReader.GetValue("LongOrigin", longOrigin))
    {
        reportConfigWarning("pLocalization: LongOrigin not set in *.moos file\n");
        m_geo_ok = false;
    }
    else
    {
        m_geo_ok = true;
        // initialize m_geodesy
        if (!m_geodesy.Initialise(latOrigin, longOrigin))
        {
            reportConfigWarning("pLocalization: Geodesy init failed.\n");
            m_geo_ok = false;
        }
    }


    registerVariables();
    return (true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void Localization::registerVariables()
{
    AppCastingMOOSApp::RegisterVariables();
    Register("GPS1_LAT", 0);
    Register("GPS1_LONG", 0);
    Register("GPS1_HEADING", 0);
    Register("GPS1_SPEED", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool Localization::buildReport()
{
    ACTable actab(2);
    actab << "Param | Val";
    actab.addHeaderLines();
    actab << "NAV_X" << m_lastX;
    actab << "NAV_Y" << m_lastY;
    actab << "" << "";
    actab << "NAV_HEADING" << m_heading;
    actab << "NAV_SPEED" << m_speed;
    m_msgs << actab.getFormattedString();

    return (true);
}




