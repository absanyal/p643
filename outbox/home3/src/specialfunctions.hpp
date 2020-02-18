#ifndef SPECIALFUNCTIONS_HPP
#define SPECIALFUNCTIONS_HPP
#include <cmath>

double hermpoly(int n, double x)
{
    if (n == 0)
    {
        return 1;
    }
    else if (n == 1)
    {
        return x;
    }
    else if (n >= 2)
    {
        return x * hermpoly(n - 1, x) - (n * 1.0 - 1.0) * hermpoly(n - 2, x);
    }
}

#endif