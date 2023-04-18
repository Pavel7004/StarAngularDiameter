set terminal pngcairo size 600,500
set output 'out.png'

set xlabel "x, m"
set ylabel "N/s"

plot "< cat -" u 1:2 w lp t "\nImpulces\nper second"
