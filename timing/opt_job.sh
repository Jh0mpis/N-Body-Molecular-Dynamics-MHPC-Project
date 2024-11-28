#!/bin/bash

#SBATCH --job-name=ljmd_108
#SBATCH --nodes=1                    # 2 node
#SBATCH --ntasks-per-node=1         # 32 tasks per node
#SBATCH --cpus-per-task=1
#SBATCH --time=0:10:00               # time limits: 30 mins
#SBATCH --mem=0
#SBATCH --hint=nomultithread
#SBATCH --error=108_opt_out-%j.err            # standard error file
#SBATCH --output=108_opt_out-%j.out           # standard output file
#SBATCH --account=ict24_mhpc       # account name
#SBATCH --partition=boost_usr_prod # partition name

echo "running opt ljmd with 108 particles"
module load gcc
#module load openmpi

#mpirun -np 2 ./jacobi
#nvc++ -stdpar=multicore -Iinclude omp_main.cpp -o jacobi --std=c++20
export OMP_NUM_THREADS=1
../build/bin/main.x < argon_108.inp
