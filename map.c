#include <string.h>
#include "map.h"

void init_map(char map[MAP_HEIGHT][MAP_WIDTH+1])
{
  const char *defalut_map[MAP_HEIGHT] = {
        "###################",
        "#........#........#",
        "#.####.#.#.####.#.#",
        "#.................#",
        "#.####.#.#.####.#.#",
        "#........#........#",
        "###################"
    };

  for(int i=0;i<MAP_HEIGHT;i++)
    {
      strcpy(map[i], defalut_map[i]);
    }
}
