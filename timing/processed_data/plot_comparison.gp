# compare_methods.gnuplot
set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'compare_methods_78732.png'
set title 'Execution Time Comparison for 78732 Atoms'
set xlabel 'Method'
set ylabel 'Execution Time (seconds)'
set style data histograms
set style fill solid border -1
set boxwidth 0.5
set grid ytics
set logscale y
set xtics rotate by -45

plot 'compare_methods_78732.dat' using 2:xtic(1) notitle

