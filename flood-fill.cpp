#include <iostream>
#include <cstring>
#include <cstdlib>
#include <list>
using namespace std;

typedef struct cell {

    short color;
    bool checked;

} cell;

typedef struct state {

    cell *state_board;
    int *state_frontier;
    int h;
} state;


cell *game;

int *coloroccurences;
int height, width, colors;
int *frontier_occurences;
list<int> movequeue;

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
        coloroccurences[adjacent_color - 1]++;
    
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
        coloroccurences[game[x * width + y].color - 1]++;

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

void check_region (int x, int y, short color) {

    if (x < 0 || x > height - 1)
        return;
    if (y < 0 || y > width - 1)
        return;
    if (game[x * width + y].color != color)
        return;
    if (game[x * width + y].checked)
        return;
    
    game[x * width + y].checked = true;
    check_region(x+1, y, color);
    check_region(x, y+1, color);
    check_region(x, y-1, color);
    
}

int region_counter (cell *game, int height, int width) {

    int num_regions = 1;

    check_border(0, 0, game[0].color);
    
    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j) {

            int index = i * width + j;

            if (!game[index].checked) {
                num_regions++;
                check_region(i+1, j, game[index].color);
                check_region(i-1, j, game[index].color);
                check_region(i, j+1, game[index].color);
                check_region(i, j-1, game[index].color);
            }
        }
    }

    printf("%d\n", num_regions);
    for (int i=0; i<colors; i++) {
        printf("%d ", coloroccurences[i]);
        if(coloroccurences[i])
            num_regions++;
    }

    return num_regions;
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

    game = (cell *) malloc(height * width * sizeof(cell));
    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j) {
            cin >> game[i * width + j].color;
            game[i * width + j].checked = false;
        }

    coloroccurences = (int *) malloc(colors * sizeof(int));
    memset(coloroccurences, 0, colors * sizeof(int));

    cout << "Regions: " << region_counter(game, height, width) << "\n";

    while (!check_victory()) {

        check_border(0, 0, game[0].color);
        for (int i=0; i<colors; ++i) {
            if (coloroccurences[i] > max_color) {
                max_color = coloroccurences[i];
                chosen_color = i+1;
            }
        }

        if ((old_color = game[0].color) != chosen_color) 
            if (chosen_color > 0 && chosen_color <= colors) {
                flood(0, 0, old_color, chosen_color);
                coloroccurences[chosen_color - 1] = 0;
                moves++;
                movequeue.push_back(chosen_color);
            }
        max_color = 0;

    }

    cout << "Regions: " << region_counter(game, height, width) << "\n";

    cout  << moves << "\n";
    showlist(movequeue);
    return 0;
}