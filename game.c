#include "game.h"
#include "map.h"
#include <stdlib.h>
#include <stdio.h>

int load_highscore(void){
    FILE *file = fopen("highscore.txt","r");
    int highscore=0;

    if(file==NULL){
        return 0;
    }

    fscanf(file,"%d", &highscore);
    fclose(file);
    
    return highscore;
}

void save_highscore(int highscore){
    FILE *file = fopen("highscore.txt","w");

    if(file==NULL){
        return;
    }

    fprintf(file,"%d",highscore);
    fclose(file);
}

static int can_player_move_in_direction(const Game *game, char direction) {
    int new_x = game->player.x;
    int new_y = game->player.y;

    if (direction == 'w') {
        new_y--;
    } else if (direction == 's') {
        new_y++;
    } else if (direction == 'a') {
        new_x--;
    } else if (direction == 'd') {
        new_x++;
    } else {
        return 0;
    }

    if (new_x < 0 || new_x >= MAP_WIDTH || new_y < 0 || new_y >= MAP_HEIGHT) {
        return 0;
    }

    if (game->map[new_y][new_x] == '#' || game->map[new_y][new_x] == '=') {
        return 0;
    }

    return 1;
}

static int can_ghost_move_in_direction(const Game *game, int ghost_index, char direction) {
    int new_x = game->ghosts[ghost_index].x;
    int new_y = game->ghosts[ghost_index].y;

    if (direction == 'w') {
        new_y--;
    } else if (direction == 's') {
        new_y++;
    } else if (direction == 'a') {
        new_x--;
    } else if (direction == 'd') {
        new_x++;
    } else {
        return 0;
    }

    if (new_x < 0 || new_x >= MAP_WIDTH || new_y < 0 || new_y >= MAP_HEIGHT) {
        return 0;
    }

    if (game->map[new_y][new_x] == '#') {
        return 0;
    }

    return 1;
}

static char get_opposite_direction(char direction) {
    if (direction == 'w') {
        return 's';
    } else if (direction == 's') {
        return 'w';
    } else if (direction == 'a') {
        return 'd';
    } else if (direction == 'd') {
        return 'a';
    }

    return direction;
}

int count_dots(const Game *game) {
    int count = 0;

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (game->map[i][j] == '.') {
                count++;
            }
        }
    }

    return count;
}

int check_player_ghost_collision(const Game *game) {
    for(int i=0;i<GHOST_COUNT;i++){
        if(game->player.x == game->ghosts[i].x && game->player.y == game->ghosts[i].y){
            return 1;
        }
    }
    return 0;
}

void reset_positions(Game *game){
    game->player.x=14;
    game->player.y=15;
    game->player.direction='d';
    game->player.future_direction='d';

    game->ghosts[0].x = 13;
    game->ghosts[0].y = 8;
    game->ghosts[0].direction = 's';
    game->ghosts[0].move_counter = 0;

    game->ghosts[1].x = 14;
    game->ghosts[1].y = 8;
    game->ghosts[1].direction = 'a';
    game->ghosts[1].move_counter = 0;

    game->ghosts[2].x = 15;
    game->ghosts[2].y = 8;
    game->ghosts[2].direction = 'd';
    game->ghosts[2].move_counter = 0;

    if(game->difficulty == EASY){
        game->ghosts[0].move_delay = 4;
        game->ghosts[1].move_delay = 5;
        game->ghosts[2].move_delay = 6;
    }
    else if(game->difficulty == MEDIUM){
        game->ghosts[0].move_delay = 2;
        game->ghosts[1].move_delay = 3;
        game->ghosts[2].move_delay = 4;
    }
    else{
        game->ghosts[0].move_delay = 1;
        game->ghosts[1].move_delay = 2;
        game->ghosts[2].move_delay = 3;
    }
}

