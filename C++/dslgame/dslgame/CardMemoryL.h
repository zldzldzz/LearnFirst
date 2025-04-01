#ifndef CARDMEMORY_H
#define CARDMEMORY_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#define WIDTH 770
#define HEIGHT 780

extern int hint;          // hint 변수를 외부에서 참조 가능하게 선언
extern int wrongCount[4]; // wrongCount 변수를 외부에서 참조 가능하게 선언

// CardMemory 클래스
class CardMemory {
public:
    SDL_Rect rect;        // 카드 위치, 크기 지정
    SDL_Texture* texture; // 카드의 뒷면 혹은 앞면
    int cardValue;        // 카드의 고유 값
    bool isRevealed;      // 카드가 공개되었는지 여부

    CardMemory();         // 생성자
    ~CardMemory();        // 소멸자
};

// 함수 선언
void shuffleArray(int* array, int size); // 배열 섞기
void flipCardPositionsHorizontally(int cardCount, CardMemory* cards); // 카드 좌우 대칭 변환
int initializeCards(int cardCount, int* cardValues, CardMemory* cards, SDL_Renderer* renderer, char cardImage[][16]); // 카드 초기화
void renderBackFaces(int cardCount, CardMemory* cards, SDL_Renderer* renderer, SDL_Texture* backTexture); // 카드 뒷면 렌더링
void showCorrectCards(int cardCount, CardMemory* cards, SDL_Renderer* renderer, int time); // 정답 카드 힌트 표시
void renderAllBack(int cardCount, CardMemory* cards, SDL_Renderer* renderer, SDL_Texture* backTexture); // 모든 카드 뒷면 렌더링
int playRound(int cardCount, int targetCount, int* cardValues, CardMemory* cards, SDL_Renderer* renderer, SDL_Texture* backTexture, int stage); // 라운드 실행

void playCardMemory(SDL_Renderer* renderer);

#endif // CARDMEMORYL_H
