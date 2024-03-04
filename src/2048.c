#include "headers/2048.h"

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "headers/menus.h"

int main(int argc, char const *argv[]){
    static struct termios oldTermios, newTermios;
    tcgetattr(STDIN_FILENO, &oldTermios);
    fcntl(0, F_SETFL, O_NONBLOCK);
    newTermios = oldTermios;
    newTermios.c_lflag &= ~(ECHO|ICANON); //Makes input characters not get printed and get them processed immediately

    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);
    printf("\033[?25l"); //hides cursor

    mainMenu();

    printf("\033[?25h"); //unhides cursor
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);
    return 0;
}
