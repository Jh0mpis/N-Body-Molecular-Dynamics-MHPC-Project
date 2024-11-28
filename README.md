# N-Body Molecular Dynamic Optimization Project

- Feitosa Benevides André
- Moreno Triana Jhon Sebastián
- Redjil Abou Bakr Essadiq

This project is a N-Body molecular dynamic implementation using the `c` programming language. The main goal is to optimize the code and make it fast and efficient as possible.

> [!IMPORTANT]
> The project can be run using a serial version, OpenMPI version, OpenMP version and a OpenMPI + OpenMP hybrid version and enable it using compilation flags.

## Project folder architecture

The location of the `main.c` file is in the root folder `./` along with the `CMakeLists.txt`, the project is split in several modules, the source code is inside of `./src` folder while the header files are included in `./include`. The project includes some example data for testing the outputs, these examples are inside the `./examples` folder and the reference data is inside `./reference` folder.

When you run the `cmake -S <source-fodler-path> -B <build-folder-path> <additional-setup-flags>`, the cmake tool will create a `./build` folder with the binary files, the executables will be created inside the `./build/bin` folder.

```
./
├── CMakeLists.txt
├── main.c
├── README.md
├── .gitignore
├── GTest.cmake
├── include/
│   ├── constants.h
│   ├── force_compute.h
│   ├── verlet_time_integration.h
│   ├── mysys.h
│   ├── utilities.h
│   ├── simulate.h
│   ├── input.h
│   ├── output.h
│   └── cleanup.h
├── src/
│   ├── constants.c
│   ├── force_compute.c
│   ├── verlet_time_integration.c
│   ├── utilities.c
│   ├── simulate.c
│   ├── input.c
│   ├── output.c
│   └── cleanup.c
├── <build-folder>/
│   └── bin/
│       ├── main.x
│       ├── omp_main.x
│       ├── mpi_main.x
│       └── hybrid_main.x
├── docs/
│   └── report.md
├── test/
├── examples/
└── reference/
```

## Compile the project

The project can be compiled using the `cmake` tool. The `CMakeLists.txt` is in the root folder (`./`). You can create the build folder running the following command in the root folder:

```
cmake -S . -B ./build
```

Then, for build the executables we can run:

```
cmake --build ./build
```

And finally, the executables are inside of `./build/bin` folder.

### Optimization flags

In order to test the improvement with and without compiler optimization flags the `CMakeLists.txt` has the option `OPTIMIZATION_FLAGS` that is `ON` by default. You can set the variable using the following command.

```
cmake -S . -B ./build -DOPTIMIZATION_FLAG=ON(or OFF) <additional-setup-flags>
```

or using the ccmake tool. 

> [!IMPORTANT]
> We suggest to enable it always. 

> [!IMPORTANT]
> It will create an executable called `main.x` in the `<build-folder>/bin/` folder.

### Compiling with OpenMPI

The project can run using different parallelization strategies, the first one is the distributed memory approach using OpenMPI. For enable the OpenMPI functionality you have to add the `ENABLE_OMPI` flag, that is `OFF` by default, and put it in `ON` using

```
cmake -S . -B ./build -DENABLE_OMPI=ON(or OFF) <additional-setup-flags>
```

or using the ccmake tool.

> [!WARNING]
> The `ENABLE_OMPI` flag is `OFF` by default, you have to set it on.

> [!IMPORTANT]
> It will create an executable called `mpi_main.x` in the `<build-folder>/bin/` folder.

### Compiling with OpenMP

Also the project can be run using a shared memory approach with OpenMP. You can enable it using the `ENABLE_OMP` flag and turn it `ON` using 

```
cmake -S . -B ./build -DENABLE_OMP=ON(or OFF) <additional-setup-flags>
```

or using the ccmake tool.

> [!WARNING]
> The `ENABLE_OMP` flag is `OFF` by default, you have to set it on.

> [!IMPORTANT]
> It will create an executable called `omp_main.x` in the `<build-folder>/bin/` folder.

### Hybrid version

If `ENABLE_OMPI` and `ENABLE_OMP` are `ON`, the cmake tool will generate an additional executable that can run with OpenMP and OpenMPI at the same time, this is called the hybrid version, for enable the two flags at the same time you need to run 

```
cmake -S . -B ./build -DENABLE_OMP=ON -DENABLE_OMPI=ON <additional-setup-flags>
```

or using the ccmake tool.

> [!WARNING]
> The `ENABLE_OMP` and `ENABLE_OMPI` flags are `OFF` by default, you have to set it on.

> [!IMPORTANT]
> It will create an executable called `hybrid_main.x` in the `<build-folder>/bin/` folder.

### Enabling and disabling multifile

The project creates multiple executable files by default, however, this option can be turn off using the `MULTIFILE` flag and turn it to `OFF` using

```
cmake -S . -B ./build -DMULTIFILE=OFF <additional-setup-flags>
```

or with the ccmake tool.

> [!WARNING]
> The `MULTIFILE` flag is `ON` by default, you have to set it off if you want to.

> [!IMPORTANT]
> It will create just one executable called `main.x` in the `<build-folder>/bin/` folder with the latest version depending in what is available in the cmake.

## Run the project



## Testing the project
