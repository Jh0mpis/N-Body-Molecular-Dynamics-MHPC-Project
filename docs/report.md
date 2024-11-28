# N-Body Molecular Dynamics Optimization Project Report 

## Authors: 
- Feitosa Benevides André  
- Moreno Triana Jhon Sebastián  
- Redjil Abou Bakr Essadiq  

---

## Introduction 

This project focuses on optimizing and parallelizing a `C` program that simulates molecular dynamics in an $N$-Body classical system. The simulation utilizes the Lennard-Jones potential under periodic boundary conditions, which is mathematically defined as:

$$V(r) = 4 \varepsilon \sum_{i \neq j}^{N} \left[ \left( \frac{\sigma}{r_{ij}} \right)^{12} - \left( \frac{\sigma}{r_{ij}} \right)^{6} \right],$$

where:  

- $r_{ij}$ is the distance between particles $i$ and $j$,  
- $\varepsilon$ represents the depth of the potential well, and  
- $\sigma$ is the characteristic distance at which the potential is zero.  

The system is simulated in a cubic box with periodic boundary conditions, which ensures continuity across the system's edges. 

The project was divided into three subtasks: **optimization**, **parallelization using OpenMP**, and **parallelization using OpenMPI**, followed by a hybrid implementation combining the two parallelization techniques. Team responsibilities were as follows:  

- **Optimization**: Redjil Abou Bakr Essadiq  
- **OpenMP Parallelization**: Feitosa Benevides André  
- **OpenMPI Parallelization**: Moreno Triana Jhon Sebastián  

While individual members led specific subtasks, all team members contributed to the overall project to ensure integration and consistency across tasks. 

---

## Methodology

### Optimization  

The optimization task involved improving the computational efficiency of the original program. The following steps were taken:  

