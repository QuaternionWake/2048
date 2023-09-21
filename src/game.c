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
