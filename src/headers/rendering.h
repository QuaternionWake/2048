#ifndef RENDERING_H
#define RENDERING_H

#include "2048-types.h"

static qw_displayElement emptyElement;
static qw_vec2 single = {.x=1, .y=1};

qw_vec2 getScreenSize();
void allocateElement(qw_displayElement *element);
void freeElement(qw_displayElement *element);
void fillElement(qw_displayElement element, char *contents[]);
void positionElement(qw_displayElement *element);
void renderScreen(qw_vec2 blueprint[], int amount, ...);

#endif
