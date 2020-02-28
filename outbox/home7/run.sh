make

rm *.dat
rm *.pdf

./rk4 1.0 0.0 0.0 0.0 0.0 100.0 100000 1.0 0.0 > natural.dat

./rk4 1.0 0.01 0.0 0.0 0.0 100.0 100000 1.0 0.0 > slightly_damped.dat

./rk4 1.0 0.3 0.0 0.0 0.0 100.0 100000 1.0 0.0 > heavily_damped.dat

./rk4 1.0 2.0 0.0 0.0 0.0 100.0 100000 1.0 0.0 > critically_damped.dat

./rk4 1.0 0.0 0.5 1.0 0.0 100.0 100000 1.0 0.0 > resonant_force.dat

./rk4 1.0 0.0 0.5 0.2 0.0 100.0 100000 1.0 0.0 > offresonant_force.dat

./rk4 1.0 0.1 0.5 1.0 0.0 100.0 100000 1.0 0.0 > damped_driven_resonant.dat

./rk4 1.0 0.1 0.5 0.2 0.0 100.0 100000 1.0 0.0 > damped_driven_offresonant.dat

./rk4 1.0 2.0 0.5 0.2 0.0 100.0 100000 1.0 0.0 > critdamped_driven.dat