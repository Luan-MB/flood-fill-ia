#include <iostream>
#include <cstring>
#include <cstdlib>
#include <list>
using namespace std;

typedef struct t_square {

    short color;
    bool checked;

} t_square;

t_square *game;

int *colors_occurences;
int height, width, colors;
int *frontier_occurences;
list<int> moves_queue;

void showlist(list<int> g)
{
    list<int>::iterator it;
    for (it = g.begin(); it != g.end(); ++it)
        cout << *it << " ";
    cout << '\n';
}

void update_region (int x, int y, short new_color) {

    if (x < 0 || x > height - 1)
        return;
    if (y < 0 || y > width - 1)
        return;
    if (!game[x * width + y].checked)
        return;
    if (game[x * width + y].color == new_color) {
        game[x * width + y].checked = false;
    
        update_region(x+1, y, new_color);
        update_region(x, y+1, new_color);
        update_region(x-1, y, new_color);
        update_region(x, y-1, new_color);
    }
}

void flood (int x, int y, short old_color, short new_color) {

    if (x < 0 || x > height - 1)
        return;
    if (y < 0 || y > width - 1)
        return;
    if (game[x * width + y].color == new_color) {
        update_region (x, y, new_color);
        return;
    }
    if (game[x * width + y].color != old_color) {
        return;
    }
    
    game[x * width + y].color = new_color;
    game[x * width + y].checked = false;
    flood(x+1, y, old_color, new_color);
    flood(x, y+1, old_color, new_color);
    flood(x-1, y, old_color, new_color);
    flood(x, y-1, old_color, new_color);
}

void check_adjacents (int x, int y, short adjacent_color) {

    if (x < 0 || x > height - 1)
        return;
    if (y < 0 || y > width - 1)
        return;
    if (game[x * width + y].checked)
        return;
    if (game[x * width + y].color == adjacent_color) {
        game[x * width + y].checked = true;
        colors_occurences[adjacent_color - 1]++;
    
        check_adjacents(x+1, y, adjacent_color);
        check_adjacents(x, y+1, adjacent_color);
        check_adjacents(x-1, y, adjacent_color);
        check_adjacents(x, y-1, adjacent_color);
    }
}

void check_border (int x, int y, short region_color) {

    if (x < 0 || x > height - 1)
        return;
    if (y < 0 || y > width - 1)
        return;
    if (game[x * width + y].checked)
        return;
    if (game[x * width + y].color != region_color) {
        game[x * width + y].checked = true;
        colors_occurences[game[x * width + y].color - 1]++;

        short adjacent_color = game[x * width + y].color;

        check_adjacents(x+1, y, adjacent_color);
        check_adjacents(x, y+1, adjacent_color);
        check_adjacents(x-1, y, adjacent_color);
        check_adjacents(x, y-1, adjacent_color);
        return;
    }
    if (game[x * width + y].color == region_color) {
        game[x * width + y].checked = true;
        check_border(x+1, y, region_color);
        check_border(x, y+1, region_color);
        check_border(x-1, y, region_color);
        check_border(x, y-1, region_color);
    }
}

void print_game() {
    cout << "\n";
    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j)
            cout << game[i * width + j].color << " ";
        cout << "\n";
    }
    cout << "\n";
}

int check_victory () {

    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j)
            if (game[0].color != game[i * width + j].color)
                return 0;
    return 1;
}

int main () {    

    short chosen_color, old_color;
    int max_color = 0;
    int moves = 0;

    cin >> height >> width >> colors;

    game = (t_square *) malloc(height * width * sizeof(t_square));
    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j) {
            cin >> game[i * width + j].color;
            game[i * width + j].checked = false;
        }

    colors_occurences = (int *) malloc(colors * sizeof(int));
    memset(colors_occurences, 0, colors * sizeof(int));

    
    while (!check_victory()) {

        check_border(0, 0, game[0].color);
        for (int i=0; i<colors; ++i) {
            if (colors_occurences[i] > max_color) {
                max_color = colors_occurences[i];
                chosen_color = i+1;
            }
        }

        if ((old_color = game[0].color) != chosen_color) 
            if (chosen_color > 0 && chosen_color <= colors) {
                flood(0, 0, old_color, chosen_color);
                colors_occurences[chosen_color - 1] = 0;
                moves++;
                moves_queue.push_back(chosen_color);
            }
        max_color = 0;
    }

    cout  << moves << "\n";
    showlist(moves_queue);
    return 0;
}