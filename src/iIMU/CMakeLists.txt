#--------------------------------------------------------
# The CMakeLists.txt for:                       iIMU
# Author(s):                              Andrew Sucato
#--------------------------------------------------------

SET(SRC
        IMU.cpp
        IMU_Info.cpp
        main.cpp
        )

ADD_LIBRARY(Bno055 bno055.c)

ADD_EXECUTABLE(iIMU ${SRC})

TARGET_LINK_LIBRARIES(iIMU
        Bno055
        ${MOOS_LIBRARIES}
        apputil
        mbutil
        m
        pthread
        i2c
        )

