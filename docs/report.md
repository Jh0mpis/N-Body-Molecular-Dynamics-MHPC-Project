# N-Body Molecular Dynamic Optimization Project Report 
## Authors: 
- Feitosa Benevides André
- Moreno Triana Jhon Sebastián
- Redjil Abou Bakr Essadiq

##  Introduction 

The aim of this project is to optimize and parallelize a provided `C` program for molecular dynamics. It simulates the dynamics of an N-Body classical system using the Leonad-Jones potenial under periodic boundary conditions. The Leonard-Jones potential is

$$
V(r) = 4 \varepsilon  \sum_{i\neq j}^{N} \{(\frac{\sigma}{r_{i,j}})^{6}-(\frac{\sigma}{r_{i,j}})^{12} \}
$$

The particles are assumed to be trapped in a cubic. 
The main tasks of the project were divided into 3 subtasks. Optimization, parallelizing with OpenMP and OpenMPI, and a hybrid version that involved mergin the two. Each member was assigned a task (Bellou ->Optimization, Andre-> parallelizing with OpenMp, Jhon->parallelizing with OpenMpi). However, everyone contributed equally to the others parts. 

## Methodology

### Optimization 

The optimization task involved, First, refactoring the provided code into logically seperated files that follow the structure described in the [`README.md`](../README.md). Also, the compututionally expensive math functions were replaced with less costly operations (eg. `pow()`, `sqrt()` with multiplications). Furthermore, invariant expressions were defined as `const`'s outside of `for` loops inside the `force` and `velverlet` functions. Moreover, compiler optimization flags were used to enhance the performance of the code. Additionally, Newton's third law of motion was utilized in the `force` function. In addition, the `verlet` 
### OpenMp 

### OpenMpi 

## Results

## Discussion 

## Conclusion 