void init_game(Game *game, Difficulty difficulty) {
    init_map(game->map);

    game->score=0;
    game->lives=3;
    game->is_running=1;
    game->difficulty = difficulty;

    reset_positions(game);

    if (game->map[game->player.y][game->player.x] == '.') {
        game->score++;
        game->map[game->player.y][game->player.x] = ' ';
    }

    if (count_dots(game) == 0) {
        game->is_running = 0;
    }
}

void set_player_direction(Game *game, char direction) {
    if (direction == 'w' || direction == 'a' || direction == 's' || direction == 'd') {
        game->player.future_direction = direction;
    }
}

void move_player(Game *game) {
    int new_x = game->player.x;
    int new_y = game->player.y;

    if (can_player_move_in_direction(game, game->player.future_direction)) {
        game->player.direction = game->player.future_direction;
    }

    if (!can_player_move_in_direction(game, game->player.direction)) {
        return;
    }

    if (game->player.direction == 'w') {
        new_y--;
    } else if (game->player.direction == 's') {
        new_y++;
    } else if (game->player.direction == 'a') {
        new_x--;
    } else if (game->player.direction == 'd') {
        new_x++;
    }

    game->player.x = new_x;
    game->player.y = new_y;

    if (game->map[new_y][new_x] == '.') {
        game->score++;
        game->map[new_y][new_x] = ' ';
    }

    if (count_dots(game) == 0) {
        game->is_running = 0;
    }
}

void move_one_ghost(Game *game, int ghost_index) {
    char directions[4] = {'w', 'a', 's', 'd'};
    char opposite = get_opposite_direction(game->ghosts[ghost_index].direction);
    char best_direction = '\0';
    int best_distance = 1000000;
    int found_non_opposite = 0;

    game->ghosts[ghost_index].move_counter++;

    if(game->ghosts[ghost_index].move_counter < game->ghosts[ghost_index].move_delay){
        return;
    }

    game->ghosts[ghost_index].move_counter = 0;

    for (int i = 0; i < 4; i++) {
        char dir = directions[i];
        int new_x = game->ghosts[ghost_index].x;
        int new_y = game->ghosts[ghost_index].y;
        int distance;

        if (!can_ghost_move_in_direction(game, ghost_index, dir)) {
            continue;
        }

        if (dir != opposite) {
            found_non_opposite = 1;
        }

        if (dir == 'w') {
            new_y--;
        } else if (dir == 's') {
            new_y++;
        } else if (dir == 'a') {
            new_x--;             
        } else if (dir == 'd') {
            new_x++;
        }

        distance = abs(game->player.x - new_x) + abs(game->player.y - new_y);

        if (distance < best_distance) {
            best_distance = distance;
            best_direction = dir;
        }
    }

    if (found_non_opposite && best_direction == opposite) {
        best_direction = '\0';
        best_distance = 1000000;

        for (int i = 0; i < 4; i++) {
            char dir = directions[i];
            int new_x = game->ghosts[ghost_index].x;
            int new_y = game->ghosts[ghost_index].y;
            int distance;

            if (dir == opposite) {
                continue;
            }

            if (!can_ghost_move_in_direction(game, ghost_index, dir)) {
                continue;
            }

            if (dir == 'w') {
                new_y--;
            } else if (dir == 's') {
                new_y++;
            } else if (dir == 'a') {
                new_x--;
            } else if (dir == 'd') {
                new_x++;
            }

            distance = abs(game->player.x - new_x) + abs(game->player.y - new_y);

            if (distance < best_distance) {
                best_distance = distance;
                best_direction = dir;
            }
        }
    }

    if (best_direction == '\0') {
        return;
    }

    game->ghosts[ghost_index].direction = best_direction;

    if (best_direction == 'w') {
        game->ghosts[ghost_index].y--;
    } else if (best_direction == 's') {
        game->ghosts[ghost_index].y++;
    } else if (best_direction == 'a') {
        game->ghosts[ghost_index].x--;
    } else if (best_direction == 'd') {
        game->ghosts[ghost_index].x++;
    }
}

void move_ghosts(Game *game){
    for(int i=0;i<GHOST_COUNT;i++){
        move_one_ghost(game,i);
    }
}