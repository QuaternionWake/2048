#ifndef RENDERING_H
#define RENDERING_H

#include "2048-types.h"

qw_pos getScreenSize();
void allocateElement(qw_displayElement *element);
void freeElement(qw_displayElement *element);
void fillElement(qw_displayElement element, char *contents[]);
void positionElement(qw_displayElement *element);
void renderScreen(qw_pos blueprint[], int amount, ...);

#endif
