g++ -o poisson main.cpp
rm *.pdf
rm *.data
./poisson
gnuplot poisson.plt