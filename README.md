===============================
Lista o ograniczonej pojemności 
===============================
-------------------------------------
Programowanie systemowe i współbieżne
-------------------------------------


:Author: Jakub Karcz 160117 <jakub.karcz@student.put.poznan.pl>
:Date:   v1.0, 2025-01-25


.. highlight:: c


Struktury danych
================

1. Struktura ``TList``::

      typedef struct {
          void **items;
          int maxSize;
          int counter;
          int head;
          int tail;
          pthread_mutex_t lock;
          pthread_cond_t notEmpty;
          pthread_cond_t notFull;
      } TList;
Funkcje
=======

W implementacji zawarto kilka podstawowych funkcji służących do zarządzania listą współbieżną. Każda z tych funkcji chroni operacje na liście za pomocą muteksów oraz zmiennych warunkowych, aby umożliwić współpracę wielu wątków.

1. ``TList *createList(int s)`` -- tworzy nową listę o maksymalnym rozmiarze ``s``.
   Inicjalizuje dynamicznie przydzieloną pamięć oraz muteksy i zmienne warunkowe.

2. ``void putItem(TList* lst, void *itm)`` -- dodaje element ``itm`` do listy ``lst``.
   Funkcja blokuje wątek, jeśli lista jest pełna, i czeka na sygnał zwolnienia miejsca w liście.

3. ``void *getItem(TList* lst)`` -- pobiera pierwszy element z listy ``lst``.
   Funkcja blokuje wątek, jeśli lista jest pusta, i czeka na sygnał o pojawieniu się elementu.

4. ``void* popItem(TList* lst)`` -- pobiera ostatni element z listy ``lst``.
   Funkcja blokuje wątek, jeśli lista jest pusta, i czeka na sygnał o pojawieniu się elementu. Po pobraniu elementu, licznik zostaje zmniejszony, a wątek czeka na możliwość dodania nowego elementu, jeśli lista była pełna.


5. ``int removeItem(TList* lst, void *itm)`` -- usuwa element ``itm`` z listy, jeśli taki istnieje.
   Elementy za usuniętym elementem są przesuwane w celu utrzymania ciągłości listy.

6. ``void showList(TList* lst)`` -- wyświetla zawartość listy, drukując wskaźniki na elementy.

7. ``int getCount(TList* lst)`` -- zwraca aktualną liczbę elementów w liście ``lst``.

8. ``void setMaxSize(TList* lst, int s)`` -- ustala nowy maksymalny rozmiar listy ``lst``.
   Jeśli nowy rozmiar jest większy, zwalnia blokujące wątki.

9. ``void appendItems(TList* lst, TList* lst2)`` -- dodaje wszystkie elementy z listy ``lst2`` do listy ``lst``.
   W razie potrzeby zwiększa rozmiar tablicy ``lst``.

10. ``void destroyList(TList* lst)`` -- usuwa listę ``lst``, zwalniając pamięć i niszcząc muteksy oraz warunki.

Algorytm / dodatkowy opis
=========================

1. **Sytuacje skrajne:**
   
   - **Pełna lista:** Jeśli lista osiągnie swój maksymalny rozmiar, wątek próbujący dodać element zostaje zablokowany do momentu zwolnienia miejsca w liście.
   - **Pusta lista:** Jeśli lista jest pusta, wątek próbujący pobrać element zostaje zablokowany do momentu pojawienia się elementu.

2. **Odporność na problemy współbieżności:**
   
   - **Zakleszczenie:** Algorytm unika zakleszczenia, dzięki warunkom synchronizacji i odpowiedniemu sygnalizowaniu zdarzeń. Warunki ``notEmpty`` i ``notFull`` zapewniają, że wątki nie czekają bez końca.
   - **Aktywne oczekiwanie:** Dzięki zmiennym warunkowym wątki czekają efektywnie, zamiast aktywnie oczekiwać na spełnienie warunku.

Przykład użycia
===============

::

	#include <stdio.h>
	#include <stdlib.h>
	#include <pthread.h>
	#include <unistd.h>
	#include "list.h"

	int main() {
    		TList *lst = createList(5);
    		pthread_t producer, consumer;
    		int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6;
    		putItem(lst, &a);
    		putItem(lst, &b);
    		putItem(lst, &c);
    		putItem(lst, &d);
   		putItem(lst, &e);
    		removeItem(lst, &d);
    		putItem(lst, &f);
    		showList(lst);
    		return 0;
}

--------------------------------------------------------------------------------
