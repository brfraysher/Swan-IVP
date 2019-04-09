#--------------------------------------------------------
# The CMakeLists.txt for:                       iGPS
# Author(s):                              Andrew Sucato
#--------------------------------------------------------

SET(NEMATOAD
  NemaToad/src/GPSFix.cpp
  NemaToad/src/GPSService.cpp
  NemaToad/src/NMEACommand.cpp
  NemaToad/src/NMEAParser.cpp
  NemaToad/src/NumberConversion.cpp
)



SET(SRC
  GPS.cpp
  GPS_Info.cpp
  main.cpp)

ADD_EXECUTABLE(iGPS ${SRC} ${NEMATOAD})

target_include_directories(iGPS PRIVATE NemaToad)
TARGET_LINK_LIBRARIES(iGPS
   serial
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

