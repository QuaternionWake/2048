#ifndef PLAYFIELD_RENDERING_H
#define PLAYFIELD_RENDERING_H

#include "2048-types.h"

static qw_displayElement playfieldBackground;
static qw_displayElement **playfieldTiles;
static qw_displayElement infoPad;
static qw_displayElement infoPadArt;
static qw_displayElement scoreDisplay;
static qw_displayElement gameOverText;

void drawTile(qw_displayElement *tile, int tileContentNum);
void prerenderField(int gridSize, int playfield[gridSize][gridSize], int score, char gameOver);
void initilizePlayfieldRenderingGlobals(int gridSize, int tileSizeUnadjusted);
void resetPlayfieldRenderngGlobals(int gridSize);

#endif
