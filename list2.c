#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "list2.h"

/*typedef struct {
void **items;
int maxSize;
int counter;
int last;
pthread_mutex_t lock;
pthread_cond_t notEmpty;
pthread_cond_t notFull;
} TList;*/
TList * createList(int s)
{
    TList *lst = (TList *)malloc(sizeof(TList));
    lst->items = (void **)malloc(sizeof(void *) * s);
    lst->first = 0;
    lst->physicalSize = s;
    lst->maxSize = s;
    lst->counter = 0;
    lst->last = 0;
    pthread_mutex_init(&lst->lock, NULL);
    pthread_cond_init(&lst->notEmpty, NULL);
    pthread_cond_init(&lst->notFull, NULL);
    return lst;
}

void putItem(TList* lst, void *itm)
{
    pthread_mutex_lock(&lst->lock);
    while(lst->counter >= lst->maxSize)
    {
        pthread_cond_wait(&lst->notFull, &lst->lock);
    }
    lst->items[lst->last] = itm;
    lst->last = (lst->last + 1) % lst->physicalSize;
    lst->counter++;
    pthread_cond_signal(&lst->notEmpty);
    pthread_mutex_unlock(&lst->lock);
}

void* getItem(TList* lst)
{
    pthread_mutex_lock(&lst->lock);
    while (lst->counter == 0) {
        pthread_cond_wait(&lst->notEmpty, &lst->lock);
    }
    void* item = lst->items[lst->first];
    lst->first = (lst->first + 1) % lst->physicalSize;
    lst->counter--;

    pthread_cond_signal(&lst->notFull); 
    pthread_mutex_unlock(&lst->lock);

    return item;
}
int removeItem(TList* lst, void *itm)
{
    pthread_mutex_lock(&lst->lock);
    for (int i = 0; i < lst->counter; i++) {
        int index = (lst->first + i) % lst->physicalSize;
        if (lst->items[index] == itm) {
            for (int j = i; j < lst->counter - 1; j++) {
                int curIndex = (lst->first + j) % lst->physicalSize;
                int nextIndex = (lst->first + j + 1) % lst->physicalSize;
                lst->items[curIndex] = lst->items[nextIndex];
            }
            lst->last = (lst->last - 1 + lst->physicalSize) % lst->physicalSize;
            lst->counter--;
            pthread_cond_signal(&lst->notFull);
            pthread_mutex_unlock(&lst->lock);
            return 1;
        }
    }
    pthread_mutex_unlock(&lst->lock);
    return 0;

}
void appendItems(TList* lst, TList* lst2)
{
    pthread_mutex_lock(&lst->lock);
    pthread_mutex_lock(&lst2->lock);
    
    int totalItems = lst2->counter + lst->counter;
    if (totalItems > lst->physicalSize)
    {
        int new_cap = totalItems;
        void** newItems = realloc(lst->items, new_cap * sizeof(void*));
        if (newItems == NULL) {
            pthread_mutex_unlock(&lst2->lock);
            pthread_mutex_unlock(&lst->lock);
            return;
        }
        
        if (lst->first > lst->last) {
            for (int i = 0; i < lst->first; i++) {
                newItems[i + lst->physicalSize] = lst->items[i];
            }
            lst->last += lst->physicalSize;
        }
        
        lst->items = newItems;
        lst->maxSize = new_cap;
        lst->physicalSize = new_cap;
    }
    
    for (int i = 0; i < lst2->counter; i++)
    {
        int i2 = (lst2->first + i) % lst2->physicalSize;
        lst->items[lst->last] = lst2->items[i2];
        lst->last = (lst->last + 1) % lst->physicalSize;
    }
    
    lst->counter += lst2->counter;
    lst2->counter = 0;
    lst2->first = 0;
    lst2->last = 0;
    
    pthread_mutex_unlock(&lst2->lock);
    pthread_cond_broadcast(&lst2->notFull);
    pthread_cond_broadcast(&lst->notEmpty);
    pthread_mutex_unlock(&lst->lock);
}
void setMaxSize(TList* lst, int newSize)
{
    pthread_mutex_lock(&lst->lock);
    if (newSize < lst->maxSize) {
        void **newItems = (void *)malloc(lst->physicalSize * sizeof(void *));
        for (int i = 0; i < lst->counter; i++) {
            newItems[i] = lst->items[(lst->first + i) % lst->physicalSize];
        }
        free(lst->items); 
        lst->items = newItems; 
        lst->first = 0; 
        lst->last = lst->counter; 
        lst->maxSize = newSize; 
        //pthread_mutex_unlock(&lst->lock);
    }
    else{
        void **newItems = (void *)malloc(newSize * sizeof(void *));
        for (int i = 0; i < lst->counter; i++) {
            newItems[i] = lst->items[(lst->first + i) % lst->physicalSize];
        }
        free(lst->items); 
        lst->items = newItems; 
        lst->first = 0; 
        lst->last = lst->counter; 
        lst->physicalSize = newSize;
        lst->maxSize = newSize; 
        pthread_cond_broadcast(&lst->notFull);
    }
    
    //pthread_cond_broadcast(&lst->notFull);
    pthread_mutex_unlock(&lst->lock);
}
void destroyList(TList* lst)
{
    if (!lst) return;
    pthread_mutex_lock(&lst->lock);
    for (int i = 0; i < lst->counter; i++) {
        int i2 = (lst->first + i) % lst->physicalSize; 
        free(lst->items[i2]);
    }
    free(lst->items);
    pthread_mutex_unlock(&lst->lock);
    pthread_mutex_destroy(&lst->lock);
    pthread_cond_destroy(&lst->notEmpty);
    pthread_cond_destroy(&lst->notFull);
    free(lst);
}
int getCount(TList* lst)
{
    pthread_mutex_lock(&lst->lock);
    int count = lst->counter;
    pthread_mutex_unlock(&lst->lock);
    return count;
}
void showList(TList* lst)
{
    pthread_mutex_lock(&lst->lock);
    printf("List: ");
    for (int i = 0; i < lst->counter; i++) {
        printf("%d ", *((int*)lst->items[(lst->first + i) % lst->physicalSize]));
    }
    printf("\n");
    pthread_mutex_unlock(&lst->lock);
}
void* popItem(TList *lst) {
    pthread_mutex_lock(&lst->lock); 

    while (lst->counter == 0) {
        pthread_cond_wait(&lst->notEmpty, &lst->lock); 
    }

    void *item = lst->items[(lst->last - 1 + lst->physicalSize) % lst->physicalSize];
    lst->last = (lst->last - 1 + lst->physicalSize) % lst->physicalSize;
    lst->counter--; 

    pthread_cond_signal(&lst->notFull);
    pthread_mutex_unlock(&lst->lock); 
    return item; 
}