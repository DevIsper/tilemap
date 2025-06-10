#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

void limparAllegro();

#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "model/valueobjects/Tipo.cpp"
#include "model/Tile.h"
#include "model/Jogador.h"
#include "model/Item.h"

const int MAPA_LARGURA = 50;
const int MAPA_ALTURA = 30;
const int TAMANHO_TILE = 20;

const float ESCALA_JANELA = 2.5f;

int LARGURA_JANELA = static_cast<int>(MAPA_LARGURA * TAMANHO_TILE * ESCALA_JANELA);
int ALTURA_JANELA = static_cast<int>(MAPA_ALTURA * TAMANHO_TILE * ESCALA_JANELA);

const int UI_OFFSET_X = 20;
const int UI_OFFSET_Y = 20;
const int UI_ITEM_GAP = 5;
const int UI_ITEM_SIZE = 48;

const int ITENS_PARA_VITORIA = 6;

ALLEGRO_DISPLAY *display = nullptr;
ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
ALLEGRO_FONT *font = nullptr;
ALLEGRO_FONT *ui_font = nullptr;

ALLEGRO_BITMAP *chao_texture = nullptr;
ALLEGRO_BITMAP *parede_texture = nullptr;

ALLEGRO_BITMAP *moeda_texture = nullptr;
ALLEGRO_BITMAP *pocao_texture = nullptr;
ALLEGRO_BITMAP *chave_texture = nullptr;

ALLEGRO_BITMAP *player1_texture = nullptr;
ALLEGRO_BITMAP *player2_texture = nullptr;

std::vector<std::vector<int>> mapa_layout(MAPA_ALTURA, std::vector<int>(MAPA_LARGURA));
std::vector<std::vector<Tile>> mapa_allegro(MAPA_ALTURA, std::vector<Tile>(MAPA_LARGURA));
std::vector<Jogador> jogadores;
std::vector<Item> itens_no_mapa;
std::vector<Item> itens_coletados_player1;
std::vector<Item> itens_coletados_player2;

bool game_over = false;
int vencedor = 0;

