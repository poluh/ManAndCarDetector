cmake_minimum_required(VERSION 3.5)
project(WebPrj)

set(SOURCE_FILES PartDetector.cpp main.cpp CapReader.cpp MainDetector.cpp)
add_executable(WebPrj ${SOURCE_FILES})

include_directories("/home/sergey/ClionProjects/darknet/src/")
target_link_libraries(WebPrj darknet pthread)

if (UNIX)
    find_package(OpenCV 3 REQUIRED)
endif(UNIX)

link_libraries(${OpenCV_LIBS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden -fpermissive -fPIC -Ofast -ffast-math -frename-registers -march=native")
target_link_libraries(WebPrj ${OpenCV_LIBS})