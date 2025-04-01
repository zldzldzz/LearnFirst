#ifndef CARDMEMORY_H
#define CARDMEMORY_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#define WIDTH 770
#define HEIGHT 780

extern int hint;          // hint ������ �ܺο��� ���� �����ϰ� ����
extern int wrongCount[4]; // wrongCount ������ �ܺο��� ���� �����ϰ� ����

// CardMemory Ŭ����
class CardMemory {
public:
    SDL_Rect rect;        // ī�� ��ġ, ũ�� ����
    SDL_Texture* texture; // ī���� �޸� Ȥ�� �ո�
    int cardValue;        // ī���� ���� ��
    bool isRevealed;      // ī�尡 �����Ǿ����� ����

    CardMemory();         // ������
    ~CardMemory();        // �Ҹ���
};

// �Լ� ����
void shuffleArray(int* array, int size); // �迭 ����
void flipCardPositionsHorizontally(int cardCount, CardMemory* cards); // ī�� �¿� ��Ī ��ȯ
int initializeCards(int cardCount, int* cardValues, CardMemory* cards, SDL_Renderer* renderer, char cardImage[][16]); // ī�� �ʱ�ȭ
void renderBackFaces(int cardCount, CardMemory* cards, SDL_Renderer* renderer, SDL_Texture* backTexture); // ī�� �޸� ������
void showCorrectCards(int cardCount, CardMemory* cards, SDL_Renderer* renderer, int time); // ���� ī�� ��Ʈ ǥ��
void renderAllBack(int cardCount, CardMemory* cards, SDL_Renderer* renderer, SDL_Texture* backTexture); // ��� ī�� �޸� ������
int playRound(int cardCount, int targetCount, int* cardValues, CardMemory* cards, SDL_Renderer* renderer, SDL_Texture* backTexture, int stage); // ���� ����

void playCardMemory(SDL_Renderer* renderer);

#endif // CARDMEMORYL_H
