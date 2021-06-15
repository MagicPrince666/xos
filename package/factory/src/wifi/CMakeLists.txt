file(GLOB wifi_srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/libnl-tiny/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/iw-tiny/*.c"
)
add_library(wifi 
    ${wifi_srcs}
)

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -DCONFIG_LIBNL20 -D_GNU_SOURCE -flto")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -DCONFIG_LIBNL20 -D_GNU_SOURCE -flto")

target_include_directories(wifi
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libnl-tiny/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/iw-tiny
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
