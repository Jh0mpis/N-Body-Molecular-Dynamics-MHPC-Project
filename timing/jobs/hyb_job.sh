#!/bin/bash

#SBATCH --job-name=hyb_ljmd_32p1t
#SBATCH --nodes=1                    # 2 node
#SBATCH --ntasks-per-node=32         # 32 tasks per node
#SBATCH --cpus-per-task=1
#SBATCH --time=1:00:00               # time limits: 30 mins
#SBATCH --mem=0
#SBATCH --hint=nomultithread
#SBATCH --error=32p1t_hybrid_out-%j.err            # standard error file
#SBATCH --output=32p1t_hybrid_out-%j.out           # standard output file
#SBATCH --account=ict24_mhpc       # account name
#SBATCH --partition=boost_usr_prod # partition name

export OMP_NUM_THREADS=1

echo "running hybrid ljmd with 32 processes 1 threads"
module load gcc
module load openmpi

#mpirun -np 2 ./jacobi
#nvc++ -stdpar=multicore -Iinclude hybrid_main.cpp -o jacobi --std=c++20
mpirun -np 32 ../build/bin/hybrid_main.x < argon_108.inp
mpirun -np 32 ../build/bin/hybrid_main.x < argon_2916.inp
mpirun -np 32 ../build/bin/hybrid_main.x < argon_78732.inp
