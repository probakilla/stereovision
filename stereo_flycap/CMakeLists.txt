cmake_minimum_required(VERSION 2.8)
project(stereo_flycap)

find_package(OpenCV REQUIRED)

add_subdirectory(serial)

# OpenCV includes
include_directories(${OpenCV_DIR}/include)
link_directories(${OpenCV_DIR}/lib ${OpenCV_DIR}/3rdparty/lib)

# Serial includes (communication with the robot)
include_directories(serial/include)

#Enable C++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Enable real warnings
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wextra")

#Add Vision libraries
set (LINKED_LIBRARIES
  ${LINKED_LIBRARIES}
  ${OpenCV_LIBS}
  flycapture
  serial
  )


#List Sources sub source directories
set(SOURCES_DIRECTORIES
  .
)

#Include Sources sub sources
foreach (DIRECTORY ${SOURCES_DIRECTORIES})
    message("-- Sources: Adding ${DIRECTORY}/")
    include (${DIRECTORY}/Sources.cmake)
    set (PREFIXED_SOURCES)
    foreach (SOURCE ${SOURCES})
        set (PREFIXED_SOURCES ${PREFIXED_SOURCES} ${DIRECTORY}/${SOURCE})
    endforeach (SOURCE)
    set (ALL_SOURCES ${ALL_SOURCES} ${PREFIXED_SOURCES})
endforeach (DIRECTORY)

#Build the RhobanServer main library
add_library (stereo_flycap SHARED ${ALL_SOURCES})
target_link_libraries (stereo_flycap ${LINKED_LIBRARIES})

#Build the test applications
add_executable (TestMono test_mono.cpp)
target_link_libraries (TestMono ${LINKED_LIBRARIES} stereo_flycap)

add_executable (TestStereo test_stereo.cpp)
target_link_libraries (TestStereo ${LINKED_LIBRARIES} stereo_flycap)

add_executable (ListGuid list_guid.cpp)
target_link_libraries (ListGuid ${LINKED_LIBRARIES} stereo_flycap)

add_executable (TestCar test_car.cpp)
target_link_libraries (TestCar ${LINKED_LIBRARIES} stereo_flycap)

add_executable (AutonomousCar autonomous_car.cpp)
target_link_libraries (AutonomousCar ${LINKED_LIBRARIES} stereo_flycap)
