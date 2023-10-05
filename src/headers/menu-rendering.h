#ifndef MENU_RENDERING_H
#define MENU_RENDERING_H

#include "t-pos.h"

void renderMenu(t_pos artSize, char menuArt[artSize.y][artSize.x], char *buttonList[], int buttonCount, int selection);

#endif
