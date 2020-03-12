#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int L, steps;

double h, accuracy, omega;

vector<vector<double>> V, Vnew, rho;

void initialize();
double relativeError();
void iterate(void);
void sor();

void initialize()
{
    int N = L + 2;

    V = vector<vector<double>>(N, vector<double>(N));
    Vnew = vector<vector<double>>(N, vector<double>(N));
    rho = vector<vector<double>>(N, vector<double>(N));

    h = 1.0 / (1.0 * L + 1.0);
    steps = 0;
    omega = 2 / (1 + 4 * atan(1.0) / double(L));

    //decorate the lattice with charges
    double q = 10.0;
    // rho[N / 2][N / 2] = q;
    for (int i = 1; i <= L; i++)
    {
        for (int j = 1; j <= L; j++)
        {
            rho[i][j] = double(sin((i + j) * M_PI / L)) * q;
        }
    }
}

double relativeError()
{
    double error = 0;
    int n = 0;

    for (int i = 0; i <= L; i++)
    {
        for (int j = 0; j <= L; j++)
        {
            if (Vnew[i][j] != 0 && Vnew[i][j] != V[i][j])
            {
                error += abs((Vnew[i][j] - V[i][j]) / Vnew[i][j]);
            }
        }
    }
    return error;
}

void iterate(void (*method)())
{
    while (true)
    {
        method();
        steps += 1;
        double error = relativeError();
        if (error < accuracy)
        {
            break;
        }
        vector<vector<double>> swap = V;
        V = Vnew;
        Vnew = swap;
    }
    cout << "Number of steps = " << steps << endl;
}

void sor()
{
    for (int i = 1; i <= L; i++)
    {
        for (int j = 1; j <= L; j++)
        {
            if ((i + j) % 2 == 0)
            {
                Vnew[i][j] = (1 - omega) * V[i][j] +
                             omega / 4 * (V[i - 1][j] + V[i + 1][j] + V[i][j - 1] + V[i][j + 1] + h * h * rho[i][j]);
            }
        }
    }
    for (int i = 1; i <= L; i++)
    {
        for (int j = 1; j <= L; j++)
        {
            if ((i + j) % 2 != 0)
            {
                Vnew[i][j] = (1 - omega) * V[i][j] + omega / 4 *
                                                         (Vnew[i - 1][j] + Vnew[i + 1][j] +
                                                          Vnew[i][j - 1] + Vnew[i][j + 1] +
                                                          h * h * rho[i][j]);
            }
        }
    }
}

int main()
{
    cout << "Enter grid size: ";
    cin >> L;
    cout << "Enter accuracy: ";
    cin >> accuracy;
    initialize();
    iterate(sor);
    cout << "Potential in file poisson.data" << endl;
    ofstream dataFile("poisson.data");
    for (int i = 0; i < L + 2; i++)
    {
        double x = i * h;
        for (int j = 0; j < L + 2; j++)
        {
            double y = j * h;
            dataFile << x << '\t' << y << '\t' << V[i][j] << '\n';
        }
        dataFile << '\n';
    }
    dataFile.close();
}