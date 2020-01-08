cmake_minimum_required(VERSION 3.0)
project( PhaseShiftSurface )

#This should not be used normally
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/.. CACHE PATH "..." FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-D_GNULINUX")

find_package( PkgConfig REQUIRED )
find_package( OpenCV REQUIRED )

################
#All the paths are relative to the CMakeList.txt location and NOT the build directory
################

#Bring the headers, such as wrap.h and unwrap.h into the project
include_directories(include)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/driver.cpp src/wrap.cpp src/unwrap.cpp)

# or 

#The file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")


include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( fizeau ${SOURCES} )

target_link_libraries( fizeau ${OpenCV_LIBS} stdc++fs )
install( DIRECTORY DESTINATION PhaseShiftSurface )  
install( DIRECTORY DESTINATION PhaseShiftSurface/Data )  
install( DIRECTORY DESTINATION PhaseShiftSurface/Images )  
install( TARGETS fizeau DESTINATION PhaseShiftSurface )
install( DIRECTORY scripts DESTINATION PhaseShiftSurface ) 
