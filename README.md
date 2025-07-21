# 💻 TList – Concurrent Bounded List in C

![Language](https://img.shields.io/badge/Language-C-blue.svg?logo=c)
![Threads](https://img.shields.io/badge/POSIX-Pthreads-green?logo=linux)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

> System-level concurrent bounded list using **POSIX threads**.  
> Handles multiple producers/consumers with blocking behavior and dynamic resizing.

A thread-safe, capacity-limited list designed for concurrent programming assignments.  
Implements mutexes and condition variables to safely coordinate access between threads.

---

## 📦 Table of Contents

- [📝 Description](#-description)
- [🚀 Features](#-features)
- [🧪 Example Usage](#-example-usage)
- [🔧 Installation](#-installation)
- [⚙️ Functions](#️-functions)
- [🧠 Concurrency Details](#-concurrency-details)
- [📁 Project Structure](#-project-structure)
- [📜 License](#-license)

---

## 📝 Description

`TList` is a fixed-size concurrent list with blocking behavior:

- Threads attempting to **insert** into a full list are blocked.
- Threads attempting to **remove** from an empty list are blocked.
- Supports safe concurrent operations using **mutexes** and **condition variables**.

It is ideal for learning and practicing **producer-consumer** problems and synchronization in C.

---

## 🚀 Features

- 🔒 Thread-safe `put`, `get`, `pop`, and `remove` operations
- ⛔ Blocking behavior with condition variables (no busy waiting)
- 🧩 Dynamic resizing support (`setMaxSize`)
- 🔁 List merging support (`appendItems`)
- 🧹 Clean memory and synchronization resource cleanup

---

## 🧪 Example Usage

```c
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main() {
    TList *lst = createList(5);
    int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6;

    putItem(lst, &a);
    putItem(lst, &b);
    putItem(lst, &c);
    putItem(lst, &d);
    putItem(lst, &e);

    removeItem(lst, &d); // make space
    putItem(lst, &f);

    showList(lst);
    destroyList(lst);
    return 0;
}
