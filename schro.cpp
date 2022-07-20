#include "schro.h"

void print_text(SDL_Renderer *renderer, char *text, int tam, SDL_Color textColor)
{
    SDL_Surface *surface;
    TTF_Font *font;

    TTF_Init();

    font = TTF_OpenFont("Lunchtime Doubly So.ttf", tam);
    surface = TTF_RenderText_Solid(font, text, textColor);


    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {10, 10, texW, texH};

    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    TTF_CloseFont(font);
}

void angle_transfer(int ox, int oy, int px, int py, float angle, int *dx, int *dy)
{
    float radians = angle * M_PI/180;
    *dx = (cos(radians) * (px - ox)) - (sin(radians) * (py - oy)) + ox;
    *dy = (sin(radians) * (px - ox)) + (cos(radians) * (py - oy)) + oy;
}

SDL_Texture* set_texture(SDL_Renderer *renderer, SDL_Rect *ret, const char* file)
{
    SDL_Texture *texture = NULL;
    SDL_Surface *surface;

    surface = IMG_Load(file); // nome da imagem
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
    // SDL_RenderCopy(renderer, texture, NULL, ret);
}

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    Uint32 *const target_pixel = (Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel);
    *target_pixel = pixel;
}

void draw_rectangle(SDL_Surface *surface, int x, int y, int width, int height, int w)
{
    // SDL_LockSurface(surface);

    Uint32 *pixels32 = (Uint32 *)surface->pixels;

    int dy, dx;
    int maxwidth = width * 3;
    for (dy = y; dy < height; dy++)
    {
        for (dx = x; dx < maxwidth; dx += 3)
        {
            pixels32[dx * w + dy] = SDL_MapRGBA(surface->format, (Uint8)100, (Uint8)100, (Uint8)100, (Uint8)100);
        }
    }

    // SDL_UnlockSurface(surface);
}

Schro::Schro(int n, int lambda, double dt, double kx, double ky, double disp)
{
    Schro::n = n;
    Schro::l = n / lambda;
    Schro::dt = dt;

    Schro::SCREEN_W = 1024;
    Schro::SCREEN_H = 768;

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
            Schro::u[i][j] = cos((Schro::kx * Schro::xx[i][j]) + (Schro::ky * Schro::yy[i][j])) * exp(-1.0 / Schro::disp * pow(Schro::xx[i][j] - floor(l / 1.5), 2.0)) * exp(-1.0 / Schro::disp * pow(Schro::yy[i][j], 2.0));
            Schro::v[i][j] = -1.0 * sin((Schro::kx * Schro::xx[i][j]) + (Schro::ky * Schro::yy[i][j])) * exp(-1.0 / Schro::disp * pow(Schro::xx[i][j] - floor(l / 1.5), 2.0)) * exp(-1.0 / Schro::disp * pow(Schro::yy[i][j], 2.0));
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

    // Init SDL

    // The window we'll be rendering to
    Schro::window = NULL;
}

bool Schro::init()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Create window
        Schro::window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Schro::SCREEN_W, Schro::SCREEN_H, SDL_WINDOW_SHOWN);
        if (Schro::window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Update the surface
            SDL_UpdateWindowSurface(Schro::window);
        }
    }

    return success;
}

