#
# App
#

# Files
set(ASSET_FILES
    Assets/appIcon.ico

    Assets/assetFiles.qrc
)

set(SOURCES
    Source/Main.cpp

    Source/App.h
    Source/App.cpp
    Source/SystemHook.h
    Source/SystemHook.cpp

    ${ASSET_FILES}
)
qt_add_executable(LetterCaseToggle)
target_sources(LetterCaseToggle PRIVATE ${SOURCES})

target_include_directories(LetterCaseToggle PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source)

# Libraries
target_link_libraries(LetterCaseToggle PRIVATE
    Qt6::Widgets
    Qt6::Core
    Core
)

# Windows-only stuff
if (WIN32)
    target_link_libraries(LetterCaseToggle PRIVATE user32)

    set_target_properties(LetterCaseToggle PROPERTIES 
        WIN32_EXECUTABLE $<CONFIG:RELEASE>
    )
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

source_group("Assets" FILES ${ASSET_FILES})
