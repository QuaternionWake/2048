#include "headers/playfield-prerendering.h"

#include <stdio.h>
#include <stdlib.h>
#include "headers/rendering.h"
#include "headers/2048-types.h"
#include "headers/rendering-globals.h"

    /*
     * Takes an integer and returns the number of digits. Used to know how much
     * space is needed to print a number.
     */
int numLen(int num){
    int len = 0;
    while(num){
        len++;
        num/=10;
    }
    return len;
}

    /*
     * Takes an integer n and returns n-th power of two. Used to convert the
     * contents of the playfield (natural numbers) into powers of two to be
     * displayed.
     */
int powOfTwo(int n){
    int i;
    int res = 1;
    for(i=0; i<n; i++)
        res*=2;
    return res;
}

    /*
     * Takes a qw_displayElement and the unprocessed contents of a tile to fill
     * its .contents with what that tile should actually look like.
     *
     * While the function should theoretically be able to fill up a whole tile
     * with digits an int is actually not capable of filling up even one row
     * before overflowing. It would likely have been more practical to hardcode
     * a bunch of powers of two in form of strings and use those instead of
     * generating them as needed.
     */
void drawTile(qw_displayElement *tile, int tileContentNum){
    int i, j;
    for(i=0; i<tile->size.y; i++)
        for(j=0; j<tile->size.x; j++)
            tile->contents[i][j] = ' ';

    if(!tileContentNum)
        return;

    int digitCount = numLen(powOfTwo(tileContentNum));
    int rows = digitCount/tile->size.x + !!(digitCount%tile->size.x);
    int partialRow = digitCount%tile->size.x;
    char tileContentStr[digitCount+1];
    sprintf(tileContentStr, "%d", powOfTwo(tileContentNum));
    qw_pos printStart = {.x=(tile->size.x-partialRow)/2,
                         .y=(tile->size.y-rows)/2};

    int digit = 0;
    if(partialRow){
        for(i=printStart.x; digit<partialRow; i++)
            tile->contents[printStart.y][i] = tileContentStr[digit++];
        printStart.y++;
    }

    for(i=printStart.y; digit<digitCount; i++)
        for(j=0; j<tile->size.x; j++)
            tile->contents[i][j] = tileContentStr[digit++];
}

    /*
     * Takes the 2d array containing the tile's values and its size and converts
     * them to a 2d array of qw_displayElements filled by renderTile, then
     * passes evertyhing on to renderScreen.
     */
void prerenderField(int gridSize, int playfield[gridSize][gridSize]){
    int i, j;
    extern qw_displayElement playfieldBackground;
    extern qw_displayElement **playfieldTiles;

    for(i=0; i<gridSize; i++)
        for(j=0; j<gridSize; j++)
            drawTile(&playfieldTiles[i][j], playfield[i][j]);

    extern qw_pos single;
    qw_pos blueprint[2] = {single, {.x=gridSize, .y=gridSize}};
    renderScreen(blueprint, 2, &playfieldBackground, playfieldTiles);
}

    /*
     * Sets all of the global varaibles used for rendering the playfield to the
     * correct initial values.
     */
void initilizePlayfieldRenderingGlobals(int gridSize, int tileSizeUnadjusted){
    int i, j;
    qw_pos tileSize = {.x = tileSizeUnadjusted*2+1,
                       .y = tileSizeUnadjusted+1};

    extern qw_displayElement playfieldBackground;
        playfieldBackground.relativePos.x = 0;
        playfieldBackground.relativePos.y = -1;
        playfieldBackground.size.x = gridSize*tileSize.x+1;
        playfieldBackground.size.y = gridSize*tileSize.y+1;
        playfieldBackground.vaguePos = M;
        playfieldBackground.render = 1;
        playfieldBackground.relativeTo = NULL;
        allocateElement(&playfieldBackground);

    for(i=0; i<playfieldBackground.size.y; i++)
        for(j=0; j<playfieldBackground.size.x; j++)
            playfieldBackground.contents[i][j] = ' ';

    for(i=0; i<playfieldBackground.size.y; i+=tileSize.y)
        for(j=0; j<playfieldBackground.size.x; j++)
            playfieldBackground.contents[i][j] = '-';

    for(i=0; i<playfieldBackground.size.y; i++)
        for(j=0; j<playfieldBackground.size.x; j+=tileSize.x)
            playfieldBackground.contents[i][j] = '|';

    for(i=0; i<playfieldBackground.size.y; i+=tileSize.y)
        for(j=0; j<playfieldBackground.size.x; j+=tileSize.x)
            playfieldBackground.contents[i][j] = '+';

    extern qw_displayElement **playfieldTiles;
    playfieldTiles = (qw_displayElement**)malloc(gridSize*sizeof(qw_displayElement*));
    for(i=0; i<gridSize; i++)
        playfieldTiles[i] = (qw_displayElement*)malloc(gridSize*sizeof(qw_displayElement));

    for(i=0; i<gridSize; i++)
        for(j=0; j<gridSize; j++){
            playfieldTiles[i][j].relativePos.x = j*tileSize.x + 2;
            playfieldTiles[i][j].relativePos.y = i*tileSize.y + 2;
            playfieldTiles[i][j].size.x = tileSize.x-3;
            playfieldTiles[i][j].size.y = tileSize.y-3;
            playfieldTiles[i][j].vaguePos = ITL;
            playfieldTiles[i][j].render = 1;
            playfieldTiles[i][j].relativeTo = &playfieldBackground;
            allocateElement(&playfieldTiles[i][j]);
        }
}

    /*
     * Resets all global varaibles used for rendering the playfield back to 0.
     */
void resetPlayfieldRenderngGlobals(int gridSize){
    int i, j;
    extern qw_displayElement emptyElement;
    extern qw_displayElement **playfieldTiles;
    extern qw_displayElement playfieldBackground;
    for(i=0; i<gridSize; i++)
        for(j=0; j<gridSize; j++){
            freeElement(&playfieldTiles[i][j]);
            playfieldTiles[i][j] = emptyElement;
        }
    for(i=0; i<gridSize; i++)
        free(playfieldTiles[i]);
    free(playfieldTiles);
    freeElement(&playfieldBackground);
    playfieldBackground = emptyElement;
}
