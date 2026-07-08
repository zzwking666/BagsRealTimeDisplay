set(DEMO_TARGET cla)

# cla 加密引擎：仅依赖 OpenSSL（AES-256-CBC + SHA-256），无 cryptopp
find_package(OpenSSL REQUIRED)

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE INCLUDE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

add_library(${DEMO_TARGET} STATIC ${SRC_FILES} ${INCLUDE_FILES})

# 必须保留三个 include 目录，否则裸 #include "cla_SymmetricEncryptor.hpp" 等无法解析
target_include_directories(${DEMO_TARGET} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/ActivationCodeModule
	${CMAKE_CURRENT_SOURCE_DIR}/include/EncrpyModule
)

target_link_libraries(${DEMO_TARGET} PUBLIC OpenSSL::Crypto)

add_library(BusinessLayer::${DEMO_TARGET} ALIAS ${DEMO_TARGET})
