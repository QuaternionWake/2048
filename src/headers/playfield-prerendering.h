#ifndef PLAYFIELD_RENDERING_H
#define PLAYFIELD_RENDERING_H

#include "2048-types.h"

static qw_displayElement playfieldBackground;
static qw_displayElement **playfieldTiles;

int numLen(int num);
int powOfTwo(int n);
void drawTile(qw_displayElement *tile, int tileContentNum);
void prerenderField(int gridSize, int playfield[gridSize][gridSize]);
void initilizePlayfieldRenderingGlobals(int gridSize, int tileSizeUnadjusted);
void resetPlayfieldRenderngGlobals(int gridSize);

#endif
