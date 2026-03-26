#ifndef GAME_H
#define GAME_H

#define MAP_HEIGHT 7
#define MAP_WIDTH 19

typedef struct{
  int x;
  int y;
}Player;

typedef struct {
  char map[MAP_HEIGHT][MAP_WIDTH + 1];
  Player player;
  int score;
} Game;

void init_game(Game *game);
void move_player(Game *game, char direction);

#endif
