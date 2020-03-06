#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;

double w, dc, F0, w0, a, b, x0, ya, ypa;
int N;

double f(double, double, double);
double g(double, double, double);

double f(double x, double y, double z)
{
    return z; //z = dy/dx
}

double g(double x, double y, double z) // g = dz/dx
{
    return -pow(w, 2) * y - dc * z - F0 * sin(w0 * x);
}

int main(int argc, char *argv[])
{
    if (argc != 10)
    {
        cerr << "Enter w, dc, F0, w0, a, b, N, y(a), y'(a)" << endl;
    }

    w = atof(argv[1]);
    dc = atof(argv[2]);
    F0 = atof(argv[3]);
    w0 = atof(argv[4]);
    a = atof(argv[5]);
    b = atof(argv[6]);
    N = atoi(argv[7]);
    ya = atof(argv[8]);
    ypa = atof(argv[9]);

    double h = (b - a) / (1.0 * N);

    double xn;
    double yn, ynp1;
    double zn, znp1;

    double k0, k1, k2, k3;
    double l0, l1, l2, l3;

    xn = a;
    yn = ya;
    zn = ypa;
    while (xn <= b)
    {
        k0 = h * f(xn, yn, zn);
        l0 = h * g(xn, yn, zn);

        k1 = h * f(xn * 0.5 * h, yn + 0.5 * k0, zn + 0.5 * l0);
        l1 = h * g(xn * 0.5 * h, yn + 0.5 * k0, zn + 0.5 * l0);

        k2 = h * f(xn * 0.5 * h, yn + 0.5 * k1, zn + 0.5 * l1);
        l2 = h * g(xn * 0.5 * h, yn + 0.5 * k1, zn + 0.5 * l1);

        k3 = h * f(xn + h, yn + k2, zn + k2);
        l3 = h * g(xn + h, yn + k2, zn + k2);

        ynp1 = yn + (1.0 / 6.0) * (k0 + 2.0 * k1 + 2.0 * k2 + k3);
        znp1 = zn + (1.0 / 6.0) * (l0 + 2.0 * l1 + 2.0 * l2 + l3);

        cout << xn << "\t" << yn << "\t" << zn << endl;

        xn += h;
        yn = ynp1;
        zn = znp1;
    }
}