#include "../include/cleanup.h"   // cleaning up function header
#include <stdlib.h>

// Deallocate and close the file
void clean(mdsys_t *sys, FILE **erg, FILE **traj){
  
    // Closing the files
    fclose(*erg);
    fclose(*traj);

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
    free(sys->cx);
    free(sys->cy);
    free(sys->cz);

}
