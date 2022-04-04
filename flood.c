// Luan Machado Bernardt | GRR20190363

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"
#include "heuristic.h"
#include "globalvars.h"
#include "types.h"

int height, width;
short colors;
cell *initial_board;

//------------------------------------------------------------------------------
// Reseta as marcações das células do mapa
void reset_board(cell *game_board) {

    for (int i=1; i<height-1; ++i)
        for (int j=1; j<width-1; ++j)
            game_board[i * width + j].s_checked = false;
}

//------------------------------------------------------------------------------
// Atualiza a região principal do mapa, marcando as cells da fronteira que já eram da nova cor
// como flooded
void update_region (cell *game_board, int x, int y, short new_color) { // Atu

    if (game_board[x * width + y].s_color == 0)
        return;
    if (!game_board[x * width + y].s_checked)
        return;
    if (game_board[x * width + y].s_color == new_color) {
        game_board[x * width + y].s_checked = false;
        game_board[x * width + y].flooded = true;
    
        update_region(game_board, x+1, y, new_color);
        update_region(game_board, x, y+1, new_color);
        update_region(game_board, x-1, y, new_color);
        update_region(game_board, x, y-1, new_color);
    }
}

//------------------------------------------------------------------------------
// Realiza o flood da região principal do mapa com a nova cor
void flood(cell *game_board, int x, int y, short old_color, short new_color) {

    if (x < 0 || x > height - 1)
        return;
    if (y < 0 || y > width - 1)
        return;
    if (game_board[x * width + y].s_color == new_color) {
        update_region (game_board, x, y, new_color);
        return;
    }
    if (game_board[x * width + y].s_color != old_color) {
        return;
    }
    
    game_board[x * width + y].flooded = true;
    game_board[x * width + y].s_color = new_color;
    flood(game_board, x+1, y, old_color, new_color);
    flood(game_board, x, y+1, old_color, new_color);
    flood(game_board, x-1, y, old_color, new_color);
    flood(game_board, x, y-1, old_color, new_color);
}

//------------------------------------------------------------------------------
// Refaz o caminho da solução a partir do estado final, empilhando as cores utilizadas
// Faz essa especie de callback por meio dos ponteiros para o parent
void get_solution (state *final_state) {

    short *color_order = malloc (final_state->g * sizeof(short));
    state *aux = final_state;

    for (int i=0; i<final_state->g; ++i) {
        color_order[i] = aux->s_color;
        aux = aux->s_parent;
    }

    printf("%d\n", final_state->g);

    for (int i=final_state->g-1; i>=0; --i) {
        printf("%d ",color_order[i]);
    }

    printf("\n");
    free(color_order);
}

//------------------------------------------------------------------------------
// Função que analisa se o mapa está com apenas uma cor
int check_victory (cell *game_board) {

    for (int i=1; i<height-1; ++i)
        for (int j=1; j<width-1; ++j)
            if (game_board[width + 1].s_color != game_board[i * width + j].s_color)
                return 0;
    return 1;
}

//------------------------------------------------------------------------------
// Cria um nodo estado e o adicona a fila
// *OBS: Como o algoritmo de busca se trata de um UCS, não haveria a necessidade 
// de calcular o g e o f, no entanto optou-se por deixar, pois é possível transformar
// o UCS em A* sem problemas, apenas trocando  parâmetro da busca pel omelhor estado.
void create_state (short color, state *parent, state **queue) {
        
    state *new_state = (state *) malloc (sizeof(state));

    new_state->prev = new_state->next = NULL;
    new_state->s_color = color;
    
    new_state->border_colors = malloc (colors * sizeof(int));
    memset(new_state->border_colors, 0, colors * sizeof(int));

    new_state->state_board = (cell *) malloc (width * height * sizeof(cell));

    // Se o estado for o inicial
    if (!parent) {
        new_state->s_parent = NULL;
        new_state->g = 0;
        memcpy(new_state->state_board, initial_board, width * height * sizeof(cell));
    }
    // Se o estado não for o inicial
    else {
        new_state->s_parent = parent;
        new_state->g = new_state->s_parent->g + 1;
        memcpy(new_state->state_board, new_state->s_parent->state_board, width * height * sizeof(cell));

        flood(new_state->state_board, 1, 1, parent->state_board[width + 1].s_color, color);
    }

    new_state->h = region_heuristic(new_state);
    new_state->f = new_state->g + new_state->h;

    reset_board(new_state->state_board);

    // Adiciona a fila
    queue_append((queue_t **) queue, (queue_t *) new_state);

}

//------------------------------------------------------------------------------
// Algoritmo de busca UCS que procura a solução para a instância do problema
// por meio dos caminhos que mais diminuem a heuristica
void ucs () {

    state *open_states = NULL;
    state *visited_states = NULL;

    create_state(initial_board[1 * width + 1].s_color, NULL, &open_states);

    state *best_node;

    // Enquanto houverem nodos na fila
    while (queue_size((queue_t *) open_states)) {

        best_node = open_states;
        
        state *aux = open_states;

        // Encontra o estado com menor heurística
        while ((aux = aux->next) != open_states) {
            if (best_node->h > aux->h)
                best_node = aux;
        }

        // Checa se o estado é solução
        if (check_victory(best_node->state_board)) {
            get_solution(best_node);

            while (open_states) {
                
                state *aux = open_states;
                queue_remove((queue_t **) &open_states, (queue_t *) aux);
                free(aux->state_board);
                free(aux->border_colors);
                free(aux);
            }

            while (visited_states) {

                state *aux = visited_states;
                queue_remove((queue_t **) &visited_states, (queue_t *) aux);
                free(aux);
            }
            return;
        }
        
        // Gera estados com as cores que aparecem na fronteira
        for (int i=0; i<colors; i++) {
            if (best_node->border_colors[i]) {
                create_state(i+1, best_node, &open_states);
            }
        }

        // Remove o estado da fila de estados abertos
        queue_remove((queue_t **) &open_states, (queue_t *) best_node);
        free(best_node->border_colors);
        free(best_node->state_board);

        // Insere o estado na fila de estados visitados
        queue_append((queue_t **) &visited_states, (queue_t *) best_node);
    
    }
}

int main () {

    int regions;
    short chosen_color;
    scanf("%d %d %hd", &height, &width, &colors);
    
    height +=2;
    width += 2;

    if ((height > 0) && (width > 0) && (colors > 0))
        initial_board = (cell *) malloc (height * width * sizeof(cell));

    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j) {

            int index =  i * width + j;
            
            if ((i == 0) || (i == height -1)
                || (j == 0) || (j == width -1)) {
                initial_board[index].s_checked = true;
                initial_board[index].s_color = 0;
                initial_board[index].flooded = false;
            } else {
                scanf("%hd", &initial_board[index].s_color);
                initial_board[index].s_checked = false;
                initial_board[index].flooded = false;
            }
        }
    
    ucs();

    free(initial_board);
    return 0;
}