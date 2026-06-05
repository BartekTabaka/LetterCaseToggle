cmake_minimum_required(VERSION 3.28)
project(LetterCaseToggle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Projects
add_subdirectory(LetterCaseToggle)
add_subdirectory(Core)

# StartUp Project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT LetterCaseToggle)
