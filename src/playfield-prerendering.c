#include "headers/playfield-prerendering.h"

#include <stdio.h>
#include <stdlib.h>
#include "headers/rendering.h"
#include "headers/2048-types.h"
#include "headers/qw_utils.h"

    //TODO replace this with hadrcoded strings
void drawTile(qw_displayElement *tile, int tileContentNum) {
    int i, j;
    for (i=0; i<tile->size.y; i++)
        for (j=0; j<tile->size.x; j++)
            tile->contents[i][j] = ' ';

    if (!tileContentNum)
        return;

    int digitCount = numLen(powOfTwo(tileContentNum));
    int rows = digitCount/tile->size.x + !!(digitCount%tile->size.x);
    int partialRow = digitCount%tile->size.x;
    char tileContentStr[digitCount+1];
    sprintf(tileContentStr, "%d", powOfTwo(tileContentNum));
    qw_vec2 printStart = {.x=(tile->size.x-partialRow)/2,
                         .y=(tile->size.y-rows)/2};

    int digit = 0;
    if (partialRow) {
        for (i=printStart.x; digit<partialRow; i++)
            tile->contents[printStart.y][i] = tileContentStr[digit++];
        printStart.y++;
    }

    for (i=printStart.y; digit<digitCount; i++)
        for (j=0; j<tile->size.x; j++)
            tile->contents[i][j] = tileContentStr[digit++];
}

void prerenderField(int gridSize, int playfield[gridSize][gridSize], int score, char gameOver) {
    int i, j;
    extern qw_displayElement playfieldBackground;
    extern qw_displayElement **playfieldTiles;
    extern qw_displayElement infoPad;
    extern qw_displayElement infoPadArt;
    extern qw_displayElement scoreDisplay;
    extern qw_displayElement gameOverText;

    for (i=0; i<gridSize; i++)
        for (j=0; j<gridSize; j++)
            drawTile(&playfieldTiles[i][j], playfield[i][j]);

    //score display
    //TODO add max score
    {
        char scoreStr[scoreDisplay.size.x+1];
        sprintf(scoreStr, "score: %d", score);
        sprintf(scoreStr, "%*s", -scoreDisplay.size.x, scoreStr);
        char *contents[] = {scoreStr};
        fillElement(scoreDisplay, contents);
    }

    gameOverText.render = gameOver;

    extern qw_vec2 single;
    qw_vec2 blueprint[6] = {single, {.x=gridSize, .y=gridSize}, single, single, single, single};
    renderScreen(blueprint, 6, &playfieldBackground, playfieldTiles, &infoPad, &infoPadArt, &scoreDisplay, &gameOverText);
}

