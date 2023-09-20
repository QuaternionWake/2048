#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define clear() printf("\033[H\033[J")

struct pos{
    int x;
    int y;
};

    /*
     * Returns a random empty tile on the playfield. Used for adding new blocks
     * to the game.
     */
struct pos randPos(int size, int playfield[size][size]){
    struct pos a;
    do{
        a.x = rand() % size;
        a.y = rand() % size;
    }while(playfield[a.y][a.x]);
    return a;
}

    /*
     * Makes a copy of the playfield. Used for checking if the game is over and
     * for rotating the playfield.
     */
void copyPlayfield(int size, int playfield[size][size], int copiedPlayfield[size][size]){
    int i, j;
    for(i=0; i<size; i++)
        for(j=0; j<size; j++)
            copiedPlayfield[i][j] = playfield[i][j];
}

    /*
     * Takes an integer and returns the number of digits. Used to know how much
     * space is needed to print a number.
     */
int numLen(int num){
    int len = 0;
    while(num){
        len++;
        num/=10;
    }
    return len;
}

    /*
     * Takes an integer n and returns n-th power of two. Used to convert the
     * contents of the playfield (natural numbers) into powers of two to be
     * displayed.
     */
int powOfTwo(int n){
    int i;
    int res = 1;
    for(i=0; i<n; i++)
        res*=2;
    return res;
}

    /*
     * Takes a matrix, its dimensions and the contents of a tile to fill the
     * matrix with what that tile should actually look like.
     *
     * While the function should theoretically be able to fill up a whole tile
     * with digits an int is actually not capable of filling up even one row
     * before overflowing. It would likely have been more practical to hardcode
     * a bunch of powers of two in form of strings and use those instead of
     * generating them as needed.
     */
void renderTile(int tileLenX, int tileLenY, char tile[tileLenY][tileLenX], int tileContentNum){
    int i, j;
    for(i=0; i<tileLenY; i++)
        for(j=0; j<tileLenX; j++)
            tile[i][j] = ' ';

    int digitCount = numLen(powOfTwo(tileContentNum));
    int rows = digitCount/tileLenX + !!(digitCount%tileLenX);
    int partialRow = digitCount%tileLenX;
    char tileContentStr[digitCount+1];
    sprintf(tileContentStr, "%d", powOfTwo(tileContentNum));
    int printStartX = (tileLenX-partialRow)/2;
    int printStartY = (tileLenY-rows)/2;

    int digit = 0;
    if(partialRow){
        for(i=printStartX; digit<partialRow; i++)
            tile[printStartY][i] = tileContentStr[digit++];
        printStartY++;
    }

    for(i=printStartY; digit<digitCount; i++)
        for(j=0; j<tileLenX; j++)
            tile[i][j] = tileContentStr[digit++];
}

    /*
     * Takes the matrix containing the tile's values and its size and converts
     * them to a matrix to be printed. Starts by filling the matrix with spaces
     * and making the borders between tiles, then gives the contents of each
     * non empty tile to renderTile to fill the individual tiles, before
     * clearing the terminal and printing the new image.
     */
void renderField(int size, int playfield[size][size], int tileSize){
    int tileLenX = tileSize*2+1;
    int tileLenY = tileSize+1;
    int totalX = size*tileLenX+1;
    int totalY = size*tileLenY+1;

    char field[totalY][totalX];
    char tile[tileLenY-3][tileLenX-3];
    int i, j;

    for(i=0; i<totalY; i++)
        for(j=0; j<totalX; j++)
            field[i][j] = ' ';

    for(i=0; i<totalY; i+=tileLenY)
        for(j=0; j<totalX; j++)
            field[i][j] = '-';

    for(i=0; i<totalY; i++)
        for(j=0; j<totalX; j+=tileLenX)
            field[i][j] = '|';

    for(i=0; i<totalY; i+=tileLenY)
        for(j=0; j<totalX; j+=tileLenX)
            field[i][j] = '+';

    for(i=0; i<size; i++)
        for(j=0; j<size; j++)
            if(playfield[i][j]){
                renderTile(tileLenX-3, tileLenY-3, tile, playfield[i][j]);
                int k, l;
                for(l=0; l<tileLenX-3; l++)
                    for(k=0; k<tileLenY-3; k++)
                        field[i*tileLenY+2+k][j*tileLenX+2+l] = tile[k][l];
            }

    clear();
    for(i=0; i<totalY; i++){
        for(j=0; j<totalX; j++)
            printf("%c", field[i][j]);
        printf("\n\r");
    }
}

    /*
     * Takes a matrix, its size, and the number of rotations and rotates it
     * 90 degrees clockwise that many times. Used to make all four moves
     * possible to execute with only one function.
     */
