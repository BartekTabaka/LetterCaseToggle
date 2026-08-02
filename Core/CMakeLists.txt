#
# Core
#

set(SOURCES
    Source/Core/TextTransform.h
    Source/Core/TextTransform.cpp
    Source/Core/TimingProfile.h
    Source/Core/TimingProfile.cpp
)

# Library definition
add_library(Core STATIC)

# Files
target_sources(Core PRIVATE ${SOURCES})

# File tree in VS
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)