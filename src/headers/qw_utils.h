#ifndef QW_UTILS
#define QW_UTILS

#include <stddef.h>

//qw_list START
    typedef struct list qw_list;

    struct list {
        int *values;
        size_t size;
        size_t capacity;
    };

    qw_list newList();
    void deinitList(qw_list *list);
    void upsizeList(qw_list *list);
    void shrinkToFitList(qw_list *list);
    void addToList(qw_list *list, int value);
//qw_list END

int numLen(int num);
int powOfTwo(int n);

#define clear() puts("\033[H\033[J")
#define moveCursor(x, y) printf("\033[%d;%dH", y, x)
#define max(a, b) (((a)>(b)) ? (a) : (b))
#define min(a, b) (((a)<(b)) ? (a) : (b))

#endif
