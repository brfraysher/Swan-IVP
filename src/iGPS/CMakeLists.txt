#--------------------------------------------------------
# The CMakeLists.txt for:                       iGPS
# Author(s):                              Andrew Sucato
#--------------------------------------------------------

SET(SRC
  GPS.cpp
  GPS_Info.cpp
  main.cpp
        Garmin19x.cpp Garmin19x.h garminTest.cpp)

ADD_EXECUTABLE(iGPS ${SRC})

TARGET_LINK_LIBRARIES(iGPS
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

