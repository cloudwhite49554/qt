#ifndef BOOK_H
#define BOOK_H

#include "gameobject.h"

class Book : public GameObject
{
public:
    explicit Book(QWidget *parent = nullptr);
    int num;
};

#endif // BOOK_H
