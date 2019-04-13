/************************************************************/
/*    NAME: Spencer Davis                                              */
/*    ORGN: The University of Alabama                       */
/*    FILE: main.cpp                                        */
/*    DATE: 04/12/19                                          */
/************************************************************/

#include <string>
#include "MBUtils.h"
#include "ColorParse.h"
#include "MotorController.h"
#include "MotorController_Info.h"

int main(int argc, char *argv[])
{
  std::string mission_file;
  std::string run_command = argv[0];

  for(int i=1; i<argc; i++) {
    std::string argi = argv[i];
    if((argi=="-v") || (argi=="--version") || (argi=="-version"))
      showReleaseInfoAndExit();
    else if((argi=="-e") || (argi=="--example") || (argi=="-example"))
      showExampleConfigAndExit();
    else if((argi == "-h") || (argi == "--help") || (argi=="-help"))
      showHelpAndExit();
    else if((argi == "-i") || (argi == "--interface"))
      showInterfaceAndExit();
    else if(strEnds(argi, ".moos") || strEnds(argi, ".moos++"))
      mission_file = argv[i];
    else if(strBegins(argi, "--alias="))
      run_command = argi.substr(8);
    else if(i==2)
      run_command = argi;
  }

  if(mission_file == "")
    showHelpAndExit();

  std::cout << termColor("green");
  std::cout << "iMotorController launching as " << run_command << std::endl;
  std::cout << termColor() << std::endl;

  MotorController MotorController("/dev/serial/by-id/usb-Arduino__www.arduino.cc__0042_5573631303835111D1B1-if00");

  MotorController.Run(run_command.c_str(), mission_file.c_str());

  return(0);
}

