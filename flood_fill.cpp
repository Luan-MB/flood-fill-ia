#include <iostream>
#include <cstdlib>
using namespace std;

typedef struct queue_t {

    queue_t *prev, *next;
    pair <int, int> coords;
    short color;

} queue_t;

void queue_append(pair<int, int> coords, int color) {

    queue_t *aux;
    aux = frontier_nodes;

    while (aux->next != queue) {
        
    }
    
    queue_t new_node;

    new_node.coords.first = coords.first;
    new_node.coords.second = coords.second;
    new_node.color = color;


}

queue_t *frontier_nodes = NULL;

int *game;
int height, width, colors;

void flood (int x, int y, int old_color, int new_color) {

    if (x < 0 || x > height - 1)
        return;
    if (y < 0 || y > width - 1)
        return;
    if (game[x * width + y] != old_color) 
        return;
    
    game[x * width + y] = new_color;
    flood(x+1, y, old_color, new_color);
    flood(x, y+1, old_color, new_color);
    flood(x-1, y, old_color, new_color);
    flood(x, y-1, old_color, new_color);
}

int checa_fronteira(int x, int y, int color) {

    return 1;
}

void print_game() {
    cout << "\n";
    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j)
            cout << game[i * width + j] << " ";
        cout << "\n";
    }
    cout << "\n";
}

int check_victory () {

    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j)
            if (game[0] != game[i * width + j])
                return 0;
    return 1;
}

int main () {    

    int chosen_color;
    int old_color;

    cin >> height >> width >> colors;

    game = (int *) malloc(height * width * sizeof(int));
    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j)
            cin >> game[i * width + j];

    while (!check_victory()) {
        cin >> chosen_color;

        if (chosen_color > 0 && chosen_color <= colors) {
            old_color = game[0];
            flood(0, 0, old_color, chosen_color);
        }
        
        print_game();
    }

    cout << "Parabens consagrado!!!!\n";
    return 0;
}