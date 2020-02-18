n=100 #number of intervals
max=1. #max value
min=0. #min value
width=(max-min)/n #interval width
#function used to map a value to the intervals
hist(x,width)=width*floor(x/width)+width/2.0
set boxwidth width*0.9
set style fill solid 0.5 # fill style

set terminal pdfcairo

set output "rngtest.pdf"
set xlabel "Random number"
set ylabel "Frequency"
plot "rngtest.txt" u (hist($1,width)):(1.0) smooth freq w boxes lc rgb"green" notitle
unset output

set output "decay.pdf"
set xlabel "Time"
set ylabel "Number of remaining atoms"
plot "decay.txt" u 1:2 w l
unset output