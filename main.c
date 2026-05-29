#define _DEFAULT_SOURCE
#include <stdio.h>
#include <unistd.h>
#include "game.h"
#include "render.h"
#include "input.h"

static void handle_collision(Game *game) {
    game->lives--;

    if (game->lives <= 0) {
        game->is_running = 0;
    } else {
        reset_positions(game);
    }
}

static int show_main_menu(void) {
    int choice;

    printf("===== PACMAN IN C =====\n");
    printf("1. Start Game\n");
    printf("2. Quit Game\n");
    printf("Choose an option: ");
    scanf("%d", &choice);

    return choice;
}

static Difficulty show_difficulty_menu(void) {
    int choice;

    printf("\n===== SELECT DIFFICULTY =====\n");
    printf("1. Easy\n");
    printf("2. Medium\n");
    printf("3. Hard\n");
    printf("Choose difficulty: ");
    scanf("%d", &choice);

    if (choice == 1) {
        return EASY;
    } else if (choice == 2) {
        return MEDIUM;
    } else {
        return HARD;
    }
}

int main() {
    Game game;
    char command;
    int main_choice;
    int highscore;
    Difficulty difficulty;

    highscore = load_highscore();

    main_choice = show_main_menu();

    if (main_choice != 1) {
        printf("Game closed.\n");
        return 0;
    }

    difficulty = show_difficulty_menu();
    init_game(&game, difficulty);

    enable_raw_mode();

    while (game.is_running) {
        printf("\033[2J\033[H");
        render_game(&game);
        printf("High Score: %d\n", highscore);

        if (game.difficulty == EASY) {
            printf("Difficulty: Easy | ");
        } else if (game.difficulty == MEDIUM) {
            printf("Difficulty: Medium | ");
        } else {
            printf("Difficulty: Hard | ");
        }

        if (game.is_paused) {
            printf("PAUSED | press p to resume | q to quit\n");
        } else {
            printf("p to pause | q to quit\n");
        }

        command = read_input_nonblocking();

        if (command == 'q') {
            game.is_running = 0;
            break;
        }

        if (command == 'p') {
            game.is_paused = !game.is_paused;
            usleep(200000);
            continue;
        }

        if (game.is_paused) {
            usleep(200000);
            continue;
        }

        if (command != '\0') {
            set_player_direction(&game, command);
        }

        move_player(&game);

        if (check_player_ghost_collision(&game)) {
            handle_collision(&game);
            if (!game.is_running) {
                break;
            }
            usleep(300000);
            continue;
        }

        move_ghosts(&game);

        if (check_player_ghost_collision(&game)) {
            handle_collision(&game);
            if (!game.is_running) {
                break;
            }
            usleep(300000);
            continue;
        }

        usleep(200000);
    }

    disable_raw_mode();

    if (game.score > highscore) {
        highscore = game.score;
        save_highscore(highscore);
    }

    printf("\033[2J\033[H");
    render_game(&game);

    printf("Final Score: %d\n", game.score);
    printf("High Score: %d\n", highscore);

    if (count_dots(&game) == 0) {
        printf("You win!\n");
    } else if (game.lives <= 0) {
        printf("Game over! Ai ramas fara vieti.\n");
    } else {
        printf("Game ended.\n");
    }

    return 0;
}