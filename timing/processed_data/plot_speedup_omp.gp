# speedup_omp_histogram.gnuplot
set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'omp_speedup_histogram.png'
set title 'Speedup vs. Number of Threads (OpenMP Version)'
set xlabel 'Number of Threads'
set ylabel 'Speedup'
set grid ytics
set style data histograms
set style histogram clustered gap 1
set style fill solid border -1
set boxwidth 0.9
set key outside
set xtics rotate by -45

plot 'speedup_omp.dat' using 2:xtic(1) title '108 Atoms', \
     '' using 3 title '2916 Atoms', \
     '' using 4 title '78732 Atoms'
