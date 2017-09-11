cmake_minimum_required(VERSION 3.0)
project(libnet)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES libnet.c libnet.h)
set(CMAKE_C_FLAGS "-w")
add_library(net STATIC ${SOURCE_FILES})
add_library(net_s SHARED ${SOURCE_FILES})
if (WIN32)
    set_target_properties( net_s PROPERTIES OUTPUT_NAME "net" )
    set_target_properties( net PROPERTIES CLEAN_DIRECT_OUTPUT 1 )
    set_target_properties( net_s PROPERTIES CLEAN_DIRECT_OUTPUT 1 )
    TARGET_LINK_LIBRARIES(net ws2_32)
    TARGET_LINK_LIBRARIES(net_s ws2_32)
endif ()


