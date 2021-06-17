file(GLOB arch_srcs "${CMAKE_CURRENT_SOURCE_DIR}/drivers/*.cpp")
add_library(arch_target 
    ${arch_srcs}
)
target_include_directories(arch_target
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/drivers)
