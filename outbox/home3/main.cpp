#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

#include "specialfunctions.hpp"

using namespace std;

double f(double);
double fprime(double, double dx);
double x0;
int n;
double stepsize;

double f(double x)
{
    return hermpoly(n, x);
}

double fprime(double x, double dx)
{
    return (f(x + (dx / 2.0)) - f(x - (dx / 2.0))) / (dx);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cerr << "Enter a, b, points" << endl;
        exit(1);
    }

    float a = atof(argv[1]);
    float b = atof(argv[2]);
    float points = atof(argv[3]);

    double dx = 0.01;
    double stepsize = (b - a) / (points);

    for (n = 0; n <= 10; n++)
    {
        ofstream outfile;
        outfile.open("H_" + to_string(n) + ".txt");
        outfile << fixed;
        x0 = a;
        while ( x0 < b)
        {
            outfile << setprecision(10);
            // cout << x0 << "\t" << pow(x0, 3) - 3.0*x0 << "\t" << f(x0) << endl; //debug code
            outfile << setw(20) << x0;
            outfile << setw(20) << f(x0);
            outfile << setw(20) << fprime(x0, dx) << endl;
            x0 += stepsize;
        }
        outfile.close();
    }
}
