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
The project was divided into 3 subtasks. Optimization, parallelizing with OpenMP and OpenMPI, and a hybrid version that involved mergin the two. Each member was assigned a task (Bellou ->Optimization, Andre-> parallelizing with OpenMp, Jhon->parallelizing with OpenMpi). 


## Methodology

### Optimization 

The optimization task involved refactoring the code into seperate files. Then, the expensive functions where replaced with less costly function. Additionally, variables where redefined using `C` keywords to help the compiler optimze the code, and expensive floating-point divisions where moved out of loops. Furthermore, Newton's third law was employed in the `force` function.    
### OpenMp 

### OpenMpi 

## Results

## Discussion 

## Conclusion 
