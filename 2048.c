#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define TILESIZE 7

#define clear() printf("\033[H\033[J")

struct pos{
    int x;
    int y;
};

struct pos randPos(int size, int contents[size][size]){
    struct pos a;
    do{
        a.x = rand() % size;
        a.y = rand() % size;
    }while(contents[a.y][a.x]);
    return a;
}

void copyPlayfield(int size, int contents[size][size], int copiedContents[size][size]){
    int i, j;
    for(i=0; i<size; i++)
        for(j=0; j<size; j++)
            copiedContents[i][j] = contents[i][j];
}

int numLen(int num){
    int len = 0;
    while(num){
        len++;
        num/=10;
    }
    return len;
}

int powOfTwo(int n){
    int i;
    int res = 1;
    for(i=0; i<n; i++)
        res*=2;
    return res;
}

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

void renderField(int size, int contents[size][size]){
    int tileLenX = TILESIZE*2+1;
    int tileLenY = TILESIZE+1;
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
            if(contents[i][j]){
                renderTile(tileLenX-3, tileLenY-3, tile, contents[i][j]);
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

void rotateGrid(int size, int contents[size][size], int rotationsTotal){
    int rotatedContents[size][size];
    int i, j;
    int rotationsDone;
    for(rotationsDone=0; rotationsDone<rotationsTotal; rotationsDone++){
        for(i=0; i<size; i++)
            for(j=0; j<size; j++)
                rotatedContents[j][size-i-1] = contents[i][j];
        copyPlayfield(size, rotatedContents, contents);
    }
}

int moveGrid(int size, int contents[size][size]){
    int currentX, nextFaller, fallHeight;
    int notMoved = 1;

    for(currentX=0; currentX<size; currentX++){
        fallHeight = 0;
        while(!contents[fallHeight][currentX] && fallHeight<size)
            fallHeight++;
        if(fallHeight >= size-1)
            continue;
        for(nextFaller=fallHeight+1; nextFaller<size; nextFaller++)
            if(contents[nextFaller][currentX])
                if(contents[fallHeight][currentX] == contents[nextFaller][currentX]){
                    contents[fallHeight][currentX]++;
                    contents[nextFaller][currentX] = 0;
                    fallHeight = nextFaller+1;
                    nextFaller++;
                    notMoved = 0;
                }else
                    fallHeight = nextFaller;
    }

    for(currentX=0; currentX<size; currentX++){
        fallHeight = 0;
        while(contents[fallHeight][currentX] && fallHeight<size)
            fallHeight++;
        if(fallHeight >= size-1)
            continue;
        for(nextFaller=fallHeight+1; nextFaller<size; nextFaller++)
            if(contents[nextFaller][currentX]){
                contents[fallHeight][currentX] = contents[nextFaller][currentX];
                contents[nextFaller][currentX] = 0;
                fallHeight++;
                notMoved = 0;
            }
    }

    return notMoved;
}

int gameOverCheck(int size, int contents[size][size]){
    int testContents[size][size];
    int gameOver = 1;
    copyPlayfield(size, contents, testContents);
    gameOver = moveGrid(size, testContents);
    if(!gameOver) return 1;
    rotateGrid(size, testContents, 1); gameOver = moveGrid(size, testContents); rotateGrid(size, testContents, 3);
    if(!gameOver) return 1;
    rotateGrid(size, testContents, 2); gameOver = moveGrid(size, testContents); rotateGrid(size, testContents, 2);
    if(!gameOver) return 1;
    rotateGrid(size, testContents, 3); gameOver = moveGrid(size, testContents); rotateGrid(size, testContents, 1);
    if(!gameOver) return 1;
    return 0;
}

void mainGameLoop(int size, int contents[size][size]){
    int invalidInput;
    int ongoing = 1;

    while(ongoing){
        do{
            char move = getchar();
            invalidInput = 0;
            switch (move){
                case 27: return;
                case 'w': invalidInput = moveGrid(size, contents);    break;
                case 'a': rotateGrid(size, contents, 1); invalidInput = moveGrid(size, contents); rotateGrid(size, contents, 3);    break;
                case 's': rotateGrid(size, contents, 2); invalidInput = moveGrid(size, contents); rotateGrid(size, contents, 2);    break;
                case 'd': rotateGrid(size, contents, 3); invalidInput = moveGrid(size, contents); rotateGrid(size, contents, 1);    break;
                default: invalidInput = 1;
            }
        }while(invalidInput);

        renderField(size, contents);

        struct pos a = randPos(size, contents);
        if(rand()%5)
            contents[a.y][a.x] = 1;
        else
            contents[a.y][a.x] = 2;

        ongoing = gameOverCheck(size, contents);

        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 100*1000*1000;
        nanosleep(&ts, NULL);
        renderField(size, contents);
    }
}

int main(int argc, char const *argv[]){
    static struct termios oldTermios, newTermios;
    tcgetattr(STDIN_FILENO, &oldTermios);
    newTermios = oldTermios;
    cfmakeraw(&newTermios);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);

    int size = 4;
    if(argc>1)
        size = atoi(argv[1]);
    srand(time(NULL));

    int contents[size][size];
    int i, j;
    for(i=0; i<size; i++)
        for(j=0; j<size; j++)
            contents[i][j] = 0;

    struct pos a;
    a = randPos(size, contents);
    contents[a.y][a.x] = 1;
    a = randPos(size, contents);
    contents[a.y][a.x] = 1;
    renderField(size, contents);
    mainGameLoop(size, contents);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);

    return 0;
}
