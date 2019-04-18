#--------------------------------------------------------
# The CMakeLists.txt for:                       pLocalization
# Author(s):                              Spencer Davis
#--------------------------------------------------------

SET(SRC
  Localization.cpp
  Localization_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pLocalization ${SRC})

TARGET_LINK_LIBRARIES(pLocalization
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

