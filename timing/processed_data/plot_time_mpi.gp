# mpi_timing_histogram.gnuplot
set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'mpi_timing_histogram.png'
set title 'Execution Time vs. Number of Processes (MPI Version)'
set xlabel 'Number of Processes'
set ylabel 'Execution Time (seconds)'
set grid ytics
set style data histograms
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set key outside
set xtics rotate by -45

plot 'timing_mpi.dat' using 2:xtic(1) title '108 Atoms', \
     '' using 3 title '2916 Atoms', \
     '' using 4 title '78732 Atoms'

