#include "headers/menus.h"

#include <stdlib.h>
#include "headers/get-input.h"
#include "headers/menu-prerendering.h"
#include "headers/game.h"
#include "headers/2048-types.h"

//stdio only here until scores and options are implemented
#include <stdio.h>

    /*
     * Handles the main menu, holds the menu art and the list of buttons.
     * Controls the inputs and what the buttons do.
     */
void mainMenu(){
    int selection = 0;
    int buttonCount = 4;
    char *buttonList[] =
        {"Start game",
         "Scores",
         "Options",
         "Exit"};
    initilizeMenuRenderingGlobals(buttonCount, buttonList);
    prerenderMenu(selection);
    while(1){
        int exit = 0;
        qw_input input = getInput();
        switch(input){
            case INPUT_UP:      if(selection>0)             selection--; break;
            case INPUT_DOWN:    if(selection<buttonCount-1) selection++; break;
            case INPUT_ENTER:
                switch(selection){
                    case 0: initializeGame(4, 5);   break;
                    case 1: printf("\033[H\033[J\n\tNothing here yet :(\n"); getchar(); break;
                    case 2: printf("\033[H\033[J\n\tNothing here yet :(\n"); getchar(); break;
                    case 3: exit = 1;   break;
                }break;
            case INPUT_EXIT:    exit = 1; break;
            default: continue;
        }
        if(exit)
            break;
        prerenderMenu(selection);
    }
    resetMenuRenderingGlobals();
}