void initilizePlayfieldRenderingGlobals(int gridSize, int tileSizeUnadjusted) {
    int i, j;
    qw_vec2 tileSize = {.x = tileSizeUnadjusted*2+1,
                       .y = tileSizeUnadjusted+1};

    extern qw_displayElement playfieldBackground;
        playfieldBackground.relativePos.x = 0;
        playfieldBackground.relativePos.y = 0;
        playfieldBackground.size.x = gridSize*tileSize.x+1;
        playfieldBackground.size.y = gridSize*tileSize.y+1;
        playfieldBackground.vaguePos = ML;
        playfieldBackground.render = 1;
        playfieldBackground.relativeTo = NULL;
        allocateElement(&playfieldBackground);

    for (i=0; i<playfieldBackground.size.y; i++)
        for (j=0; j<playfieldBackground.size.x; j++)
            playfieldBackground.contents[i][j] = ' ';

    for (i=0; i<playfieldBackground.size.y; i+=tileSize.y)
        for (j=0; j<playfieldBackground.size.x; j++)
            playfieldBackground.contents[i][j] = '-';

    for (i=0; i<playfieldBackground.size.y; i++)
        for (j=0; j<playfieldBackground.size.x; j+=tileSize.x)
            playfieldBackground.contents[i][j] = '|';

    for (i=0; i<playfieldBackground.size.y; i+=tileSize.y)
        for (j=0; j<playfieldBackground.size.x; j+=tileSize.x)
            playfieldBackground.contents[i][j] = '+';

    extern qw_displayElement **playfieldTiles;
    playfieldTiles = (qw_displayElement**)malloc(gridSize*sizeof(qw_displayElement*));
    for (i=0; i<gridSize; i++)
        playfieldTiles[i] = (qw_displayElement*)malloc(gridSize*sizeof(qw_displayElement));

    for (i=0; i<gridSize; i++)
        for (j=0; j<gridSize; j++) {
            playfieldTiles[i][j].relativePos.x = j*tileSize.x + 2;
            playfieldTiles[i][j].relativePos.y = i*tileSize.y + 2;
            playfieldTiles[i][j].size.x = tileSize.x-3;
            playfieldTiles[i][j].size.y = tileSize.y-3;
            playfieldTiles[i][j].vaguePos = ITL;
            playfieldTiles[i][j].render = 1;
            playfieldTiles[i][j].relativeTo = &playfieldBackground;
            allocateElement(&playfieldTiles[i][j]);
        }

    extern qw_displayElement infoPad;
        infoPad.relativePos.x = 1;
        infoPad.relativePos.y = 0;
        infoPad.size.x = 40 ; //TODO
        infoPad.size.y = 20; //TODO
        infoPad.vaguePos = RM;
        infoPad.render = 0;
        infoPad.relativeTo = &playfieldBackground;

    extern qw_displayElement infoPadArt;
        infoPadArt.relativePos.x = 0;
        infoPadArt.relativePos.y = 0;
        infoPadArt.size.x = 24;
        infoPadArt.size.y = 6;
        infoPadArt.vaguePos = ITR;
        infoPadArt.render = 1;
        infoPadArt.relativeTo = &infoPad;
        allocateElement(&infoPadArt);
        {
            char *contents[] =
                {" ___   ___  _  _   ___  ",
                 "|__ \\ / _ \\| || | / _ \\ ",
                 "   ) | | | | || || (_) |",
                 "  / /| | | |__  | > _ < ",
                 " / /_| |_| |  | || (_) |",
                 "|____|\\___/   |_| \\___/ "};
            fillElement(infoPadArt, contents);
        }

    extern qw_displayElement scoreDisplay;
        scoreDisplay.relativePos.x = 0;
        scoreDisplay.relativePos.y = 1;
        scoreDisplay.size.x = 20; //TODO
        scoreDisplay.size.y = 1; //TODO
        scoreDisplay.vaguePos = BR;
        scoreDisplay.render = 1;
        scoreDisplay.relativeTo = &infoPadArt;
        allocateElement(&scoreDisplay);

    extern qw_displayElement gameOverText;
        gameOverText.relativePos.x = 0;
        gameOverText.relativePos.y = 0;
        gameOverText.size.x = 51;
        gameOverText.size.y = 6;
        gameOverText.vaguePos = M;
        gameOverText.render = 0;
        gameOverText.relativeTo = NULL;
        allocateElement(&gameOverText);
        {
            char *contents[] =
                {"\0\0_____\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
                 " / ____|\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
                 "| |  __  __ _ _ __ ___   ___    _____   _____ _ __ ",      // "\0\0\0" <--- statements dreamed up
                 "| | |_ |/ _` | '_ ` _ \\ / _ \\  / _ \\ \\ / / _ \\ '__|", //               by the utterly deranged
                 "| |__| | (_| | | | | | |  __/ | (_) \\ V /  __/ |\0\0\0",
                 "\0\\_____|\\__,_|_| |_| |_|\\___|  \\___/ \\_/ \\___|_|\0\0\0"};
            fillElement(gameOverText, contents);
        }
}

void resetPlayfieldRenderngGlobals(int gridSize) {
    int i, j;
    extern qw_displayElement emptyElement;
    extern qw_displayElement **playfieldTiles;
    extern qw_displayElement playfieldBackground;
    extern qw_displayElement infoPad;
    extern qw_displayElement infoPadArt;
    extern qw_displayElement scoreDisplay;
    extern qw_displayElement gameOverText;
    for (i=0; i<gridSize; i++)
        for (j=0; j<gridSize; j++) {
            freeElement(&playfieldTiles[i][j]);
            playfieldTiles[i][j] = emptyElement;
        }
    for (i=0; i<gridSize; i++)
        free(playfieldTiles[i]);
    free(playfieldTiles);
    freeElement(&playfieldBackground);
    playfieldBackground = emptyElement;
    infoPad = emptyElement;
    freeElement(&infoPadArt);
    infoPadArt = emptyElement;
    freeElement(&scoreDisplay);
    scoreDisplay = emptyElement;
    freeElement(&gameOverText);
    gameOverText = emptyElement;
}
