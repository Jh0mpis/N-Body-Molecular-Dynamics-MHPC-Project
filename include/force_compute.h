#ifndef FORCE_COMPUTE_H
#define FORCE_COMPUTE_H

#include "mdsys.h"  // struct definition header file

/**
   * @brief Computes the forces.
   * @param[in] *sys `mdsys_t struct pointer, containing the systems information (i.e. number of atoms, box dimensions, ...)`
   * @note `The results are saved to a mdsys_t struct members: sys->fx, sys->fy, sys->fz`
   */
void force(mdsys_t *sys);
/**
   * @brief Computes the kentic energies.
   * @param[in] *sys `Struct pointer, containing the systems information (i.e. number of atoms, box dimensions, ...)`
   */
void ekin(mdsys_t *sys);
/**
   * @brief Periodic boundary conditions.
   * @param[in] x `One dimensional coordinate of the i th particle (i.e. x, y, or z)`
   * @param[in] boxby2 `Box dimension in the given direction`
   * @param[out] x `New position of the i th particle`
   * @note `The results are saved to a mdsys_t struct member sys->epot`
   */
static inline double pbc(double x, const double boxby2);

#endif /* FORCE_COMPUTE_H */
