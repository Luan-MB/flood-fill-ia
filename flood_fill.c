#include <stdio.h>
#include <stdlib.h>

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
    printf("\n");
    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j)
            printf("%d ", game[i * width + j]);
        printf("\n");
    }
    printf("\n");
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

    scanf("%d %d %d", &height, &width, &colors);

    game = malloc(height * width * sizeof(int));
    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j)
            scanf("%d", &game[i * width + j]);

    while (!check_victory()) {
        scanf("%d", &chosen_color);

        if (chosen_color > 0 && chosen_color <= colors) {
            old_color = game[0];
            flood(0, 0, old_color, chosen_color);
        }
        
        print_game();
    }

    printf("Parabens consagrado!!!!\n");
    return 0;
}