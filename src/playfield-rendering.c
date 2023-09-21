#define clear() printf("\033[H\033[J")

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
