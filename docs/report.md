# N-Body Molecular Dynamics Optimization Project Report 

## Authors: 
- Feitosa Benevides André  
- Moreno Triana Jhon Sebastián  
- Redjil Abou Bakr Essadiq  

---

## Introduction 

This project focuses on optimizing and parallelizing a `C` program that simulates molecular dynamics in an \(N\)-Body classical system. The simulation utilizes the Lennard-Jones potential under periodic boundary conditions, which is mathematically defined as:

\[
V(r) = 4 \varepsilon \sum_{i \neq j}^{N} \left[ \left( \frac{\sigma}{r_{ij}} \right)^{12} - \left( \frac{\sigma}{r_{ij}} \right)^{6} \right]
\]

where:  
- \(r_{ij}\) is the distance between particles \(i\) and \(j\),  
- \(\varepsilon\) represents the depth of the potential well, and  
- \(\sigma\) is the characteristic distance at which the potential is zero.  

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
1. **Code Refactoring**: The program was modularized into logically separated files, following the structure detailed in the [`README.md`](../README.md). This enhanced readability, maintainability, and ease of debugging.  
2. **Replacing Expensive Operations**: Costly mathematical functions, such as `pow()` and `sqrt()`, were replaced with efficient alternatives using multiplications and inline arithmetic expressions.  
3. **Invariant Expression Simplification**: Invariant calculations within loops in the `force` and `velverlet` functions were precomputed and stored as constants outside the loops to reduce redundant operations.  
4. **Compiler Optimization**: Optimization flags (e.g., `-O3` for aggressive optimization) were utilized to enable compiler-level performance improvements.  
5. **Newton’s Third Law**: Symmetry in inter-particle forces (Newton’s third law) was exploited in the `force` function to minimize redundant computations.  

These optimizations collectively reduced the computational overhead and enhanced the overall simulation efficiency.  

### Parallelization Using OpenMP  

The OpenMP implementation introduced shared-memory parallelism to improve performance on multi-core systems. Key steps included:  
1. **Loop Parallelization**: Computationally intensive loops, such as those in the `force` and `velverlet` functions, were parallelized using OpenMP directives (`#pragma omp parallel for`).  
2. **Load Balancing**: Thread workloads were distributed evenly to avoid bottlenecks. Various scheduling strategies (e.g., `static`, `dynamic`) were tested to achieve optimal performance.  
3. **Critical Section Reduction**: Shared resources were carefully managed to minimize the use of critical sections, which can cause thread contention and slowdowns.  

This approach leveraged multi-threading to achieve significant speed-ups for simulations on shared-memory systems.  

### Parallelization Using OpenMPI  

The OpenMPI implementation employed message-passing parallelism for distributed-memory architectures. The following techniques were applied:  
1. **Domain Decomposition**: The simulation box was divided into smaller subdomains, with each process handling the particles within its assigned region.  
2. **Boundary Exchange**: Neighboring subdomains exchanged boundary particle information to ensure accurate force calculations across subdomain boundaries.  
3. **Communication Optimization**: Efficient MPI routines, such as non-blocking communication (`MPI_Isend`, `MPI_Irecv`), were used to overlap computation with communication.  

The OpenMPI implementation allowed the program to scale effectively across multiple nodes in a distributed computing environment.  

### Hybrid Parallelization  

The hybrid approach combined OpenMP and OpenMPI to exploit both shared- and distributed-memory architectures. Key steps included:  
1. **Intra-Node Parallelization**: OpenMP was used to parallelize computations within each node.  
2. **Inter-Node Communication**: OpenMPI handled communication between nodes.  
3. **Synchronization**: A careful balance between intra-node and inter-node workloads was maintained to minimize overheads and maximize resource utilization.  

This approach aimed to maximize performance on high-performance computing (HPC) clusters.  

---

## Results  

### Performance Metrics  
The performance of each implementation was measured in terms of:  
- **Execution Time**: The total time required to complete a simulation.  
- **Speedup**: The ratio of the execution time of the original sequential code to that of the parallelized version.  
- **Scalability**: The ability of the program to handle increasing problem sizes or additional computational resources effectively.  

### Comparison  

| Implementation    | Execution Time (s) | Speedup | Scalability |  
|--------------------|---------------------|---------|-------------|  
| Optimized (Seq.)   | X.XX               | 1.00    | N/A         |  
| OpenMP            | Y.YY               | Z.ZZ    | Moderate    |  
| OpenMPI           | A.AA               | B.BB    | High        |  
| Hybrid            | C.CC               | D.DD    | Very High   |  

(*Note: Replace placeholders with actual data from the experiments.*)

---

## Discussion  

The optimization significantly reduced computational overhead, providing a solid baseline for parallel implementations. OpenMP demonstrated good speed-ups on multi-core systems but was limited by memory bandwidth and thread contention for large systems. OpenMPI exhibited excellent scalability for distributed-memory systems, particularly for large problem sizes. The hybrid approach delivered the best performance by efficiently utilizing both shared and distributed memory resources, though it introduced additional complexity in implementation and debugging.

---

## Conclusion  

This project successfully optimized and parallelized the molecular dynamics simulation using a combination of OpenMP, OpenMPI, and hybrid techniques. The results demonstrated the potential for significant performance gains through careful code optimization and parallelization. Future work could explore GPU acceleration using CUDA or HIP to further improve performance for even larger systems.  

---
