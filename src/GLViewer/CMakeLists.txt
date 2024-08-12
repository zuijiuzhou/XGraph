SET(CURRENT_TARGET_NAME GLViewer)

find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgText osgGA osgUtil osgViewer)
find_package(opencascade REQUIRED)
find_package(assimp REQUIRED)
find_package(PCL CONFIG REQUIRED)
find_package(VTK REQUIRED)
# find_package(osgVerse REQUIRED)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(GLUTILITIES_SRC_FILES
    ${SRC_GLUTILITIES_DIR}/Initializer.cpp
)
set(GLUTILITIES_HEADER_FILES
    ${SRC_GLUTILITIES_DIR}/Initializer.hpp
)

add_executable(${CURRENT_TARGET_NAME} ${SRC_FILES} ${HEADER_FILES} ${GLAD_SRC_FILES} ${GLUTILITIES_SRC_FILES} ${GLUTILITIES_HEADER_FILES})
target_include_directories(${CURRENT_TARGET_NAME} 
    PRIVATE 
        $<BUILD_INTERFACE:${osgVerse_INCLUDE_DIRS}>
        $<BUILD_INTERFACE:${OPENSCENEGRAPH_INCLUDE_DIRS}>)

target_link_directories(${CURRENT_TARGET_NAME} 
    PRIVATE
        $<BUILD_INTERFACE:${osgVerse_LIB_DIRS}>
)

if(NOT DEFINED osgVerse_LIBRARIES)
    list(APPEND osgVerse_LIBRARIES osgVerseDependency$<$<CONFIG:Debug>:d>)
    list(APPEND osgVerse_LIBRARIES osgVerseReaderWriter$<$<CONFIG:Debug>:d>)
    list(APPEND osgVerse_LIBRARIES osgVerseScript$<$<CONFIG:Debug>:d>)
    list(APPEND osgVerse_LIBRARIES osgVersePipeline$<$<CONFIG:Debug>:d>)
    list(APPEND osgVerse_LIBRARIES osgVerseAnimation$<$<CONFIG:Debug>:d>)
    list(APPEND osgVerse_LIBRARIES osgVerseModeling$<$<CONFIG:Debug>:d>)
    list(APPEND osgVerse_LIBRARIES osgVerseUI$<$<CONFIG:Debug>:d>)
    list(APPEND osgVerse_LIBRARIES osgVerseAI$<$<CONFIG:Debug>:d>)

    # foreach(item IN LISTS osgVerse_LIBRARIES)
    #     set(item ${item})
    # endforeach()
endif()
message(---------${PCL_LIBRARIES})
target_link_libraries(${CURRENT_TARGET_NAME} 
    PRIVATE 
        Utilities 
        glfw 
        ${OPENSCENEGRAPH_LIBRARIES} 
        ${osgVerse_LIBRARIES} 
        ${PCL_LIBRARIES}  
        assimp::assimp 
        TKernel TKXSBase TKMesh TKBRep TKDESTEP TKDEIGES TKMath TKGeomBase TKGeomAlgo TKG3d 
        imm32.lib 
        opengl32.lib
        VTK::CommonCore VTK::IOPLY VTK::FiltersCore VTK::IOGeometry)