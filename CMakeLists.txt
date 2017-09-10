cmake_minimum_required(VERSION 3.0)
project(libnet)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES libnet.c libnet.h)
add_library(net STATIC ${SOURCE_FILES})
add_library(net_s SHARED ${SOURCE_FILES})
