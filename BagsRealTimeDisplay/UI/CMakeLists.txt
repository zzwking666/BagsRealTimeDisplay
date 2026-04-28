add_subdirectory(UIModule)

set(BRTD_TARGET BagsRealTimeDisplay)

#设置cpp文件路径
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

#设置头文件路径
file(GLOB INCLUDE_H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB INCLUDE_HPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
list(APPEND INCLUDE_FILES ${INCLUDE_H_FILES} ${INCLUDE_HPP_FILES})

#设置ui文件路径
file(GLOB UI_FILES  "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui")

#ui文件UIC编译器手动编译为*_ui.h
qt_wrap_ui(GENERATED_UI_HEADERS ${UI_FILES})

#设置qrc文件路径
file(GLOB QRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/qrc/*.qrc")

# 添加可执行文件生成目标
add_executable(${BRTD_TARGET} ${SRC_FILES} ${INCLUDE_FILES} ${GENERATED_UI_HEADERS} ${QRC_FILES})

#添加头文件搜索路径
#${CMAKE_CURRENT_BINARY_DIR}及生成的ui头文件路径
target_include_directories(${BRTD_TARGET}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

# 链接本项目的层级库
target_link_libraries(${BRTD_TARGET}
	PUBLIC Modules::CameraModule
	PUBLIC Modules::Modules
	PUBLIC Modules::IModule
	PUBLIC BusinessLayer::ImageStitch
)

#--------------------链接第三方库--------------------

#链接QT库
target_link_libraries(${BRTD_TARGET}
	PRIVATE Qt6::Core Qt6::Widgets Qt6::Concurrent
)

target_link_libraries(${BRTD_TARGET}
	PUBLIC ${OpenCV_LIBS}
)

target_link_directories(${BRTD_TARGET}
	PUBLIC ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(${BRTD_TARGET}
	PRIVATE RWUL::rqwu
)

# 编译好后调用qt中的windeployqt.exe
add_custom_command(TARGET ${BRTD_TARGET} POST_BUILD
	COMMAND ${QT_PATH}/bin/windeployqt.exe --no-compiler-runtime --no-opengl-sw $<TARGET_FILE:${BRTD_TARGET}>
	COMMENT "Running windeployqt to package Qt dependencies for ${BRTD_TARGET}"
)