#include "headers/2048.h"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "headers/menus.h"

    /*
     * Where the program starts, does some termios and escape sequence magic to
     * make the game pleasant to actually play and look at, calls mainMenu, and
     * undoes said magic on exit.
     */
int main(int argc, char const *argv[]){
    static struct termios oldTermios, newTermios;
    tcgetattr(STDIN_FILENO, &oldTermios);
    newTermios = oldTermios;
    newTermios.c_lflag &= ~(ECHO|ICANON); //Makes input characters not get written and get them processed immediately

    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);
    printf("\033[?25l"); //hides cursor

    mainMenu();

    printf("\033[?25h"); //unhides cursor
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);
    return 0;
}
