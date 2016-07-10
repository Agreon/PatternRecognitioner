cmake_minimum_required(VERSION 2.8)
project(OpenCVTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES helper.h util.cpp main.cpp PatternRecognitioner.cpp Recognizer.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package( OpenCV REQUIRED )

add_executable(OpenCVTest ${SOURCE_FILES})
target_link_libraries( OpenCVTest ${OpenCV_LIBS} )

