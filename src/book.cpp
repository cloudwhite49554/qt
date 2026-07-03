#include "book.h"

Book::Book(QWidget *parent)
    : GameObject(parent)
{
    this->resize(40, 40);
    type="book";
    this->setImage(":/images/book.png");
}