1. **Code Refactoring**: The program was modularized into logically separated files, following the structure detailed in the [`README.md`](https://github.com/Jh0mpis/N-Body-Molecular-Dynamics-MHPC-Project/blob/main/README.md). This enhanced readability, maintainability, and ease of debugging.  
2. **Replacing Expensive Operations**: Costly mathematical functions, such as `pow()` and `sqrt()`, were replaced with efficient alternatives using multiplications and inline arithmetic expressions.  
3. **Invariant Expression Simplification**: Invariant calculations within loops in the `force()` and `velverlet()` functions were precomputed and stored as constants outside the loops to reduce redundant operations.  
4. **Compiler Optimization**: Optimization flags (e.g., `-O3` for aggressive optimization) were utilized to enable compiler-level performance improvements.  
5. **Newton’s Third Law**: Symmetry in inter-particle forces (Newton’s third law) was exploited in the `force()` function to minimize redundant computations.  

These optimizations collectively reduced the computational overhead and enhanced the overall simulation efficiency.  

### Parallelization Using OpenMP  

The OpenMP implementation introduced shared-memory parallelism to improve performance on multi-core systems. Key steps included:  

1. **Loop Parallelization**: Computationally intensive loops, such as those in the `force()` and `velverlet()` functions, were parallelized using OpenMP directives (`#pragma omp parallel` and `#pragma omp parallel for` with `reduction`).  
2. **Critical Section Reduction**: Shared resources were carefully managed to minimize the use of critical sections, which can cause thread contention and slowdowns.  

This approach leveraged multi-threading to achieve significant speed-ups for simulations on shared-memory systems.  

### Parallelization Using OpenMPI  

The OpenMPI implementation employed message-passing parallelism for distributed-memory architectures. The following techniques were applied:  

1. **Replicated data**: The simulation share the data in all the process using `MPI_Bcast()` function for the particles positions from process 0 to the other process. Then process a smaller region of the same data divided by chunks of data. In the `force()` function each process computes the interaction forces and the potential energy for $\frac{num_particles}{num_process}$ particles and the force produced over the other particles due to interaction, and finally, we recover the original data performing the `MPI_Reduce()` function to process `0`.

### Hybrid Parallelization  

The hybrid approach combined OpenMP and OpenMPI to exploit both shared-and distributed-memory architectures and reduce the communication time performed in the openMPI version. Key steps included:  

1. **Intra-Node Parallelization**: OpenMP was used to parallelize computations within each node.  
2. **Inter-Node Communication**: OpenMPI handled communication between nodes.  
3. **Synchronization**: A careful balance between intra-node and inter-node workloads was maintained to minimize overheads and maximize resource utilization.  

This approach aimed to maximize performance.

---

## Results  

### Serial Performance 

![serial timing](../timing/serial_timing_histogram.png)

### OpenMPI Performance

![mpi timing](../timing/mpi_timing_histogram.png)
![mpi speed-up](../timing/mpi_speedup_histogram.png)

### OpenMP Performance

![omp timing](../timing/omp_timing_histogram.png)
![omp speed-up](../timing/omp_speedup_histogram.png)

### Hybrid Performance 

To reduce the communication time in openMPI, we implemented the code to support a hybrid model using both openMP and openMPI simultaneously. Then, we compared the time for different process-threads combinations on the Leonardo cluster, considering different number of atoms. 

![hybrid timing](../timing/hybrid_timing_histogram.png)

For the hybrid version, we tested with 1 process and 32 threads, which is the slowest for each particle size. We then gradually decreased the thread number until 32 process and 1 thread, where we observed the optimal performance for our tests. The results show that the hybrid implementation continues to scale, and we did not identify a combination where the simulation time stops decreasing.

![hybrid speed-up](../timing/hybrid_speedup_histogram.png)

In terms of speed-up, while perfect scaling was not achieved, the hybrid implementation still increasing in speed-up. For example, we observed a speed-up close to 14 using 32 process and 1 thread. Furthermore, the hybrid version showed better performance as the number of atoms in the simulation increased.

### Methods Comparison 

Finally, we compared each optimization method:

| Implementation    | 108 Timing (s) | 2916 Timing (s) | 78732 Timing (s) | 108 Speedup | 2816 Speedup | 78732 Speedup  |
|-------------------|--------|---------|----------|--------|---------|---------|
| Optimized (Seq.)  | 0.8758 | 33.3646 | 193.0099 | 1.0000 | 1.0000  | 1.0000  |
| OpenMP            | 0.3830 | 2.7248  | 7.3134   | 2.2867 | 12.2448 | 26.3913 |
| OpenMPI           | 0.1399 | 3.6102  | 16.3669  | 6.2602 | 9.2417  | 11.7927 |
| Hybrid            | 0.1729 | 3.5955  | 13.7019  | 5.0653 | 9.2795  | 14.0864 |

We selected the best time for each method and calculated the corresponding speed-up for each simulation. Overall, the openMP performed best for simulations with a large number of particles, while OpenMPI showed better scaling for smaller particle systems.

**Simulation with 108 Particles**

![108-comparison](../timing/compare_methods_108.png)

For the simulation system with 108 atoms, the openMPI implementation was the fastest,  achieving a speed-up of approximately 6.2. The OpenMP is the slowest, with a speed-up of around 2.28. The hybrid version performed comparably to openMPI, with a speed-up of 5.1.

**Simulation with 2916 Particles**

![2916-comparison](../timing/compare_methods_2916.png)

For the simulation with 2916 particles, openMP was the fastest, achieving a speed-up up to 12.24. The openMPI and hybrid implementations performed similarly, with speed-ups of between 9.24 and 2.27, respectively.

**Simulation with 78732 Particles**

![78732-comparison](../timing/compare_methods_78732.png)

For largest data set, with 78732 atoms, the openMP implementation scaled the best, achieving a speed-up of 26.39. In contrast, the openMPI implementation was the slowest, with a speed-up of 11.79 using 32 process. The hybrid implementation fell in between, with a speed-up of 14.08.

## Discussion  

The optimization significantly reduced computational overhead, providing a solid baseline for parallel implementations. OpenMP demonstrated good speed-ups on multi-core systems but was limited by memory bandwidth and thread contention for large systems. OpenMPI exhibited excellent scalability for distributed-memory systems, particularly for large problem sizes. The hybrid approach delivered the best performance by efficiently utilizing both shared and distributed memory resources, though it introduced additional complexity in implementation and debugging.

---

## Conclusion  

This project successfully optimized and parallelized the molecular dynamics simulation using a combination of OpenMP, OpenMPI, and hybrid techniques. The results demonstrated the potential for significant performance gains through careful code optimization and parallelization. Future work could explore GPU acceleration using CUDA or HIP to further improve performance for even larger systems.  

---
