set(BRTD_TARGET LicenseIssuer)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# ui 文件 UIC 编译器手动编译为 ui_*.h（项目未全局开启 AUTOUIC）
file(GLOB UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
qt_wrap_ui(GENERATED_UI_HEADERS ${UI_FILES})

add_executable(${BRTD_TARGET}
	main.cpp
	DlgLicenseIssuer.cpp
	DlgLicenseIssuer.h
	${GENERATED_UI_HEADERS}
)

# 输出到与主程序相同的 output/<config>/ 目录
set_target_properties(${BRTD_TARGET} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${BRTD_OUTPUT_DIR}"
)

target_include_directories(${BRTD_TARGET}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${BRTD_TARGET}
	PRIVATE Qt6::Core Qt6::Widgets
	PRIVATE BusinessLayer::actCry
	PRIVATE BusinessLayer::LicenseManager
)

# 打包 Qt 依赖与 RWUL SDK DLL，使 exe 目录自包含
add_windeployqt(${BRTD_TARGET})
rw_deploy(${BRTD_TARGET})
