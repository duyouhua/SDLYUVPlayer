#include <stdio.h>
#include "SDL.h"

int main(int argc, char * argv[])
{
	int ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret)
	{
		printf("SDL_init Failed!\n");
	}
	SDL_Window * window = SDL_CreateWindow("SDL YUV Player", 100, 100, 960, 540, 0);
	getchar();
	return 0;
}