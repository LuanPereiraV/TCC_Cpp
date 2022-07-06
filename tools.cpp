#include "tools.h"

#define _USE_MATH_DEFINES

std::vector<double> linspace(double start_in, double end_in, int num_in)
{

    std::vector<double> linspaced;

    double start = start_in;
    double end = end_in;
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
    return double((1 / (2 * M_PI * pow(alpha, 2))) * exp(-1 * ((pow(x, 2) / 1.0 + pow(y, 2) / 1.0)) / (2 * pow(alpha, 2))));
}

double*** meshgrid(std::vector<double> x, std::vector<double> y)
{
    double **xx; 
    double **yy;
    int size_x = x.size();
    int size_y = y.size();

    xx = new double*[size_y];
    yy = new double*[size_y];
    for(int i = 0; i < size_y; i++)
    {
        xx[i] = new double[size_x];
        yy[i] = new double[size_x];
        for(int j = 0; j < size_x; j++)
        {
            xx[i][j] = x[j];
            yy[i][j] = y[i];
        }
    }

    double*** res = new double**[2];
    res[0] = xx;
    res[1] = yy;

    return res;

    
}

double max(double* a, int n)
{
    if(a == NULL)
        return 0;
    double max = a[0];
    for(int i = 0; i < n; i++)
        if(a[i] > max)
            max = a[i];
    return max;
}

double max_m(double** a, int n, int m)
{
    if(a == NULL)
        return 0;
    double max = a[0][0];
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            if(a[i][j] > max)
                max = a[i][j];
    return max;
}
