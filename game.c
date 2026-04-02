#include "game.h"
#include "map.h"

static int can_move_in_direction(const Game *game, char direction) {
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

void init_game(Game *game) {
    init_map(game->map);
    game->player.x = 14;
    game->player.y = 13;
    game->player.direction = 'd';
    game->player.future_direction = 'd';

    game->ghost.x = 14;
    game->ghost.y=9;
    game->ghost.direction='a';
    
    game->score = 0;
    game->is_running = 1;

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
    int new_x;
    int new_y;

    if (can_move_in_direction(game, game->player.future_direction)) {
        game->player.direction = game->player.future_direction;
    }

    if (!can_move_in_direction(game, game->player.direction)) {
        return;
    }

    new_x = game->player.x;
    new_y = game->player.y;

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