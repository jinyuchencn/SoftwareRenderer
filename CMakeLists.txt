cmake_minimum_required(VERSION 3.0.0)
project(SoftwareRenderer VERSION 0.1.0)

set(TARGET SoftwareRenderer)
set(HEADER
    
)
set(SOURCES
    SoftwareRenderer/main.cpp
    SoftwareRenderer/tools/Timer.cpp
    SoftwareRenderer/rasterizer/Rasterizer.cpp
    SoftwareRenderer/platform/windows.cpp
)

add_executable(SoftwareRenderer ${SOURCES})


# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(SoftwareRenderer
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/SoftwareRenderer
)
