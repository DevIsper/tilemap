// model/Jogador.h
#ifndef JOGADOR_H
#define JOGADOR_H

#include <vector>

class Jogador {
    int x;
    int y;
    // Sem inventário aqui, conforme combinado

public:
    Jogador(int x, int y) : x(x), y(y) {}

    int getX() const { return this->x; }
    void setX(int x) { this->x = x; }
    int getY() const { return this->y; }
    void setY(int y) { this->y = y; }

    // Método para tentar mover o jogador, verificando paredes
    bool mover(int delta_x, int delta_y,
               const std::vector<std::vector<int>>& mapa_layout,
               int mapa_largura, int mapa_altura) {
        int nova_x = x + delta_x;
        int nova_y = y + delta_y;

        // 1. Verificar limites do mapa
        if (nova_x < 0 || nova_x >= mapa_largura || nova_y < 0 || nova_y >= mapa_altura) {
            return false; // Fora dos limites
        }

        // 2. Verificar colisão com paredes (presume que '1' é parede no mapa_layout)
        if (mapa_layout[nova_y][nova_x] == 1) {
            return false; // Colidiu com parede
        }

        // Se passou pelas verificações, move o jogador
        this->x = nova_x;
        this->y = nova_y;
        return true;
    }
};

#endif // JOGADOR_H