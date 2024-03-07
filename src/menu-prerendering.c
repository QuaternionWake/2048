#include "headers/menu-prerendering.h"

#include <stdio.h>
#include <string.h>
#include "headers/2048-types.h"
#include "headers/rendering.h"

void prerenderMenu(int selection, int gridSize) {
    extern qw_displayElement buttons;
    extern qw_displayElement menuArt;
    extern qw_displayElement gridSizeDisplay;
    extern qw_displayElement selectionElement;

    selectionElement.relativePos.y = selection;
    {
        char tempContents[selectionElement.size.x+1];
        sprintf(tempContents, "%dx%d", gridSize, gridSize);
        sprintf(tempContents, "%*s", -gridSizeDisplay.size.x, tempContents);
        char *contents[] = {tempContents};
        fillElement(gridSizeDisplay, contents); //dear god this section is a mess
    }

    extern qw_vec2 single;
    qw_vec2 blueprint[4] = {single, single, single, single};
    renderScreen(blueprint, 4, &buttons, &menuArt, &gridSizeDisplay, &selectionElement);
}

void initilizeMenuRenderingGlobals(int buttonCount, char *buttonList[]) {
    int i;
    extern qw_displayElement buttons;
        buttons.relativePos.x = 0;
        buttons.relativePos.y = 0;
        for (i=0; i<buttonCount; i++)
            buttons.size.x = max(buttons.size.x, strlen(buttonList[i]));
        buttons.size.y = buttonCount;
        buttons.vaguePos = MB;
        buttons.render = 1;
        buttons.relativeTo = NULL;
        allocateElement(&buttons);
        for (i=0; i<buttonCount; i++)
            strcpy(buttons.contents[i], buttonList[i]);

    extern qw_displayElement menuArt;
        menuArt.relativePos.x = 0;
        menuArt.relativePos.y = -1;
        menuArt.size.x = 24;
        menuArt.size.y = 6;
        menuArt.vaguePos = TM;
        menuArt.render = 1;
        menuArt.relativeTo = &buttons;
        allocateElement(&menuArt);
        {
            char *contents[] =
                {" ___   ___  _  _   ___  ",
                 "|__ \\ / _ \\| || | / _ \\ ",
                 "   ) | | | | || || (_) |",
                 "  / /| | | |__  | > _ < ",
                 " / /_| |_| |  | || (_) |",
                 "|____|\\___/   |_| \\___/ "};
            fillElement(menuArt, contents);
        }

    extern qw_displayElement gridSizeDisplay;
        gridSizeDisplay.relativePos.x = strlen(buttonList[0]) + 1;
        gridSizeDisplay.relativePos.y = 0;
        gridSizeDisplay.size.x = 5;
        gridSizeDisplay.size.y = 1;
        gridSizeDisplay.vaguePos = ITL;
        gridSizeDisplay.render = 1;
        gridSizeDisplay.relativeTo = &buttons;
        allocateElement(&gridSizeDisplay);

    extern qw_displayElement selectionElement;
        selectionElement.relativePos.x = 0;
        selectionElement.relativePos.y = 0;
        selectionElement.size.x = 1;
        selectionElement.size.y = 1;
        selectionElement.vaguePos = LT;
        selectionElement.render = 1;
        selectionElement.relativeTo = &buttons;
        allocateElement(&selectionElement);
        {
            char *contents[] = {">"};
            fillElement(selectionElement, contents);
        }
}

void resetMenuRenderingGlobals() {
    extern qw_displayElement emptyElement;
    extern qw_displayElement buttons;
    extern qw_displayElement menuArt;
    extern qw_displayElement gridSizeDisplay;
    extern qw_displayElement selectionElement;

    freeElement(&buttons);
    buttons = emptyElement;
    freeElement(&menuArt);
    menuArt = emptyElement;
    freeElement(&gridSizeDisplay);
    gridSizeDisplay = emptyElement;
    freeElement(&selectionElement);
    selectionElement = emptyElement;
}
