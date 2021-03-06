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
double measure_counter;
double theta1, theta2;

Matrix<cd> H, H_cluster;
Matrix<cd> theta;
Matrix<cd> phi;
vector<double> eigs_, eigs_cluster_;
Matrix<cd> SiSj;
Matrix<cd> Sq;

int ns;
int accepted = 0;
int total_change = 0;

pair<int, int> kinv(int M)
{
    int x = (int)M / prm.Ly;
    int y = M % prm.Ly;
    return std::make_pair(x, y);
}

int k(int x, int y)
{
    return y + prm.Ly * x;
}

pair<int, int> kinv_cluster(int M)
{
    int x = (int)M / prm.c_Ly;
    int y = M % prm.Ly;
    return std::make_pair(x, y);
}

int k_cluster(int x, int y)
{
    return y + prm.c_Ly * x;
}

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
            n1 +=
                double(1.0 / (exp((eigs_[j] - mutemp) *
                                  (1.0 / temperature)) +
                              1.0));
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

double getmu_cluster()
{
    double target_N, n1;
    double mu1, mu2, mutemp, muout;
    bool converged;
    int nstates = int(2.0 * prm.c_Lx * prm.c_Ly);
    target_N = prm.filling * 2.0 * prm.c_Lx * prm.c_Ly;
    mu1 = eigs_cluster_[0];
    mu2 = eigs_cluster_[nstates - 1];
    mutemp = (eigs_cluster_[0] + eigs_cluster_[nstates - 1]) / 2.0;
    // mutemp = global_present_mu;
    for (int i = 0; i < 40000; i++)
    {
        n1 = 0.0;
        for (int j = 0; j < nstates; j++)
        {
            n1 +=
                double(1.0 / (exp((eigs_cluster_[j] - mutemp) *
                                  (1.0 / temperature)) +
                              1.0));
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
            eigs_[i] * 1.0 /
            (1.0 + exp((eigs_[i] - this_mu) * (1.0 / temperature)));
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
        H(i, i + ns) =
            0.5 * prm.JH * sin(theta(0, i)) * exp(-imagi * phi(0, i));
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

    SiSj.resize(ns, ns);

    // TB Part
    makeTB();

    cout << "Populated TB matrix." << endl;

    measure_counter = 0;

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
    // cout << "Stopping T = " << prm.T_stop << endl;
    double epsilon = pow(10, -6);

    if (prm.TCA == 0)
    {
        cout << "Performing ED" << endl;
        while (temperature >= prm.T_stop - epsilon)
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

                // Do measurements if thermalized
                if (abs(double(temperature - prm.T_stop)) < pow(10, -4))
                {
                    if ((t + 1) >= prm.measure_after &&
                        (t - int(prm.measure_after) + 1) %
                                int(prm.measure_every) ==
                            0)
                    {
                        // Measurements here
                        measure_counter += 1.0;
                        cout << "Measuring Si.Sj" << endl;
                        for (int i = 0; i < ns; i++)
                        {
                            for (int j = 0; j < ns; j++)
                            {
                                theta1 = real(theta(0, i));
                                theta2 = real(theta(0, j));
                                SiSj(i, j) += cos(theta1 - theta2);
                            }
                        }
                    }
                }
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
            if (temperature <= 0.5)
            {
                temperature -= 0.01;
            }
        }
    }
    else
    {
        cout << "Performing TCA" << endl;
    }

    // Printing the final angles
    cout << ":::::::::::Angles:::::::::::::" << endl;
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

    ofstream Srplot;
    Srplot.open("data/Sr_vs_r.txt");
    // Average out by diving SiSj
    cout << "Times measured = " << measure_counter << endl;
    for (int i = 0; i < ns; i++)
    {
        for (int j = 0; j < ns; j++)
        {
            // theta1 = real(theta(0, i));
            // theta2 = real(theta(0, j));
            SiSj(i, j) = SiSj(i, j) / measure_counter;
            Srplot << i << "\t" << j << "\t"
                   << real(SiSj(i, j)) << "\t" << imag(SiSj(i, j)) << endl;
        }
        Srplot << endl;
    }
    Srplot.close();

    Sq.resize(prm.Lx, prm.Ly);

    // cout << ":::::::::::S(q):::::::::::" << endl;

    double qx, qy, ix, iy, jx, jy;

    ofstream sqplot;
    sqplot.open("data/Sq.txt");

    for (int qnx = 0; qnx < prm.Lx; qnx++)
    {
        for (int qny = 0; qny < prm.Ly; qny++)
        {
            for (int i = 0; i < ns; i++)
            {
                for (int j = 0; j < ns; j++)
                {
                    qx = 2.0 * 3.1415926535 * qnx / double(prm.Lx);
                    qy = 2.0 * 3.1415926535 * qny / double(prm.Ly);
                    ix = kinv(i).first;
                    iy = kinv(i).second;
                    jx = kinv(j).first;
                    jy = kinv(j).second;
                    // cout << ix << " "
                    //      << iy << " " << jx << " " << jy << endl;
                    Sq(qnx, qny) +=
                        (1.0 / double(ns)) *
                        exp(imagi * qx * (ix - jx)) *
                        exp(imagi * qy * (iy - jy)) * SiSj(i, j);
                }
            }
            // cout << qx << "\t" << qy << "\t" << real(Sq(qnx, qny))
            //      << "\t" << imag(Sq(qnx, qny)) << endl;
            sqplot << qx << "\t" << qy << "\t" << real(Sq(qnx, qny))
                   << "\t" << imag(Sq(qnx, qny)) << endl;
        }
        // cout << endl;
        sqplot << endl;
    }

    sqplot.close();
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