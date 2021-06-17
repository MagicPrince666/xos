# $SRC_DIR: .proto 所在的源目录
# --cpp_out: 生成 c++ 代码
# $DST_DIR: 生成代码的目标目录
# xxx.proto: 要针对哪个 proto 文件生成接口代码
execute_process(COMMAND 
        protoc
        -I=${CMAKE_SOURCE_DIR}/protobuf
        --cpp_out=${CMAKE_SOURCE_DIR}/protobuf
        ${CMAKE_SOURCE_DIR}/protobuf/cxx.proto
)

file(GLOB protobuf_srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
add_library(protobuf_dev
    ${protobuf_srcs}
)
target_include_directories(protobuf_dev
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
 