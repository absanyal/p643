#include <iostream>
#include <fstream>
#include <time.h>

#include "rngesus.hpp"

using namespace std;

xorshift64 rng;

int lattice_size = 100000;
int n_rand = 1000000;
double decay_chance = 0.05;
int max_time = 100;

int main()
{
    cout << "Starting randomness test" << endl;
    int atoms[lattice_size];
    ofstream testfile;
    testfile.open("rngtest.txt");
    rng.set_seed(32515465623251);

    clock_t start_time, stop_time;
    start_time = clock();
    for (int i = 0; i < n_rand; i++)
    {
        testfile << rng.random() << endl;
    }
    stop_time = clock();
    double elpasedtime =
        (stop_time - start_time) * (1.0) / (CLOCKS_PER_SEC * 1.0);
    testfile.close();
    cout << "Done." << endl;
    cout << "Total time taken for " << n_rand << " random numbers is "
         << elpasedtime << " s." << endl;
    cout << "Average time per random() call is "
         << (elpasedtime / (n_rand * 1.0)) << " s." << endl;

    cout << "Populating lattice." << endl;

    ofstream a;
    a.open("decay.txt");

    //Populate the latice with undecayed atoms
    for (int i = 0; i < lattice_size; i++)
    {
        atoms[i] = 1;
    }

    cout << "Done." << endl;

    cout << "Starting radioactive decay." << endl;

    int atoms_left = lattice_size;
    a << "0"
      << "\t" << atoms_left << endl;

    //Start decays
    for (int t = 0; t < max_time; t++)
    {
        for (int i = 0; i < lattice_size; i++)
        {
            if (atoms[i] == 1)
            {
                double r = rng.random();
                if (r < decay_chance)
                {
                    atoms[i] = 0;
                    atoms_left -= 1;
                }
            }
        }
        a << t << "\t" << atoms_left << endl;
    }

    a.close();

    cout << "Done." << endl;
}