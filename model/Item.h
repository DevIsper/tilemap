// model/Item.h
#ifndef ITEM_H
#define ITEM_H

#include "valueobjects/Tipo.cpp" // Para TipoItem

class Item {
    int x;
    int y;
    TipoItem tipo;

public:
    Item(int x, int y, TipoItem tipo) : x(x), y(y), tipo(tipo) {}

    int getX() const { return this->x; }
    int getY() const { return this->y; }
    TipoItem getTipo() const { return this->tipo; }

    // Setters para mover o item na tela (útil para a área de UI)
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
};

#endif // ITEM_H