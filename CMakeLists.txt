# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(CProject LANGUAGES C)

# set policy to silence warnings about timestamps of downloaded files. Review occasionally if it may be set to NEW
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
# Add compiler flags (equivalent to CFLAGS)
option(OPTIMIZATION_FLAGS "Enable optimization flags" ON)
# Enable openMPI
option(ENABLE_OMPI "Enabling open MPI" OFF)

# Find all .c files in the src directory
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.c")

if(ENABLE_OMPI)
  message("\n    [info] USING OPENMPI" )
  message("     [info] Building the openMPI version")
  # setting the compiler to mpicc
  set(CMAKE_C_COMPILER mpicc)

  # Find the MPI package
  find_package(MPI REQUIRED)

  # If MPI is found, print details 
  if (MPI_FOUND)
    message(STATUS "    [info] MPI include path: ${MPI_INCLUDE_PATH}")
    message(STATUS "    [info] MPI libraries: ${MPI_LIBRARIES}")
    message(STATUS "    [info] MPI C compiler: ${MPI_C_COMPILER}")
  endif()

  add_executable(mpi_main.x ${CMAKE_SOURCE_DIR}/main.c ${SRC_FILES})

  # Include directories
  target_include_directories(mpi_main.x PRIVATE ${INCLUDE_DIR})

  # Set output directories
  set_target_properties(mpi_main.x PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  )

  if(OPTIMIZATION_FLAGS) 
    message("    [info] USING OPTIMIZATION FLAGS\n")

    # Deffining compiler flags
    target_compile_options(mpi_main.x PRIVATE
      -Wall
      -O3
      -ffast-math
      -fomit-frame-pointer
      -ffp-contract=fast
      -march=native
      -DENABLE_OPENMPI
    )

    target_compile_definitions(mpi_main.x PRIVATE LJMD_VERSION=1.3)
  else(OPTIMIZATION_FLAGS)
    message(STATUS "    [info] NOT USING OPTIMIZATION FLAGS")
    target_compile_options(mpi_main.x PRIVATE
      -Wall
      -g
      -DENABLE_OPENMPI
    )
  endif(OPTIMIZATION_FLAGS)

  # Add macro definitions
  target_compile_definitions(mpi_main.x PRIVATE LJMD_VERSION=1.0)

  # Add linker flags (equivalent to LDLIBS)
  target_link_libraries(mpi_main.x PRIVATE m) # `-lm` links the math library

else(ENABLE_OMPI)
  message("\n   [info] NOT USING OPENMPI")
endif(ENABLE_OMPI)

# setting the compiler back to gcc
# set(CMAKE_C_COMPILER gcc)
message("     [info] Building the gcc version")

# Add an executable for the main program
add_executable(main.x ${CMAKE_SOURCE_DIR}/main.c ${SRC_FILES})

# Include directories
target_include_directories(main.x PRIVATE ${INCLUDE_DIR})

# Set output directories
set_target_properties(main.x PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if(OPTIMIZATION_FLAGS) 
  message("    [info] USING OPTIMIZATION FLAGS\n")
  target_compile_options(main.x PRIVATE
    -Wall
    -O3
    -ffast-math
    -fomit-frame-pointer
    -ffp-contract=fast
    -march=native
  )
else(OPTIMIZATION_FLAGS)
  message(STATUS "    [info] NOT USING OPTIMIZATION FLAGS")
  target_compile_options(main.x PRIVATE
    -Wall
    -g
  )
endif(OPTIMIZATION_FLAGS)

# Add macro definitions
target_compile_definitions(main.x PRIVATE LJMD_VERSION=1.0)

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
