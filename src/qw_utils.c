#include "headers/qw_utils.h"

#include <stdlib.h>

#define GROWFAC 2

qw_list newList() {
    qw_list list = {
        .size = 0,
        .capacity = 100,
        .values = (int*)malloc(list.capacity * sizeof(int))
    };
    return list;
}

void deinitList(qw_list *list) {
    list->size = 0;
    list->capacity = 0;
    free(list->values);
    list->values = NULL;
}

void upsizeList(qw_list *list) {
    list->capacity *= GROWFAC;
    list->values = realloc(list->values, list->capacity * sizeof(int));
}

void shrinkToFitList(qw_list *list) {
    list->capacity = list->size;
    list->values = realloc(list->values, list->capacity * sizeof(int));
}

void addToList(qw_list *list, int value) {
    if(list->size == list->capacity)
        upsizeList(list);
    list->values[list->size] = value;
    list->size++;
}
