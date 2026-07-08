set(DEMO_TARGET actCry)

# actCry 依赖新 RWUL 的 hoei（仅 actCry_HWID.cpp / actCry_utility.cpp 用到）
find_package(RWUL CONFIG REQUIRED COMPONENTS hoei)

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE INCLUDE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

add_library(${DEMO_TARGET} STATIC ${SRC_FILES} ${INCLUDE_FILES})

target_include_directories(${DEMO_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${DEMO_TARGET}
	PRIVATE BusinessLayer::cla
	PRIVATE RWUL::hoei
)

add_library(BusinessLayer::${DEMO_TARGET} ALIAS ${DEMO_TARGET})
