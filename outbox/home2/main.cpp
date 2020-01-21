#include <iostream>
#include <iomanip>

#include "function.hpp"
#include "int_engine.hpp"

using namespace std;

int_engine ie;

int main(int argc, char *argv[])
{
    // cout << "Modes:\n" \
    //     << "1: Midpoint\n" << "2: Trapezoid\n" << "3: Simpson 1/3\n";
    if (argc != 5)
    {
        cerr << "Enter a, b, n, mode." << endl;
        exit(1);
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int n = atoi(argv[3]);
    int mode = atoi(argv[4]);

    cout << fixed;
    switch (mode)
    {
    case 1: //midpoint
        cout << "Midpoint method: ";
        cout << setprecision(15) << ie.midpoint(a, b, n) << endl;
        break;
    case 2: //trapezoid
        cout << "Trapezoid method: ";
        cout << setprecision(15) << ie.trapezoid(a, b, n) << endl;
        break;
    case 3: //Simpson 1/3
        cout << "Simpson's 1/3 rule: ";
        cout << setprecision(15) << ie.simpsononethird(a, b, n) << endl;
        break;
    default:
        cout << "Invalid mode selected." << endl;
        break;
    }
}
