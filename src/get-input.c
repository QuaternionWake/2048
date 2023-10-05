#include "headers/get-input.h"

#include <stdio.h>
#include "headers/t-input.h"

    /*
     * Converts raw input into a format understood by the rest of the program.
     */
t_input getInput(){
    char rawInput = getchar();
    t_input processedInput;
    switch(rawInput){
        case 'w':   processedInput = INPUT_UP;      break;
        case 's':   processedInput = INPUT_DOWN;    break;
        case 'a':   processedInput = INPUT_LEFT;    break;
        case 'd':   processedInput = INPUT_RIGHT;   break;
        case 27:    processedInput = INPUT_EXIT;    break;
        case 'e':   processedInput = INPUT_ENTER;   break;
    }
    return processedInput;
}
