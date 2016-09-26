cmake_minimum_required(VERSION 3.6)
project(FacebookFaceRecognition)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp FaceRecog.cpp FaceRecog.h FaceDetector.cpp FaceDetector.h)
add_executable(FacebookFaceRecognition ${SOURCE_FILES})

find_package(OpenCV REQUIRED)
target_link_libraries(FacebookFaceRecognition ${OpenCV_LIBS} )

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(FacebookFaceRecognition ${Boost_LIBRARIES})