#include "headers/rendering.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/ioctl.h>
#include "headers/2048-types.h"

#include <unistd.h>

#define alignElementTopExt(VAR) element->absolutePos.VAR = element->relativeTo->absolutePos.VAR + element->relativePos.VAR - element->size.VAR
#define alignElementTopInt(VAR) element->absolutePos.VAR = element->relativeTo->absolutePos.VAR + element->relativePos.VAR
#define alignElementTopMid(VAR) element->absolutePos.VAR = element->relativeTo->absolutePos.VAR + element->relativePos.VAR + element->relativeTo->size.VAR/2 - element->size.VAR
#define alignElementMiddle(VAR) element->absolutePos.VAR = element->relativeTo->absolutePos.VAR + element->relativePos.VAR + (element->relativeTo->size.VAR - element->size.VAR)/2
#define alignElementBtmMid(VAR) element->absolutePos.VAR = element->relativeTo->absolutePos.VAR + element->relativePos.VAR + element->relativeTo->size.VAR/2
#define alignElementBtmInt(VAR) element->absolutePos.VAR = element->relativeTo->absolutePos.VAR + element->relativePos.VAR + element->relativeTo->size.VAR - element->size.VAR
#define alignElementBtmExt(VAR) element->absolutePos.VAR = element->relativeTo->absolutePos.VAR + element->relativePos.VAR + element->relativeTo->size.VAR

qw_pos getScreenSize(){
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    qw_pos screenSize = {.x=w.ws_col, .y=w.ws_row};

    return screenSize;
}

void allocateElement(qw_displayElement *element){
    int i;

    element->contents = (char**)malloc(sizeof(char*)*element->size.y);
    if(element->contents==NULL){
        printf("ERROR: Could not allocate memory");
        exit(EXIT_FAILURE);
    }

    for(i=0; i<element->size.y; i++){
        element->contents[i] = (char*)malloc(sizeof(char)*element->size.x);
        if(element->contents[i]==NULL){
            printf("ERROR: Could not allocate memory");
            exit(EXIT_FAILURE);
        }
    }
}

void freeElement(qw_displayElement *element){
    int i;

    for(i=0; i<element->size.y; i++)
        free(element->contents[i]);

    free(element->contents);
}

void positionElement(qw_displayElement *element){
    switch(element->vaguePos){  //Tremble before the ALMIGHTY SWITCH CASE

        case TLC:   alignElementTopExt(y);  alignElementTopExt(x);  break;
        case TL:    alignElementTopExt(y);  alignElementTopInt(x);  break;
        case TML:   alignElementTopExt(y);  alignElementTopMid(x);  break;
        case TM:    alignElementTopExt(y);  alignElementMiddle(x);  break;
        case TMR:   alignElementTopExt(y);  alignElementBtmMid(x);  break;
        case TR:    alignElementTopExt(y);  alignElementBtmInt(x);  break;
        case TRC:   alignElementTopExt(y);  alignElementBtmExt(x);  break;

        case LT:    alignElementTopInt(y);  alignElementTopExt(x);  break;
        case ITL:   alignElementTopInt(y);  alignElementTopInt(x);  break;
        case ITML:  alignElementTopInt(y);  alignElementTopMid(x);  break;
        case ITM:   alignElementTopInt(y);  alignElementMiddle(x);  break;
        case ITMR:  alignElementTopInt(y);  alignElementBtmMid(x);  break;
        case ITR:   alignElementTopInt(y);  alignElementBtmInt(x);  break;
        case RT:    alignElementTopInt(y);  alignElementBtmExt(x);  break;

        case LMT:   alignElementTopMid(y);  alignElementTopExt(x);  break;
        case ILMT:  alignElementTopMid(y);  alignElementTopInt(x);  break;
        case MTL:   alignElementTopMid(y);  alignElementTopMid(x);  break;
        case MT:    alignElementTopMid(y);  alignElementMiddle(x);  break;
        case MTR:   alignElementTopMid(y);  alignElementBtmMid(x);  break;
        case IRMT:  alignElementTopMid(y);  alignElementBtmInt(x);  break;
        case RMT:   alignElementTopMid(y);  alignElementBtmExt(x);  break;

        case LM:    alignElementMiddle(y);  alignElementTopExt(x);  break;
        case ILM:   alignElementMiddle(y);  alignElementTopInt(x);  break;
        case ML:    alignElementMiddle(y);  alignElementTopMid(x);  break;
        case M:     alignElementMiddle(y);  alignElementMiddle(x);  break;
        case MR:    alignElementMiddle(y);  alignElementBtmMid(x);  break;
        case IRM:   alignElementMiddle(y);  alignElementBtmInt(x);  break;
        case RM:    alignElementMiddle(y);  alignElementBtmExt(x);  break;

        case LMB:   alignElementBtmMid(y);  alignElementTopExt(x);  break;
        case ILMB:  alignElementBtmMid(y);  alignElementTopInt(x);  break;
        case MBL:   alignElementBtmMid(y);  alignElementTopMid(x);  break;
        case MB:    alignElementBtmMid(y);  alignElementMiddle(x);  break;
        case MBR:   alignElementBtmMid(y);  alignElementBtmMid(x);  break;
        case IRMB:  alignElementBtmMid(y);  alignElementBtmInt(x);  break;
        case RMB:   alignElementBtmMid(y);  alignElementBtmExt(x);  break;

        case LB:    alignElementBtmInt(y);  alignElementTopExt(x);  break;
        case IBL:   alignElementBtmInt(y);  alignElementTopInt(x);  break;
        case IBML:  alignElementBtmInt(y);  alignElementTopMid(x);  break;
        case IBM:   alignElementBtmInt(y);  alignElementMiddle(x);  break;
        case IBMR:  alignElementBtmInt(y);  alignElementBtmMid(x);  break;
        case IBR:   alignElementBtmInt(y);  alignElementBtmInt(x);  break;
        case RB:    alignElementBtmInt(y);  alignElementBtmExt(x);  break;

        case BLC:   alignElementBtmExt(y);  alignElementTopExt(x);  break;
        case BL:    alignElementBtmExt(y);  alignElementTopInt(x);  break;
        case BML:   alignElementBtmExt(y);  alignElementTopMid(x);  break;
        case BM:    alignElementBtmExt(y);  alignElementMiddle(x);  break;
        case BMR:   alignElementBtmExt(y);  alignElementBtmMid(x);  break;
        case BR:    alignElementBtmExt(y);  alignElementBtmInt(x);  break;
        case BRC:   alignElementBtmExt(y);  alignElementBtmExt(x);  break;
    }
}

    /*
     * The order in which the qw_displayElements are given to the function is
     * important; if element b is positioned relative to element a, but given to
     * the function before (or even without) element a, it will be placed as if
     * element a was in the top left corner.
     * Additionally, any elements given later will overwrite anything a previous
     * one might have placed down, basically; elements given later in the list
     * are renendered over those given earlier.
     */
