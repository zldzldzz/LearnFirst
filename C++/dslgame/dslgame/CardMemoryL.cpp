#include "CardMemoryL.h"
#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <stdlib.h>
#include <algorithm> 

int hint = 0;     
int wrongCount[4] = { 0 }; 

CardMemory::CardMemory() : texture(nullptr), cardValue(0), isRevealed(false) {
    rect = { 0, 0, 0, 0 };
}

CardMemory::~CardMemory() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void shuffleArray(int* array, int size) {
    for (int i = size - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(array[i], array[j]);
    }
}

void flipCardPositionsHorizontally(int cardCount, CardMemory* cards) {
    printf("반전\n");
    for (int i = 0; i < cardCount; i++) {
        cards[i].rect.x = WIDTH - cards[i].rect.x - cards[i].rect.w;
    }
}

int initializeCards(int cardCount, int* cardValues, CardMemory* cards, SDL_Renderer* renderer, char cardImage[][16]) {
    SDL_Surface* imageSurface[16];
    int positions[16];
    int currentCardOffset = 0; // targetCount를 계산하기 위한 변수

    for (int i = 0; i < cardCount; i++) {
        positions[i] = i;
        cards[i].isRevealed = false;
    }
    shuffleArray(positions, cardCount);

    for (int i = 0; i < cardCount; i++) {
        imageSurface[i] = IMG_Load(cardImage[cardValues[i]]);
        if (!imageSurface[i]) {
            printf("Image Load Fail for %s: %s\n", cardImage[cardValues[i]], IMG_GetError());
            SDL_Quit();
            exit(1);
        }

        int newWidth = 180;
        int newHeight = 180;

        cards[i].texture = SDL_CreateTextureFromSurface(renderer, imageSurface[i]);

        SDL_Rect cardRect = {
            (positions[i] % 4) * (newWidth + 10) + 10,
            (positions[i] / 4) * (newHeight + 10) + 10,
            newWidth,
            newHeight };
        cards[i].rect = cardRect;
        cards[i].cardValue = cardValues[i];

        if (cardValues[i] == 14) {
            currentCardOffset++;
        }if (cardValues[i] == 15) {
            currentCardOffset++;
        }


        SDL_FreeSurface(imageSurface[i]);
    }

    return cardCount - currentCardOffset; // targetCount 반환
}

void renderBackFaces(int cardCount, CardMemory* cards, SDL_Renderer* renderer, SDL_Texture* backTexture) {
    for (int i = 0; i < cardCount; i++) {
        if (!cards[i].isRevealed) {
            SDL_RenderCopy(renderer, backTexture, NULL, &cards[i].rect);
        }
        else {
            SDL_RenderCopy(renderer, cards[i].texture, NULL, &cards[i].rect);
        }
    }
    SDL_RenderPresent(renderer);
}

void showCorrectCards(int cardCount, CardMemory* cards, SDL_Renderer* renderer, int time = 2) {
    hint++;
    for (int i = 0; i < cardCount; i++) {
        SDL_RenderCopy(renderer, cards[i].texture, NULL, &cards[i].rect);
    }
    printf("\n힌트 %d초\n", time);
    SDL_RenderPresent(renderer);
    SDL_Delay(time * 1000);

    renderBackFaces(cardCount, cards, renderer, nullptr);
}

void renderAllBack(int cardCount, CardMemory* cards, SDL_Renderer* renderer, SDL_Texture* backTexture) {
    for (int i = 0; i < cardCount; i++) {
        SDL_RenderCopy(renderer, backTexture, NULL, &cards[i].rect);
    }
    SDL_RenderPresent(renderer);
}

