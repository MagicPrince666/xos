file(GLOB ntp_srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(ntp_client
    ${ntp_srcs}
)
target_include_directories(ntp_client
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
