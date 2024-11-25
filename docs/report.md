# Project Report 

The first step in the code optimization we took was to refactor the code. We seprated the mian file logically into header `include` that contain the prototypes of the functions and constants, along with source `src` files which include funcions definitions.  

## Observations: 

1. `pbc` function should be kept at the same file where it is used in computing the force, and to be flagged as `static` so that the compiler `inline`'s it. in the case when it is seprated the code gives unreasonable output.   