bool inicializarAllegro() {
    if (!al_init()) {
        std::cerr << "ERRO: Falha ao inicializar o Allegro!" << std::endl;
        return false;
    }
    if (!al_init_primitives_addon()) {
        std::cerr << "ERRO: Falha ao inicializar o addon de primitivas! Certifique-se que ALLEGRO_PRIMITIVES_ADDON está habilitado." << std::endl;
        al_uninstall_system();
        return false;
    }
    if (!al_install_keyboard()) {
        std::cerr << "AVISO: Falha ao inicializar o teclado." << std::endl;
    }
    al_init_font_addon();
    if (!al_init_ttf_addon()) {
        std::cerr << "ERRO: Falha ao inicializar o addon TTF de fonte!" << std::endl;
        limparAllegro();
        return false;
    }
    if (!al_init_image_addon()) {
        std::cerr << "ERRO: Falha ao inicializar o addon de imagem!" << std::endl;
        limparAllegro();
        return false;
    }
    font = al_load_font("../assets/arial.ttf", 96, 0);
    if (!font) {
        std::cerr << "AVISO: Falha ao carregar arial.ttf da pasta assets. Tentando fonte embutida para mensagem de vitória..." << std::endl;
        font = al_create_builtin_font();
        if (!font) {
            std::cerr << "ERRO: Falha ao criar fonte embutida! O texto não será exibido." << std::endl;
        }
    }

    ui_font = al_load_font("../assets/arial.ttf", 36, 0);
    if (!ui_font) {
        std::cerr << "AVISO: Falha ao carregar arial.ttf da pasta assets. Tentando fonte embutida para UI..." << std::endl;
        ui_font = al_create_builtin_font();
        if (!ui_font) {
            std::cerr << "ERRO: Falha ao criar fonte embutida para UI! O texto da UI não será exibido." << std::endl;
        }
    }

    chao_texture = al_load_bitmap("../assets/chao1.png");
    if (!chao_texture) {
        std::cerr << "ERRO: Falha ao carregar assets/chao1.png! Verifique o caminho e o arquivo." << std::endl;
        limparAllegro();
        return false;
    }

    parede_texture = al_load_bitmap("../assets/parede.png");
    if (!parede_texture) {
        std::cerr << "ERRO: Falha ao carregar assets/parede.png! Verifique o caminho e o arquivo." << std::endl;
        limparAllegro();
        return false;
    }

    moeda_texture = al_load_bitmap("../assets/banana.png");
    if (!moeda_texture) {
        std::cerr << "ERRO: Falha ao carregar assets/banana.png! Verifique o caminho e o arquivo." << std::endl;
        limparAllegro();
        return false;
    }

    pocao_texture = al_load_bitmap("../assets/uva.png");
    if (!pocao_texture) {
        std::cerr << "ERRO: Falha ao carregar assets/uva.png! Verifique o caminho e o arquivo." << std::endl;
        limparAllegro();
        return false;
    }

    chave_texture = al_load_bitmap("../assets/cereja.png");
    if (!chave_texture) {
        std::cerr << "ERRO: Falha ao carregar assets/cereja.png! Verifique o caminho e o arquivo." << std::endl;
        limparAllegro();
        return false;
    }

    player1_texture = al_load_bitmap("../assets/player1.png");
    if (!player1_texture) {
        std::cerr << "ERRO: Falha ao carregar assets/player1.png! Verifique o caminho e o arquivo." << std::endl;
        limparAllegro();
        return false;
    }

    player2_texture = al_load_bitmap("../assets/player2.png");
    if (!player2_texture) {
        std::cerr << "ERRO: Falha ao carregar assets/player2.png! Verifique o caminho e o arquivo." << std::endl;
        limparAllegro();
        return false;
    }

    return true;
}

void limparAllegro() {
    if (chao_texture) {
        al_destroy_bitmap(chao_texture);
        chao_texture = nullptr;
    }
    if (parede_texture) {
        al_destroy_bitmap(parede_texture);
        parede_texture = nullptr;
    }
    if (moeda_texture) {
        al_destroy_bitmap(moeda_texture);
        moeda_texture = nullptr;
    }
    if (pocao_texture) {
        al_destroy_bitmap(pocao_texture);
        pocao_texture = nullptr;
    }
    if (chave_texture) {
        al_destroy_bitmap(chave_texture);
        chave_texture = nullptr;
    }
    if (player1_texture) {
        al_destroy_bitmap(player1_texture);
        player1_texture = nullptr;
    }
    if (player2_texture) {
        al_destroy_bitmap(player2_texture);
        player2_texture = nullptr;
    }

    if (font) {
        al_destroy_font(font);
        font = nullptr;
    }
    if (ui_font) {
        al_destroy_font(ui_font);
        ui_font = nullptr;
    }
    if (event_queue) {
        al_destroy_event_queue(event_queue);
        event_queue = nullptr;
    }
    if (display) {
        al_destroy_display(display);
        display = nullptr;
    }

    al_shutdown_primitives_addon();
    al_shutdown_font_addon();
    al_shutdown_image_addon();
    al_uninstall_system();
}

void desenharMapa(const std::vector<std::vector<Tile>>& mapa) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    for (int y = 0; y < MAPA_ALTURA; ++y) {
        for (int x = 0; x < MAPA_LARGURA; ++x) {
            float pos_x = static_cast<float>(x * TAMANHO_TILE * ESCALA_JANELA);
            float pos_y = static_cast<float>(y * TAMANHO_TILE * ESCALA_JANELA);

            if (mapa[y][x].getTipoTile() == CHAO) {
                al_draw_scaled_bitmap(chao_texture,
                                      0, 0,
                                      al_get_bitmap_width(chao_texture), al_get_bitmap_height(chao_texture),
                                      pos_x, pos_y,
                                      TAMANHO_TILE * ESCALA_JANELA, TAMANHO_TILE * ESCALA_JANELA,
                                      0);
            } else {
                al_draw_scaled_bitmap(parede_texture,
                                      0, 0,
                                      al_get_bitmap_width(parede_texture), al_get_bitmap_height(parede_texture),
                                      pos_x, pos_y,
                                      TAMANHO_TILE * ESCALA_JANELA, TAMANHO_TILE * ESCALA_JANELA,
                                      0);
            }
        }
    }
}

