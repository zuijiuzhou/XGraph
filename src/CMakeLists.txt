
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${LIBRARY_OUTPUT_PATH})


add_subdirectory(StdUtils)
add_subdirectory(GLRenderer)
add_subdirectory(GLViewer)
add_subdirectory(VKViewer)