void Schro::restart()
{
    for (int i = 0; i < Schro::n; i++)
    {
        for (int j = 0; j < Schro::n; j++)
        {
            Schro::u[i][j] = cos((Schro::kx * Schro::xx[i][j]) + (Schro::ky * Schro::yy[i][j])) * exp(-1.0 / Schro::disp * pow(Schro::xx[i][j] - floor(l / 1.5), 2.0)) * exp(-1.0 / Schro::disp * pow(Schro::yy[i][j], 2.0));
            Schro::v[i][j] = -1.0 * sin((Schro::kx * Schro::xx[i][j]) + (Schro::ky * Schro::yy[i][j])) * exp(-1.0 / Schro::disp * pow(Schro::xx[i][j] - floor(l / 1.5), 2.0)) * exp(-1.0 / Schro::disp * pow(Schro::yy[i][j], 2.0));
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

void Schro::set_barrier(int x, int y, int w, int h, double value)
{
    for (int i = x; i < x + w; i++)
    {
        for (int j = y; j < y + h; j++)
        {
            Schro::V[i][j] = value;
        }
    }

    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    Schro::barriers.push_back(r);
    Schro::barriersEnergy.push_back(value);
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

void Schro::set_kxy(int kx, int ky)
{
    Schro::kx = kx;
    Schro::ky = ky;
}

int Schro::display()
{
    Schro::init();

    SDL_Renderer *renderer = SDL_CreateRenderer(Schro::window, -1, SDL_RENDERER_ACCELERATED);

    Schro::gameTexture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           Schro::SCREEN_W,
                                           Schro::SCREEN_H);

    Schro::gameSurface = SDL_CreateRGBSurface(0,
                                              Schro::SCREEN_W,
                                              Schro::SCREEN_H,
                                              32,
                                              0x00000000,
                                              0x00000000,
                                              0x00000000,
                                              0xff000000);

    Uint32 *pixels32 = (Uint32 *)Schro::gameSurface->pixels;

    if (renderer == NULL)
    {
        SDL_Log("Could not create a renderer: %s", SDL_GetError());
        return -1;
    }

    Schro::gameTexture = SDL_CreateTexture(renderer, 0, 0, Schro::n, Schro::n);

    SDL_Rect rr;
    rr.x = 0;
    rr.y = 0;
    rr.h = Schro::SCREEN_H;
    rr.w = Schro::SCREEN_H;

    SDL_Rect menu;
    menu.x = Schro::SCREEN_H;
    menu.y = 0;
    menu.h = Schro::SCREEN_H;
    menu.w = Schro::SCREEN_W - Schro::SCREEN_H;

    SDL_Rect start;
    start.x = Schro::SCREEN_H + 10;
    start.y = 10;
    start.h = Schro::SCREEN_H/8;
    start.w = (Schro::SCREEN_W - Schro::SCREEN_H) - 20;

    SDL_Rect pause;
    pause.x = Schro::SCREEN_H + 10;
    pause.y = (Schro::SCREEN_H / 8) + 20;
    pause.h = Schro::SCREEN_H / 8;
    pause.w = (Schro::SCREEN_W - Schro::SCREEN_H) - 20;

    SDL_Rect rewind;
    rewind.x = Schro::SCREEN_H + 10;
    rewind.y = (Schro::SCREEN_H / 8) * 2 + 30;
    rewind.h = Schro::SCREEN_H / 8;
    rewind.w = (Schro::SCREEN_W - Schro::SCREEN_H) - 20;

    SDL_Rect set_vector;
    set_vector.x = Schro::SCREEN_H + 10;
    set_vector.y = (Schro::SCREEN_H / 8) * 3 + 40;
    set_vector.h = Schro::SCREEN_H / 8;
    set_vector.w = (Schro::SCREEN_W - Schro::SCREEN_H) - 20;

    SDL_Rect stop;
    stop.x = Schro::SCREEN_H + 10;
    stop.y = (Schro::SCREEN_H / 8) * 4 + 50;
    stop.h = Schro::SCREEN_H / 8;
    stop.w = (Schro::SCREEN_W - Schro::SCREEN_H) - 20;

    SDL_Rect map;
    map.x = Schro::SCREEN_H + 10;
    map.y = (Schro::SCREEN_H / 8) * 5 + 50;
    map.h = (Schro::SCREEN_W - Schro::SCREEN_H) - 20;
    map.w = (Schro::SCREEN_W - Schro::SCREEN_H) - 20;

    SDL_Rect hole;
    hole.w = 50;
    hole.h = 50;
    hole.x = Schro::n / 2 - hole.w / 2;
    hole.y = Schro::n / 2 - hole.h / 2 - 150;

    int map_center_x = map.x + map.w/2;
    int map_center_y = map.y + map.h/2;

    SDL_Point mouse_position;


    SDL_SetRenderDrawColor(renderer, 0, 40, 0, 255);
    SDL_RenderFillRect(renderer, &menu);


    SDL_Texture* btStart = set_texture(renderer, &start, "buttons/Play.png");
    SDL_Texture *btPause = set_texture(renderer, &pause, "buttons/Pause.png");
    SDL_Texture *btRewind = set_texture(renderer, &pause, "buttons/Rewind.png");
    SDL_Texture *btStop = set_texture(renderer, &stop, "buttons/Stop.png");
    SDL_Texture *btSetVec = set_texture(renderer, &set_vector, "buttons/Foward.png");
    SDL_Texture *btMap = set_texture(renderer, &map, "buttons/map.png");

    SDL_RenderPresent(renderer);

    int dx = map_center_x;
    int dy = map_center_y + 90;
    int angle = -90;

    bool play = false;

    SDL_Color textColor;
    textColor.a = 0;
    textColor.r = 255;
    textColor.g = 255;
    textColor.b = 255;

    int points = 0;
    char* charPoints = (char*)malloc(sizeof(char)*100);
    sprintf(charPoints, "Points: %d", points);

    while (true)
    {

        Uint32 start_time, frame_time;
        float fps;
        start_time = SDL_GetTicks();

        SDL_GetMouseState(     //    Sets mouse_position to...
            &mouse_position.x, // ...mouse arrow coords on window
            &mouse_position.y);

        // Get the next event
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                // Break out of the loop on quit
                break;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_LEFT)
                    angle -= 15;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    angle += 15;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (SDL_PointInRect(&mouse_position, &start))
                {
                    play = true;
                    std::cout << "Play\n";
                }
                if (SDL_PointInRect(&mouse_position, &pause))
                {
                    play = false;
                    std::cout << "Pause\n";
                }
                if (SDL_PointInRect(&mouse_position, &rewind))
                {
                    play = false;
                    Schro::restart();
                    sprintf(charPoints, "Points: %d", points);
                    std::cout << "Restart\n";
                }
                if (SDL_PointInRect(&mouse_position, &set_vector))
                {
                    set_kxy((map_center_x - dx) / 18, (map_center_y - dy) / 18);
                    Schro::restart();
                    std::cout << "SetVetor\n";
                }
                if (SDL_PointInRect(&mouse_position, &stop))
                {
                    for (int i = hole.x + 1; i < hole.x + hole.w - 1; i++)
                    {
                        for (int j = hole.y + 1; j < hole.y + hole.h - 1; j++)
                        {
                            points += (int)pixels32[i * Schro::SCREEN_W + j]/10000;
                        }
                    }
                    std::cout << "Stop\n";
                    std::cout << points << std::endl;
                    sprintf(charPoints, "Points: %d", points);
                    points = 0;
                    play = false;
                }
            }   
        }

        // Aqui acontece a magica
        if(play)
        {
            Schro::iterate();
        }


        for (int i = 0; i < Schro::n; i++)
        {
            for (int j = 0; j < Schro::n; j++)
            {
                int value = (int)(sqrt(pow(Schro::grid[i][j], 2)) * 255);
                if (value > 255)
                    value = 255;

                pixels32[i * Schro::SCREEN_W + j] = SDL_MapRGBA(Schro::gameSurface->format, (Uint8)value, (Uint8)value, (Uint8)value, (Uint8)value);
            }
        }

        for (int i = 0; i < Schro::barriers.size(); i++)
        {
            for (int dy = Schro::barriers[i].y; dy < Schro::barriers[i].y + Schro::barriers[i].h; dy++)
            {
                for (int dx = Schro::barriers[i].x; dx < Schro::barriers[i].x + Schro::barriers[i].w; dx++)
                {
                    pixels32[dx * Schro::SCREEN_W + dy] = SDL_MapRGBA(Schro::gameSurface->format, 0x1E, (Uint8)Schro::barriersEnergy[i], 0x1E, 0xFF);
                }
            }
        }

        for(int i = hole.x; i < hole.x + hole.w; i++)
        {
            for(int j = hole.y; j < hole.y + hole.h; j++)
            {
                if (i == hole.x || i == hole.x + hole.w - 1 || j == hole.y || j == hole.y + hole.h - 1)
                    pixels32[i * Schro::SCREEN_W + j] = SDL_MapRGBA(Schro::gameSurface->format, (Uint8)255, (Uint8)0, (Uint8)0, (Uint8)155);
            }
        }
            // SDL_RenderFillRect(renderer, &Schro::barriers[i]);

        SDL_UpdateTexture(Schro::gameTexture, NULL, Schro::gameSurface->pixels, Schro::gameSurface->pitch);
        SDL_RenderCopy(renderer, Schro::gameTexture, NULL, &rr);
        // Set render color to blue ( rect will be rendered in this color )
        // Render Barrier

        // Render menu and butons
        
        SDL_SetRenderDrawColor(renderer, 0, 40, 0, 255);
        SDL_RenderFillRect(renderer, &menu);
        SDL_RenderCopy(renderer, btStart, NULL, &start);
        SDL_RenderCopy(renderer, btPause, NULL, &pause);
        SDL_RenderCopy(renderer, btRewind, NULL, &rewind);
        SDL_RenderCopy(renderer, btStop, NULL, &stop);
        SDL_RenderCopy(renderer, btSetVec, NULL, &set_vector);
        SDL_RenderCopy(renderer, btMap, NULL, &map);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        angle_transfer(map_center_x, map_center_y, map_center_x, map_center_y - 90, angle, &dx, &dy);
        SDL_RenderDrawLine(renderer, map_center_x, map_center_y, dx, dy);

        print_text(renderer, charPoints, 32, textColor);

        // Show the renderer contents
        SDL_RenderPresent(renderer);

        frame_time = SDL_GetTicks() - start_time;
        fps = (frame_time > 0) ? 1000.0f / frame_time : 0.0f;
        // std::cout << (map_center_x - dx) / 18 << " " << (map_center_y - dy) / 18 << std::endl;

        // std::cout << fps << std::endl;
    }

    // Destroy window
    SDL_DestroyWindow(Schro::window);

    // Quit SDL subsystems
    TTF_Quit();
    SDL_Quit();
    return 0;
}