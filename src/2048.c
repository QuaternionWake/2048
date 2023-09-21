#include "2048.h"

    /*
     * Where the program starts, responsible for initializing the playfield,
     * checking if a custom playfield size was requested, placing the initial
     * two blocks, the first call to renderField, and finally calling
     * mainGameLoop. Also does some termios magic to make the inputs unbuffered.
     */
int main(int argc, char const *argv[]){
    static struct termios oldTermios, newTermios;
    tcgetattr(STDIN_FILENO, &oldTermios);
    newTermios = oldTermios;
    cfmakeraw(&newTermios);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);

    int size = 4;
    int tileSize = 5;
    if(argc>1)
        size = atoi(argv[1]);
    if(argc>2)
        tileSize = atoi(argv[2]);
    srand(time(NULL));

    int playfield[size][size];
    int i, j;
    for(i=0; i<size; i++)
        for(j=0; j<size; j++)
            playfield[i][j] = 0;

    struct pos a;
    a = randPos(size, playfield);
    playfield[a.y][a.x] = 1;
    a = randPos(size, playfield);
    playfield[a.y][a.x] = 1;
    renderField(size, playfield, tileSize);
    mainGameLoop(size, playfield, tileSize);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);

    return 0;
}
