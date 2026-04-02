#ifndef GAME_H
#define GAME_H

#define MAP_HEIGHT 7
#define MAP_WIDTH 19

typedef struct{
  int x;
  int y;
  char direction;
  char future_direction;
}Player;

typedef struct {
  char map[MAP_HEIGHT][MAP_WIDTH + 1];
  Player player;
  int score;
  int is_running;
} Game;

void init_game(Game *game);
void move_player(Game *game);
void set_player_direction(Game *game, char direction);
int count_dots(const Game *game);

#endif