void desenharJogadores(const std::vector<Jogador>& jogadores) {
    float draw_player_size = TAMANHO_TILE * ESCALA_JANELA;

    float img_w1 = al_get_bitmap_width(player1_texture);
    float img_h1 = al_get_bitmap_height(player1_texture);
    float offset_x1 = (draw_player_size / 2) - (img_w1 * (draw_player_size / TAMANHO_TILE) / 2);
    float offset_y1 = (draw_player_size / 2) - (img_h1 * (draw_player_size / TAMANHO_TILE) / 2);

    float img_w2 = al_get_bitmap_width(player2_texture);
    float img_h2 = al_get_bitmap_height(player2_texture);
    float offset_x2 = (draw_player_size / 2) - (img_w2 * (draw_player_size / TAMANHO_TILE) / 2);
    float offset_y2 = (draw_player_size / 2) - (img_h2 * (draw_player_size / TAMANHO_TILE) / 2);

    if (jogadores.size() > 0) {
        al_draw_scaled_bitmap(player1_texture,
                              0, 0,
                              img_w1, img_h1,
                              jogadores[0].getX() * TAMANHO_TILE * ESCALA_JANELA + offset_x1,
                              jogadores[0].getY() * TAMANHO_TILE * ESCALA_JANELA + offset_y1,
                              img_w1 * (draw_player_size / TAMANHO_TILE), img_h1 * (draw_player_size / TAMANHO_TILE),
                              0);
    }
    if (jogadores.size() > 1) {
        al_draw_scaled_bitmap(player2_texture,
                              0, 0,
                              img_w2, img_h2,
                              jogadores[1].getX() * TAMANHO_TILE * ESCALA_JANELA + offset_x2,
                              jogadores[1].getY() * TAMANHO_TILE * ESCALA_JANELA + offset_y2,
                              img_w2 * (draw_player_size / TAMANHO_TILE), img_h2 * (draw_player_size / TAMANHO_TILE),
                              0);
    }
}

void desenharItensNoMapa(const std::vector<Item>& itens) {
    for (const auto& item : itens) {
        float pos_x = static_cast<float>(item.getX() * TAMANHO_TILE * ESCALA_JANELA);
        float pos_y = static_cast<float>(item.getY() * TAMANHO_TILE * ESCALA_JANELA);

        ALLEGRO_BITMAP* current_item_texture = nullptr;

        if (item.getTipo() == MOEDA) {
            current_item_texture = moeda_texture;
        } else if (item.getTipo() == POCAO) {
            current_item_texture = pocao_texture;
        } else if (item.getTipo() == CHAVE) {
            current_item_texture = chave_texture;
        }

        if (current_item_texture) {
            float img_w = al_get_bitmap_width(current_item_texture);
            float img_h = al_get_bitmap_height(current_item_texture);
            float draw_item_size = TAMANHO_TILE * ESCALA_JANELA;
            float draw_x = pos_x + (draw_item_size / 2) - (img_w * (draw_item_size / TAMANHO_TILE) / 2);
            float draw_y = pos_y + (draw_item_size / 2) - (img_h * (draw_item_size / TAMANHO_TILE) / 2);

            al_draw_scaled_bitmap(current_item_texture,
                                  0, 0,
                                  img_w, img_h,
                                  draw_x, draw_y,
                                  img_w * (draw_item_size / TAMANHO_TILE), img_h * (draw_item_size / TAMANHO_TILE),
                                  0);
        } else {
            ALLEGRO_COLOR item_color;
            if (item.getTipo() == MOEDA) item_color = al_map_rgb(255, 223, 0);
            else if (item.getTipo() == POCAO) item_color = al_map_rgb(128, 0, 128);
            else if (item.getTipo() == CHAVE) item_color = al_map_rgb(255, 255, 0);
            else item_color = al_map_rgb(200, 200, 200);

            al_draw_filled_circle(pos_x + (TAMANHO_TILE * ESCALA_JANELA) / 2,
                                 pos_y + (TAMANHO_TILE * ESCALA_JANELA) / 2,
                                 (TAMANHO_TILE * ESCALA_JANELA) / 4,
                                 item_color);
        }
    }
}

