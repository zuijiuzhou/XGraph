find_package(GTest CONFIG REQUIRED)
link_libraries(GTest::gtest GTest::gmock_main GTest::gtest_main GTest::gmock) # GTest::gmock GTest::gmock_main

macro(GLS_ADD_TEST test_name)
    FILE(GLOB_RECURSE SRC_FILE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
    add_executable(${test_name} ${SRC_FILE_LIST})
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_target_properties(${test_name} PROPERTIES FOLDER tests)
    
endmacro()

add_subdirectory(osgtest)