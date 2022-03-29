cmake_minimum_required(VERSION 3.10)

project(server)
set(APP_NAME server)

set(SOURCE server.cpp ring_buffer.cpp thread_pool.cpp
    ${CMAKE_SOURCE_DIR}/src/common/socket.cpp
    ${CMAKE_SOURCE_DIR}/src/common/utils.cpp
)

add_executable(${APP_NAME} ${SOURCE})

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_library(PTHREAD_LIBRARY pthread)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing -Wall -Wextra -Werror")

include_directories(${CMAKE_SOURCE_DIR}/src/common)
include_directories(${CMAKE_SOURCE_DIR}/3party/mbedtls/include/)
add_library(mbedcrypto STATIC IMPORTED)
set_property(TARGET mbedcrypto PROPERTY
             IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/3party/mbedtls/libs/libmbedcrypto.a)
target_link_libraries(${APP_NAME} mbedcrypto ${PTHREAD_LIBRARY})