void desenharItensColetadosUI(const std::vector<Item>& itens_coletados, int player_id) {
    int ui_start_x;
    int ui_start_y = ALTURA_JANELA - (UI_ITEM_SIZE + UI_OFFSET_Y + 20);

    if (player_id == 1) {
        ui_start_x = UI_OFFSET_X;
    } else {
        ui_start_x = LARGURA_JANELA - (ITENS_PARA_VITORIA * (UI_ITEM_SIZE + UI_ITEM_GAP)) - UI_ITEM_GAP - UI_OFFSET_X - 100;
    }

    float ui_bg_width = (ITENS_PARA_VITORIA * (UI_ITEM_SIZE + UI_ITEM_GAP)) + UI_ITEM_GAP;
    al_draw_filled_rectangle(ui_start_x - 5, ui_start_y - 5,
                             ui_start_x + ui_bg_width + 5,
                             ui_start_y + UI_ITEM_SIZE + 5,
                             al_map_rgb(40, 40, 40));

    for (size_t i = 0; i < itens_coletados.size(); ++i) {
        const auto& item = itens_coletados[i];
        ALLEGRO_BITMAP* ui_item_texture = nullptr;

        if (item.getTipo() == MOEDA) {
            ui_item_texture = moeda_texture;
        } else if (item.getTipo() == POCAO) {
            ui_item_texture = pocao_texture;
        } else if (item.getTipo() == CHAVE) {
            ui_item_texture = chave_texture;
        }

        if (ui_item_texture) {
            al_draw_scaled_bitmap(ui_item_texture,
                                  0, 0,
                                  al_get_bitmap_width(ui_item_texture), al_get_bitmap_height(ui_item_texture),
                                  ui_start_x + (i * (UI_ITEM_SIZE + UI_ITEM_GAP)),
                                  ui_start_y,
                                  UI_ITEM_SIZE, UI_ITEM_SIZE,
                                  0);
        } else {
            ALLEGRO_COLOR item_color;
            if (item.getTipo() == MOEDA) item_color = al_map_rgb(255, 223, 0);
            else if (item.getTipo() == POCAO) item_color = al_map_rgb(128, 0, 128);
            else if (item.getTipo() == CHAVE) item_color = al_map_rgb(255, 255, 0);
            else item_color = al_map_rgb(200, 200, 200);

            al_draw_filled_rectangle(ui_start_x + (i * (UI_ITEM_SIZE + UI_ITEM_GAP)),
                                     ui_start_y,
                                     ui_start_x + (i * (UI_ITEM_SIZE + UI_ITEM_GAP)) + UI_ITEM_SIZE,
                                     ui_start_y + UI_ITEM_SIZE,
                                     item_color);
        }
    }
    if (ui_font) {
        int text_x_offset;
        if (player_id == 1) {
            text_x_offset = ui_start_x + ui_bg_width + 10;
        } else {
            text_x_offset = ui_start_x - 10;
        }
        al_draw_textf(ui_font, al_map_rgb(255, 255, 255), text_x_offset, ui_start_y + (UI_ITEM_SIZE / 2) - (al_get_font_line_height(ui_font) / 2),
                      (player_id == 1 ? 0 : ALLEGRO_ALIGN_RIGHT),
                      "P%d: %d/%d", player_id, (int)itens_coletados.size(), ITENS_PARA_VITORIA);
    }
}

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

