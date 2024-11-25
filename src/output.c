#include <stdio.h>
#include "../include/output.h"  // Output function header file

// Output function for data
void output(mdsys_t *sys, FILE *erg, FILE *traj) {
    printf("Outputting:");
    printf("%8d %20.8f %20.8f %20.8f %20.8f\n",
           sys->nfi, sys->temp, sys->ekin, sys->epot, sys->ekin + sys->epot);
    fprintf(erg, "%8d %20.8f %20.8f %20.8f %20.8f\n",
            sys->nfi, sys->temp, sys->ekin, sys->epot, sys->ekin + sys->epot);
    fprintf(traj, "%d\n nfi=%d etot=%20.8f\n",
            sys->natoms, sys->nfi, sys->ekin + sys->epot);
    for (int i = 0; i < sys->natoms; ++i) {
        fprintf(traj, "Ar  %20.8f %20.8f %20.8f\n",
                sys->rx[i], sys->ry[i], sys->rz[i]);
    }
}
