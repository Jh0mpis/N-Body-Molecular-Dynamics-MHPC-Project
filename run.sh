#!/bin/bash

executables=("main.x" "mpi_main.x" "omp_main.x" "hybrid_main.x")
files=("argon_108.inp" "argon_2916.inp" "argon_78732.inp")

# How to use it: ./run.sh <version> <particle_num> <num_treads or process> <num_process>
# version = {all, omp, mpi, hybrid, serial}
# particle_num = {all, "file_name"}
# num_treads or process = num_treads if omp or hybrid, num_proces if mpi
# num_process = num_process if hybrid

cd ./examples

run_all_executables=0

if [[ "$1" = "serial"  ]]; then
  executable=0
elif [[ "$1" = "mpi" ]]; then
  executable=1
elif [[ "$1" = "omp" ]]; then
  executable=2
elif [[ "$1" = "hybrid" ]]; then
  executable=3
elif [[ "$1" = "all" ]]; then
  executable=4
  run_all_executables=1
elif [[ "$1" = "latest" ]]; then
  executable=5
else
  echo "How to use it: ./run.sh <version> <particle_num> <num_treads or process> <num_process>"
  echo "version = {all, omp, mpi, hybrid, serial, latest}"
  echo "particle_num = {all, 'file_name'}"
  echo "num_treads or process = num_treads if omp or hybrid, num_proces if mpi"
  echo "num_process = num_process if hybrid"
  exit 1
fi

if [ -z "$2" ];then
  echo "How to use it: ./run.sh <version> <particle_num> <num_treads or process> <num_process>"
  echo "version = {all, omp, mpi, hybrid, serial, latest}"
  echo "particle_num = { all, 'file_name'}"
  echo "num_treads or process = num_treads if omp or hybrid, num_proces if mpi"
  echo "num_process = num_process if hybrid"
  exit 1
fi

if [[ "$2" = "all"  ]]; then

  run_all=1
  # Create a list of all .inp files in the current folder
  inp_files=($(ls *.inp 2>/dev/null))

  # Print the list
  if [ ${#inp_files[@]} -eq 0 ]; then
    echo "No .inp files found in ./examples folder"
    exit 1
  fi

else
  run_all=0
fi

if [ $executable -eq 4 ]; then
  executable=0
fi

if [ $executable -eq 5 ]; then
  for i in "${!executables[@]}"; do
    file="${executables[$i]}"
    if [ -f "../build/bin/${file}" ]; then
      last_file=$i          # Update the index
    fi
    executables[$i]=${executables[$last_file]}
  done
  executable=$last_file
fi

if [ $executable -eq 0 ]; then
  # File to check (passed as a parameter to the script)
  file_to_check=${executables[$executable]}

  # Check if the file exists
  if [ -f "$file_to_check" ]; then
    echo "File found"
  else
    echo "The file '$file_to_check' does not exist."
    exit 1
  fi

  if [ $run_all -eq 0 ]; then
    ../build/bin/${executables[$executable]} < $2
  else 
    for file in "${inp_files[@]}"; do
      ../build/bin/${executables[$executable]} < $file
    done
  fi
  if [ $run_all_executables -eq 1 ]; then
    executable=$((executable+1))
  fi
fi

if [ $executable -eq 1 ]; then
  # File to check (passed as a parameter to the script)
  file_to_check=../build/bin/${executables[$executable]}

  # Check if the file exists
  if [ -f "$file_to_check" ]; then
    echo "File found"
  else
    echo "The file '$file_to_check' does not exist."
    exit 1
  fi

  if [ -z "$3" ];then
    echo "You need to pass the number of process as a third parameter"
    exit 1
  fi

  if [ $run_all -eq 0 ]; then
    mpirun -np $3 ../build/bin/${executables[$executable]} < $2
  else 
    for file in "${inp_files[@]}"; do
      mpirun -np $3 ../build/bin/${executables[$executable]} < $file
    done
  fi
  if [ $run_all_executables -eq 1 ]; then
    executable=$((executable+1))
  fi
fi

if [ $executable -eq 2 ]; then
  # File to check (passed as a parameter to the script)
  file_to_check=../build/bin/${executables[$executable]}

  # Check if the file exists
  if [ -f "$file_to_check" ]; then
    echo "File found"
  else
    echo "The file '$file_to_check' does not exist."
    exit 1
  fi

  if [ -z "$3" ]; then
    echo "Using default omp threads"
  else
    export OMP_NUM_THREADS=$3
  fi
  if [ $run_all -eq 0 ]; then
    ../build/bin/${executables[$executable]} < $2
  else 
    for file in "${inp_files[@]}"; do
      ../build/bin/${executables[$executable]} < $file
    done
  fi
  if [ $run_all_executables -eq 1 ]; then
    executable=$((executable+1))
  fi
fi

if [ $executable -eq 3 ]; then
  # File to check (passed as a parameter to the script)
  file_to_check=../build/bin/${executables[$executable]}

  # Check if the file exists
  if [ -f "$file_to_check" ]; then
    echo "File found"
  else
    echo "The file '$file_to_check' does not exist."
    exit 1
  fi

  if [ -z "$3" ];then
    echo "You need to pass the number of threads as the third parameter."
    exit 1
  fi
  export OMP_NUM_THREADS=$3
  if [ -z "$4" ];then
    echo "You need to pass the number of process as the fourth parameter."
    exit 1
  fi

  if [ $run_all -eq 0 ]; then
    mpirun -np $3 ../build/bin/${executables[$executable]} < $2
  else 
    for file in "${inp_files[@]}"; do
      mpirun -np $3 ../build/bin/${executables[$executable]} < $file
    done
  fi
fi
