set terminal pngcairo size 600,500
set output 'out.png'

set xlabel "t, ms"
set ylabel "N/s"

plot "data.csv" u 1:2 w l t "Collected data", \
     "< cat -" u 1:2 w l t "\nImpulces\nper second"
