# hybrid_speedup_histogram.gnuplot
set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'hybrid_speedup_histogram.png'
set title 'Speedup vs. Hybrid Configurations'
set xlabel 'Configuration (Processes x Threads)'
set ylabel 'Speedup'
set grid ytics
set style data histograms
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set key outside
set xtics rotate by -45

# Serial times
serial_time_108 = 0.9394512177
serial_time_2916 = 32.9249379635
serial_time_78732 = 194.1158668995

# hybrid_speedup_histogram.gnuplot
plot 'hybrid_speedup.dat' using 2:xtic(1) title '108 Atoms', \
     '' using 3 title '2916 Atoms', \
     '' using 4 title '78732 Atoms'


