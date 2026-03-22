#include <stdio.h>
#include "render.h"

void render_game(const Game *game)
{
  for(int i=0;i<MAP_HEIGHT;i++)
    {
      for(int j=0;j<MAP_WIDTH;j++)
	{
	  if(i==game->player.y && j==game->player.x)
	    {
	      printf("P");
	    }
	  else
	    {
	      printf("%c", game->map[i][j]);
	    }
	}
      printf("\n");
    }
}
