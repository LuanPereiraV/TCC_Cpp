#include "schro.h"

Schro::Schro(int n, int lambda, double dt, double kx, double ky, double disp)
{
    Schro::n = n;
    Schro::l = n / lambda;
    Schro::dt = dt;


    Schro::x = linspace(-l, l, n);
    Schro::y = linspace(-l, l, n);

    Schro::u = new double *[n];
    Schro::v = new double *[n];

    Schro::grid = new double *[n];
    Schro::H = new double *[n];
    

    for (int i = 0; i < n; i++)
    {
        Schro::u[i] = new double[n]();
        Schro::v[i] = new double[n]();

        Schro::grid[i] = new double[n]();
        Schro::H[i] = new double[n]();
    }

    

    Schro::xxyy = meshgrid(Schro::x, Schro::y);

    Schro::xx = Schro::xxyy[0];
    Schro::yy = Schro::xxyy[1];

    Schro::kx = kx;
    Schro::ky = ky;

    Schro::disp = disp;

    for (int i = 0; i < Schro::n; i++)
    {
        for (int j = 0; j < Schro::n; j++)
        {
            Schro::u[i][j] = cos((Schro::kx * Schro::xx[i][j]) + (Schro::ky * Schro::yy[i][j])) * exp(-1.0 / Schro::disp * pow(Schro::xx[i][j] - floor(l / 2.0), 2.0)) * exp(-1.0 / Schro::disp * pow(Schro::yy[i][j], 2.0));
            Schro::v[i][j] = -1.0 * sin((Schro::kx * Schro::xx[i][j]) + (Schro::ky * Schro::yy[i][j])) * exp(-1.0 / Schro::disp * pow(Schro::xx[i][j] - floor(l / 2.0), 2.0)) * exp(-1.0 / Schro::disp * pow(Schro::yy[i][j], 2.0));
        }
    }

    Schro::a = 2.0 * Schro::l / (Schro::n + 1);

    double max = sqrt(pow(Schro::u[0][0], 2) + pow(Schro::v[0][0], 2));

    for (int i = 0; i < Schro::n; i++)
    {
        for (int j = 0; j < Schro::n; j++)
        {
            double res = sqrt(pow(Schro::u[i][j], 2) + pow(Schro::v[i][j], 2));
            if (res > max)
                max = res;
        }
    }

    Schro::sc = 1.0 / max;

    for (int i = 0; i < Schro::n; i++)
    {
        for (int j = 0; j < Schro::n; j++)
        {
            Schro::u[i][j] *= Schro::sc;
            Schro::v[i][j] *= Schro::sc;
        }
    }

    Schro::sc = 0.5 / pow(Schro::a, 2);

    Schro::V = new double *[Schro::n];
    for (int i = 0; i < Schro::n; i++)
        Schro::V[i] = new double[Schro::n]();

    for (int i = 0; i < Schro::n; i++)
    {
        for (int j = 0; j < Schro::n; j++)
        {
            Schro::grid[i][j] = sqrt(pow(Schro::u[i][j], 2) + pow(Schro::v[i][j], 2));
        }
    }
}

void Schro::iterate()
{
    for (int i = 1; i < Schro::n - 1; i++)
    {
        for (int j = 1; j < Schro::n - 1; j++)
        {
            Schro::H[i][j] = -Schro::sc * ((Schro::u[i + 1][j] + Schro::u[i - 1][j] + Schro::u[i][j + 1] + Schro::u[i][j - 1]) - 4.0 * Schro::u[i][j]);
        }
    }

    for (int i = 0; i < Schro::n; i++)
    {
        for (int j = 0; j < Schro::n; j++)
        {
            Schro::H[i][j] = Schro::H[i][j] + (Schro::V[i][j] * Schro::u[i][j]);
            Schro::v[i][j] = Schro::v[i][j] - (Schro::H[i][j] * Schro::dt);
        }
    }

    for (int i = 1; i < Schro::n - 1; i++)
    {
        for (int j = 1; j < Schro::n - 1; j++)
        {
            Schro::H[i][j] = -Schro::sc * ((Schro::v[i + 1][j] + Schro::v[i - 1][j] + Schro::v[i][j + 1] + Schro::v[i][j - 1]) - 4.0 * Schro::v[i][j]);
        }
    }

    for (int i = 0; i < Schro::n; i++)
    {
        for (int j = 0; j < Schro::n; j++)
        {
            Schro::H[i][j] = Schro::H[i][j] + (Schro::V[i][j] * Schro::v[i][j]);
            Schro::u[i][j] = Schro::u[i][j] + (Schro::H[i][j] * Schro::dt);
        }
    }

    for (int i = 0; i < Schro::n; i++)
    {
        for (int j = 0; j < Schro::n; j++)
        {
            Schro::grid[i][j] = sqrt(pow(Schro::u[i][j], 2) + pow(Schro::v[i][j], 2));
        }
    }
}

void Schro::set_barrier(int x1, int y1, int x2, int y2, double value)
{
    for (int i = x1; i < x2; i++)
    {
        for (int j = y1; j < y2; j++)
        {
            Schro::V[i][j] = value;
        }
    }
}

void Schro::draw()
{
    FILE *arq = fopen("data.txt", "w");

    for (int i = 0; i < Schro::n; i++)
    {
        for (int j = 0; j < Schro::n; j++)
        {
            fprintf(arq, "%lf ", Schro::grid[i][j]);
        }
        fprintf(arq, "\n");
    }
    
    fclose(arq);
}

int Schro::display()
{
    // The window we'll be rendering to
    SDL_Window *window = NULL;

    // The surface contained by the window
    SDL_Surface *screenSurface = NULL;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        // Create window
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Schro::n, Schro::n, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            // Get window surface
            screenSurface = SDL_GetWindowSurface(window);

            // Fill the surface white
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

            // Update the surface
            SDL_UpdateWindowSurface(window);

            SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                SDL_Log("Could not create a renderer: %s", SDL_GetError());
                return -1;
            }

            SDL_Rect r;
            r.x = 200;
            r.y = 200;
            r.w = 50;
            r.h = 50;

            while (true)
            {

                Uint32 start_time, frame_time;
                float fps;
                start_time = SDL_GetTicks();

                // Get the next event
                SDL_Event event;
                if (SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT)
                    {
                        // Break out of the loop on quit
                        break;
                    }
                }

                // Set the color to cornflower blue and clear
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                Schro::iterate();

                for (int i = 0; i < Schro::n; i++)
                {
                    for (int j = 0; j < Schro::n; j++)
                    {
                        double value = sqrt(pow(Schro::grid[i][j], 2)) * 255;
                        if(value > 255)
                            value = 255;
                        SDL_SetRenderDrawColor(renderer, value, value, value, 255);
                        SDL_RenderDrawPoint(renderer, i, j);
                    }
                }

                // Set render color to blue ( rect will be rendered in this color )
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

                // Render rect
                SDL_RenderFillRect(renderer, &r);

                // Show the renderer contents
                SDL_RenderPresent(renderer);

                frame_time = SDL_GetTicks() - start_time;
                fps = (frame_time > 0) ? 1000.0f / frame_time : 0.0f;

                std::cout << fps << std::endl;
            }
        }
    }
    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();
    return 0;
}