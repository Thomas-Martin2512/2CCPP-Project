cmake_minimum_required(VERSION 3.20)
project(2CCPP_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(
        ${CMAKE_SOURCE_DIR}/src/Tile/Shapes.json
        ${CMAKE_CURRENT_BINARY_DIR}/Shapes.json
        COPYONLY
)

include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(project_lib
        src/Board/Board.cpp
        src/Display_Board/Display_Board.cpp
        src/Game/Game.cpp
        src/Player/Player.cpp
        src/Render/Renderer.cpp
        src/Tile/InitTiles.cpp
        src/Tile/Tile.cpp
        src/Tile/TileQueue.cpp
)

target_include_directories(project_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(project_lib PUBLIC nlohmann_json::nlohmann_json)

add_executable(${PROJECT_NAME}
        src/main.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE project_lib)

include(CTest)
enable_testing()

add_executable(tests tests/tests.cpp)
target_link_libraries(tests PRIVATE project_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)