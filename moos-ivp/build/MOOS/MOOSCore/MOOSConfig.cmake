# Configuration file for MOOS library.

# Pick up the auto-generated file which knows how to add the library targets
# This will mean that we don't have to supply full paths for the libraries
set(exports_file "/home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSCore/UseMOOS.cmake")

if( NOT MOOS_FOUND)
    #only pull in if not already done!
    if (EXISTS ${exports_file})
        #message(STATUS "CMake is running ${exports_file}")
        include(${exports_file})
    endif ()
endif()


include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( MOOS DEFAULT_MSG exports_file )
#message(STATUS "MOOS_FOUND=${MOOS_FOUND} time to dance")


# Project specific variables are dynamically inserted here

# Export variables for MOOS Library
set(MOOS_LIBRARIES "MOOS")
set(MOOS_INCLUDE_DIRS "/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/include;/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/App/include;/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/Comms/include;/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/DB/include;/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/Utils/include;/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/Thirdparty/PocoBits/include;/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/Thirdparty/getpot/include;/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/Thirdparty/AppCasting/include")
set(MOOS_DEPEND_LIBRARIES "pthread;m")
set(MOOS_DEPEND_INCLUDE_DIRS "/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/include;/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/App/include;/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/Comms/include;/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/DB/include;/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/Utils/include;/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/Thirdparty/PocoBits/include;/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/Thirdparty/getpot/include;/home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSCore/Core/libMOOS/Thirdparty/AppCasting/include")
