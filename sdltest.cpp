#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

int main(int argc, char **argv)
{
    enum
    {
        WIDTH = 640,
        HEIGHT = 480
    };
    SDL_Window *window = SDL_CreateWindow(
        "SDL test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture *texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             WIDTH,
                                             HEIGHT);
    SDL_Surface *surface = SDL_CreateRGBSurface(
        0,
        WIDTH,
        HEIGHT,
        32,
        0x00000000,
        0x00000000,
        0x00000000,
        0xff000000);
    Uint32 gray = SDL_MapRGBA(surface->format, 0x1E, 0x1E, 0x1E, 0xFF);
    Uint32 red = SDL_MapRGBA(surface->format, 0xFA, 0x32, 0x32, 0xFF);

    int i, j;
    Uint32 *pixels32 = (Uint32 *)surface->pixels;
    for (i = 0; i < HEIGHT; ++i)
    {
        for (j = 0; j < WIDTH; ++j)
        {
            pixels32[i * WIDTH + j] = j % 10 ? gray : red;
        }
    }
    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);

    SDL_RenderPresent(renderer);

    SDL_Rect rr;
    rr.x = 0;
    rr.y = 0;
    rr.h = 200;
    rr.w = 500;

    int quit = 0;
    SDL_Event event;
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
                quit = 1;
        }

        SDL_RenderCopy(renderer, texture, NULL, &rr);

        SDL_RenderPresent(renderer);
    }
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}