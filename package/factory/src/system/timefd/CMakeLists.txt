file(GLOB timefd_srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
add_library(timefd 
    ${timefd_srcs}
)

set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/multiplexing/epoll
)

include_directories(${INCLUDE_DIRS}
)

target_include_directories(timefd
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
