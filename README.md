# N-Body Molecular Dynamic Optimization Project

- Feitosa Benevides André
- Moreno Triana Jhon Sebastián
- Redjil Abou Bakr Essadiq

This project is a N-Body molecular dynamic implementation using the `c` programming language. The main goal is to optimize the code and make it fast and efficient as possible.

The project can be run using a serial version, `OpenMPI` version, `OpenMP` version and a `OpenMPI + OpenMP` hybrid version and enable it using compilation flags.

# Project folder architecture

The location of the `main.c` file is in the root folder `./` along with the `CMakeList.txt`, the project is split in several modules, the source code is inside of `./src` folder while the header files are included in `./build`. The project includes some example data for testing the outputs, these examples are inside the `./examples` folder and the reference data is inside `./reference` folder.

When you run the `cmake -S <source-fodler-path> -B <build-folder-path>`, will create a `./build` folder with the binary files, the executables are created inside `./build/bin` folder.

```
./
├── CMakeLists.txt
├── main.c
├── README.md
├── .gitignore
├── Report.md
├── include/
├── src/
├── build/
│   ├── bin
├── test/
├── examples/
└── reference/
```
