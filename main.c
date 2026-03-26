#include <stdio.h>
#include "game.h"
#include "render.h"
#include "input.h"

int main(void)
{
  Game game;
  char command;
  
  init_game(&game);

  while(game.is_running)
    {
      printf("\033[2J\033[H");
      render_game(&game);
      printf("Mover with WASD, q to quit: ");

      command = read_input();

      if(command == 'q')
	{
	  break;
	}

      move_player(&game, command);

      if(!game.is_running)
	break;
    }

  printf("\033[2J\033[H");
  render_game(&game);

  if(count_dots(&game) == 0)
    printf("You win!\n");
  else
    printf("Game over.\n");

  return 0;
}