int playRound(int cardCount, int targetCount, int* cardValues, CardMemory* cards, SDL_Renderer* renderer, SDL_Texture* backTexture, int stage) {
    SDL_Event event;
    int currentCard = 0;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return -1;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return -1;
                }
                if (event.key.keysym.sym == SDLK_h) {
                    showCorrectCards(cardCount, cards, renderer);
                    renderBackFaces(cardCount, cards, renderer, backTexture);
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                for (int i = 0; i < cardCount; i++) {
                    if (x >= cards[i].rect.x && x <= cards[i].rect.x + cards[i].rect.w &&
                        y >= cards[i].rect.y && y <= cards[i].rect.y + cards[i].rect.h) {

                        if (cards[i].cardValue == 14) {
                            printf("좌우 대칭!\n");
                            renderAllBack(cardCount, cards, renderer, backTexture);
                            flipCardPositionsHorizontally(cardCount, cards);
                            renderBackFaces(cardCount, cards, renderer, backTexture);
                            break;
                        }
                        if (cards[i].cardValue == 15) {
                            wrongCount[stage]++;
                            printf("틀린 횟수: %d\n", wrongCount[stage]);
                            SDL_RenderCopy(renderer, cards[i].texture, NULL, &cards[i].rect);
                            SDL_RenderPresent(renderer);
                            SDL_Delay(350);
                            renderBackFaces(cardCount, cards, renderer, backTexture);
                            break;
                        }

                        if (cards[i].cardValue == cardValues[currentCard] && !cards[i].isRevealed) {
                            cards[i].isRevealed = true;
                            SDL_RenderCopy(renderer, cards[i].texture, NULL, &cards[i].rect);
                            SDL_RenderPresent(renderer);
                            currentCard++;

                            if (currentCard == targetCount) {
                                SDL_Delay(1000);
                                renderAllBack(cardCount, cards, renderer, backTexture);
                                return 1;
                            }
                        }
                        else if (cards[i].cardValue != 14 && cards[i].cardValue != 15) {
                            wrongCount[stage]++;
                            printf("틀린 횟수: %d\n", wrongCount[stage]);
                            SDL_RenderCopy(renderer, cards[i].texture, NULL, &cards[i].rect);
                            SDL_RenderPresent(renderer);
                            SDL_Delay(350);
                            renderBackFaces(cardCount, cards, renderer, backTexture);

                            if ((wrongCount[stage] % 5) == 0) {
                                printf("h키를 누르면 힌트가 나와요\n");
                                showCorrectCards(cardCount, cards, renderer, (wrongCount[stage] / 5));
                                renderBackFaces(cardCount, cards, renderer, backTexture);
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    return 0;
}

void playCardMemory(SDL_Renderer* renderer) {
    char cardImage[16][16] = {
        "imgs/free01.png", "imgs/free02.png", "imgs/free03.png", "imgs/free04.png",
        "imgs/free05.png", "imgs/free06.png", "imgs/free07.png", "imgs/free08.png",
        "imgs/free09.png", "imgs/free10.png", "imgs/free11.png", "imgs/free12.png",
        "imgs/free13.png", "imgs/free14.png", "imgs/free15.png","imgs/free16.png"
    };

    srand(time(NULL));
    SDL_Surface* backSurface = IMG_Load("imgs/free00.png");
    SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);
    SDL_FreeSurface(backSurface);

    int cardValues[16];
    cardValues[0] = 14;
    for (int i = 1; i < 16; i++) {
        cardValues[i] = rand() % 15;
    }
    std::sort(cardValues, cardValues + 16);

    int stages[4] = { 4, 6, 9, 16 };
    CardMemory cards[16];

    for (int stage = 0; stage < 4; stage++) {
        int cardCount = stages[stage];
        int targetCount = initializeCards(cardCount, cardValues, cards, renderer, cardImage);

        for (int i = 0; i < cardCount; i++) {
            SDL_RenderCopy(renderer, cards[i].texture, NULL, &cards[i].rect);
        }
        SDL_RenderPresent(renderer);
        if (stage==0) {
            SDL_Delay((stage + 1) * 2000);

        }
        SDL_Delay((stage + 1) * 2000);

        renderBackFaces(cardCount, cards, renderer, backTexture);

        if (playRound(cardCount, targetCount, cardValues, cards, renderer, backTexture, stage) == -1) {
            break;
        }
    }

    SDL_DestroyTexture(backTexture);
    for (int i = 0; i < 16; i++) {
        if (cards[i].texture != NULL) {
            SDL_DestroyTexture(cards[i].texture);
        }
    }

    printf("\n1라운드 틀린 횟수:%d\n", wrongCount[0]);
    printf("2라운드 틀린 횟수:%d\n", wrongCount[1]);
    printf("3라운드 틀린 횟수:%d\n", wrongCount[2]);
    printf("4라운드 틀린 횟수:%d\n", wrongCount[3]);
    printf("전체 힌트 사용 횟수:%d\n", hint);
}