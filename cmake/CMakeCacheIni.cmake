set(QT_DIR "D:" CACHE PATH "Path to Qt installation")
set(RWUL_DIR "D:" CACHE PATH "Path to RWUL installation")

message(STATUS "QT_DIR : ${QT_DIR}")
if(NOT EXISTS "${QT_DIR}")
    message(WARNING "QT_DIR does not exist: ${QT_DIR}")
endif()

message(STATUS "RWUL_DIR : ${RWUL_DIR}")
if(NOT EXISTS "${RWUL_DIR}")
    message(WARNING "RWUL_DIR does not exist: ${RWUL_DIR}")
endif()
list(APPEND CMAKE_PREFIX_PATH ${QT_DIR})
list(APPEND CMAKE_PREFIX_PATH ${RWUL_DIR})
