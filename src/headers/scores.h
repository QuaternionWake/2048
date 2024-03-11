#ifndef SCORES_H
#define SCORES_H

#include "qw_utils.h"

void initScoreSystem();
void openScoresFile();
void getScoresFromFile();
void deinitScoreSystem();
void addScore(int playfieldSize, int score);
int getBestScore(int playfieldSize);

#endif
