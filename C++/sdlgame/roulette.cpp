#include "roulette.h"
#include <SDL.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

void playRoulette(SDL_Renderer* renderer) {
    srand(time(NULL));

    const char* imagePaths[14] = {
        "imgs/free01.png", "imgs/free02.png", "imgs/free03.png", "imgs/free04.png",
        "imgs/free05.png", "imgs/free06.png", "imgs/free07.png", "imgs/free08.png",
        "imgs/free09.png", "imgs/free10.png", "imgs/free11.png", "imgs/free12.png",
        "imgs/free13.png", "imgs/free14.png"
    };

    SDL_Texture* images[14];
    for (int i = 0; i < 14; i++) {
        SDL_Surface* surface = IMG_Load(imagePaths[i]);
        images[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    SDL_Rect rects[3] = {
        { 100, 200, 150, 150 },
        { 300, 200, 150, 150 },
        { 500, 200, 150, 150 }
    };

    int indices[3] = { rand() % 14, rand() % 14, rand() % 14 };
    bool stopped[3] = { false, false, false };
    int stopCount = 0;

    bool running = true;
    Uint32 times[3] = { SDL_GetTicks(), SDL_GetTicks(), SDL_GetTicks() };

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                    break;
                }
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                if (stopCount < 3) {
                    stopped[stopCount] = true;
                    stopCount++;

                    if (stopCount == 2 && indices[0] != indices[1]) {
                        SDL_Delay(2000);
                        std::cout << "이걸 틀리누\n";
                        stopped[1] = false;
                        stopCount--;
                    }

                    if (stopCount == 3 && (indices[0] != indices[1] || indices[1] != indices[2])) {
                        SDL_Delay(2000);
                        std::cout << "이걸 틀리누\n";
                        stopped[2] = false;
                        stopCount = 0;
                    }

                    if (stopCount == 3 && indices[0] == indices[1] && indices[1] == indices[2]) {
                        std::cout << "승리! 모든 이미지가 일치합니다!" << std::endl;
                        running = false;
                    }
                }
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        if (!stopped[0] && currentTime > times[0] + 1000) {
            indices[0] = rand() % 14;
            times[0] = currentTime;
        }
        if (!stopped[1] && currentTime > times[1] + 800) {
            indices[1] = rand() % 14;
            times[1] = currentTime;
        }
        if (!stopped[2] && currentTime > times[2] + 700) {
            indices[2] = rand() % 14;
            times[2] = currentTime;
        }

        SDL_RenderClear(renderer);
        for (int i = 0; i < 3; i++) {
            SDL_RenderCopy(renderer, images[indices[i]], NULL, &rects[i]);
        }
        SDL_RenderPresent(renderer);
    }

    for (int i = 0; i < 14; i++) {
        SDL_DestroyTexture(images[i]);
    }
}
