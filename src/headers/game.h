#ifndef GAME_H
#define GAME_H

#include "2048-types.h"

void initializeGame(int size, int tileSize);
void mainGameLoop(int size, int playfield[size][size]);
void rotateGrid(int size, int playfield[size][size], int rotationsTotal);
int moveGrid(int size, int playfield[size][size], int *score);
int gameOverCheck(int size, int playfield[size][size]);
void copyPlayfield(int size, int playfield[size][size], int copiedPlayfield[size][size]);
qw_vec2 randVec2(int size, int playfield[size][size]);

#endif
