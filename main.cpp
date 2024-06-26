#include <iostream>
#include <SDL2/SDL.h>
#include <math.h>
#include <ctime>
#define PI 3.141592654

int WIDTH = 400, HEIGHT = 400;

int main( int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("SDL Clock", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        std::cout << "Could now create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    //Clocksize used for dynamic sizing.
    int ClockSize = (HEIGHT - 100) / 2;
    if(WIDTH < HEIGHT){
        ClockSize = (WIDTH - 100) / 2;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    
    const int ClockBoardCorners = 32;
    int LastSecond = 0;

    SDL_Event windowEvent;

    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                break;
            }
        }
        
        // -- Calculating the time -- //
        const time_t now = time(nullptr);
        const tm calendar_time = *localtime(std::addressof(now));
        float Seconds = (float)calendar_time.tm_sec;
        
        //Skips rendering if data doesn't change.
        if(Seconds == LastSecond){
            continue;
        }
        else{
            LastSecond = Seconds;
        }

        float Minutes = (float)calendar_time.tm_min + Seconds / 60.0; // Adds division of the lower unit, so arrays work like in a real analog clock
        float Hours = fmod(((float)calendar_time.tm_hour + Minutes / 60.0), 12.0);

        // Background Color
        SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
        //SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

        // -- Clock board -- //
        for (int i = 0; i < ClockBoardCorners; i++)
        {
            SDL_Vertex vertex_1 = {{(float)(WIDTH / 2.0), (float)(HEIGHT / 2.0)}, {255, 255, 255, 255}, {1, 1}};
            SDL_Vertex vertex_2 = {{(float)(WIDTH / 2.0 + (cos(PI * 2.0 / ClockBoardCorners * i)) * 1.05 * ClockSize), (float)(HEIGHT / 2.0 + (sin(PI * 2.0 / ClockBoardCorners * i)) * 1.05 * ClockSize)}, {255, 255, 255, 255}, {1, 1}};
            SDL_Vertex vertex_3 = {{(float)(WIDTH / 2.0 + (cos(PI * 2.0 / ClockBoardCorners * (i + 1))) * 1.05 * ClockSize), (float)(HEIGHT / 2.0 + (sin(PI * 2.0 / ClockBoardCorners * (i + 1))) * 1.05 * ClockSize)}, {255, 255, 255, 255}, {1, 1}};

            SDL_Vertex vertices[] = {
                vertex_1,
                vertex_2,
                vertex_3
            };

            SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
        }

        // -- Marks -- //
        for (int i = 0; i < 60; i ++)
        {
            float angle = fmod(PI * 2.0 / 60 * i, PI * 2.0);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

            if(i % 5 == 0)
            {
                SDL_RenderDrawLine(renderer, WIDTH / 2.0 + cos(angle) * 0.92 * ClockSize, HEIGHT / 2.0 + sin(angle) * 0.92 * ClockSize, WIDTH / 2.0 + cos(angle) * ClockSize, HEIGHT / 2.0 + sin(angle) * ClockSize);
            }
            SDL_RenderDrawLine(renderer, WIDTH / 2.0 + cos(angle) * 0.95 * ClockSize, HEIGHT / 2.0 + sin(angle) * 0.95 * ClockSize, WIDTH / 2.0 + cos(angle) * ClockSize, HEIGHT / 2.0 + sin(angle) * ClockSize);
        }

        // -- Arrows -- //
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, WIDTH / 2, HEIGHT / 2, WIDTH / 2.0 + cos(fmod((Hours + 9.0) / 12.0 * PI * 2.0, PI * 2.0)) * 0.6 * ClockSize, HEIGHT / 2.0 + sin(fmod((Hours + 9.0) / 12.0 * PI * 2.0, PI * 2.0)) * 0.6 * ClockSize);

        SDL_RenderDrawLine(renderer, WIDTH / 2, HEIGHT / 2, WIDTH / 2.0 + cos(fmod((Minutes + 45.0) / 60.0 * PI * 2.0, PI * 2.0)) * 0.80 * ClockSize, HEIGHT / 2.0 + sin(fmod((Minutes + 45.0) / 60.0 * PI * 2.0, PI * 2.0)) * 0.80 * ClockSize);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, WIDTH / 2, HEIGHT / 2, WIDTH / 2.0 + cos(fmod((Seconds + 45.0) / 60.0 * PI * 2.0, PI * 2.0)) * 0.90 * ClockSize, HEIGHT / 2.0 + sin(fmod((Seconds + 45.0) / 60.0 * PI * 2.0, PI * 2.0)) * 0.90 * ClockSize);
        for (int i = 0; i < ClockBoardCorners; i++)
        {
            SDL_Vertex vertex_1 = {{(float)(WIDTH / 2.0), (float)(HEIGHT / 2.0)}, {50, 50, 50, 0}, {1, 1}};
            SDL_Vertex vertex_2 = {{(float)(WIDTH / 2.0 + (cos(PI * 2.0 / ClockBoardCorners * i)) * 5), (float)(HEIGHT / 2.0 + (sin(PI * 2.0 / ClockBoardCorners * i)) * 5)}, {50, 50, 50, 255}, {1, 1}};
            SDL_Vertex vertex_3 = {{(float)(WIDTH / 2.0 + (cos(PI * 2.0 / ClockBoardCorners * (i + 1))) * 5), (float)(HEIGHT / 2.0 + (sin(PI * 2.0 / ClockBoardCorners * (i + 1))) * 5)}, {50, 50, 50, 255}, {1, 1}};

            SDL_Vertex vertices[] = {
                vertex_1,
                vertex_2,
                vertex_3};

            SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 1;
}