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
    double m_gps_heading;
    int m_gps_quality;
    bool m_gps_active;
    std::string m_gps_status;

    double m_imu_heading;
    int m_imu_mag_status;
    int m_imu_gyro_status;
    int m_imu_status;
    bool m_imu_active;

    double m_speed;
    double m_heading;
    double m_north;
    double m_east;
    
    double m_headingBias;

    bool m_geo_ok;

    CMOOSGeodesy m_geodesy;
};

#endif
