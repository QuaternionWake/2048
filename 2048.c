#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void renderField(int size, int contents[size][size]){
    int tileLenX =TILESIZE*2+1;
    int tileLenY =TILESIZE+1;
    int numLine = tileLenY/2;
    int totalX = size*tileLenX+1;
    int totalY = size*tileLenY+1;
    int i, j;
    int notPrinted = 1;
    clear();
    for(i=0; i<totalY; i++){
        for(j=0; j<totalX; j++)
            if(!(i%tileLenY)){
                if(!(j%tileLenX))
                    printf("+");
                else
                    printf("-");
            }else if(i%tileLenY == numLine){
                if(!(j%tileLenX)){
                    printf("|");
                    notPrinted = 1;
                }else if(notPrinted && contents[i/tileLenY][j/tileLenX]){
                    int numSkip = numLen(powOfTwo(contents[i/tileLenY][j/tileLenX]));
                    int skip = (tileLenX-numSkip)/2+numSkip;
                    printf("%*d", skip, powOfTwo(contents[i/tileLenY][j/tileLenX]));
                    j+= skip-1;
                    notPrinted = 0;
                }else
                    printf(" ");
            }else{
                if(!(j%tileLenX))
                    printf("|");
                else
                    printf(" ");
            }
        printf("\n");
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
    //get input correctly here
    int invalidInput;
    int ongoing = 1;

    while(ongoing){
        do{
            char move = getchar();
            invalidInput = 0;
            switch (move){
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

    return 0;
}
