cmake_minimum_required (VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeCacheIni.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/func.cmake)

project ("BagsRealTimeDisplay" LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源文件为 UTF-8（含中文注释/字符串字面量），MSVC 需显式指定 /utf-8
# 以正确解析中文，避免代码页(936)误读导致的 C2001 等错误
if(MSVC)
	add_compile_options(/utf-8)
endif()

# 对于qt项目他俩默认开
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(BRTD_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/output/${CMAKE_BUILD_TYPE}")

#查找第三方库
find_package(OpenCV CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Concurrent)
find_package(RWUL CONFIG REQUIRED COMPONENTS core oso hoecd rqwu imgcore imgqt rqwcd hoepZMotion hoei)

if(BUILD_WITHOUT_HARDWARE)
	add_compile_definitions(BUILD_WITHOUT_HARDWARE)
endif()

add_subdirectory ("BagsRealTimeDisplay")