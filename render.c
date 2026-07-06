#include "raylib.h"
#include "render.h"

#define CELL_SIZE 32
#define HUD_HEIGHT 80

void render_game(const Game *game) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int screen_x = x * CELL_SIZE;
            int screen_y = HUD_HEIGHT + y * CELL_SIZE;
            char cell = game->map[y][x];
            int ghost_here = 0;

            DrawRectangle(screen_x, screen_y, CELL_SIZE, CELL_SIZE, BLACK);

            if (cell == '#') {
                DrawRectangle(screen_x, screen_y, CELL_SIZE, CELL_SIZE, BLUE);
            } else if (cell == '.') {
                DrawCircle(screen_x + CELL_SIZE / 2,
                           screen_y + CELL_SIZE / 2,
                           4,
                           GOLD);
            } else if (cell == '=') {
                DrawRectangle(screen_x + 4,
                              screen_y + CELL_SIZE / 2 - 4,
                              CELL_SIZE - 8,
                              8,
                              GRAY);
            }

            for (int i = 0; i < GHOST_COUNT; i++) {
                if (game->ghosts[i].x == x && game->ghosts[i].y == y) {
                    ghost_here = 1;
                    break;
                }
            }

            if (ghost_here) {
                DrawCircle(screen_x + CELL_SIZE / 2,
                           screen_y + CELL_SIZE / 2,
                           CELL_SIZE / 2 - 4,
                           RED);
            }

            if (game->player.x == x && game->player.y == y) {
                DrawCircle(screen_x + CELL_SIZE / 2,
                           screen_y + CELL_SIZE / 2,
                           CELL_SIZE / 2 - 4,
                           YELLOW);
            }
        }
    }

    DrawText(TextFormat("Score: %d", game->score), 20, 20, 24, WHITE);
    DrawText(TextFormat("Lives: %d", game->lives), 200, 20, 24, WHITE);

    if (game->difficulty == EASY) {
        DrawText("Difficulty: Easy", 350, 20, 24, GREEN);
    } else if (game->difficulty == MEDIUM) {
        DrawText("Difficulty: Medium", 350, 20, 24, ORANGE);
    } else {
        DrawText("Difficulty: Hard", 350, 20, 24, RED);
    }

    if (game->is_paused) {
        DrawText("PAUSED", 650, 20, 24, SKYBLUE);
    }
}