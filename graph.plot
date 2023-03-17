set terminal png
set output 'out.png'
unset xtics
plot "< ./star" u 1:2 with lines t "impulces per second"