bool is_valid_maze_pos(int r, int c) {
    return r >= 1 && r < MAPA_ALTURA - 1 && c >= 1 && c < MAPA_LARGURA - 1;
}

void generateMaze(std::vector<std::vector<int>>& maze) {
    for (int y = 0; y < MAPA_ALTURA; ++y) {
        for (int x = 0; x < MAPA_LARGURA; ++x) {
            maze[y][x] = 1;
        }
    }

    int start_y = (std::uniform_int_distribution<>(1, (MAPA_ALTURA - 2) / 2)(rng) * 2) + 1;
    int start_x = (std::uniform_int_distribution<>(1, (MAPA_LARGURA - 2) / 2)(rng) * 2) + 1;

    std::vector<std::pair<int, int>> stack;
    stack.push_back({start_y, start_x});
    maze[start_y][start_x] = 0;

    std::vector<std::pair<int, int>> directions = { {-2, 0}, {2, 0}, {0, -2}, {0, 2} };

    while (!stack.empty()) {
        int current_y = stack.back().first;
        int current_x = stack.back().second;
        stack.pop_back();

        std::shuffle(directions.begin(), directions.end(), rng);

        for (const auto& dir : directions) {
            int next_y = current_y + dir.first;
            int next_x = current_x + dir.second;

            int wall_y = current_y + dir.first / 2;
            int wall_x = current_x + dir.second / 2;

            if (is_valid_maze_pos(next_y, next_x) && maze[next_y][next_x] == 1) {
                maze[next_y][next_x] = 0;
                maze[wall_y][wall_x] = 0;
                stack.push_back({current_y, current_x});
                stack.push_back({next_y, next_x});
            }
        }
    }

    for (int y = 0; y < MAPA_ALTURA; ++y) {
        maze[y][0] = 1;
        maze[y][MAPA_LARGURA - 1] = 1;
    }
    for (int x = 0; x < MAPA_LARGURA; ++x) {
        maze[0][x] = 1;
        maze[MAPA_ALTURA - 1][x] = 1;
    }
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

    al_set_window_title(display, "Jardim das Guloseimas");

    event_queue = al_create_event_queue();
    if (!event_queue) {
        std::cerr << "ERRO: Falha ao criar a fila de eventos!" << std::endl;
        limparAllegro();
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    generateMaze(mapa_layout);

    for (int y = 0; y < MAPA_ALTURA; ++y) {
        for (int x = 0; x < MAPA_LARGURA; ++x) {
            mapa_allegro[y][x].setTipoTile(static_cast<Tipo>(mapa_layout[y][x]));
        }
    }

    int j1_start_x = 1, j1_start_y = 1;
    while (mapa_layout[j1_start_y][j1_start_x] == PAREDE) {
        j1_start_x++;
        if (j1_start_x >= MAPA_LARGURA - 1) {
            j1_start_x = 1;
            j1_start_y++;
        }
    }
    jogadores.emplace_back(j1_start_x, j1_start_y);

    int j2_start_x = MAPA_LARGURA - 2, j2_start_y = MAPA_ALTURA - 2;
    while (mapa_layout[j2_start_y][j2_start_x] == PAREDE) {
        j2_start_x--;
        if (j2_start_x <= 1) {
            j2_start_x = MAPA_LARGURA - 2;
            j2_start_y--;
        }
    }
    jogadores.emplace_back(j2_start_x, j2_start_y);

    std::uniform_int_distribution<> dis_x(1, MAPA_LARGURA - 2);
    std::uniform_int_distribution<> dis_y(1, MAPA_ALTURA - 2);

    std::vector<TipoItem> tipos_disponiveis = {MOEDA, MOEDA, MOEDA, POCAO, POCAO, POCAO, CHAVE, CHAVE, MOEDA, POCAO};

    for (int i = 0; i < 10; ++i) {
        int item_x, item_y;
        do {
            item_x = dis_x(rng);
            item_y = dis_y(rng);
        } while (mapa_layout[item_y][item_x] == PAREDE);

        itens_no_mapa.emplace_back(item_x, item_y, tipos_disponiveis[i % tipos_disponiveis.size()]);
    }

    desenharMapa(mapa_allegro);
    desenharItensNoMapa(itens_no_mapa);
    desenharJogadores(jogadores);
    desenharItensColetadosUI(itens_coletados_player1, 1);
    desenharItensColetadosUI(itens_coletados_player2, 2);
    al_flip_display();

    bool running = true;
    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN && !game_over) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_W) {
                jogadores[0].mover(0, -1, mapa_layout, MAPA_LARGURA, MAPA_ALTURA);
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_S) {
                jogadores[0].mover(0, 1, mapa_layout, MAPA_LARGURA, MAPA_ALTURA);
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_A) {
                jogadores[0].mover(-1, 0, mapa_layout, MAPA_LARGURA, MAPA_ALTURA);
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_D) {
                jogadores[0].mover(1, 0, mapa_layout, MAPA_LARGURA, MAPA_ALTURA);
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
                jogadores[1].mover(0, -1, mapa_layout, MAPA_LARGURA, MAPA_ALTURA);
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                jogadores[1].mover(0, 1, mapa_layout, MAPA_LARGURA, MAPA_ALTURA);
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                jogadores[1].mover(-1, 0, mapa_layout, MAPA_LARGURA, MAPA_ALTURA);
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                jogadores[1].mover(1, 0, mapa_layout, MAPA_LARGURA, MAPA_ALTURA);
            }
        }
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            running = false;
        }

        if (!game_over) {
            std::vector<Item> itens_coletados_neste_frame;
            std::vector<int> indices_a_remover;

            for (size_t i = 0; i < itens_no_mapa.size(); ++i) {
                if (itens_no_mapa[i].getX() == jogadores[0].getX() && itens_no_mapa[i].getY() == jogadores[0].getY()) {
                    itens_coletados_player1.push_back(itens_no_mapa[i]);
                    indices_a_remover.push_back(i);
                    std::cout << "Porcão Bola Bola coletou um item tipo " << itens_no_mapa[i].getTipo() << std::endl;
                }
                else if (itens_no_mapa[i].getX() == jogadores[1].getX() && itens_no_mapa[i].getY() == jogadores[1].getY()) {
                    itens_coletados_player2.push_back(itens_no_mapa[i]);
                    indices_a_remover.push_back(i);
                    std::cout << "The King The Power The Best coletou um item tipo " << itens_no_mapa[i].getTipo() << std::endl;
                }
            }

            std::sort(indices_a_remover.rbegin(), indices_a_remover.rend());
            for (int idx : indices_a_remover) {
                itens_no_mapa.erase(itens_no_mapa.begin() + idx);
            }

            if (itens_coletados_player1.size() >= ITENS_PARA_VITORIA) {
                game_over = true;
                vencedor = 1;
            } else if (itens_coletados_player2.size() >= ITENS_PARA_VITORIA) {
                game_over = true;
                vencedor = 2;
            }
        }

        desenharMapa(mapa_allegro);
        desenharItensNoMapa(itens_no_mapa);
        desenharJogadores(jogadores);
        desenharItensColetadosUI(itens_coletados_player1, 1);
        desenharItensColetadosUI(itens_coletados_player2, 2);

        if (game_over && font) {
            std::string msg = "Jogador " + std::to_string(vencedor) + " VENCEU!";
            al_draw_text(font, al_map_rgb(255, 0, 0), LARGURA_JANELA / 2, ALTURA_JANELA / 2 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, msg.c_str());
        }

        al_flip_display();
    }

    limparAllegro();
    return 0;
}