void renderScreen(qw_pos blueprint[], int amount, ...){
    va_list elements;
    va_start(elements, amount);

    qw_displayElement screen = {.relativePos.x=0, .relativePos.y=0, .size.x=0, .size.y=0,
                                .vaguePos=TLC, .relativeTo=NULL, .render=1, .contents=NULL};
    screen.size = getScreenSize();
    allocateElement(&screen);
    qw_displayElement window = {.relativePos.x=0, .relativePos.y=0, .size.x=0, .size.y=0,
                                .vaguePos=M, .relativeTo=&screen, .render=0, .contents=NULL};
    window.size = screen.size;
    window.size.x -= 2;
    window.size.y -= 2;
    allocateElement(&window);
    positionElement(&window);
    int i, j, k, l, m;
    for(i=0; i<window.size.y; i++)
        for(j=0; j<window.size.x; j++)
            window.contents[i][j] = ' ';
    for(i=0; i<screen.size.y; i++)
        for(j=0; j<screen.size.x; j++)
            screen.contents[i][j] = ' ';

    for(m=0; m<amount; m++)
        if(blueprint[m].x==1 && blueprint[m].y==1){
            qw_displayElement *currentElement;
            currentElement = va_arg(elements, qw_displayElement*);
            if(currentElement->relativeTo == NULL)
                currentElement->relativeTo = &window;
            positionElement(currentElement);
            if(currentElement->render == 0)
                continue;

            qw_pos start = {.x = -min(currentElement->absolutePos.x, 0),
                            .y = -min(currentElement->absolutePos.y, 0)};
            qw_pos end = {.x = min(currentElement->absolutePos.x + currentElement->size.x, window.size.x) - currentElement->absolutePos.x,
                          .y = min(currentElement->absolutePos.y + currentElement->size.y, window.size.y) - currentElement->absolutePos.y};
            for(k=start.y; k<end.y; k++)
                for(l=start.x; l<end.x; l++)
                    if(currentElement->contents[k][l])
                        window.contents[currentElement->absolutePos.y + k][currentElement->absolutePos.x + l] = currentElement->contents[k][l];
            if(currentElement->relativeTo == &window)
                currentElement->relativeTo = NULL;
        }else{
            qw_displayElement **currentElement;
            currentElement = va_arg(elements, qw_displayElement**);
            for(i=0; i<blueprint[m].y; i++)
                for(j=0; j<blueprint[m].x; j++){
                    if(currentElement[i][j].relativeTo == NULL)
                        currentElement[i][j].relativeTo = &window;
                    positionElement(&currentElement[i][j]);
                    if(currentElement[i][j].render == 0)
                        continue;

                    qw_pos start = {.x = -min(currentElement[i][j].absolutePos.x, 0),
                                    .y = -min(currentElement[i][j].absolutePos.y, 0)};
                    qw_pos end = {.x = min(currentElement[i][j].absolutePos.x + currentElement[i][j].size.x, window.size.x) - currentElement[i][j].absolutePos.x,
                                  .y = min(currentElement[i][j].absolutePos.y + currentElement[i][j].size.y, window.size.y) - currentElement[i][j].absolutePos.y};
                    for(k=start.y; k<end.y; k++)
                        for(l=start.x; l<end.x; l++)
                            window.contents[currentElement[i][j].absolutePos.y + k][currentElement[i][j].absolutePos.x + l] = currentElement[i][j].contents[k][l];
                    if(currentElement[i][j].relativeTo == &window)
                        currentElement[i][j].relativeTo = NULL;
                    }
        }

    for(i=0; i<window.size.y; i++)
        for(j=0; j<window.size.x; j++)
            screen.contents[window.absolutePos.y + i][window.absolutePos.x + j] = window.contents[i][j];

    clear();
    for(i=0; i<screen.size.y; i++){
        for(j=0; j<screen.size.x; j++)
            putchar(screen.contents[i][j]);
        if(i != screen.size.y-1)
            printf("\n\r");
    }

    freeElement(&screen);
    freeElement(&window);

    va_end(elements);
}