void rotateGrid(int size, int playfield[size][size], int rotationsTotal){
    int rotatedPlayfield[size][size];
    int i, j;
    int rotationsDone;
    for(rotationsDone=0; rotationsDone<rotationsTotal; rotationsDone++){
        for(i=0; i<size; i++)
            for(j=0; j<size; j++)
                rotatedPlayfield[j][size-i-1] = playfield[i][j];
        copyPlayfield(size, rotatedPlayfield, playfield);
    }
}

    /*
     * Takes a playfield and its size and makes an up move. Returns 0 if at
     * least one merge or shift was done. First all the merges are done in one
     * loop and then all shifts without merging in another.
     *
     *
     * This was done because it made it easier to ensure no block merges twice
     * in a single move and that no blocks are left floating. The return value
     * is used for makeing sure that a new block doesn't spawn if the move was
     * invalid and for making sure that a valid move exists in gameOverCheck.
     */
int moveGrid(int size, int playfield[size][size]){
    int currentX, nextFaller, fallHeight;
    int notMoved = 1;

    for(currentX=0; currentX<size; currentX++){
        fallHeight = 0;
        while(!playfield[fallHeight][currentX] && fallHeight<size)
            fallHeight++;
        if(fallHeight >= size-1)
            continue;
        for(nextFaller=fallHeight+1; nextFaller<size; nextFaller++)
            if(playfield[nextFaller][currentX])
                if(playfield[fallHeight][currentX] == playfield[nextFaller][currentX]){
                    playfield[fallHeight][currentX]++;
                    playfield[nextFaller][currentX] = 0;
                    fallHeight = nextFaller+1;
                    nextFaller++;
                    notMoved = 0;
                }else
                    fallHeight = nextFaller;
    }

    for(currentX=0; currentX<size; currentX++){
        fallHeight = 0;
        while(playfield[fallHeight][currentX] && fallHeight<size)
            fallHeight++;
        if(fallHeight >= size-1)
            continue;
        for(nextFaller=fallHeight+1; nextFaller<size; nextFaller++)
            if(playfield[nextFaller][currentX]){
                playfield[fallHeight][currentX] = playfield[nextFaller][currentX];
                playfield[nextFaller][currentX] = 0;
                fallHeight++;
                notMoved = 0;
            }
    }

    return notMoved;
}

    /*
     * Checks if the game is over by trying all possible moves. If none succeed
     * the game is finished.
     */
int gameOverCheck(int size, int playfield[size][size]){
    int testPlayfield[size][size];
    int gameOver = 1;
    copyPlayfield(size, playfield, testPlayfield);
    gameOver = moveGrid(size, testPlayfield);
    if(!gameOver) return 1;
    rotateGrid(size, testPlayfield, 1); gameOver = moveGrid(size, testPlayfield); rotateGrid(size, testPlayfield, 3);
    if(!gameOver) return 1;
    rotateGrid(size, testPlayfield, 2); gameOver = moveGrid(size, testPlayfield); rotateGrid(size, testPlayfield, 2);
    if(!gameOver) return 1;
    rotateGrid(size, testPlayfield, 3); gameOver = moveGrid(size, testPlayfield); rotateGrid(size, testPlayfield, 1);
    if(!gameOver) return 1;
    return 0;
}

    /*
     * The main function running the game, the one connecting everything
     * together. It keeps running so long as there is at least one valid move.
     * In charge of getting the input and making sure it is valid, and giving
     * all other functions what they need to do their job.
     */
void mainGameLoop(int size, int playfield[size][size], int tileSize){
    int invalidInput;
    int ongoing = 1;

    while(ongoing){
        do{
            char move = getchar();
            invalidInput = 0;
            switch (move){
                case 27: return;
                case 'w': invalidInput = moveGrid(size, playfield);    break;
                case 'a': rotateGrid(size, playfield, 1); invalidInput = moveGrid(size, playfield); rotateGrid(size, playfield, 3);    break;
                case 's': rotateGrid(size, playfield, 2); invalidInput = moveGrid(size, playfield); rotateGrid(size, playfield, 2);    break;
                case 'd': rotateGrid(size, playfield, 3); invalidInput = moveGrid(size, playfield); rotateGrid(size, playfield, 1);    break;
                default: invalidInput = 1;
            }
        }while(invalidInput);

        renderField(size, playfield, tileSize);

        struct pos a = randPos(size, playfield);
        if(rand()%5)
            playfield[a.y][a.x] = 1;
        else
            playfield[a.y][a.x] = 2;

        ongoing = gameOverCheck(size, playfield);

        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 100*1000*1000;
        nanosleep(&ts, NULL);
        renderField(size, playfield, tileSize);
    }
}

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
