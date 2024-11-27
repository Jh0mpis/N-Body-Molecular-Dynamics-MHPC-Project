# Project Report 

##  Introduction 
The aim of this project is to optimize and parallelize a `C` program that simulates the mollecular dynamics of N-body system govenered by the Leonard-Jones potential. The problem was divided into 3 folds. Optimization, Parallelzing with OpenMp, and with OpenMpi.  

## Methodology
### Optimization 
The optimization task involved refactoring the code into seperate files. Then, the expensive functions where replaced with less costly function. Additionally, variables where redefined using `C` keywords to help the compiler optimze the code, and expensive floating-point divisions where moved out of loops. Furthermore, Newton's third law was employed in the `force` function.    
### OpenMp 

### OpenMpi 

## Results

## Discussion 

## Conclusion 