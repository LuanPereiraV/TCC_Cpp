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

    double **u = new double *[n];
    double **v = new double *[n];

    for(int i = 0; i < n; i++){
        u[i] = new double[n]();
        v[i] = new double[n]();
    }

    double*** xxyy;

    xxyy = meshgrid(x, y);

    double** xx = xxyy[0];
    double** yy = xxyy[1];

    double kx = 1.0 * (2.0 * M_PI);
    double ky = 0;

    double disp = 3.0;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            u[i][j] = cos((kx * xx[i][j]) + (ky * yy[i][j])) * exp(-1.0 / disp * pow(xx[i][j] - floor(l/2.0), 2.0)) * exp(-1.0 / disp * pow(yy[i][j], 2.0));
            v[i][j] = -1.0*sin((kx * xx[i][j]) + (ky * yy[i][j])) * exp(-1.0 / disp * pow(xx[i][j] - floor(l/2.0), 2.0)) * exp(-1.0 / disp * pow(yy[i][j], 2.0));
        }
    }

    double a = 2.0 * l / (n + 1);
    double sc;

    double max = sqrt(pow(u[0][0], 2) + pow(v[0][0], 2));

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            double res = sqrt(pow(u[i][j], 2) + pow(v[i][j], 2));
            if(res > max)
                max = res;
        }
    }

    sc = 1.0 / max;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            u[i][j] *= sc;
            v[i][j] *= sc;
        }
    }

    sc = 0.5 / pow(a, 2);

    double** V = new double *[n];
    for(int i = 0; i < n; i++)
        V[i] = new double[n]();

    
    
    // std::cout.precision(17);
    // std::cout << sc << std::endl;

    // FILE* arq = fopen("data.txt", "w");


    // for (int i = 0; i < y.size(); i++)
    // {
    //     for (int j = 0; j < x.size(); j++)
    //     {
    //         fprintf(arq, "%lf ", v[i][j]);
    //         // std::cout << xx[i][j] << ' ';
    //     }
    //     // std::cout << std::endl;
    //     fprintf(arq, "\n");
    // }

    // fclose(arq);

    return 0;
}
