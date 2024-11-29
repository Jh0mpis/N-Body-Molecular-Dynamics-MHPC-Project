#!/bin/bash

#SBATCH --job-name=omp_ljmd_78732
#SBATCH --nodes=1                    # 2 node
#SBATCH --ntasks-per-node=1         # 32 tasks per node
#SBATCH --cpus-per-task=32
#SBATCH --time=1:00:00               # time limits: 30 mins
#SBATCH --mem=0
#SBATCH --hint=nomultithread
#SBATCH --error=78732_omp_out-%j.err            # standard error file
#SBATCH --output=78732_omp_out-%j.out           # standard output file
#SBATCH --account=ict24_mhpc       # account name
#SBATCH --partition=boost_usr_prod # partition name

echo "running omp ljmd with 78732 particles"
module load gcc
#module load openmpi

#mpirun -np 2 ./jacobi
#nvc++ -stdpar=multicore -Iinclude omp_main.cpp -o jacobi --std=c++20
export OMP_NUM_THREADS=1
../build/bin/omp_main.x < argon_78732.inp
export OMP_NUM_THREADS=2
../build/bin/omp_main.x < argon_78732.inp
export OMP_NUM_THREADS=4
../build/bin/omp_main.x < argon_78732.inp
export OMP_NUM_THREADS=8
../build/bin/omp_main.x < argon_78732.inp
export OMP_NUM_THREADS=16
../build/bin/omp_main.x < argon_78732.inp
export OMP_NUM_THREADS=32
../build/bin/omp_main.x < argon_78732.inp
