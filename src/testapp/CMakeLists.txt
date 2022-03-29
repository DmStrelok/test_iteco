cmake_minimum_required(VERSION 3.10)

project(testapp)
set(APP_NAME testapp)

set(SOURCE testapp.cpp)

add_executable(${APP_NAME} ${SOURCE})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing -Wall -Wextra -Werror")

include_directories(${CMAKE_SOURCE_DIR}/src/common)
