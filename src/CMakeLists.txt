set(SRC_GLUTILITIES_DIR ${CMAKE_CURRENT_LIST_DIR}/GLUtilities)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${SRC_GLUTILITIES_DIR})
link_directories(${LIBRARY_OUTPUT_PATH})


add_subdirectory(Utilities)
add_subdirectory(GLRenderer)
add_subdirectory(GLViewer)