#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <complex>
#include "Matrix.h"
#include "diag.h"
#include "parameters.h"
#include "rngesus.hpp"

using namespace std;

parameters prm;
xorshift64 rng;

int k(int x, int y) { return x + prm.Lx * y; }
pair<int, int> kinv(int);
double lorentzian(double, double);
// cd phasex(int), phasey(int);

const double kB = 1.38 * pow(10, -23);
cd imagi = cd(0, 1);

double mu;

Matrix<cd> H;
Matrix<cd> theta;
Matrix<cd> phi;
vector<double> eigs_;

int ns;
int accepted = 0;
int total_change = 0;

pair<int, int> kinv(int M)
{
    int x = (int)M / prm.Lx;
    int y = M % prm.Lx;
    return std::make_pair(x, y);
}

double getmu()
{
    int mid = eigs_.size() / 2;
    double mu = (eigs_[mid] + eigs_[mid + 1]) / (2.0);
    return mu;
}

double filter(double x)
{
    if (x > 2.0 * M_PI)
    {
        x -= 2.0 * M_PI;
    }
    else if (x < 0)
    {
        x += 2.0 * M_PI;
    }
    return x;
}

double lorentzian(double x, double x0)
{
    return (1 / M_PI) * 0.5 * prm.G / (pow(x - x0, 2) + pow(0.5 * prm.G, 2));
}

void makeTB()
{
    H.resize(2 * prm.Lx * prm.Ly, 2 * prm.Lx * prm.Ly);
    for (int i = 0; i < prm.Lx; i++)
    {
        for (int j = 0; j < prm.Ly; j++)
        {
            // cout << "TB step " << i << "," << j << endl;
            int pxy = k(i, j);

            int pxp1y = k((i + 1) % prm.Lx, j);
            int pxm1y = k((i - 1 + prm.Lx) % prm.Lx, j);

            int pxyp1 = k(i, (j + 1) % prm.Ly);
            int pxym1 = k(i, (j - 1 + prm.Ly) % prm.Ly);

            int pxp1ym1 = k((i + 1) % prm.Lx, (j - 1 + prm.Ly) % prm.Ly);
            int pxm1yp1 = k((i - 1 + prm.Lx) % prm.Lx, (j + 1) % prm.Ly);

            H(pxp1y, pxy) = cd(prm.tx, 0.0);
            H(pxm1y, pxy) = cd(prm.tx, 0.0);

            H(pxyp1, pxy) = cd(prm.ty, 0.0);
            H(pxym1, pxy) = cd(prm.ty, 0.0);

            H(pxp1ym1, pxy) = cd(prm.td, 0.0);
            H(pxm1yp1, pxy) = cd(prm.td, 0.0);

            H(pxp1y + ns, pxy + ns) = cd(prm.tx, 0.0);
            H(pxm1y + ns, pxy + ns) = cd(prm.tx, 0.0);

            H(pxyp1 + ns, pxy + ns) = cd(prm.ty, 0.0);
            H(pxym1 + ns, pxy + ns) = cd(prm.ty, 0.0);

            H(pxp1ym1 + ns, pxy + ns) = cd(prm.td, 0.0);
            H(pxm1yp1 + ns, pxy + ns) = cd(prm.td, 0.0);
        }
    }
}

void makeHund()
{
    // Hund's terms
    for (int i = 0; i < ns; i++)
    {
        H(i, i) = 0.5 * prm.JH * cos(theta(0, i));
        H(i + ns, i + ns) = -0.5 * prm.JH * cos(theta(0, i));
        H(i, i + ns) = 0.5 * prm.JH * sin(theta(0, i)) * exp(-imagi * phi(0, i));
    }
}

double perturb()
{
    double p;
    p = (rng.random() * 2.0 * prm.window - prm.window) * 2.0 * M_PI;
    return p;
}

double lnP(int i)
{
    double sum = 1.0;
    double mu = getmu();
    for (int lambda = 0; lambda < eigs_.size(); lambda++)
    {
        sum += log((1.0 + exp(-(1 / (prm.T)) * (eigs_[lambda] - mu))));
        // cout << sum << endl;
    }
    return sum;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Enter name of input file";
    }
    string inputfile = argv[1];
    prm.load(inputfile);

    ns = prm.Lx * prm.Ly;

    theta.resize(1, ns);
    phi.resize(1, ns);
    rng.set_seed(32515465623251);

    // TB Part, construct just once
    makeTB();

    cout << "Populated TB matrix." << endl;

    // Initially fill theta with random values
    for (int i = 0; i < prm.Lx; i++)
    {
        for (int j = 0; j < prm.Ly; j++)
        {
            int pos = k(i, j);
            theta(0, pos) = rng.random() * 2.0 * M_PI;
            phi(0, pos) = 0.0;
            // cout << theta(0, pos) << endl;
        }
    }
    cout << "Populated random theta array." << endl;
    // makeHund();
    cout << "Added interaction terms to hamiltonian." << endl;

    // for (int i = 0; i < 100; i++){
    //     cout << perturb() << endl;
    // }

    for (int t = 0; t < 100; t++)
    {
        for (int i = 0; i < prm.Lx; i++)
        {
            for (int j = 0; j < prm.Ly; j++)
            {
                cout << "Sweep: " << t + 1
                     << " (" << i + 1 << ", " << j + 1 << ")\n";
                double theta_old, theta_new, P_old, P_new, P_ratio, r;
                int pos;
                pos = k(i, j);
                theta_old = real(theta(0, pos));
                makeHund();
                Diagonalize('V', H, eigs_);
                P_old = lnP(pos);
                theta_new = theta_old + perturb();
                theta(0, pos) = theta_new;
                makeHund();
                Diagonalize('V', H, eigs_);
                P_new = lnP(pos);
                r = rng.random();
                P_ratio = exp(P_old - P_new);
                if (r < P_ratio)
                {
                    accepted += 1;
                    cout << "Accepted" << endl;
                }
                else
                {
                    cout << "Rejected" << endl;
                    theta(0, pos) = theta_old;
                }
                total_change += 1;
            }
        }
    }
    cout << "Acceptance ratio = "
         << (accepted * 1.0) / (total_change * 1.0) << endl;

    return 0;
}

// Diagonalize('V', H, eigs_);
// cout << eigs_.size() << endl;
// cout << "Finished diagonalization." << endl;
// ofstream testdos;
// testdos.open("testdos.txt");
// for (int i = 0; i < eigs_.size(); i++)
// {
//     testdos << eigs_[i] << endl;
// }
// testdos.close();
// cout << getmu() << endl;