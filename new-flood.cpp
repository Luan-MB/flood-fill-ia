#include <iostream>
#include <cstring>
#include <cstdlib>
#include <list>
#include <set>

using namespace std;

typedef struct cell {

    short s_color;
    bool s_checked;

} cell;

typedef struct state {

    int s_id;
    state *s_parent;
    short s_color;
    int g, h, f;
    cell *state_board;
    state *s_sucessors;
    bool operator < (const state &other_state) const {
        return f < other_state.f;
    }

} state;

int height, width;
short colors;
cell *initial_board;
int state_id = 0;

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

void create_state (short color, state *parent, state *new_state) {
    
    new_state->state_board = (cell *) malloc (width * height * sizeof(cell));
    new_state->s_sucessors = (state *) malloc (colors * sizeof(state));

    if (parent == NULL) {
        new_state->s_color = color;
        new_state->s_parent = NULL;
        new_state->s_id = state_id++;
        new_state->g = 0;

        memcpy(new_state->state_board, initial_board, width * height * sizeof(cell));
        
        new_state->h = region_counter(new_state->state_board, height, width);
        new_state->f = new_state->g + new_state->h;

    }
    else {
        new_state->s_color = color;
        new_state->s_parent = parent;
        new_state->s_id = state_id++;

        memcpy(new_state->state_board, new_state->s_parent->state_board, width * height * sizeof(cell));
        
        new_state->g = parent->g + 1;

        flood(new_state->state_board, 0, 0, parent->state_board[0].s_color, color);
        new_state->h = region_counter(new_state->state_board, height, width);
        new_state->f = new_state->g + new_state->h;

    }

    reset_board(new_state->state_board);

}

int check_victory (cell *game_board) {

    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j)
            if (game_board[0].s_color != game_board[i * width + j].s_color)
                return 0;
    return 1;
}

void a_star (state *initial) {

    set<state> open_nodes;
    
    set<state> visited_nodes;

    set<state>::iterator it;

    state *best_node;

    open_nodes.insert(*initial);

    while (!open_nodes.empty()) {
        
        it = open_nodes.begin();
        *best_node = *it;
        open_nodes.erase(it);
        visited_nodes.insert(*best_node);

        if (check_victory(best_node->state_board)) {
            cout << "Achamo caralho\n";
            return;
        }  

        for (int i=0; i<colors; i++) {
            create_state(i+1, best_node, &best_node->s_sucessors[i]);
            open_nodes.insert(best_node->s_sucessors[i]); 
        }
        
    }
    cout << "Não achei, merda\n";
}

void print_board (cell *game_board) {

    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j)
            cout << game_board[i * width + j].s_color << " ";
        cout << "\n";
    }
}

int main () {

    int regions;
    short chosen_color;
    cin >> height >> width >> colors;

    if ((height > 0) && (width > 0) && (colors > 0))
        initial_board = (cell *) malloc (height * width * sizeof(cell));

    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j) {
            
            int index =  i * width + j;
            
            cin >> initial_board[index].s_color;
            initial_board[index].s_checked = false;
        }

    /* while (!check_victory(initial_board)) {

        cout << region_counter(initial_board, height, width) << " Regiões\n";
        cin >> chosen_color;

        if ((chosen_color != initial_board[0].s_color) && (chosen_color <= colors) && (chosen_color > 0)) {
            flood(initial_board, 0, 0, initial_board[0].s_color, chosen_color);
            reset_board(initial_board);
            print_board(initial_board);
        }
    } */

    state initial_state;
    create_state(initial_board[0].s_color, NULL, &initial_state);
    free(initial_board);

    a_star(&initial_state);

    return 0;
}