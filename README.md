# N-Body Molecular Dynamic Optimization Project

- Feitosa Benevides André
- Moreno Triana Jhon Sebastián
- Redjil Abou Bakr Essadiq

This project is a N-Body molecular dynamic implementation using the `c` programming language. The main goal is to optimize the code and make it fast and efficient as possible.

The project can be run using a serial version, `OpenMPI` version, `OpenMP` version and a `OpenMPI + OpenMP` hybrid version and enable it using compilation flags.

## Project folder architecture

The location of the `main.c` file is in the root folder `./` along with the `CMakeLists.txt`, the project is split in several modules, the source code is inside of `./src` folder while the header files are included in `./include`. The project includes some example data for testing the outputs, these examples are inside the `./examples` folder and the reference data is inside `./reference` folder.

When you run the `cmake -S <source-fodler-path> -B <build-folder-path>`, will create a `./build` folder with the binary files, the executables are created inside `./build/bin` folder.

```
./
├── CMakeLists.txt
├── main.c
├── README.md
├── .gitignore
├── Report.md
├── GTest.cmake
├── include/
│   ├── constants.h
│   ├── force_compute.h
│   ├── verlet_time_integration.h
│   ├── utilities.h
│   ├── input.h
│   ├── output.h
│   └── cleanup.h
├── src/
│   ├── constants.c
│   ├── force_compute.c
│   ├── verlet_time_integration.c
│   ├── utilities.c
│   ├── input.c
│   ├── output.c
│   └── cleanup.c
├── build/
│   └── bin/
│       └── main.x
├── test/
├── examples/
└── reference/
```

## Compile the project

The project can be compiled using the `cmake` tool. The `CMakeLists.txt` is in the root folder. You can create the build folder running the following command in the root folder:

```
cmake -S . -B ./build
```

Then to build the executables we can run:

```
cmake --build ./build
```

And then, the executable is inside of `./build/bin` folder.

## Run the project


## Testing the project
