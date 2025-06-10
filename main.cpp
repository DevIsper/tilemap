// main.cpp
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <cstdlib>
#include "model/Tile.h"
#include "model/Jogador.h"

// Dimensões do mapa - Mantidas em 50x30
const int MAPA_LARGURA = 50;
const int MAPA_ALTURA = 30;
// Tamanho em pixels de cada tile - Mantido em 20 para boa visibilidade
const int TAMANHO_TILE = 20;

// Dimensões da janela - Calculadas
const int LARGURA_JANELA = MAPA_LARGURA * TAMANHO_TILE;
const int ALTURA_JANELA = MAPA_ALTURA * TAMANHO_TILE;

ALLEGRO_DISPLAY *display = nullptr;
ALLEGRO_EVENT_QUEUE *event_queue = nullptr;

bool inicializarAllegro() {
    if (!al_init()) {
        std::cerr << "ERRO: Falha ao inicializar o Allegro!" << std::endl;
        return false;
    }
    if (!al_init_primitives_addon()) {
        std::cerr << "ERRO: Falha ao inicializar o addon de primitivas!" << std::endl;
        al_uninstall_system();
        return false;
    }
    if (!al_install_keyboard()) {
        std::cerr << "AVISO: Falha ao inicializar o teclado." << std::endl;
    }
    return true;
}

void limparAllegro() {
    if (event_queue) {
        al_destroy_event_queue(event_queue);
    }
    if (display) {
        al_destroy_display(display);
    }
    al_shutdown_primitives_addon();
    al_uninstall_system();
}

void desenharMapa(const std::vector<std::vector<Tile>>& mapa) {
    al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpa a tela para preto

    for (int y = 0; y < MAPA_ALTURA; ++y) {
        for (int x = 0; x < MAPA_LARGURA; ++x) {
            float pos_x = static_cast<float>(x * TAMANHO_TILE);
            float pos_y = static_cast<float>(y * TAMANHO_TILE);

            if (mapa[y][x].getTipoTile() == CHAO) {
                // Cor para o CHAO (um cinza claro para contrastar com preto)
                al_draw_filled_rectangle(pos_x, pos_y, pos_x + TAMANHO_TILE, pos_y + TAMANHO_TILE, al_map_rgb(100, 100, 100));
            } else { // PAREDE
                // Cor para a PAREDE (BRANCA)
                al_draw_filled_rectangle(pos_x, pos_y, pos_x + TAMANHO_TILE, pos_y + TAMANHO_TILE, al_map_rgb(255, 255, 255));
            }
        }
    }
    al_flip_display(); // Atualiza a tela após desenhar
}

int main(int argc, char **argv) {
    if (!inicializarAllegro()) {
        limparAllegro();
        return -1;
    }

    display = al_create_display(LARGURA_JANELA, ALTURA_JANELA);
    if (!display) {
        std::cerr << "ERRO: Falha ao criar a janela! Verifique as dimensões ou drivers gráficos." << std::endl;
        limparAllegro();
        return -1;
    }

    al_set_window_title(display, "The King");

    event_queue = al_create_event_queue();
    if (!event_queue) {
        std::cerr << "ERRO: Falha ao criar a fila de eventos!" << std::endl;
        limparAllegro();
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    const std::vector<std::vector<int>> mapa_layout = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    // Preenche o mapa de Tiles usando o mapa_layout
    std::vector<std::vector<Tile>> mapa_allegro(MAPA_ALTURA, std::vector<Tile>(MAPA_LARGURA));
    for (int y = 0; y < MAPA_ALTURA; ++y) {
        for (int x = 0; x < MAPA_LARGURA; ++x) {
            if (mapa_layout[y][x] == 0) {
                mapa_allegro[y][x].setTipoTile(CHAO);
            } else {
                mapa_allegro[y][x].setTipoTile(PAREDE);
            }
        }
    }

    // Desenha o mapa uma vez assim que a janela é criada
    desenharMapa(mapa_allegro);

    // Loop principal de eventos
    bool running = true;
    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                running = false;
            }
        }
    }

    // Limpeza final de recursos
    limparAllegro();

    return 0;
}