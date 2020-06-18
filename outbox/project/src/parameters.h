#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <cassert>

using std::ifstream;
using std::string;

typedef std::complex<double> cd;

class parameters
{
    // private:
    //     /* data */
public:
    int Lx, Ly, numw, Mx, My, nx, ny, seed;
    double minw, maxw, JH, JH_d, G, tx, ty,
        td, d_theta_x, d_theta_y, d_phi_x, d_phi_y, T, 
        window, sweeps, filling;
    bool plot_up_down_separately;

    double matchstring(string, string);
    void load(string);
};

double parameters::matchstring(string file, string match)
{
    string test;
    string line;
    ifstream readFile(file);
    double amount;
    bool pass = false;
    while (std::getline(readFile, line))
    {
        std::istringstream iss(line);
        if (std::getline(iss, test, '=') && pass == false)
        {
            // ---------------------------------
            if (iss >> amount && test == match)
            {
                // cout << amount << endl;
                pass = true;
            }
            else
            {
                pass = false;
            }
            // ---------------------------------
            if (pass)
                break;
        }
    }
    if (pass == false)
    {
        // string errorout = match;
        // errorout += "= argument is missing in the input file!";
        // throw std::invalid_argument(errorout);
        cout << match << "= argument is missing in the input file!";
    }
    cout << match << " = " << amount << endl;
    return amount;
}

void parameters::load(string inputfile)
{
    seed = matchstring(inputfile, "seed");
    Lx = matchstring(inputfile, "Lx");
    Ly = matchstring(inputfile, "Ly");
    JH = matchstring(inputfile, "JH");
    JH_d = matchstring(inputfile, "JH_d");
    tx = matchstring(inputfile, "tx");
    ty = matchstring(inputfile, "ty");
    td = matchstring(inputfile, "td");
    d_theta_x = matchstring(inputfile, "d_theta_x");
    d_theta_y = matchstring(inputfile, "d_theta_y");
    d_phi_x = matchstring(inputfile, "d_phi_x");
    d_phi_y = matchstring(inputfile, "d_phi_y");
    numw = matchstring(inputfile, "numw");
    minw = matchstring(inputfile, "minw");
    maxw = matchstring(inputfile, "maxw");
    G = matchstring(inputfile, "G");
    plot_up_down_separately = matchstring(inputfile, "plot_up_down_separately");
    T = matchstring(inputfile, "temperature");
    window = matchstring(inputfile, "window");
    sweeps = matchstring(inputfile, "sweeps");
    filling = matchstring(inputfile, "filling");
}

#endif