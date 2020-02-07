/************************************************************/
/*    NAME: Spencer Davis                                              */
/*    ORGN: The University of Alabama                       */
/*    FILE: Localization.h                                          */
/*    DATE: 04/17/19                                          */
/************************************************************/

#ifndef Localization_HEADER
#define Localization_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "MOOS/libMOOSGeodesy/MOOSGeodesy.h"

class Localization : public AppCastingMOOSApp
{
 public:
   Localization();
   ~Localization();

 protected: // Standard MOOSApp functions to overload
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables
   double m_lat;
   double m_long;
   double m_headingIMU;
   double m_headingGPS1;
   double m_speedGPS1;
   
   double m_north;
   double m_east;
   
   double m_headingBias;

   bool m_geo_ok;

   CMOOSGeodesy m_geodesy;
};

#endif
