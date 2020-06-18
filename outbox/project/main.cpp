#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <complex>
#include "Matrix.h"
#include "diag.h"
#include "parameters.h"
#include "rngesus.hpp"
#include <iomanip>

using namespace std;

parameters prm;
xorshift64 rng;

pair<int, int> kinv(int);
double lorentzian(double, double);
// cd phasex(int), phasey(int);

const double kB = 1.38 * pow(10, -23);
cd imagi = cd(0, 1);

double mu;
double global_present_mu;
double temperature;

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

int k(int x, int y)
{
    return x + prm.Lx * y;
}

// double getmu()
// {
//     int req_state;
//     req_state = int(prm.filling * 2.0 * prm.Lx * prm.Ly);
//     double mu = (eigs_[req_state - 1] + eigs_[req_state]) / (2.0);
//     return mu;
// }

double getmu()
{
    double target_N, n1;
    double mu1, mu2, mutemp, muout;
    bool converged;
    int nstates = int(2.0 * prm.Lx * prm.Ly);
    target_N = prm.filling * 2.0 * prm.Lx * prm.Ly;
    mu1 = eigs_[0];
    mu2 = eigs_[nstates - 1];
    mutemp = (eigs_[0] + eigs_[nstates - 1]) / 2.0;
    // mutemp = global_present_mu;
    for (int i = 0; i < 40000; i++)
    {
        n1 = 0.0;
        for (int j = 0; j < nstates; j++)
        {
            n1 += double(1.0 / (exp((eigs_[j] - mutemp) * (1.0 / temperature)) + 1.0));
        }
        if (abs(target_N - n1) < double(0.00001))
        {
            converged = true;
            break;
        }
        else
        {
            if (n1 < target_N)
            {
                mu1 = mutemp;
                mutemp = 0.5 * (mutemp + mu2);
            }
            else
            {
                mu2 = mutemp;
                mutemp = 0.5 * (mutemp + mu1);
            }
        }
    }
    if (!converged)
    {
        cout << "mu not converged, stopping at N= " << n1 << endl;
    }
    global_present_mu = mutemp;
    return mutemp;
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

double getQuantumEnergy()
{
    double q_energy = 0;
    double this_mu = getmu();
    for (int i = 0; i < 2 * prm.Lx * prm.Ly; i++)
    {
        q_energy +=
            eigs_[i] * 1.0 / (1.0 + exp((eigs_[i] - this_mu) * (1.0 / temperature)));
    }
    return q_energy;
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
    // cout << "JH=" << prm.JH << endl;
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
    double sum = 0.0;
    double mu = getmu();
    for (int lambda = 0; lambda < eigs_.size(); lambda++)
    {
        sum += log((1.0 + exp(-(1 / (temperature)) * (eigs_[lambda] - mu))));
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
    rng.set_seed(prm.seed);

    // TB Part
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
    // cout << "Added interaction terms to hamiltonian." << endl;

    // for (int i = 0; i < 100; i++){
    //     cout << perturb() << endl;
    // }

    ofstream tvsE;
    tvsE.open("data/T_vs_E.txt");

    temperature = prm.T;
    double totalQE;
    while (temperature > 0.01)
    {
        totalQE = 0;
        for (int t = 0; t < prm.sweeps; t++)
        {
            total_change = 0;
            accepted = 0;
            // cout << "Sweep number: " << t + 1 << endl;
            for (int i = 0; i < prm.Lx; i++)
            {
                for (int j = 0; j < prm.Ly; j++)
                {
                    // cout << "Sweep: " << t + 1
                    //      << " (" << i + 1 << ", " << j + 1 << ")\n";
                    double theta_old, theta_new, P_old, P_new, P_ratio, r;
                    int pos;
                    pos = k(i, j);

                    theta_old = real(theta(0, pos));
                    makeTB();
                    makeHund();

                    // cout << "***************************" << endl;
                    // H.print();
                    // cout << "***************************" << endl;

                    Diagonalize('V', H, eigs_);
                    // cout << getmu() << " ";
                    P_old = lnP(pos);

                    theta_new = filter(theta_old + perturb());
                    theta(0, pos) = theta_new;
                    makeTB();
                    makeHund();
                    Diagonalize('V', H, eigs_);
                    // cout << getmu() << "\n";
                    P_new = lnP(pos);

                    r = rng.random();
                    P_ratio = exp(P_new - P_old);
                    // cout << r << " " << P_old << " " << P_new << " "
                    // << exp(P_old) << " " << exp(P_new) << endl;
                    if (r < P_ratio)
                    {
                        accepted += 1;
                        theta(0, pos) = theta_new;
                        // cout << "Accepted" << endl;
                    }
                    else
                    {
                        // cout << "Rejected" << endl;
                        theta(0, pos) = theta_old;
                    }
                    total_change += 1;
                }
            }
            double presentQE = getQuantumEnergy();
            totalQE += presentQE;
            cout << "T: " << temperature << ", SN: " << t + 1 << ", AR: "
                 << (accepted * 1.0) / (total_change * 1.0) << ", En: "
                 << presentQE << ", mu: " << getmu() << endl;
        }
        tvsE << temperature << "\t" << totalQE / (1.0 * prm.sweeps) << endl;
        if (temperature > 2.0)
        {
            temperature -= 0.5;
        }
        if (temperature > 0.5 && temperature <= 2.0)
        {
            temperature -= 0.1;
        }
        if (temperature > 0.01 && temperature <= 0.5)
        {
            temperature -= 0.01;
        }
    }

    for (int i = 0; i < prm.Lx; i++)
    {
        for (int j = 0; j < prm.Ly; j++)
        {
            double pos;
            pos = k(i, j);
            cout << fixed << setprecision(3) << setfill('0');
            cout << real(theta(0, pos)) / (2.0 * 3.1415926535) << "\t";
        }
        cout << endl;
    }
    tvsE.close();
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