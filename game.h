#ifndef GAME_H
#define GAME_H

#define MAP_HEIGHT 17
#define MAP_WIDTH 29
#define GHOST_COUNT 3

typedef enum{
  EASY = 1,
  MEDIUM = 2,
  HARD = 3
}Difficulty;

typedef struct{
  int x;
  int y;
  char direction;
  char future_direction;
}Player;

typedef struct{
  int x;
  int y;
  char direction;
  int move_delay;
  int move_counter;
}Ghost;

typedef struct {
  char map[MAP_HEIGHT][MAP_WIDTH + 1];
  Player player;
  Ghost ghosts[GHOST_COUNT];
  int score;
  int lives;
  int is_running;
  Difficulty difficulty;
} Game;

void init_game(Game *game, Difficulty difficulty);
void reset_positions(Game *game);
void move_player(Game *game);
void move_ghosts(Game *game);
void set_player_direction(Game *game, char direction);
int count_dots(const Game *game);
int check_player_ghost_collision(const Game *game);

#endif
