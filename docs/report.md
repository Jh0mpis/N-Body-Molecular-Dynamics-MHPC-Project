# N-Body Molecular Dynamic Optimization Project Report 
## Authors: 
- Feitosa Benevides André
- Moreno Triana Jhon Sebastián
- Redjil Abou Bakr Essadiq

##  Introduction 

This project aims to optimize and parallelize a `C` program used for mollecular dynamics. This code simulates the dynamics of an N-Body system with classical particles trapped in cubic box with periodic boundary conditions, and interracting according to the Leonard-Jones potential. 

$$V(r) = 4 \varepsilon  \sum_{i\neq j}^{N} \{(\frac{\sigma}{r_{i,j}})^{6}-(\frac{\sigma}{r_{i,j}})^{12} \}$$

## Methodology
### Optimization 
The optimization task involved refactoring the code into seperate files. Then, the expensive functions where replaced with less costly function. Additionally, variables where redefined using `C` keywords to help the compiler optimze the code, and expensive floating-point divisions where moved out of loops. Furthermore, Newton's third law was employed in the `force` function.    
### OpenMp 

### OpenMpi 

## Results

## Discussion 

## Conclusion 
