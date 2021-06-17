file(GLOB web_srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
add_library(CXXCGI
    ${web_srcs}
)
target_include_directories(CXXCGI
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
