#include "headers/menu-rendering.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "headers/t-pos.h"

#define clear() printf("\033[H\033[J")
#define max(a, b) (((a)>(b)) ? (a) : (b))

    /*
     * Takes the menu art, its size, the amount and contents of buttons, and
     * which button is selected. starts by figuring out how tall and wide the
     * the button selection and final menu will need to be, and where the top
     * left corner of every element is. Fills the final menu matrix and button
     * matrix with spaces, copies contents of all buttons into the matrix,
     * copies all submatrices into the main one, clears the screen and prints
     * everything out.
     */
void renderMenu(t_pos artSize, char menuArt[artSize.y][artSize.x], char *buttonList[], int buttonCount, int selection){
    int i, j;
    t_pos artStart = {.x=1, .y=1};
    t_pos buttonSize = {.x=0, .y=buttonCount*2-1};
    for(i=0; i<buttonCount; i++)
        buttonSize.x = max(buttonSize.x, strlen(buttonList[i]));
    buttonSize.x++;
    char buttons[buttonSize.y][buttonSize.x];
    t_pos buttonStart = {.x=2, .y=artStart.y+artSize.y+2};
    t_pos menuSize = {.x=max(buttonStart.x+buttonSize.x, artStart.x+artSize.x)+1, .y=buttonStart.y+buttonSize.y+1};
    char menu[menuSize.y][menuSize.x];

    for(i=0; i<menuSize.y; i++)
        for(j=0; j<menuSize.x; j++)
            menu[i][j] = ' ';

    for(i=0; i<artSize.y; i++)
        for(j=0; j<artSize.x; j++)
            menu[artStart.y+i][artStart.x+j] = menuArt[i][j];

    for(i=0; i<buttonSize.y; i++)
        for(j=0; j<buttonSize.x; j++)
            buttons[i][j] = ' ';

    for(i=0; i<buttonSize.y; i+=2)
        strcpy(&buttons[i][1], buttonList[i/2]);

    buttons[2*selection][0] = '>';

    for(i=0; i<buttonSize.y; i++)
        for(j=0; j<buttonSize.x; j++)
            menu[buttonStart.y+i][buttonStart.x+j] = buttons[i][j];

    clear();
    for(i=0; i<menuSize.y; i++){
        for(j=0; j<menuSize.x; j++)
            printf("%c", menu[i][j]);
        printf("\n\r");
    }
}
