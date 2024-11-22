# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(CProject LANGUAGES C)

# Set the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Define directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Find all .c files in the src directory
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.c")

# Add an executable for the main program
add_executable(main.x ${CMAKE_SOURCE_DIR}/main.c ${SRC_FILES})

# Include directories
target_include_directories(main.x PRIVATE ${INCLUDE_DIR})

# Set output directories
set_target_properties(main.x PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Add compiler flags (equivalent to CFLAGS)
target_compile_options(main.x PRIVATE
    -Wall
    -g
    -O3
    -ffast-math
    -fomit-frame-pointer
    -DLJMD_VERSION=1.0
)

# Add linker flags (equivalent to LDLIBS)
target_link_libraries(main.x PRIVATE m) # `-lm` links the math library
