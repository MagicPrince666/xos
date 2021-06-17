file(GLOB inotify_srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
add_library(xinotify 
    ${inotify_srcs}
)

include_directories(${CMAKE_SOURCE_DIR}/multiplexing/epoll
)

target_include_directories(xinotify
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
