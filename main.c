#include "raylib.h"
#include "game.h"
#include "render.h"

#define CELL_SIZE 32
#define HUD_HEIGHT 80
#define SCREEN_WIDTH (MAP_WIDTH * CELL_SIZE)
#define SCREEN_HEIGHT (HUD_HEIGHT + MAP_HEIGHT * CELL_SIZE)

typedef enum {
    STATE_MAIN_MENU,
    STATE_DIFFICULTY_MENU,
    STATE_PLAYING,
    STATE_GAME_OVER
} AppState;

static void handle_collision(Game *game) {
    game->lives--;

    if (game->lives <= 0) {
        game->is_running = 0;
    } else {
        reset_positions(game);
    }
}

int main(void) {
    Game game;
    AppState state = STATE_MAIN_MENU;
    Difficulty selected_difficulty = MEDIUM;
    int highscore = load_highscore();
    int game_result = 0; /* 0 = quit, 1 = win, 2 = lose */
    float tick_timer = 0.0f;
    const float tick_interval = 0.20f;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pacman in C - raylib");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (state == STATE_MAIN_MENU) {
            if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_ENTER)) {
                state = STATE_DIFFICULTY_MENU;
            }
            if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_ESCAPE)) {
                break;
            }
        } else if (state == STATE_DIFFICULTY_MENU) {
            if (IsKeyPressed(KEY_ONE)) {
                selected_difficulty = EASY;
                init_game(&game, selected_difficulty);
                tick_timer = 0.0f;
                state = STATE_PLAYING;
            } else if (IsKeyPressed(KEY_TWO)) {
                selected_difficulty = MEDIUM;
                init_game(&game, selected_difficulty);
                tick_timer = 0.0f;
                state = STATE_PLAYING;
            } else if (IsKeyPressed(KEY_THREE)) {
                selected_difficulty = HARD;
                init_game(&game, selected_difficulty);
                tick_timer = 0.0f;
                state = STATE_PLAYING;
            } else if (IsKeyPressed(KEY_ESCAPE)) {
                state = STATE_MAIN_MENU;
            }
        } else if (state == STATE_PLAYING) {
            if (IsKeyPressed(KEY_Q)) {
                game_result = 0;
                state = STATE_GAME_OVER;
            }

            if (IsKeyPressed(KEY_P)) {
                game.is_paused = !game.is_paused;
            }

            if (!game.is_paused) {
                if (IsKeyPressed(KEY_W)) {
                    set_player_direction(&game, 'w');
                } else if (IsKeyPressed(KEY_A)) {
                    set_player_direction(&game, 'a');
                } else if (IsKeyPressed(KEY_S)) {
                    set_player_direction(&game, 's');
                } else if (IsKeyPressed(KEY_D)) {
                    set_player_direction(&game, 'd');
                }

                tick_timer += GetFrameTime();

                if (tick_timer >= tick_interval) {
                    tick_timer = 0.0f;

                    move_player(&game);

                    if (check_player_ghost_collision(&game)) {
                        handle_collision(&game);
                    } else {
                        move_ghosts(&game);

                        if (check_player_ghost_collision(&game)) {
                            handle_collision(&game);
                        }
                    }

                    if (game.score > highscore) {
                        highscore = game.score;
                        save_highscore(highscore);
                    }

                    if (count_dots(&game) == 0) {
                        game_result = 1;
                        state = STATE_GAME_OVER;
                    } else if (game.lives <= 0) {
                        game_result = 2;
                        state = STATE_GAME_OVER;
                    }
                }
            }
        } else if (state == STATE_GAME_OVER) {
            if (IsKeyPressed(KEY_ENTER)) {
                state = STATE_MAIN_MENU;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                break;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (state == STATE_MAIN_MENU) {
            DrawText("PACMAN IN C", 260, 120, 40, YELLOW);
            DrawText("1. Start Game", 280, 220, 30, WHITE);
            DrawText("2. Quit Game", 280, 270, 30, WHITE);
            DrawText("Press 1 or Enter to continue", 220, 360, 20, GRAY);
        } else if (state == STATE_DIFFICULTY_MENU) {
            DrawText("SELECT DIFFICULTY", 210, 120, 40, ORANGE);
            DrawText("1. Easy", 320, 220, 30, GREEN);
            DrawText("2. Medium", 320, 270, 30, YELLOW);
            DrawText("3. Hard", 320, 320, 30, RED);
            DrawText("Press ESC to go back", 260, 400, 20, GRAY);
        } else if (state == STATE_PLAYING) {
            render_game(&game);

            DrawText(TextFormat("High Score: %d", highscore), 20, 50, 24, LIGHTGRAY);

            if (game.is_paused) {
                DrawText("PAUSED", 650, 20, 28, SKYBLUE);
            }
        } else if (state == STATE_GAME_OVER) {
            DrawText("GAME FINISHED", 230, 120, 40, YELLOW);
            DrawText(TextFormat("Final Score: %d", game.score), 280, 220, 30, WHITE);
            DrawText(TextFormat("High Score: %d", highscore), 280, 270, 30, WHITE);

            if (game_result == 1) {
                DrawText("YOU WIN!", 310, 330, 36, GREEN);
            } else if (game_result == 2) {
                DrawText("GAME OVER!", 280, 330, 36, RED);
            } else {
                DrawText("GAME ENDED", 280, 330, 36, LIGHTGRAY);
            }

            DrawText("Press ENTER for menu", 250, 420, 24, GRAY);
            DrawText("Press ESC to exit", 280, 460, 24, GRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}