#ifndef GAME_H
#define GAME_H

#include "2048-types.h"

qw_pos randPos(int size, int playfield[size][size]);
void copyPlayfield(int size, int playfield[size][size], int copiedPlayfield[size][size]);
void rotateGrid(int size, int playfield[size][size], int rotationsTotal);
int moveGrid(int size, int playfield[size][size]);
int gameOverCheck(int size, int playfield[size][size]);
void mainGameLoop(int size, int playfield[size][size]);
void initializeGame(int size, int tileSize);

#endif
