# N-Body Molecular Dynamic Optimization Project

- Feitosa Benevides André
- Moreno Triana Jhon Sebastián
- Redjil Abou Bakr Essadiq


The main goal of this project is to optimize runtime of a molecular dynamics `C` program. 
> [!IMPORTANT]
> You can run the code using a serial version, OpenMPI version, or OpenMP version and a OpenMPI + OpenMP hybrid version. Enabled with compilation flags in the `CMakeLists.txt`.

> [!CAUTION]
> Please read the following instruction to know how to compile and run the project.

## Index

---

- [Cloning the repository](#cloning-the-repository)
- [Project folder architecture](#project-folder-architecture)
- [Compile the project](#compile-the-project)
    - [Optimization flags](#optimization-flags)
    - [Compiling with OpenMPI](#compilingwith-openmpi)
    - [Compiling with OpenMP](#compiling-with-openmp)
    - [Hybrid version](#hybrid-version)
    - [Enabling and disabling multifile](#enabling-and-disabling-multifile)
- [Run the project](#run-the-project)
    - [Run with the multifile](#run-with-the-multifile)
    - [Run without the multifile](#run-without-the-multifile)
- [Testing the project](#testing-the-project)

## Cloning the repository

---

The repository is public, so you can clone into your local using:

```
git clone https://github.com/Jh0mpis/N-Body-Molecular-Dynamics-MHPC-Project.git
```

## Project folder architecture

---

The `main.c` file is located int the root folder `./` along with the `CMakeLists.txt`, the code is split modules/. Source code inside `./src`. While, the header files are included in `./include`. Some example data is included for testing the excutables in `./examples`, and reference data for comparision in `./reference`.

Running `cmake -S <source-fodler-path> -B <build-folder-path> <additional-setup-flags>` in the command line will create a `./build` folder with the binary files, the executables will be inside the `./build/bin`.

```
./
├── CMakeLists.txt
├── main.c
├── README.md
├── .gitignore
├── GTest.cmake
├── run.sh/
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

## Compile The Project


To compiled the project `cmake` tool was used. To create the build folder run the following command from the root folder:

```
cmake -S . -B ./build
```

Then, for build the executables we can run:

```
cmake --build ./build
```

Finally, the executables will be generated inside of `./build/bin` folder.

### Optimization flags

`CMakeLists.txt` has the option `OPTIMIZATION_FLAGS` `ON` by default. You can set the variable using the following command to view the perforamnce of the code with and without optimization flags. 

```
cmake -S . -B ./build -DOPTIMIZATION_FLAG=ON(or OFF) <additional-setup-flags>
```

Alternatively, you can use ccmake tool. 

> [!IMPORTANT]
> We suggest to enable it always. 

> [!IMPORTANT]
> To generate an excutable, run `cmake --build ./build`, this will create executable called `main.x` in `<build-folder>/bin/`.

### Compiling with OpenMPI

To run the code using OpenMPI, enable the `ENABLE_OMPI` flag, by setting it to `ON` in the `CMakeLists.txt`. Or by running the following command line. 

```
cmake -S . -B ./build -DENABLE_OMPI=ON(or OFF) <additional-setup-flags>
```

or using the `ccmake` tool. 
> [!WARNING]
> The `ENABLE_OMPI` flag is `OFF` by default.

> [!IMPORTANT]
> To generate an excutable, run `cmake --build ./build`, this will create executable called `mpi_main.x` in `<build-folder>/bin/`.

### Compiling with OpenMP

To run using OpenMP, enable it in the `CMakeLists.txt` by setting `ENABLE_OMP` flag to `ON`, or by running the following command line 

```
cmake -S . -B ./build -DENABLE_OMP=ON(or OFF) <additional-setup-flags>
```

or using the ccmake tool.

> [!WARNING]
> `ENABLE_OMP` is `OFF` by default.

> [!IMPORTANT]
> To generate an excutable, run `cmake --build ./build`, this will create executable called `omp_main.x` in `<build-folder>/bin/`.

### Hybrid version

When both `ENABLE_OMPI` and `ENABLE_OMP` are `ON`, the cmake tool will generate an additional executable that runs with OpenMP and OpenMPI simulatniously, called hybrid version. To enable both flags from the command line, run 

```
cmake -S . -B ./build -DENABLE_OMP=ON -DENABLE_OMPI=ON <additional-setup-flags>
```

or using the `ccmake` tool.

> [!WARNING]
> The `ENABLE_OMP` and `ENABLE_OMPI` flags are `OFF` by default, you have to set it on.

> [!IMPORTANT]
> To generate an excutable, run `cmake --build ./build`, this will create executable called `hybrid_main.x` in `<build-folder>/bin/`.

### Enabling and Disabling Multifile

The project creates multiple executable files by default, however, this option can be turn off using the `MULTIFILE` flag and turn it to `OFF` using

```
cmake -S . -B ./build -DMULTIFILE=OFF <additional-setup-flags>
```

or with the ccmake tool.

> [!WARNING]
> The `MULTIFILE` flag is `ON` by default, you have to set it off if you want to.

> [!IMPORTANT]
> Running `cmake --build ./build` will create only one executable `main.x` in the `<build-folder>/bin/` with the latest version depending in what is available in the `cmake`.

## Run the project

---

### Run with the multifile

> [!WARNING]
> In order to make it work properly you build folder should be called `build` otherwise the `run.sh` script is not going to work.

> [!CAUTION]
> Please be sure that the files are compiled previously.

In the project's root folder (`./`) there's a bash script called `run.sh`, you can use it in the following way

```
bash ./run.sh <project_version> <file_name> <num_treads or process> <num_process>
```

where,

- ***<project_version>*** is one of the following values
    - **serial**: For run the optimized serial version.
    - **mpi**: For run the optimized openMPI version.
    - **omp**: For run the optimized openMP verison.
    - **hybrid**: For run the hybrid openMPI + openMP version.
    - **latest**: For run the latest available option (depend on the enabled compilation flags).
    - **all**: For run all the available versions (depend on the enabled compilation flags).
- ***<file_name>*** is the name of the input file inside of examples folder including the extension. You can use the word `all` for run with all the `.inp` files in examples folder.
- ***<num_treads or num_process>*** the number of treads if openMP or the hybrid version is enabled, number of process if just openMPI is enabled.
- ***<num_process>*** if the hybrid version is the target.

### Run without the multifile

For run with just the main.x file you need to run with the following steps:

**If openMP is enabled**

```
export OMP_NUM_THREADS=<num_treads>
```

where ***<num_treads>*** is the number of threads that you want to use.

**If openMPI is enabled**

```
cd examples
mpirun -np <num_process> <build_folder_path>/bin/main.x < <file_name>
```

where,

- ***<num_process>*** is the number of process to use for the simulation.
- ***<build_folder_path>*** is the relative path from `examples/` folder to the build folder.
- ***<file_name>*** is the input file's name.

**If openMPI is disabled**

```
cd examples
<build_folder_path>/bin/main.x < <file_name>
```

- ***<build_folder_path>*** is the relative path from `examples/` folder to the build folder.
- ***<file_name>*** is the input file's name.

## Testing the project

---
The testing flags are enabled by default in the `CMakeLists.txt`. To disable them changed the flag value to `OFF`, or run the following command.

```
cmake -S . -B ./build -DENABLE_TESTING=OFF <additional-setup-flags>
```

To run the tests run from the command line 

```
    cd ./build
    ctest ../build 
```
> [!IMPORTANT]
> Make sure the `examples` folder contains the output data to be compared with the refrence data in `refrence`. Before running the tests, make sure to run `./main.x > examples/argon_#.dat` to get the output results first.
