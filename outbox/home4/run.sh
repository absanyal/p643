rm rng
rm *.txt
rm *.pdf
> output.txt
make
./rng > output.txt
gnuplot -e "load 'plot.p'"