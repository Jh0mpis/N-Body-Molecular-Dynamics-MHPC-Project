#!/bin/bash

#SBATCH --job-name=mpi_ljmd_78732
#SBATCH --nodes=1                    # 2 node
#SBATCH --ntasks-per-node=32         # 32 tasks per node
#SBATCH --cpus-per-task=1
#SBATCH --time=1:00:00               # time limits: 30 mins
#SBATCH --mem=0
#SBATCH --hint=nomultithread
#SBATCH --error=78732_mpi_out-%j.err            # standard error file
#SBATCH --output=78732_mpi_out-%j.out           # standard output file
#SBATCH --account=ict24_mhpc       # account name
#SBATCH --partition=boost_usr_prod # partition name

export OMP_NUM_THREADS=1

echo "running mpi ljmd with 78732 particles"
module load gcc
module load openmpi

#mpirun -np 2 ./jacobi
#nvc++ -stdpar=multicore -Iinclude mpi_main.cpp -o jacobi --std=c++20
mpirun -np 1 ../build/bin/mpi_main.x < argon_78732.inp
mpirun -np 2 ../build/bin/mpi_main.x < argon_78732.inp
mpirun -np 4 ../build/bin/mpi_main.x < argon_78732.inp
mpirun -np 8 ../build/bin/mpi_main.x < argon_78732.inp
mpirun -np 16 ../build/bin/mpi_main.x < argon_78732.inp
mpirun -np 32 ../build/bin/mpi_main.x < argon_78732.inp
