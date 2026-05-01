#define _DEFAULT_SOURCE
#include <stdio.h>
#include <unistd.h>
#include "game.h"
#include "render.h"
#include "input.h"

int main() {
    Game game;
    char command;

    init_game(&game);
    enable_raw_mode();

    while (game.is_running) {
        printf("\033[2J\033[H");
        render_game(&game);
        printf("Score target: collect all dots | q to quit\n");

        command = read_input_nonblocking();

        if (command == 'q') {
            game.is_running = 0;
            break;
        }

        if (command != '\0') {
            set_player_direction(&game, command);
        }

        move_player(&game);

        if (check_player_ghost_collision(&game)) {
            game.is_running = 0;
            break;
        }

        move_ghosts(&game);

        if (check_player_ghost_collision(&game)) {
            game.is_running = 0;
            break;
        }

        usleep(200000);
    }

    disable_raw_mode();

    printf("\033[2J\033[H");
    render_game(&game);

    if (count_dots(&game) == 0) {
        printf("You win!\n");
    } else if (check_player_ghost_collision(&game)) {
        printf("Game over! The ghost caught you.\n");
    } else {
        printf("Game ended.\n");
    }

    return 0;
}