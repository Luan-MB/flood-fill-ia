// Luan Machado Bernardt | GRR20190363

#ifndef __TYPES__
#define __TYPES__

#include <stdbool.h>

typedef struct cell {

    short s_color;
    bool s_checked;
    bool flooded;

} cell;

typedef struct state {

    struct state *prev, *next;
    struct state *s_parent;
    short s_color;
    int g, h, f;
    cell *state_board;
    int *border_colors;

} state;

#endif