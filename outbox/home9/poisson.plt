set xlabel "x"
set ylabel "y"
set zlabel "z"
set surface
set contour both
set cntrparam levels auto 8
splot "poisson.data" with lines
set terminal pdf
set output "poisson.pdf"
replot
