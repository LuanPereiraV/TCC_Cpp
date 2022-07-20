#ifndef SCHRO_H
#define SCHRO_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <complex>
#include <vector>
#include <chrono>
#include <ctime>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

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

    std::vector<SDL_Rect> barriers;
    std::vector<int> barriersEnergy;

    // SDL
    SDL_Window *window;
    SDL_Surface *gameSurface;
    SDL_Texture *gameTexture;

    SDL_Surface *menuSurface;
    std::vector<SDL_Texture*> buttonsTexture;

    int SCREEN_W;
    int SCREEN_H;

    void set_kxy(int kx, int ky);

    bool init();
    void restart();

public:
    Schro(int n, int lambda, double dt, double kx, double ky, double disp);
    ~Schro();

    void iterate();
    void set_barrier(int x, int y, int w, int h, double value);
    void draw();
    int display();
};

#endif // MACRO


