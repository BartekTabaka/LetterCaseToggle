#
# Core
#

set(SOURCES
    Source/Core/TrayManager.h
    Source/Core/TrayManager.cpp
)

# Definicja biblioteki
add_library(Core STATIC)

# Pliki
target_sources(Core PRIVATE ${SOURCES})

# Drzewo plikow w VS takie jak na dysku
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})