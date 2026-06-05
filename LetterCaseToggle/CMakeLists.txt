#
# App
#

# Konsolowa wersja
add_executable(LetterCaseToggle)

# Finalna wersja
#add_executable(LetterCaseToggle WIN32)

# Pliki
set(SOURCES
    Source/Main.cpp
)
target_sources(LetterCaseToggle PRIVATE ${SOURCES})

target_include_directories(LetterCaseToggle PRIVATE Source)

# Biblioteki
target_link_libraries(LetterCaseToggle PRIVATE
    Core
#   user32
#   shell32
)


# Definicje kompilacji
#target_compile_definitions(LetterCaseToggle PRIVATE
#    UNICODE
#    _UNICODE
#    WIN32_LEAN_AND_MEAN
#    _WIN32_WINNT=0x0601
#)

# Opcje kompilacji
target_compile_options(LetterCaseToggle PRIVATE
    /W4
    /WX
    /utf-8
)

# Working dir
set_target_properties(LetterCaseToggle PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/LetterCaseToggle"
)
# Output dir
set_target_properties(LetterCaseToggle PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)