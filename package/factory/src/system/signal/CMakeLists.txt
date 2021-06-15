file(GLOB signal_srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
add_library(xsignal
    ${signal_srcs}
)
target_include_directories(xsignal
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
