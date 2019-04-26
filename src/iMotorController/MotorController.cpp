/************************************************************/
/*    NAME: Andrew Sucato                                   */
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

MotorController::MotorController()
:   m_address("UNINITIALIZED"),
    m_baud(0),
    m_rudder(0),
    m_thrust(0),
    m_maxPwmVal(160),
    m_minPwmVal(90),
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

    left_command = m_thrust + (0.5 * m_rudder);
    right_command = m_thrust - (0.5 * m_rudder);

    m_leftMotorSpeed = (left_command * (m_maxPwmVal - m_minPwmVal)) / 200 + 90;
    m_rightMotorSpeed = (right_command * (m_maxPwmVal - m_minPwmVal)) / 200 + 90;

    uint8_t left = static_cast<uint8_t>(m_leftMotorSpeed);
    uint8_t right = static_cast<uint8_t>(m_rightMotorSpeed);

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
            reportUnhandledConfigWarning(orig);

    }

    if (m_address == "UNINITIALIZED")
        reportConfigWarning("Serial port address not given");
    if (m_baud == 0)
        reportConfigWarning("Serial port baud rate not given");

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

bool MotorController::initializeSerial()
{
    try
    {
        m_port.setPort(m_address);
        m_port.setBaudrate(m_baud);
        m_port.open();
    }
    catch (const serial::IOException& e)
    {
        reportRunWarning(e.what());
        return false;
    }
    catch (const serial::SerialException& e)
    {
        reportRunWarning(e.what());
        return false;
    }
    return true;
}



