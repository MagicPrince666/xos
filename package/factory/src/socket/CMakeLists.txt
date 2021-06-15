file(GLOB socket_srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/icmp/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/net/*.cc"
)

add_library(xsocket
    ${socket_srcs}
)

target_include_directories(xsocket
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/net
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/icmp
)
