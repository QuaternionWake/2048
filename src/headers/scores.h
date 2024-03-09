#ifndef SCORES_H
#define SCORES_H

#include "qw_utils.h"

void openScoresFile();
void addScore(int playfieldSize, int score);
void getScores();
int getBestScore(int playfieldSize);
void initScoreSystem();

#endif
