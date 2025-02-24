cmake_minimum_required(VERSION 3.10)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)

# Add source files
set(SOURCE_FILES
    src/GameState.cpp
    src/move.cpp
    src/castlingRights.cpp
    src/AIMove.cpp
    src/window.cpp
)

# Add include directories
include_directories(includes /opt/homebrew/Cellar/sfml@2/2.6.2/include/)

# Add the executable
add_executable(window ${SOURCE_FILES})

# Link SFML libraries
target_link_libraries(window /opt/homebrew/Cellar/sfml@2/2.6.2/lib/libsfml-graphics.dylib /opt/homebrew/Cellar/sfml@2/2.6.2/lib/libsfml-window.dylib /opt/homebrew/Cellar/sfml@2/2.6.2/lib/libsfml-system.dylib /opt/homebrew/Cellar/sfml@2/2.6.2/lib/libsfml-audio.dylib)

# Google Test
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test files
set(TEST_FILES
    tests/GameStateTest.cpp
    tests/AIMoveTest.cpp
)

# Add the test executable
add_executable(ChessEngineTests ${TEST_FILES} ${SOURCE_FILES})

# Define UNIT_TEST for the test executable
target_compile_definitions(ChessEngineTests PRIVATE UNIT_TEST)

# Link Google Test and SFML libraries
target_link_libraries(ChessEngineTests ${GTEST_LIBRARIES} pthread /opt/homebrew/Cellar/sfml@2/2.6.2/lib/libsfml-graphics.dylib /opt/homebrew/Cellar/sfml@2/2.6.2/lib/libsfml-window.dylib /opt/homebrew/Cellar/sfml@2/2.6.2/lib/libsfml-system.dylib /opt/homebrew/Cellar/sfml@2/2.6.2/lib/libsfml-audio.dylib)

# Add tests
add_test(NAME ChessEngineTests COMMAND ChessEngineTests)