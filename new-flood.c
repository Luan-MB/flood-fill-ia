#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"

typedef struct cell {

    short s_color;
    bool s_checked;

} cell;

typedef struct state {

    struct state *prev, *next;
    struct state *s_parent;
    short s_color;
    int g, h, f;
    cell *state_board;

} state;

int height, width;
short colors;
cell *initial_board;

void reset_board(cell *game_board) {

    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j)
            game_board[i * width + j].s_checked = false;
}

void flood(cell *game_board, int x, int y, short old_color, short new_color) {

    if (x < 0 || x > height - 1)
        return;
    if (y < 0 || y > width - 1)
        return;
    if (game_board[x * width + y].s_color != old_color) {
        return;
    }
    
    game_board[x * width + y].s_color = new_color;
    flood(game_board, x+1, y, old_color, new_color);
    flood(game_board, x, y+1, old_color, new_color);
    flood(game_board, x-1, y, old_color, new_color);
    flood(game_board, x, y-1, old_color, new_color);
}

void check_adjacents (cell *game_board, short x, short y, short color) {

    if (x < 0 || x > height - 1)
        return;
    if (y < 0 || y > width - 1)
        return;
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

int region_counter (cell *game_board, int height, int width) {

    int num_regions = 0;
    
    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j) {

            int index = i * width + j;

            if (!game_board[index].s_checked) {
                num_regions++;
                check_adjacents(game_board, i+1, j, game_board[index].s_color);
                check_adjacents(game_board, i-1, j, game_board[index].s_color);
                check_adjacents(game_board, i, j+1, game_board[index].s_color);
                check_adjacents(game_board, i, j-1, game_board[index].s_color);
            }
        }
    }
    return num_regions;
} 

void create_state (short color, state *parent, state **queue) {

    state *new_state = (state *) malloc (sizeof(state));
    
    if (!parent) {
        new_state->prev = new_state->next = NULL;
        new_state->s_color = color;
        new_state->s_parent = NULL;
        new_state->g = 0;

        new_state->state_board = (cell *) malloc (width * height * sizeof(cell));
        memcpy(new_state->state_board, initial_board, width * height * sizeof(cell));
        
        new_state->h = region_counter(new_state->state_board, height, width);
        new_state->f = new_state->g + new_state->h;
    }
    else {
        new_state->prev = new_state->next = NULL;
        new_state->s_color = color;
        new_state->s_parent = parent;

        new_state->state_board = (cell *) malloc (width * height * sizeof(cell));
        memcpy(new_state->state_board, new_state->s_parent->state_board, width * height * sizeof(cell));
        
        new_state->g = new_state->s_parent->g + 1;

        flood(new_state->state_board, 0, 0, parent->state_board[0].s_color, color);
        new_state->h = region_counter(new_state->state_board, height, width);
        new_state->f = new_state->g + new_state->h;

    }

    reset_board(new_state->state_board);
    queue_append((queue_t **) queue, (queue_t *) new_state);

}

int check_victory (cell *game_board) {

    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j)
            if (game_board[0].s_color != game_board[i * width + j].s_color)
                return 0;
    return 1;
}

void a_star () {

    state *open_states = NULL;
    state *visited_states = NULL;

    create_state(initial_board[0].s_color, NULL, &open_states);

    state *best_node;

    while (queue_size((queue_t *) open_states)) {

        best_node = open_states;
        
        state *aux = open_states;

        while ((aux = aux->next) != open_states) {
            if (best_node->f > aux->f)
                best_node = aux;
        }

        if (check_victory(best_node->state_board)) {
            printf("Achei pora\n");
            return;
        }

        for (int i=1; i<=colors; i++) {
            if (i != best_node->s_color) {
                create_state(i, best_node, &open_states);
            }
        }

        queue_remove((queue_t **) &open_states, (queue_t *) best_node);
    
    }
    printf("Não achei, merda\n");
}

void print_board (cell *game_board) {

    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j)
            printf("%d ", game_board[i * width + j].s_color);
        printf("\n");
    }
}

int main () {

    int regions;
    short chosen_color;
    scanf("%d %d %hd", &height, &width, &colors);

    if ((height > 0) && (width > 0) && (colors > 0))
        initial_board = (cell *) malloc (height * width * sizeof(cell));

    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j) {
            
            int index =  i * width + j;
            
            scanf("%hd", &initial_board[index].s_color);
            initial_board[index].s_checked = false;
        }

    /* while (!check_victory(initial_board)) {

        printf("%d Regiões\n", region_counter(initial_board, height, width)) ;
        reset_board(initial_board);
        scanf("%hd", &chosen_color);

        if ((chosen_color != initial_board[0].s_color) && (chosen_color <= colors) && (chosen_color > 0)) {
            flood(initial_board, 0, 0, initial_board[0].s_color, chosen_color);
            print_board(initial_board);
        }
    } */
    
    a_star();

    free(initial_board);
    return 0;
}