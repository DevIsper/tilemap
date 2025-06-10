//
// Created by Gabriel Isper on 10/06/25.
//

#ifndef JOGADOR_H
#define JOGADOR_H



class Jogador {
    int x;
    int y;

public:
    Jogador(int x, int y)
    {
        this->x = x,
        this->y = y;
    }

    int getX() const
    {
        return this->x;
    }

    void setX(int x)
    {
        this->x = x;
    }

    int getY() const
    {
        return this->y;
    }

    void setY(int y)
    {
        this->y = y;
    }

};



#endif //JOGADOR_H
