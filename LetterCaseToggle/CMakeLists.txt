#
# App
#

# Files
set(SOURCES
    Source/Main.cpp

    Source/App.h
    Source/App.cpp
    Source/SystemHook.h
    Source/SystemHook.cpp
)
qt_add_executable(LetterCaseToggle)
target_sources(LetterCaseToggle PRIVATE ${SOURCES})

target_include_directories(LetterCaseToggle PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source)

# Libraries
target_link_libraries(LetterCaseToggle PRIVATE
    Qt6::Widgets
    Core
)

# Windows-only stuff
if (WIN32)
    target_link_libraries(LetterCaseToggle PRIVATE user32)
endif()

# Compilation options
target_compile_options(LetterCaseToggle PRIVATE
    /W4
    /WX
    /utf-8
)

# Working dir
set_target_properties(LetterCaseToggle PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/LetterCaseToggle"
)
