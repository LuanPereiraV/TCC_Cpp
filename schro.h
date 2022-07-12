#ifndef SCHRO_H
#define SCHRO_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <complex>
#include <vector>
#include <chrono>
#include <ctime>

#include "tools.h"

class Schro
{
private:
    int n;
    double l;
    double dt;

    std::vector<double> x;
    std::vector<double> y;

    double **u;
    double **v;
    double ***xxyy;

    double kx;
    double ky;

    double disp;

    double a;
    double sc;

    double **H;
    double **V;

    double **xx;
    double **yy;

    double **grid;

public:
    Schro(int n, int lambda, double dt, double kx, double ky, double disp);
    ~Schro();

    void iterate();
    void set_barrier(int x1, int y1, int x2, int y2, double value);
    void draw();
};

#endif // MACRO


