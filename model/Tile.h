#ifndef TILE_H
#define TILE_H

#include "valueobjects/Tipo.cpp"


class Tile {
    Tipo tipoTile;

public:
    Tile(Tipo tipo = CHAO, int esp = 0) : tipoTile(tipo) {}

    Tipo getTipoTile() const
    {
        return this->tipoTile;
    }

    void setTipoTile(Tipo tipoTile)
    {
        this->tipoTile = tipoTile;
    }
};



#endif //TILE_H
