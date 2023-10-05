#ifndef PLAYFIELD_RENDERING_H
#define PLAYFIELD_RENDERING_H

int numLen(int num);
int powOfTwo(int n);
void renderTile(int tileLenX, int tileLenY, char tile[tileLenY][tileLenX], int tileContentNum);
void renderField(int size, int playfield[size][size], int tileSize);

#endif
