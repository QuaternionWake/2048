#ifndef MENU_PRERENDERING
#define MENU_PRERENDERING

#include "2048-types.h"

static qw_displayElement menuArt;
static qw_displayElement buttons;
static qw_displayElement selectionElement;

void prerenderMenu(int selection);
void initilizeMenuRenderingGlobals(int buttonCount, char *buttonList[]);
void resetMenuRenderingGlobals();

#endif
