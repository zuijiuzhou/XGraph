SET(CURRENT_TARGET_NAME StdUtils)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
add_library(${CURRENT_TARGET_NAME} STATIC ${SRC_FILES} ${HEADER_FILES})