set grid
show grid

set terminal pdfcairo

set output "f1.pdf"
set xlabel "f1"
set ylabel "x"
plot "f1.dat" u 1:2 w l, 0 w l
unset output

set output "f2.pdf"
set xlabel "f2"
set ylabel "x"
plot "f2.dat" u 1:2 w l, 0 w l
unset output