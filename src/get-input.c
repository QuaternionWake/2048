#include "headers/get-input.h"

#include <stdio.h>
#include "headers/2048-types.h"

qw_input getInput(){
    char rawInput = getchar();
    qw_input processedInput;
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
