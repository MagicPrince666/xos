SET(CMAKE_SYSTEM_PROCESSOR aarch64)
# 工具链地址
# SET(TOOLCHAIN_DIR  "/home/leo/rk3308_sdk/bin")

# 设置头文件所在目录
include_directories(${CMAKE_SOURCE_DIR}/include)
# 设置第三方库所在位置
link_directories(${CMAKE_SOURCE_DIR}/arch/rk3308/protobuf/lib)

# rockchip3308
SET(CMAKE_C_COMPILER aarch64-rockchip-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER aarch64-rockchip-linux-gnu-g++)
