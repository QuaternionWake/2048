#ifndef QW_UTILS
#define QW_UTILS

#include <stddef.h>

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

#endif
