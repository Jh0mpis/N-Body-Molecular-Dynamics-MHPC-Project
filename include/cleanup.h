#ifndef CLEANUP_H
#define CLEANUP_H

#include <stdio.h>
#include "../include/mdsys.h"   // Struct definition

// Function for deallocate the allocated arrays
void clean(mdsys_t *sys, FILE **erg, FILE **traj);

#endif  /*CLEANUP_H */ 
