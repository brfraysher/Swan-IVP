#--------------------------------------------------------
# The CMakeLists.txt for:              iMotorController
# Author(s):                              Spencer Davis
#--------------------------------------------------------

SET(SRC
        MotorController.cpp
        MotorController_Info.cpp
        main.cpp
        )

ADD_EXECUTABLE(iMotorController ${SRC})

TARGET_LINK_LIBRARIES(iMotorController
        ${MOOS_LIBRARIES}
        apputil
        mbutil
        m
        pthread
        serial)

