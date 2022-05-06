#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <cmath>
#include <complex>
#include <vector>
#include <utility>

std::vector<double> linspace(double start_in, double end_in, int num_in);

double gauss(double x, double y);

double*** meshgrid(std::vector<double> x, std::vector<double> y);

#endif