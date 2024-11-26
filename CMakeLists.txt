# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(CProject LANGUAGES C)

# set policy to silence warnings about timestamps of downloaded files. review occasionally if it may be set to NEW
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 OLD)
endif()

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

# **Find and link OpenMP**
find_package(OpenMP REQUIRED)

# Add compiler flags (equivalent to CFLAGS)
option(OPTIMIZATION_FLAGS "Enable optimization flags" ON)

if(OPTIMIZATION_FLAGS) 
  message(STATUS "USING OPTIMIZATION FLAGS")
  target_compile_options(main.x PRIVATE
    -Wall
    -O3
    -ffast-math
    -fomit-frame-pointer
    -ffp-contract=fast
    -march=native
  )
else(OPTIMIZATION_FLAGS)
  message(STATUS "NON USING OPTIMIZATION FLAGS")
  target_compile_options(main.x PRIVATE
    -Wall
    -g
  )
endif(OPTIMIZATION_FLAGS)

# Add macro definitions
target_compile_definitions(main.x PRIVATE LJMD_VERSION=1.4)

# Add compiler flags (equivalent to CFLAGS)
option(ENABLE_OMP "Enable OpenMP flag" ON)
# **Link OpenMP and add OpenMP compile options**
if(ENABLE_OMP)
  target_link_libraries(main.x PRIVATE m OpenMP::OpenMP_C)
  target_compile_options(main.x PRIVATE ${OpenMP_C_FLAGS})
endif(ENABLE_OMP)

# Add linker flags (equivalent to LDLIBS)
target_link_libraries(main.x PRIVATE m) # `-lm` links the math library


option(ENABLE_TESTING "Enable building unit tests" ON)
if(ENABLE_TESTING)
  enable_testing()
  enable_language(CXX)

  # Added temporarily so we can make some first testings
  add_library(mdlib tests/utils.c tests/verlet.c)

  set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
  include(GTest)
  add_executable(test_verlet tests/test_verlet.cpp)
  target_include_directories(test_verlet PRIVATE ${CMAKE_SOURCE_DIR}/src)
  target_link_libraries(test_verlet PRIVATE GTest::GTestMain GTest::GTest mdlib)
  add_test(NAME Verlet COMMAND test_verlet)

  add_executable(test_utils tests/test_utils.cpp)
  target_include_directories(test_utils PRIVATE ${CMAKE_SOURCE_DIR}/src)
  target_link_libraries(test_utils PRIVATE GTest::GTestMain GTest::GTest mdlib)
  add_test(NAME Utils COMMAND test_utils)
endif()
