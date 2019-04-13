/************************************************************/
/*    NAME: Spencer Davis                                              */
/*    ORGN: The University of Alabama                       */
/*    FILE: MotorController.cpp                                        */
/*    DATE: 04/12/19                                          */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "MotorController.h"


//---------------------------------------------------------
// Constructor

MotorController::MotorController(const std::string &port)
:   m_port(port, 115200),
    m_rudder(0),
    m_thrust(0),
    m_leftMotorSpeed(90),
    m_rightMotorSpeed(90)

{
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
    for (p = NewMail.begin(); p != NewMail.end(); p++)
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
            m_rudder = msg.GetDouble();
        else if (key == "DESIRED_THRUST")
            m_thrust = msg.GetDouble();
        else if (key != "APPCAST_REQ") // handled by AppCastingMOOSApp
            reportRunWarning("Unhandled Mail: " + key);
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

    double left_command;
    double right_command;

    left_command = m_thrust + m_rudder + 90;
    right_command = m_thrust - m_rudder + 90;

    left_command = std::min(160.0, std::max(20.0, left_command));
    right_command = std::min(160.0, std::max(20.0, right_command));

    uint8_t left = static_cast<uint8_t>(left_command);
    uint8_t right = static_cast<uint8_t>(right_command);

    std::vector<uint8_t> data = {'K', left, right, '\n'};
    const std::string arduinoPortWarning = "Arduino Port not open!!!";
    if (m_port.isOpen())
    {
        retractRunWarning(arduinoPortWarning);
        m_port.write(data);
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
    actab << "Left Motor Speed" << m_leftMotorSpeed;
    actab << "Right Motor Speed" << m_rightMotorSpeed;
    m_msgs << actab.getFormattedString();

    return (true);
}




