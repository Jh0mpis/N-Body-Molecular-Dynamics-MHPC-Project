#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/input.h"       // input functions header file
#include "../include/constants.h"   // constants definition header file
#include "../include/mdsys.h"       // struct definition header file
#include "../include/utilities.h"   // wallclock, pbc and azzero functions header file
#ifdef ENABLE_OPENMPI
  #include <mpi.h>
#endif

// read 1 line from the input file
static int get_a_line(FILE *fp, char *buf) {
    char tmp[BLEN], *ptr;

    if (fgets(tmp, BLEN, fp)) {
        unsigned int i;

        ptr = strchr(tmp, '#');
        if (ptr) *ptr = '\0';
        i = strlen(tmp) - 1;
        while (i >= 0 && isspace(tmp[i])) {
            tmp[i--] = '\0';
        }
        ptr = tmp;
        while (*ptr && isspace(*ptr)) { ++ptr; }
        strcpy(buf, ptr);
        return 0;
    } else {
        perror("Problem reading input");
        return -1;
    }
}

// Read the entire file and associated files
int read_input_files(mdsys_t *sys, FILE **erg, FILE **traj){
   
      char restfile[BLEN], line[BLEN];
      char trajfile[BLEN], ergfile[BLEN];
      FILE *fp;
      int nprint;
      
    #ifdef ENABLE_OPENMPI
      /* Read input file and setting some system properties */
      if(!sys->rank){
    #endif //ENABLE_OPENMPI
        if (get_a_line(stdin, line)) return -1;
        sys->natoms = atoi(line);
        if (get_a_line(stdin, line)) return -1;
        sys->mass = atof(line);
        if (get_a_line(stdin, line)) return -1;
        sys->epsilon = atof(line);
        if (get_a_line(stdin, line)) return -1;
        sys->sigma = atof(line);
        if (get_a_line(stdin, line)) return -1;
        sys->rcut = atof(line);
        if (get_a_line(stdin, line)) return -1;
        sys->box = atof(line);

        if (get_a_line(stdin, restfile)) return -1;
        if (get_a_line(stdin, trajfile)) return -1;
        if (get_a_line(stdin, ergfile)) return -1;
        if (get_a_line(stdin, line)) return -1;
        sys->nsteps = atoi(line);

        if (get_a_line(stdin, line)) return -1;
        sys->dt = atof(line);
        if (get_a_line(stdin, line)) return -1;
        nprint = atoi(line);

    #ifdef ENABLE_OPENMPI
      }    

      MPI_Bcast(&(sys->natoms), 1, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Bcast(&(sys->mass), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(&(sys->epsilon), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(&(sys->sigma), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(&(sys->rcut), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(&(sys->box), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(&(sys->dt), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(&(sys->nsteps), 1, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Bcast(&nprint, 1, MPI_INT, 0, MPI_COMM_WORLD);

      sys->local_size = sys->natoms / sys->nps;
      sys->offset = 0;
      if(sys->rank < sys->natoms % sys->nps){
        sys->local_size++;
      }else{
        sys->offset = sys->natoms % sys->nps;
      }
    #endif /* ifdef ENABLE_OPENMPI */
      /* Allocate memory */
      sys->rx = (double *)malloc(sys->natoms * sizeof(double));
      sys->ry = (double *)malloc(sys->natoms * sizeof(double));
      sys->rz = (double *)malloc(sys->natoms * sizeof(double));
      sys->vx = (double *)malloc(sys->natoms * sizeof(double));
      sys->vy = (double *)malloc(sys->natoms * sizeof(double));
      sys->vz = (double *)malloc(sys->natoms * sizeof(double));
      sys->fx = (double *)malloc(sys->natoms * sizeof(double));
      sys->fy = (double *)malloc(sys->natoms * sizeof(double));
      sys->fz = (double *)malloc(sys->natoms * sizeof(double));

    #if defined(ENABLE_OPENMPI) || defined(ENABLE_OPENMP)
      sys->cx = (double *)malloc(sys->nthreads * sys->natoms * sizeof(double));
      sys->cy = (double *)malloc(sys->nthreads * sys->natoms * sizeof(double));
      sys->cz = (double *)malloc(sys->nthreads * sys->natoms * sizeof(double));
    #endif // ENABLE_OPENMPI || ENABLE_OPENMP


    #ifdef ENABLE_OPENMPI
      if(!sys->rank){
    #endif // ENABLE_OPENMPI
        /* Read restart */
        fp = fopen(restfile, "r");
        if (fp) {
            for (int i = 0; i < sys->natoms; ++i) {
                fscanf(fp, "%lf%lf%lf", sys->rx + i, sys->ry + i, sys->rz + i);
            }
            for (int i = 0; i < sys->natoms; ++i) {
                fscanf(fp, "%lf%lf%lf", sys->vx + i, sys->vy + i, sys->vz + i);
            }
            fclose(fp);
            azzero(sys->fx, sys->natoms);
            azzero(sys->fy, sys->natoms);
            azzero(sys->fz, sys->natoms);
        } else {
            perror("Cannot read restart file");
            return 3;
        }
  
        // Open the output files
        *erg = fopen(ergfile, "w");
        *traj = fopen(trajfile, "w");
    #ifdef ENABLE_OPENMPI
      }
    #endif //ENABLE_OPENMPI

      return nprint; // returning the printing steps
}
