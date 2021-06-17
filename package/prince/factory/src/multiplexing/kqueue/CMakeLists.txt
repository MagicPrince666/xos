file(GLOB kqueue_srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
add_library(KQUEUE
    ${kqueue_srcs}
)
target_include_directories(KQUEUE
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
