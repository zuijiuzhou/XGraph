SET(CURRENT_TARGET_NAME VS_SDK_REFERENCESViewer)

find_package(opencascade REQUIRED)
find_package(assimp REQUIRED)
find_package(PCL CONFIG REQUIRED)
find_package(vsg CONFIG REQUIRED)
# find_package(osgVerse REQUIRED)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(${CURRENT_TARGET_NAME} ${SRC_FILES} ${HEADER_FILES} ${GLAD_SRC_FILES})
target_include_directories(${CURRENT_TARGET_NAME} 
    PRIVATE 
        $<BUILD_INTERFACE:${osgVerse_INCLUDE_DIRS}>
        $<BUILD_INTERFACE:${OPENSCENEGRAPH_INCLUDE_DIRS}>)

target_link_directories(${CURRENT_TARGET_NAME} 
    PRIVATE
        $<BUILD_INTERFACE:${osgVerse_LIB_DIRS}>
)

target_link_libraries(${CURRENT_TARGET_NAME} 
    PRIVATE 
        StdUtils 
        glfw
        ${PCL_LIBRARIES}  
        assimp::assimp 
        vsg::vsg
        TKernel TKXSBase TKMesh TKBRep TKDESTEP TKDEIGES TKMath TKGeomBase TKGeomAlgo TKG3d)