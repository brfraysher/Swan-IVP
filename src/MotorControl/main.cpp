// This is the CMOOSApp example from the MOOS website documentation
// Included here for convenience
//
// Feb 10th, 2013

#include "MOOS/libMOOS/App/MOOSApp.h"
#include "MotorControl.h"

int main(int argc, char *argv[]) 
{
  //here we do some command line parsing ...
  MOOS::CommandLineParser P(argc, argv);
 
  //mission file could be first free parameter
  std::string mission_file = P.GetFreeParameter(0, "MotorControl.moos");
  
  //app name can be the second free parameter
  std::string app_name = P.GetFreeParameter(1, "MotorControl");
  
  MotorControl App("/dev/serial/by-id/usb-Arduino__www.arduino.cc__0042_5573631303835111D1B1-if00");
  App.Run(app_name, mission_file, argc, argv);
  
  return(0);
}

