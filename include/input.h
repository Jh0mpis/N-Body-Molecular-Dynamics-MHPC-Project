#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>  
#include "mdsys.h"  // struct definition header file

/**
 * @brief Read a line from an input file
 * @param[in] fp `File pointer`
 * @param[in] buf `... pointer`
 * @param[out] error message  
 */
static int get_a_line(FILE *fp, char *buf);
/**
 * @brief Read all input files and intilize system paramters. 
 * @param[in] sys `Struct to store the systems paramters`
 * @param[in] ergfile `... pointer`
 * @param[in] trajfile ``
 */
int read_input_files(mdsys_t *sys, FILE **ergfile, FILE **trajfile);

#endif /* INPUT_H */
