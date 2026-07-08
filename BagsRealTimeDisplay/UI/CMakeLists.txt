add_subdirectory(UIModule)

set(BRTD_TARGET BagsRealTimeDisplay)

# GLOB扫一层目录下的所有.cpp文件,GLOB_RECURSE扫多层目录下的所有.cpp文件
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# GLOB扫一层目录下的所有.h和.hpp文件,GLOB_RECURSE扫多层目录下的所有.h和.hpp文件
file(GLOB INCLUDE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB INCLUDE_HPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
list(APPEND INCLUDE_FILES ${INCLUDE_HPP_FILES})

# 添加可执行文件生成目标
add_executable(${BRTD_TARGET} ${SRC_FILES} ${INCLUDE_FILES})

# 设置可执行文件的输出目录
set_target_properties(${BRTD_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${BRTD_OUTPUT_DIR}"
)

# 链接本项目的层级库
target_link_libraries(${BRTD_TARGET}
    PRIVATE UI::UIModule
	PRIVATE Qt6::Core Qt6::Widgets
	PRIVATE RWUL::rqwu
	PRIVATE Modules::Modules
	PRIVATE Tools::PanZoomLabel
	PRIVATE BusinessLayer::LicenseManager
)

target_include_directories(${BRTD_TARGET}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/UIModule/include
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_windeployqt(${BRTD_TARGET})

rw_deploy(${BRTD_TARGET})