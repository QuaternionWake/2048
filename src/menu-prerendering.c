#include "headers/menu-prerendering.h"

#include <string.h>
#include "headers/rendering.h"
#include "headers/rendering-globals.h"

    /*
     * Sets values of globals used for rendering menus to the correct value to
     * represent current state of the menu and passes everything on to
     * renderScreen.
     */
void prerenderMenu(int selection){
    extern qw_displayElement buttons;
    extern qw_displayElement menuArt;
    extern qw_displayElement selectionElement;

    selectionElement.relativePos.y = selection;

    extern qw_pos single;
    qw_pos blueprint[3] = {single, single, single};
    renderScreen(blueprint, 3, &buttons, &menuArt, &selectionElement);
}

    /*
     * Sets all of the global varaibles used for rendering the menus to the
     * correct initial values.
     */
void initilizeMenuRenderingGlobals(int buttonCount, char *buttonList[]){
    int i;
    extern qw_displayElement buttons;
        buttons.relativePos.x = 0;
        buttons.relativePos.y = 0;
        for(i=0; i<buttonCount; i++)
            buttons.size.x = max(buttons.size.x, strlen(buttonList[i]));
        buttons.size.y = buttonCount;
        buttons.vaguePos = MB;
        buttons.render = 1;
        buttons.relativeTo = NULL;
        allocateElement(&buttons);
        for(i=0; i<buttonCount; i++)
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

    extern qw_displayElement selectionElement;
        selectionElement.relativePos.x = 0,
        selectionElement.relativePos.y = 0,
        selectionElement.size.x = 1,
        selectionElement.size.y = 1,
        selectionElement.vaguePos = LT,
        selectionElement.render = 1,
        selectionElement.relativeTo = &buttons,
        allocateElement(&selectionElement);
        {
            char *contents[] = {">"};
            fillElement(selectionElement, contents);
        }
}

    /*
     * Resets all global varaibles used for rendering the menus back to 0.
     */
void resetMenuRenderingGlobals(){
    extern qw_displayElement emptyElement;
    extern qw_displayElement buttons;
    extern qw_displayElement menuArt;
    extern qw_displayElement selectionElement;

    freeElement(&buttons);
    buttons = emptyElement;
    freeElement(&menuArt);
    menuArt = emptyElement;
    freeElement(&selectionElement);
    selectionElement = emptyElement;
}
