set(DEMO_TARGET LicenseManager)

file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB INCLUDE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

add_library(${DEMO_TARGET} ${SRC_FILES} ${INCLUDE_FILES})

target_include_directories(${DEMO_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 只链 actCry/hoei/Qt，保持精简以便 LicenseIssuer 复用
target_link_libraries(${DEMO_TARGET}
	PUBLIC BusinessLayer::actCry
	PUBLIC RWUL::hoei
	PUBLIC Qt6::Core Qt6::Widgets
)

add_library(BusinessLayer::${DEMO_TARGET} ALIAS ${DEMO_TARGET})
