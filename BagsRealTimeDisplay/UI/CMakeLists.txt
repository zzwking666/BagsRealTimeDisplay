add_subdirectory(UIModule)

set(BRTD_TARGET BagsRealTimeDisplay)

# 添加可执行文件生成目标
add_executable(${BRTD_TARGET}
	src/main.cpp
)

# 链接本项目的层级库
target_link_libraries(${BRTD_TARGET}
    PRIVATE UI::UIModule
	PRIVATE Qt6::Core Qt6::Widgets
	PRIVATE RWUL::rqwu
)

target_include_directories(${BRTD_TARGET}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/UIModule/include
)


# 编译好后调用qt中的windeployqt.exe
add_custom_command(TARGET ${BRTD_TARGET} POST_BUILD
	COMMAND ${QT_PATH}/bin/windeployqt.exe --no-compiler-runtime --no-opengl-sw $<TARGET_FILE:${BRTD_TARGET}>
	COMMENT "Running windeployqt to package Qt dependencies for ${BRTD_TARGET}"
)