cmake_minimum_required(VERSION 3.0...3.22)
if(${CMAKE_VERSION} VERSION_LESS 3.22)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(camera_fusion)

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

message(STATUS "OpenCV Library Setup")
message(STATUS "config: " ${OpenCV_DIR})
message(STATUS "version: " ${OpenCV_VERSION})
message(STATUS "libraries: " ${OpenCV_LIBS})
message(STATUS "include: " ${OpenCV_INCLUDE_DIRS})

set(SOURCES
    src/matching2D_Student.cpp
    src/MidTermProject_Camera_Student.cpp
)

add_executable (2D_feature_tracking  ${SOURCES})
target_link_libraries (2D_feature_tracking ${OpenCV_LIBRARIES})
target_compile_features(2D_feature_tracking PRIVATE cxx_std_17)

