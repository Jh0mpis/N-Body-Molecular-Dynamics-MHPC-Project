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
option(ENABLE_OMPI "Enabling openMPI" OFF)
# Enable openMP
option(ENABLE_OMP "Enable OpenMP" OFF)

# Find all .c files in the src directory
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.c")

#--------------------------------Generating openMPI Files-----------------------------#
if(ENABLE_OMPI)
  message("\n    [info] USING OPENMPI" )
  message("     [info] Building the openMPI version")

  # Find the MPI package
  find_package(MPI REQUIRED)

  # If MPI is found, print details 
  if (MPI_FOUND)
    message(STATUS "    [info] MPI include path: ${MPI_INCLUDE_PATH}")
    message(STATUS "    [info] MPI libraries: ${MPI_LIBRARIES}")
    message(STATUS "    [info] MPI C compiler: ${MPI_C_COMPILER}")
  else()
    message(FATAL_ERROR "OpenMPI not found!")
  endif()

  # setting the compiler to mpicc
  set(CMAKE_C_COMPILER mpicc)
  message("     [info] Building using the mpicc compiler")

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

  else(OPTIMIZATION_FLAGS)
    message(STATUS "    [info] NOT USING OPTIMIZATION FLAGS\n")
    target_compile_options(mpi_main.x PRIVATE
      -Wall
      -g
      -DENABLE_OPENMPI
    )
  endif(OPTIMIZATION_FLAGS)

  # Add macro definitions
  target_compile_definitions(mpi_main.x PRIVATE LJMD_VERSION=1.3)

  # Add linker flags (equivalent to LDLIBS)
  target_link_libraries(mpi_main.x PRIVATE m) # `-lm` links the math library

else(ENABLE_OMPI)

  message("\n   [info] NOT USING OPENMPI")
  # Set the compiler to gcc again
  set(CMAKE_C_COMPILER gcc)
  message("     [info] Building using the gcc compiler")

endif(ENABLE_OMPI)
#--------------------------------End of openMPI Files-----------------------------#

#--------------------------------Generating openMP Files-----------------------------#
if(ENABLE_OMP)
  message("\n    [info] USING OPENMP" )
  message("     [info] Building the openMP version")

  # Find and link OpenMP
  find_package(OpenMP REQUIRED)
  
  if (OPENMP_FOUND)
    message(STATUS "    [info] openMP include path: ${OpenMP_C_INCLUDE_DIRS}")    
    message(STATUS "    [info] openMP libraries: ${OpenMP_C_LIBRARIES}")
    message(STATUS "    [info] OpenMP C Compiler Flags: ${OpenMP_C_FLAGS}")
  else()
    message(FATAL_ERROR "OpenMP not found!")
  endif()

  add_executable(omp_main.x ${CMAKE_SOURCE_DIR}/main.c ${SRC_FILES})

  # Include directories
  target_include_directories(omp_main.x PRIVATE ${INCLUDE_DIR})

  # Set output directories
  set_target_properties(omp_main.x PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  )

  if(OPTIMIZATION_FLAGS) 
    message("    [info] USING OPTIMIZATION FLAGS\n")

    # Deffining compiler flags
    target_compile_options(omp_main.x PRIVATE
      -Wall
      -O3
      -ffast-math
      -fomit-frame-pointer
      -ffp-contract=fast
      -march=native
      -DENABLE_OPENMP
    )

  else(OPTIMIZATION_FLAGS)
    message(STATUS "    [info] NOT USING OPTIMIZATION FLAGS\n")
    target_compile_options(omp_main.x PRIVATE
      -Wall
      -g
      -DENABLE_OPENMP
    )
  endif(OPTIMIZATION_FLAGS)

  # Add macro definitions
  target_compile_definitions(omp_main.x PRIVATE LJMD_VERSION=1.4)

  # Add linker flags (equivalent to LDLIBS)
  target_link_libraries(omp_main.x PRIVATE m OpenMP::OpenMP_C) # Adding omp and math libraries
  target_compile_options(omp_main.x PRIVATE ${OpenMP_C_FLAGS})

else(ENABLE_OMP)

  message("\n   [info] NOT USING OPENMP")

endif(ENABLE_OMP)
#--------------------------------End of openMP Files-----------------------------#

#--------------------------------Generating serial version Files-----------------------------#
message("\n     [info] Building the serial version")

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
  message(STATUS "    [info] NOT USING OPTIMIZATION FLAGS\n")
  target_compile_options(main.x PRIVATE
    -Wall
  )
endif(OPTIMIZATION_FLAGS)

# Add macro definitions
target_compile_definitions(main.x PRIVATE LJMD_VERSION=1.2)

# Add linker flags (equivalent to LDLIBS)
target_link_libraries(main.x PRIVATE m) # `-lm` links the math library

#--------------------------------End of serial version Files-----------------------------#


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
