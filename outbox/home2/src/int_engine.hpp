#ifndef INT_ENGINE_HPP
#define INT_ENGINE_HPP

#include <assert.h>
#include <typeinfo>

#include "function.hpp"

class int_engine
{
public:
    double midpoint(double, double, int);
    double trapezoid(double, double, int);
    double simpsononethird(double, double, int);
};

double int_engine::midpoint(double a, double b, int n)
{
    double h, x, sum;

    h = (b - a) / n;
    x = a;
    sum = 0;

    for (int i = 0; i < n; i++)
    {
        sum += f(x + h / 2) * h;
        x += h;
    }

    return sum;
}

double int_engine::trapezoid(double a, double b, int n)
{
    double h, x, sum;

    h = (b - a) / n;
    x = a;
    sum = 0;

    for (int i = 0; i < n; i++)
    {
        sum += (h / 2) * (f(x) + f(x + h));
        x += h;
    }

    return sum;
}

double int_engine::simpsononethird(double a, double b, int n)
{
    double h, x, sum;

    h = (b - a) / n;
    x = a;
    sum = 0;

    for (int i = 0; i < n; i++)
    {
        sum += (h / 6) * (f(x) + 4 * f(x + h / 2) + f(x + h));
        x += h;
    }

    return sum;
}

#endif