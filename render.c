#include <stdio.h>
#include "render.h"

void render_game(const Game *game)
{
  printf("Score: %d | Lives: %d\n", game->score, game->lives);
  
  for(int i=0;i<MAP_HEIGHT;i++)
    {
      for(int j=0;j<MAP_WIDTH;j++)
		{
			int ghost_here=0;
			if(i==game->player.y && j==game->player.x)
	    	{
	     	 	printf("☻");
		  		continue;
	    	}
			for(int k=0; k<GHOST_COUNT;k++){
				if(i==game->ghosts[k].y && j==game->ghosts[k].x){
					ghost_here=1;
					break;
				}
			}

			if(ghost_here){
				printf("☠");
			}
			else{
				printf("%c", game->map[i][j]);
			}
		}
      printf("\n");
    }
}
