// This is the CMOOSApp example from the MOOS website documentation
// Included here for convenience
//
// Feb 10th, 2013

#include "MOOS/libMOOS/App/MOOSApp.h"
#include <serial/serial.h>

class MotorControl : public CMOOSApp
{
  serial::Serial _port;

  double _rudder;
  double _thrust;

public:
  MotorControl(const std::string& port) : _port(port, 115200)
  {

  }

private:

  bool OnNewMail (MOOSMSG_LIST & Mail) override
  {
    MOOSMSG_LIST::iterator p;
    for (p = Mail.begin(); p != Mail.end(); p++)
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
        _rudder = p->GetDouble();
        std::cout << "RUDDER: " << _rudder << '\n';
      }
      else if (key == "DESIRED_THRUST")
      {
        _thrust = p->GetDouble();
        std::cout << "THRUST: " << _thrust << '\n';
      }
    }

    return (true);
  }
  
  bool OnConnectToServer () override
  {
    return Register("DESIRED_RUDDER") && Register("DESIRED_THRUST");
  } 
  
  bool Iterate() override
  {
    double left_command;
    double right_command;

    left_command = _thrust + _rudder + 90;
    right_command = _thrust - _rudder + 90;

    left_command = std::min(160.0, std::max(20.0, left_command));
    right_command = std::min(160.0, std::max(20.0, right_command));

    uint8_t left = static_cast<uint8_t>(left_command);
    uint8_t right = static_cast<uint8_t>(right_command);

    std::vector<uint8_t> data = { 'K', left, right, '\n' };
    if (_port.isOpen())
    {
      std::cout << "Writing" << std::endl;
      std::cout << "Left: " << (int)left << std::endl;
      std::cout << "Right: " << (int)right << std::endl;
      _port.write(data);
    }
    else
    {
      std::cout << "Not writing\n";
      return false;
    }

    return true ;
  }
};

