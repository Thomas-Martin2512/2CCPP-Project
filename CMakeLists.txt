cmake_minimum_required(VERSION 3.20)
project(2CCPP_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(project_lib
        src/Utils_example.cpp
)
target_include_directories(project_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(${PROJECT_NAME}
        src/main.cpp
        include/Player.cpp
        include/Player.h
        include/Board.cpp
        include/Board.h
)
target_link_libraries(${PROJECT_NAME} PRIVATE project_lib)

include(CTest)
enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests
        tests/tests.cpp
)
target_link_libraries(tests PRIVATE
        project_lib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)