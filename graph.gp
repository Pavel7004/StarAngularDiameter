set terminal png
set output 'out.png'
set xlabel "x, m"
set ylabel "N/s"
plot "< ./star" u 1:2 with lines t "Impulses per second"
