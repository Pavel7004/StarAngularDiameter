set terminal pngcairo size 600,500
set output 'out.png'
set datafile separator ","

set xlabel "t, ms"
set ylabel "N/s"

plot "data.txt" u 2:4 t "Collected data", \
     "data.txt" u 2:3 w l t "\nImpulces\nper second"
