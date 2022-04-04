// Luan Machado Bernardt | GRR20190363

#include <stdbool.h>
#include <stdio.h>

#include "globalvars.h"
#include "types.h"
#include "heuristic.h"

int height, width;
short colors;

//------------------------------------------------------------------------------
// Checa e marca a célula atual, chamando a a função recursivamente para as adjacentes
static void check_adjacents (cell *game_board, short x, short y, short color) {

    if (game_board[x * width + y].s_color != color)
        return;
    if (game_board[x * width + y].s_checked)
        return;
    
    game_board[x * width + y].s_checked = true;
    check_adjacents(game_board, x+1, y, color);
    check_adjacents(game_board, x-1, y, color);
    check_adjacents(game_board, x, y+1, color);
    check_adjacents(game_board, x, y-1, color);
    
}

//------------------------------------------------------------------------------
// Guarda quantas são e de quais cores são as regiões na fronteira da área "alagada"
static void check_border (cell *game_board, int x, int y, short region_color, int *coloroccurences) {

    if (game_board[x * width + y].s_checked)
        return;

    // Se encontrou uma região de outra cor na fronteira
    if (game_board[x * width + y].s_color != region_color) {
        game_board[x * width + y].s_checked = true;
        coloroccurences[game_board[x * width + y].s_color - 1]++;

        short frontier_color = game_board[x * width + y].s_color;

        // Marca todas as peças dessa fronteira
        check_adjacents(game_board, x+1, y, frontier_color);
        check_adjacents(game_board, x-1, y, frontier_color);
        check_adjacents(game_board, x, y+1, frontier_color);
        check_adjacents(game_board, x, y-1, frontier_color);
        return;
    }
    if (game_board[x * width + y].s_color == region_color) {
        game_board[x * width + y].s_checked = true;
        check_border(game_board, x+1, y, region_color, coloroccurences);
        check_border(game_board, x, y+1, region_color, coloroccurences);
        check_border(game_board, x-1, y, region_color, coloroccurences);
        check_border(game_board, x, y-1, region_color, coloroccurences);
    }
}

//------------------------------------------------------------------------------
// Região heurística que calcula o total de regiões distintas existentes no mapa
int region_heuristic (state *state) {

    int num_regions = 1;

    // Checa a fronteira
    check_border(state->state_board, 1, 1, state->s_color, state->border_colors);

    // Conta todas as regiões que não estão na fronteira e nem são a principal
    for (int i=1; i<height-1; ++i) {
        for (int j=1; j<width-1; ++j) {

            int index = i * width + j;

            if (!state->state_board[index].s_checked) {
                num_regions++;
                check_adjacents(state->state_board, i+1, j, state->state_board[index].s_color);
                check_adjacents(state->state_board, i-1, j, state->state_board[index].s_color);
                check_adjacents(state->state_board, i, j+1, state->state_board[index].s_color);
                check_adjacents(state->state_board, i, j-1, state->state_board[index].s_color);
            }
        }
    }
    
    // Por fim, soma-se em um o numero de regiões para cada cor encontrada na fronteira
    for (int i=0; i<colors; i++)
        if (state->border_colors[i])
            num_regions++;

    return num_regions;
}
