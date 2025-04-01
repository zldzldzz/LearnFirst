#include "CardMemoryL.h"
#include "roulette.h"
#include <iostream>
#include <SDL.h>

#define WIDTH 770
#define HEIGHT 780

void runCardMemory(SDL_Renderer* renderer) {
    playCardMemory(renderer);
    std::cout << "CardMemory 게임 종료" << std::endl;
}

void runRoulette(SDL_Renderer* renderer) {
    playRoulette(renderer);
    std::cout << "Roulette 게임 종료" << std::endl;
}

int main(int argc, char* argv[]) {


    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL Initialization Fail: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "SDL Window Creation Fail: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "SDL Renderer Creation Fail: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    while (running) {
        std::cout << "게임을 선택하세요:\n1. CardMemory\n2. Roulette\n0. 종료" << std::endl;
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            runCardMemory(renderer);
            break;
        case 2:
            runRoulette(renderer);
            break;
        case 0:
            running = false;
            break;
        default:
            std::cout << "잘못된 선택입니다." << std::endl;
            break;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
