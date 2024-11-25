#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>  
#include "mdsys.h"  // struct definition header file

// Read one line from the input file
static int get_a_line(FILE *fp, char *buf);
// Read the entire files and initialize the system parameters from the input
int read_input_files(mdsys_t *sys, FILE **ergfile, FILE **trajfile);

#endif /* INPUT_H */
