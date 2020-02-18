rm sf
rm H_*
rm *.pdf
make
./sf -1.0 1.0 1000
gnuplot -e "load 'plot.p'"