#include "headers/game.h"

#include <stdlib.h>
#include <time.h>
#include "headers/get-input.h"
#include "headers/playfield-prerendering.h"
#include "headers/2048-types.h"

qw_pos randPos(int size, int playfield[size][size]){
    qw_pos a;
    do{
        a.x = rand() % size;
        a.y = rand() % size;
    }while(playfield[a.y][a.x]);
    return a;
}

void copyPlayfield(int size, int source[size][size], int dest[size][size]){
    int i, j;
    for(i=0; i<size; i++)
        for(j=0; j<size; j++)
            dest[i][j] = source[i][j];
}

void rotateGrid(int size, int playfield[size][size], int rotationsTotal){
    int rotatedPlayfield[size][size];
    int i, j;
    int rotationsDone;
    for(rotationsDone=0; rotationsDone<rotationsTotal; rotationsDone++){
        for(i=0; i<size; i++)
            for(j=0; j<size; j++)
                rotatedPlayfield[j][size-i-1] = playfield[i][j];
        copyPlayfield(size, rotatedPlayfield, playfield);
    }
}

int moveGrid(int size, int playfield[size][size]){
    int currentX, nextFaller, fallHeight;
    int notMoved = 1;

    for(currentX=0; currentX<size; currentX++){
        fallHeight = 0;
        while(!playfield[fallHeight][currentX] && fallHeight<size)
            fallHeight++;
        if(fallHeight >= size-1)
            continue;
        for(nextFaller=fallHeight+1; nextFaller<size; nextFaller++)
            if(playfield[nextFaller][currentX]){
                if(playfield[fallHeight][currentX] == playfield[nextFaller][currentX]){
                    playfield[fallHeight][currentX]++;
                    playfield[nextFaller][currentX] = 0;
                    fallHeight = nextFaller+1;
                    nextFaller++;
                    notMoved = 0;
                }else
                    fallHeight = nextFaller;
        }
    }

    for(currentX=0; currentX<size; currentX++){
        fallHeight = 0;
        while(playfield[fallHeight][currentX] && fallHeight<size)
            fallHeight++;
        if(fallHeight >= size-1)
            continue;
        for(nextFaller=fallHeight+1; nextFaller<size; nextFaller++)
            if(playfield[nextFaller][currentX]){
                playfield[fallHeight][currentX] = playfield[nextFaller][currentX];
                playfield[nextFaller][currentX] = 0;
                fallHeight++;
                notMoved = 0;
            }
    }

    return notMoved;
}

int gameOverCheck(int size, int playfield[size][size]){
    int testPlayfield[size][size];
    int gameOver = 1;
    copyPlayfield(size, playfield, testPlayfield);
    gameOver = moveGrid(size, testPlayfield); if(!gameOver) return 1;
    rotateGrid(size, testPlayfield, 1); gameOver = moveGrid(size, testPlayfield); rotateGrid(size, testPlayfield, 3); if(!gameOver) return 1;
    rotateGrid(size, testPlayfield, 2); gameOver = moveGrid(size, testPlayfield); rotateGrid(size, testPlayfield, 2); if(!gameOver) return 1;
    rotateGrid(size, testPlayfield, 3); gameOver = moveGrid(size, testPlayfield); rotateGrid(size, testPlayfield, 1); if(!gameOver) return 1;
    return 0;
}

void mainGameLoop(int size, int playfield[size][size]){
    int invalidInput;
    int ongoing = 1;
    int exit = 0;

    while(ongoing){
        do{
            qw_input move = getInput();
            invalidInput = 0;
            switch(move){ //non-up moves are transformed into up moves by rotating the playfield
                case INPUT_EXIT:    exit = 1; break;
                case INPUT_UP:      invalidInput = moveGrid(size, playfield);   break;
                case INPUT_LEFT:    rotateGrid(size, playfield, 1); invalidInput = moveGrid(size, playfield); rotateGrid(size, playfield, 3);   break;
                case INPUT_DOWN:    rotateGrid(size, playfield, 2); invalidInput = moveGrid(size, playfield); rotateGrid(size, playfield, 2);   break;
                case INPUT_RIGHT:   rotateGrid(size, playfield, 3); invalidInput = moveGrid(size, playfield); rotateGrid(size, playfield, 1);   break;
                default: invalidInput = 1;
            }
        }while(invalidInput);
        if(exit)
            break;

        prerenderField(size, playfield);

        qw_pos a = randPos(size, playfield);
        if(rand()%5) //chance a tile spawns as a 4 rather than a 2
            playfield[a.y][a.x] = 1;
        else
            playfield[a.y][a.x] = 2;

        ongoing = gameOverCheck(size, playfield);

        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 100*1000*1000;
        nanosleep(&ts, NULL);
        prerenderField(size, playfield);
    }
    resetPlayfieldRenderngGlobals(size);
}

void initializeGame(int size, int tileSize){
    int playfield[size][size];
    int i, j;
    for(i=0; i<size; i++)
        for(j=0; j<size; j++)
            playfield[i][j] = 0;

    srand(time(NULL));
    qw_pos a;
    a = randPos(size, playfield); //starting two tiles
    playfield[a.y][a.x] = 1;
    a = randPos(size, playfield);
    playfield[a.y][a.x] = 1;

    initilizePlayfieldRenderingGlobals(size, tileSize);
    prerenderField(size, playfield);
    mainGameLoop(size, playfield);
}
