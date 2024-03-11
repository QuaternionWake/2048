#include "headers/scores.h"

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "headers/qw_utils.h"

FILE *scoresFile = NULL;
qw_list scoreLists[11];

void initScoreSystem() {
    int i;
    openScoresFile();
    for(i=2; i<=10; i++){
        scoreLists[i] = newList();
    }
    getScoresFromFile();
}

void openScoresFile() {
    char scoreFilePath[PATH_MAX];
    strcpy(scoreFilePath, getenv("HOME"));
    strcat(scoreFilePath, "/.config/QuaternionWake");
    mkdir(scoreFilePath, 0777);
    strcat(scoreFilePath, "/2048");
    mkdir(scoreFilePath, 0777);
    strcat(scoreFilePath, "/scores");
    scoresFile = fopen(scoreFilePath, "a+");
    if (scoresFile == NULL)
        exit(EXIT_FAILURE);
}

void getScoresFromFile() {
    rewind(scoresFile);
    while (1) {
        int currSize, currScore;
        if (fscanf(scoresFile, "%d %d\n", &currSize, &currScore) == EOF)
            break;
        addToList(&scoreLists[currSize], currScore);
    }
}

void deinitScoreSystem() {
    fclose(scoresFile);
}

void addScore(int playfieldSize, int score) {
    addToList(&scoreLists[playfieldSize], score);
    fprintf(scoresFile, "%d %d\n", playfieldSize, score);
    fflush(scoresFile);
}

int getBestScore(int playfieldSize) {
    int i;
    int bestScore = 0;
    for (i=0; i<scoreLists[playfieldSize].size; i++)
        bestScore = max(scoreLists[playfieldSize].values[i], bestScore);
    return bestScore;
}
