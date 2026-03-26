#include "game.h"
#include "map.h"

int count_dots(const Game *game)
{
  int count = 0;

  for(int i=0;i < MAP_HEIGHT; i++)
    {
      for(int j=0;j < MAP_WIDTH; j++)
	{
	  if(game->map[i][j] == '.')
	    count++;
	}
    }
  return count;
}

void init_game(Game *game)
{
  init_map(game->map);

  game->player.x = 1;
  game->player.y = 1;
  game->score = 0;
  game->is_running = 1;

  if(game->map[game->player.y][game->player.x] == '.')
    {
      game->score++;
      game->map[game->player.y][game->player.x] = ' ';
    }

  if(count_dots(game) == 0)
    game->is_running = 0;
}

void move_player(Game *game, char direction)
{
  int new_x = game->player.x;
  int new_y = game->player.y;

  if(direction == 'w')
    {
      new_y--;
    }
  else if(direction == 's')
    {
      new_y++;
    }
  else if(direction == 'a')
    {
      new_x--;
    }
  else if(direction == 'd')
    {
      new_x++;
    }
  else return;

  if (new_x < 0 || new_x >= MAP_WIDTH || new_y < 0 || new_y >= MAP_HEIGHT) {
        return;
    }

  if(game->map[new_y][new_x] !='#')
    {
      game->player.x = new_x;
      game->player.y = new_y;

      if (game->map[new_y][new_x] == '.')
	{
	  game->score++;
	  game->map[new_y][new_x] = ' ';
	}

      if(count_dots(game) == 0)
	game->is_running = 0;
    }
}
