#include <iostream>
#include <fstream>
#include <cmath>
#include <complex>
#include <vector>
#include <chrono>
#include <ctime>

#include "tools.h"

// #include <SDL2/SDL.h>


double** set_barrier(double** V, int n, int x1, int y1, int x2, int y2, double value)
{
    if(V == NULL)
    {
        V = new double *[n];
        for(int i = 0; i < n; i++)
            V[i] = new double[n]();
    }
    for(int i = x1; i < x2; i++)
    {
        for(int j = y1; j < y2; j++)
        {
            V[i][j] = value;
        }
    }
    return V;
}

double** iterate(int n, double** u, double** v, double** V, double sc, double dt)
{   
    double** grid = new double *[n];
    double** H = new double *[n];


    for(int i = 0; i < n; i++)
    {
        grid[i] = new double[n]();
        H[i] = new double[n]();
    }

    for(int i = 1; i < n-1; i++)
    {
        for(int j = 1; j < n-1; j++)
        {
            H[i][j] = -sc * ((u[i + 1][j] + u[i - 1][j] + u[i][j + 1] + u[i][j - 1]) - 4.0 * u[i][j]);
        }
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            H[i][j] = H[i][j] + (V[i][j] * u[i][j]);
            v[i][j] = v[i][j] - (H[i][j] * dt);
        }
    }

    for(int i = 1; i < n-1; i++)
    {
        for(int j = 1; j < n-1; j++)
        {
            H[i][j] = -sc * ((v[i + 1][j] + v[i - 1][j] + v[i][j + 1] + v[i][j - 1]) - 4.0 * v[i][j]);
        }
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            H[i][j] = H[i][j] + (V[i][j] * v[i][j]);
            u[i][j] = u[i][j] + (H[i][j] * dt);
        }
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            grid[i][j] = sqrt(pow(u[i][j], 2) + pow(v[i][j], 2));
        }
    }
    
    return grid;
}


int main(int argc, char const *argv[])
{
    int n = 200;
    double l = n/10;
    double dt = 0.002;

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

    double kx = -5.0;
    double ky = 0.0;

    double disp = 4.0;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            u[i][j] = cos((kx * xx[i][j]) + (ky * yy[i][j])) * exp(-1.0 / disp * pow(xx[i][j], 2.0)) * exp(-1.0 / disp * pow(yy[i][j], 2.0));
            v[i][j] = -1.0*sin((kx * xx[i][j]) + (ky * yy[i][j])) * exp(-1.0 / disp * pow(xx[i][j], 2.0)) * exp(-1.0 / disp * pow(yy[i][j], 2.0));
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

    
    V = set_barrier(V, n, 0, 0, n, 1, 30);
    V = set_barrier(V, n, 0, 0, 1, n, 30);
    V = set_barrier(V, n, n-1, 0, n, n, 30);
    V = set_barrier(V, 0, 0, n-1, n, n, 30);

    double **grid;

    auto start = std::chrono::system_clock::now();

    for(int i = 0; i < 1; i++)
    {
        grid = iterate(n, u, v, V, sc, dt);
    }

    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s" << std::endl;

    FILE* arq = fopen("data.txt", "w");

    for (int i = 0; i < y.size(); i++)
    {
        for (int j = 0; j < x.size(); j++)
        {
            fprintf(arq, "%lf ", grid[i][j]);
            // std::cout << xx[i][j] << ' ';
        }
        // std::cout << std::endl;
        fprintf(arq, "\n");
    }

    fclose(arq);
    
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
