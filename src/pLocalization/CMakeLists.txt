#--------------------------------------------------------
# The CMakeLists.txt for:                       pLocalization
# Author(s):                              Spencer Davis
#--------------------------------------------------------

SET(SRC
  Localization.cpp
  Localization_Info.cpp
  main.cpp
)

find_package(MOOSGeodesy)
include_directories(${MOOSGeodesy_INCLUDE_DIRS})

ADD_EXECUTABLE(pLocalization ${SRC})

TARGET_LINK_LIBRARIES(pLocalization
   ${MOOS_LIBRARIES}
   ${MOOSGeodesy_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

