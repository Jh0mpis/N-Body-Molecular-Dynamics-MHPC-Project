# timing_serial_histogram.gnuplot
set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'serial_timing_histogram.png'
set title 'Execution Time vs. Number of Atoms (Serial Version)'
set xlabel 'Number of Atoms'
set ylabel 'Execution Time (seconds)'
set grid ytics
set style data histograms
set style fill solid border -1
set boxwidth 0.5
set logscale y 10
set xtics rotate by -45
set format x "%.0s%ce%+03i"  # Scientific notation for x-axis labels

plot 'timing_opt.dat' using 2:xtic(1) title 'Serial Execution Time'
