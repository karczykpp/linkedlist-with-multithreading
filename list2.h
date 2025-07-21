#ifndef LCL_LIST_H
#define LCL_LIST_H
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
// ==============================================
//
//  Version 1.0, 2025-01-15
//
// ==============================================

struct TList{
    void **items;
    int maxSize;
    int first;
    int physicalSize;
    int counter;
    int last;
    pthread_mutex_t lock;
    pthread_cond_t notEmpty;
    pthread_cond_t notFull;
};
typedef struct TList TList;

TList *createList(int s);

void destroyList(TList *lst);

void putItem(TList *lst, void *itm);

void *getItem(TList *lst);

void *popItem(TList *lst);

int removeItem(TList *lst, void *itm);

int getCount(TList *lst);

void setMaxSize(TList *lst, int s);

void appendItems(TList *lst, TList *lst2);

void showList(TList *lst);

#endif //LCL_LIST_H
