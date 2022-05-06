#include <iostream>
#include <fstream>
#include <cmath>
#include <complex>
#include <vector>

#include "tools.h"

// #include <SDL2/SDL.h>

int main(int argc, char const *argv[])
{
    int n = 80;
    double l = n/10;

    std::vector<double> x;
    std::vector<double> y;

    x = linspace(-l, l, n);
    y = linspace(-l, l, n);
    // x = linspace(-1, 1, 3);
    // y = linspace(-1, 1, 3);

    double **u = new double *[n];
    double **v = new double *[n];
    for(int i = 0; i < n; i++){
        u[i] = new double[n]();
        v[i] = new double[n]();
    }

    double*** xxyy;

    xxyy = meshgrid(x, y);

    // for (int i = 0; i < y.size(); i++)
    // {
    //     for (int j = 0; j < x.size(); j++)
    //     {
    //         std::cout << xxyy[0][i][j] << " ";
    //     }
    //     std::cout << "\n";
    // }

    return 0;
}
