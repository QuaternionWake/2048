#include "headers/game.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "headers/get-input.h"
#include "headers/playfield-prerendering.h"
#include "headers/2048-types.h"
#include "headers/qw_utils.h"
#include "headers/scores.h"

qw_vec2 randVec2(int size, int playfield[size][size]) {
    qw_vec2 a;
    do {
        a.x = rand() % size;
        a.y = rand() % size;
    } while (playfield[a.y][a.x]);
    return a;
}

void copyPlayfield(int size, int source[size][size], int dest[size][size]) {
    int i, j;
    for (i=0; i<size; i++)
        for (j=0; j<size; j++)
            dest[i][j] = source[i][j];
}

void rotateGrid(int size, int playfield[size][size], int rotationsTotal) {
    int rotatedPlayfield[size][size];
    int i, j;
    int rotationsDone;
    for (rotationsDone=0; rotationsDone<rotationsTotal; rotationsDone++) {
        for (i=0; i<size; i++)
            for (j=0; j<size; j++)
                rotatedPlayfield[j][size-i-1] = playfield[i][j];
        copyPlayfield(size, rotatedPlayfield, playfield);
    }
}

int moveGrid(int size, int playfield[size][size], int *score) {
    int currentX, nextFaller, fallHeight;
    int notMoved = 1;

    //merging tiles
    for (currentX=0; currentX<size; currentX++) {
        fallHeight = 0;
        while (!playfield[fallHeight][currentX] && fallHeight<size)
            fallHeight++;
        if (fallHeight >= size-1)
            continue;
        for (nextFaller=fallHeight+1; nextFaller<size; nextFaller++)
            if (playfield[nextFaller][currentX]) {
                if (playfield[fallHeight][currentX] == playfield[nextFaller][currentX]) {
                    *score += powOfTwo(playfield[fallHeight][currentX]);
                    playfield[fallHeight][currentX]++;
                    playfield[nextFaller][currentX] = 0;
                    fallHeight = nextFaller+1;
                    nextFaller++;
                    notMoved = 0;
                } else {
                    fallHeight = nextFaller;
                }
            }
    }

    //shifting tiles
    for (currentX=0; currentX<size; currentX++) {
        fallHeight = 0;
        while (playfield[fallHeight][currentX] && fallHeight<size)
            fallHeight++;
        if (fallHeight >= size-1)
            continue;
        for (nextFaller=fallHeight+1; nextFaller<size; nextFaller++)
            if (playfield[nextFaller][currentX]) {
                playfield[fallHeight][currentX] = playfield[nextFaller][currentX];
                playfield[nextFaller][currentX] = 0;
                fallHeight++;
                notMoved = 0;
            }
    }

    return notMoved;
}

int gameOverCheck(int size, int playfield[size][size]) {
    int testPlayfield[size][size];
    int gameOver = 1;
    int dummyScore;
    copyPlayfield (size, playfield, testPlayfield);
    gameOver = moveGrid(size, testPlayfield, &dummyScore); if (!gameOver) return 0;
    rotateGrid(size, testPlayfield, 1); gameOver = moveGrid(size, testPlayfield, &dummyScore); if (!gameOver) return 0;
    rotateGrid(size, testPlayfield, 1); gameOver = moveGrid(size, testPlayfield, &dummyScore); if (!gameOver) return 0;
    rotateGrid(size, testPlayfield, 1); gameOver = moveGrid(size, testPlayfield, &dummyScore); if (!gameOver) return 0;
    return 1;
}

void mainGameLoop(int size, int playfield[size][size]) {
    int invalidInput;
    int score = 0;

    while (1) {
        do {
            qw_input move = getInput();
            invalidInput = 0;
            switch (move) { //non-up moves are transformed into up moves by rotating the playfield
            case INPUT_EXIT:    goto Exit;
            case INPUT_UP:      invalidInput = moveGrid(size, playfield, &score);   break;
            case INPUT_LEFT:    rotateGrid(size, playfield, 1); invalidInput = moveGrid(size, playfield, &score); rotateGrid(size, playfield, 3);   break;
            case INPUT_DOWN:    rotateGrid(size, playfield, 2); invalidInput = moveGrid(size, playfield, &score); rotateGrid(size, playfield, 2);   break;
            case INPUT_RIGHT:   rotateGrid(size, playfield, 3); invalidInput = moveGrid(size, playfield, &score); rotateGrid(size, playfield, 1);   break;
            default: invalidInput = 1;
            }
        } while (invalidInput);

        prerenderField(size, playfield, score, 0);

        qw_vec2 a = randVec2(size, playfield);
        if (rand()%5) //chance a tile spawns as a 4 rather than a 2
            playfield[a.y][a.x] = 1;
        else
            playfield[a.y][a.x] = 2;

        int gameOver = gameOverCheck(size, playfield);

        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 100*1000*1000;
        nanosleep(&ts, NULL);
        prerenderField(size, playfield, score, gameOver);

        if (gameOver){
            while (getInput() == NO_INPUT);
            prerenderField(size, playfield, score, 0);
            while (getInput() == NO_INPUT);
            break;
        }
    }
    Exit:
    addScore(size, score);
    resetPlayfieldRenderngGlobals(size);
}

void initializeGame(int size, int tileSize) {
    int playfield[size][size];
    int i, j;
    for (i=0; i<size; i++)
        for (j=0; j<size; j++)
            playfield[i][j] = 0;

    srand(time(NULL));
    qw_vec2 a;
    a = randVec2(size, playfield); //starting two tiles
    playfield[a.y][a.x] = 1;
    a = randVec2(size, playfield);
    playfield[a.y][a.x] = 1;

    initilizePlayfieldRenderingGlobals(size, tileSize);
    prerenderField(size, playfield, 0, 0);
    mainGameLoop(size, playfield);
}
