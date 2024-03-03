#include "headers/get-input.h"

#include <stdio.h>
#include <sys/select.h>
#include <bits/types/struct_timeval.h>
#include "headers/2048-types.h"

int dontWaitForInput(){
    struct timeval timeout = {0, 10*1000};
    fd_set set;
    FD_ZERO(&set);
    FD_SET(0, &set);
    return select(1, &set, NULL, NULL, &timeout);
}

qw_input getInput(){
    if(dontWaitForInput() == 0)
        return NO_INPUT;

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
