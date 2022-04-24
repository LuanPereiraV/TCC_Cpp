#include <iostream>
#include <fstream>
#include <cmath>
#include <complex>
#include <SDL2/SDL.h>

#define _USE_MATH_DEFINES

double gauss(double x, double y)
{
    double alpha = 0.2;
    return double((1/(2 * M_PI * pow(alpha, 2))) * exp(-1*((pow(x, 2)/1.0 + pow(y, 2)/1.0))/(2*pow(alpha, 2))));
}

int main(int argc, char const *argv[])
{
    
    std::complex<double>** grid;
    std::ofstream myfile;

    myfile.open("data.dat");

    for(float i = -1; i < 1; i+=0.01)
    {
        for(float j = -1; j < 1; j+=0.01)
        {
            myfile << gauss(i, j) << " ";
        }
        myfile << "\n";
    }
    myfile.close();

    return 0;
}
