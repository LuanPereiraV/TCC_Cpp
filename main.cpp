#include <iostream>
#include <fstream>
#include <cmath>
#include <complex>
// #include <SDL2/SDL.h>
#include <vector>

#define _USE_MATH_DEFINES

template <typename T>
std::vector<double> linspace(T start_in, T end_in, int num_in)
{

    std::vector<double> linspaced;

    double start = static_cast<double>(start_in);
    double end = static_cast<double>(end_in);
    double num = static_cast<double>(num_in);

    if (num == 0)
    {
        return linspaced;
    }
    if (num == 1)
    {
        linspaced.push_back(start);
        return linspaced;
    }

    double delta = (end - start) / (num - 1);

    for (int i = 0; i < num - 1; ++i)
    {
        linspaced.push_back(start + delta * i);
    }

    linspaced.push_back(end); 

    return linspaced;
}

double gauss(double x, double y)
{
    double alpha = 0.2;
    return double((1/(2 * M_PI * pow(alpha, 2))) * exp(-1*((pow(x, 2)/1.0 + pow(y, 2)/1.0))/(2*pow(alpha, 2))));
}

int main(int argc, char const *argv[])
{
    int n = 80;
    double l = n/10;

    std::vector<double> x;
    std::vector<double> y;

    x = linspace(-l, l, n);
    y = linspace(-l, l, n);

    double **u = new double *[n];
    double **v = new double *[n];
    for(int i = 0; i < n; i++){
        u[i] = new double[n]();
        v[i] = new double[n]();
    }

    

    return 0;
}
