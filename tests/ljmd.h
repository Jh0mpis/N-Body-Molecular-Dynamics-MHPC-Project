/*
 * simple lennard-jones potential MD code with velocity verlet.
 * units: Length=Angstrom, Mass=amu; Energy=kcal
 *
 * baseline c version.
 */

#ifndef LJMD_H
#define LJMD_H

#ifdef __cplusplus
extern "C"
{
#endif

/* structure to hold the complete information
 * about the MD system */
struct _mdsys {
    int natoms;
    double mass;
    double *rx;
    double *vx;
    double *fx;
};
typedef struct _mdsys mdsys_t;

/* velocity verlet */
extern void verlet_1(mdsys_t *sys);
extern void verlet_2(mdsys_t *sys);

/* helper functions */
extern void azzero(double *d, const int n);
extern double wallclock();
extern void doublesleep(double t);

#ifdef __cplusplus
}
#endif

#endif
