#include "headers/2048.h"

#include <termios.h>
#include <unistd.h>
#include "headers/menus.h"

    /*
     * Where the program starts, does some termios magic to make the inputs
     * unbuffered (and undoes in on exit) and calls mainMenu.
     */
int main(int argc, char const *argv[]){
    static struct termios oldTermios, newTermios;
    tcgetattr(STDIN_FILENO, &oldTermios);
    newTermios = oldTermios;
    cfmakeraw(&newTermios);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);

    mainMenu();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);
    return 0;
}
