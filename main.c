#include <stdio.h>
#include "game.h"
#include "render.h"
#include "input.h"

int main(void)
{
  Game game;
  char command;
  
  init_game(&game);

  while(1)
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
    }

  return 0;
}
