set terminal pdfcairo
set xlabel "x"
do for [i=0:10]{
    set output 'H_'.i.'.pdf'
    plot 'H_'.i.'.txt' u 1:2 w l title 'H'.i.'(x)', \
    'H_'.i.'.txt' u 1:3 w l title 'H'''.i.'(x)'
    unset output
}
set output "hermites.pdf"
plot for [i=0:10] 'H_'.i.'.txt' u 1:2 w l title 'H'.i.'(x)'
unset output

unset terminal