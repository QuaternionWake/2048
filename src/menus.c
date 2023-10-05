#include "headers/menus.h"

#include "headers/get-input.h"
#include "headers/menu-rendering.h"
#include "headers/game.h"
#include "headers/t-pos.h"
#include "headers/t-input.h"

//theese are only here until scores and options are implemented
#include <stdio.h>
#include <unistd.h>

    /*
     * Handles the main menu, holds the menu art and the list of buttons.
     * Controls the inputs and what the buttons do.
     */
void mainMenu(){
    t_pos artSize = {.x=24, .y=6};
    char menuArt[6][24] =
        {R"( ___   ___  _  _   ___  )",
         R"(|__ \ / _ \| || | / _ \ )",
         R"(   ) | | | | || || (_) |)",
         R"(  / /| | | |__  | > _ < )",
         R"( / /_| |_| |  | || (_) |)",
         R"(|____|\___/   |_| \___/ )"};
    int buttonCount = 4;
    char *buttonList[] =
        {"Start game",
         "Scores",
         "Options",
         "Exit"};
    int selection = 0;
    renderMenu(artSize, menuArt, buttonList, buttonCount, selection);
    while(1){
        t_input input = getInput();
        switch(input){
            case INPUT_UP:      if(selection>0) selection--;    break;
            case INPUT_DOWN:    if(selection<3) selection++;    break;
            case INPUT_ENTER:
                switch(selection){
                    case 0: initializeGame(4, 5);   break;
                    case 1: printf("\033[H\033[J\n\tNothing here yet :(\n"); sleep(2); break;
                    case 2: printf("\033[H\033[J\n\tNothing here yet :(\n"); sleep(2); break;
                    case 3: return;
                }break;
            case INPUT_EXIT:    return;
        }
        renderMenu(artSize, menuArt, buttonList, buttonCount, selection);
    }
}
