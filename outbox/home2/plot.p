set terminal pdfcairo
set output "plot.pdf"
set title "Fermi function"
set xlabel "x"
set ylabel "f(x)"
f(x) = 1/(1+exp(x));
plot [-5:5] f(x) w l
unset output
unset terminal