SET(CURRENT_TARGET_NAME GLRenderer)

find_package(PCL CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(sdl2 CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)
find_package(Vine REQUIRED)

qt_standard_project_setup()

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cppt)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(${CURRENT_TARGET_NAME} ${SRC_FILES} ${HEADER_FILES} ${GLAD_SRC_FILES})

target_link_libraries(${CURRENT_TARGET_NAME} PRIVATE
    StdUtils 
    vi::Core vi::Ge 
    glfw
    glm::glm 
    assimp::assimp 
    SDL2::SDL2 
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets
    ${PCL_LIBRARIES} 
    ${OpenCV_LIBS}
    opengl32.lib)

 message(----------- ${VTK_LIBRARIES})

target_compile_definitions(${CURRENT_TARGET_NAME} PRIVATE CURRENT_MODULE_NAME="${CURRENT_TARGET_NAME}" CURRENT_RES_DIR="${GLSTART_RES_ROOT_DIR}${CURRENT_TARGET_NAME}")

set(RES_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res/)
set(RES_OUTPUT_DIR ${CONFIG_GLSTART_RES_OUTPUT_DIR}/${CURRENT_TARGET_NAME}/)
set(RES_SHADER_OUTPUT_DIR ${RES_OUTPUT_DIR}shaders/)
set(RES_IMAGE_OUTPUT_DIR ${RES_OUTPUT_DIR}images/)
file(GLOB RES_FILES LIST_DIRECTORIES true ${RES_SRC_DIR}*)
file(GLOB RES_SHADER_FILES ${RES_SRC_DIR}shaders/*.*)
file(GLOB RES_IMAGE_FILES ${RES_SRC_DIR}images/*.*)

add_custom_command(TARGET ${CURRENT_TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${RES_SHADER_OUTPUT_DIR})
add_custom_command(TARGET ${CURRENT_TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${RES_IMAGE_OUTPUT_DIR})
add_custom_command(TARGET ${CURRENT_TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RES_SHADER_FILES} ${RES_SHADER_OUTPUT_DIR})
add_custom_command(TARGET ${CURRENT_TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RES_IMAGE_FILES} ${RES_IMAGE_OUTPUT_DIR})

# get_target_property(WINDEPLOYQT_EXE Qt6::windeployqt IMPORTED_LOCATION)
# message(--------windeployqt=${WINDEPLOYQT_EXE})
# message(--------VCINSTALLDIR=${VCINSTALLDIR})
# if(WIN32)
#     add_custom_command (
#         TARGET ${CURRENT_TARGET_NAME}
#         POST_BUILD
#         COMMAND ${WINDEPLOYQT_EXE} --verbose 0 $<TARGET_FILE:AnyRenderer>
#         COMMENT "--------Run:windeployqt")
# endif()