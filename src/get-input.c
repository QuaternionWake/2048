#include "headers/get-input.h"

#include <stdio.h>
#include <sys/select.h>
#include <bits/types/struct_timeval.h>
#include "headers/2048-types.h"

#define KEY_MULTI_START 27
#define KEY_ENTER 10
#define KEY_ARROW 91
#define ARROW_UP 65
#define ARROW_DOWN 66
#define ARROW_RIGHT 67
#define ARROW_LEFT 68
#define KEY_BACKSPACE 127

qw_input getInput() {
    if (dontWaitForInput() == 0)
        return NO_INPUT;

    switch (getchar()) {
    case EOF: return NO_INPUT;
    case 'w': return INPUT_UP;
    case 's': return INPUT_DOWN;
    case 'a': return INPUT_LEFT;
    case 'd': return INPUT_RIGHT;
    case 'e': return INPUT_ENTER;
    case KEY_ENTER: return INPUT_ENTER;
    case KEY_MULTI_START:
        switch (getchar()) {
        case EOF: return INPUT_EXIT;
        case KEY_ARROW:
            switch (getchar()) {
            case ARROW_UP:    return INPUT_UP;
            case ARROW_DOWN:  return INPUT_DOWN;
            case ARROW_RIGHT: return INPUT_RIGHT;
            case ARROW_LEFT:  return INPUT_LEFT;
            }
        }
    }

    return NO_INPUT;
}

int dontWaitForInput() {
    struct timeval timeout = {0, 10*1000};
    fd_set set;
    FD_ZERO(&set);
    FD_SET(0, &set);
    return select(1, &set, NULL, NULL, &timeout);
}
