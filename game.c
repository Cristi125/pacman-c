#include "game.h"
#include "map.h"

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

static int can_ghost_move_in_direction(const Game *game, char direction) {
    int new_x = game->ghost.x;
    int new_y = game->ghost.y;

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

static char get_left_direction(char direction) {
    if (direction == 'w') {
        return 'a';
    } else if (direction == 'a') {
        return 's';
    } else if (direction == 's') {
        return 'd';
    } else if (direction == 'd') {
        return 'w';
    }

    return direction;
}

static char get_right_direction(char direction) {
    if (direction == 'w') {
        return 'd';
    } else if (direction == 'd') {
        return 's';
    } else if (direction == 's') {
        return 'a';
    } else if (direction == 'a') {
        return 'w';
    }

    return direction;
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
    return game->player.x == game->ghost.x && game->player.y == game->ghost.y;
}

void init_game(Game *game) {
    init_map(game->map);

    game->player.x = 14;
    game->player.y = 13;
    game->player.direction = 'd';
    game->player.future_direction = 'd';

    game->ghost.x = 14;
    game->ghost.y = 8;
    game->ghost.direction = 's';

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

void move_ghost(Game *game) {
    char forward = game->ghost.direction;
    char left = get_left_direction(forward);
    char right = get_right_direction(forward);
    char back = get_opposite_direction(forward);
    char chosen_direction = '\0';
    int new_x = game->ghost.x;
    int new_y = game->ghost.y;

    if (can_ghost_move_in_direction(game, forward)) {
        chosen_direction = forward;
    } else if (can_ghost_move_in_direction(game, left)) {
        chosen_direction = left;
    } else if (can_ghost_move_in_direction(game, right)) {
        chosen_direction = right;
    } else if (can_ghost_move_in_direction(game, back)) {
        chosen_direction = back;
    } else {
        return;
    }

    game->ghost.direction = chosen_direction;

    if (chosen_direction == 'w') {
        new_y--;
    } else if (chosen_direction == 's') {
        new_y++;
    } else if (chosen_direction == 'a') {
        new_x--;
    } else if (chosen_direction == 'd') {
        new_x++;
    }

    game->ghost.x = new_x;
    game->ghost.y = new_y;
}