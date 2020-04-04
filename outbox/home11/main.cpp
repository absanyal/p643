#include <cmath>
#include <iostream>
#include <fstream>
#include <complex>

using namespace std;

double u, a;
double error_threshold;
int max_iter = 100000;

double f1(double t)
{
    return u * t + 0.5 * a * pow(t, 2);
}

double f2(double x)
{
   return x*x*x +4.0 *x*x -1.0;
}

double secant(double (*f)(double), double xnm1, double xnm2)
{
    double xn;
    int iterc = 0;
    xn = xnm1 - f(xnm1) * (xnm1 - xnm2) / (f(xnm1) - f(xnm2));
    double e = abs((xnm1 - xn) / xnm1);
    // cout << xn << " " << e << " " << xnm1 << " " << xnm2 << endl;
    while (e > error_threshold && iterc < max_iter)
    {
        xnm2 = xnm1;
        xnm1 = xn;
        iterc += 1;
        xn = xnm1 - f(xnm1) * (xnm1 - xnm2) / (f(xnm1) - f(xnm2));
        e = abs((xnm1 - xn) / xnm1);
        // cout << xn << " " << e << " " << xnm1 << " " << xnm2 << endl;
    }
    return xn;
}

int main()
{
    u = 13.0;
    a = -9.8;
    error_threshold = 0.0001;
    cout << "Roots of first function\n";
    cout << secant(f1, -1.0, 5.0) << endl;
    cout << secant(f1, 2.0, 10.0) << endl;
    cout << "Possible roots of second function\n";
    cout << secant(f2, 0.0, 1.0) << endl;
    cout << secant(f2, -3.0, -2.0) << endl;
    cout << secant(f2, -1.0, 0.0) << endl;
    ofstream file1;
    file1.open("f1.dat");
    double x = -1.0;
    while (x < 4.0){
        file1 << x << "\t" << f1(x) << endl;
        x += 0.01;
    }
    file1.close();
    ofstream file2;
    file2.open("f2.dat");
    x = -4.0;
    while (x < 1.0){
        file2 << x << "\t" << f2(x) << endl;
        x += 0.01;
    }
    file2.close();
}