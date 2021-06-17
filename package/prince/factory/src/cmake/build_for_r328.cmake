SET(CMAKE_SYSTEM_PROCESSOR arm)

# 设置头文件所在目录
include_directories(${CMAKE_SOURCE_DIR}/include)
# 设置第三方库所在位置
link_directories(${CMAKE_SOURCE_DIR}/arch/r328/protobuf/lib)

#工具链路径
# SET(TOOLCHAIN_DIR  "/home/leo/r328_sdk/bin")
# r328 设置好环境变量可以不指定工具链路径
SET(CMAKE_C_COMPILER arm-openwrt-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER arm-openwrt-linux-gnueabi-g++)
