clear
rm secant
rm *.dat
g++ main.cpp -o secant 
./secant
gnuplot -e "load 'plot.p'"