#include <iostream>
#include <stdint-gcc.h>

#include "mpi.h"
#include "rngesus.hpp"

#define ASIZE 10000

using namespace std;

xorshift64 rng;

int main(int argc, char *argv[])
{
    rng.set_seed(32515465623251);
    int rank, numtasks, namelen, rc;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int time_to_halve, total_time_to_halve;
    double avg_halflife;
    double lattice[ASIZE];
    MPI_Status status;

    if (rank == 0)
    {
        for (int i = 0; i < ASIZE; i++)
        {
            lattice[i] = 1;
        }
    }

    rc = MPI_Bcast(lattice, ASIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rc != MPI_SUCCESS)
    {
        cerr << "MPI_Bcast error. Aborting." << endl;
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    double r;
    r = rng.random();
    cout << rank << ": " << r << endl;

    MPI_Finalize();
}