#include "../include/cleanup.h"   // cleaning up function header
#include <stdlib.h>

// Deallocate and close the file
void clean(mdsys_t *sys, FILE **erg, FILE **traj){
  
    #ifdef ENABLE_OPENMPI
      if(!sys->rank){
        // Closing the files
        fclose(*erg);
        fclose(*traj);
      }
    #else
      // Closing the files
      fclose(*erg);
      fclose(*traj);
    #endif

    // Deallocating the arrays
    free(sys->rx);
    free(sys->ry);
    free(sys->rz);
    free(sys->vx);
    free(sys->vy);
    free(sys->vz);
    free(sys->fx);
    free(sys->fy);
    free(sys->fz);

    #ifdef ENABLE_OPENMP
    free(sys->cx);
    free(sys->cy);
    free(sys->cz);
    #endif

}
