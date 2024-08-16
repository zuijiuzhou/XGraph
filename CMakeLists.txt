cmake_minimum_required(VERSION 3.26)


project(XGraph 
    VERSION 1.0.0
    )

    
# set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
# set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist)


add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /DEBUG")
# set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /DEBUG")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CONFIG_BUILD_TYPE $<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>)
set(CONFIG_GLSTART_RES_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG_BUILD_TYPE}/res/)
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third/)

set(GLSTART_RES_ROOT_DIR res/)
add_definitions(-DRES_ROOT_DIR="${GLSTART_RES_ROOT_DIR}")

set(THIRD_PARTY_LIBS
    glad
    glfw
    glm
    stb
 )

# add_compile_options("/EHa")

foreach(lib_name ${THIRD_PARTY_LIBS})
    list(APPEND CMAKE_PREFIX_PATH "${THIRD_PARTY_DIR}${lib_name}")
endforeach()
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)

# add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

set(STB_INCLUDE_DIR "${THIRD_PARTY_DIR}stb/include")
set(GLAD_INCLUDE_DIR "${THIRD_PARTY_DIR}glad/include")
set(GLAD_SRC_DIR "${THIRD_PARTY_DIR}glad/src")
file(GLOB_RECURSE GLAD_SRC_FILES ${GLAD_SRC_DIR}/glad.c ${GLAD_SRC_DIR}/glad_wgl.c)

include_directories(${GLAD_INCLUDE_DIR})
include_directories(${STB_INCLUDE_DIR})

add_subdirectory(src)
enable_testing()
add_subdirectory(tests)

#configure_file(".clang-format" ${CMAKE_BINARY_DIR} COPYONLY)