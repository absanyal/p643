The random number generator uses the Xorshift64 algorithm, which generates a random 64 bit word (starting from a given seed) and then converts it a double betweeen 0 and 1. The random chain is achieved by three successive bit-shift and xor operations. The resultant RNG has a period of (2^64 - 1). Each random number takes about 2E-6 seconds to be generated on the acf. The output from the main file is logged in "output.txt". "rngtest.txt" has a list of 1000000 random numbers, while "rngtest.pdf" has a histogram of these numbers to show uniformity.

For the simulation, I have demonstrated radioactive decay. Starting from a lattice of 10000 atoms, at each intstant, any nucleus has a 5% chance to decay. If we plot the number of remaining atoms with time, we get an exponential curve, shown in "decay.pdf".

The whole program is compiled, executed and plotted with """bash run.sh".