#include <stdio.h>
#include "SDL.h"

typedef struct PlayContext_s
{
	int quit;
	SDL_Renderer * renderer;
	SDL_Texture * texture;
	SDL_mutex * mutex;
	char filepath[256];
}PlayContext_t;

static int YUV_PlayThread(void * arg)
{
	PlayContext_t * PlayCtx = (PlayContext_t *)arg;
	if (PlayCtx == NULL)
	{
		return -1;
	}
	printf("YUVPlayThread start successfully.\n");
	printf("####################\n");
	printf("# filename : %s\n", PlayCtx->filepath);
	printf("####################\n\n");
	while (1)
	{
		if (PlayCtx->quit)
		{
			break;
		}
		//printf("1111111111111111111111111\n");
		SDL_Delay(500);
	}

	printf("YUVPlayThread exit successfully.\n");
	return 0;
}

int main(int argc, char * argv[])
{
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret)
	{
		printf("SDL_init Failed!\n");
	}
	SDL_Thread * thread = NULL;
	SDL_Window * window = NULL;
	SDL_RendererInfo rendererinfo;
	PlayContext_t PlayCtx = { 0 };

	window = SDL_CreateWindow("SDL YUV Player", 100, 100, 960, 540, 0);
	if (window == NULL)
	{
		goto Quit;
	}
	SDL_Event event;
	int done = 0;

	int rendercnt = SDL_GetNumRenderDrivers();
	for (int i=0; i < rendercnt; i++)
	{
		SDL_GetRenderDriverInfo(i, &rendererinfo);
		printf("\n####################\n");
		printf("render index:\t%d\nrender name:\t%s\nrender flags:\t%d", i, rendererinfo.name, rendererinfo.flags);
		printf("\n####################\n");
	}
	unsigned char stat = SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	while (!done)
	{
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				done = 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				//printf("capture mouse down event\n");
				break;
			case SDL_DROPFILE:
				strcpy(PlayCtx.filepath, event.drop.file);
				PlayCtx.renderer = SDL_CreateRenderer(window, -1, 0);
				PlayCtx.texture = SDL_CreateTexture(PlayCtx.renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 960,540);
				thread = SDL_CreateThread(YUV_PlayThread, "YUVPlayThread", (void *)&PlayCtx);
				if (thread == NULL)
				{
					printf("Create YUVPlayThread failed.\n");
				}
				SDL_free(event.drop.file);
				//SDL_EventState(SDL_DROPFILE, SDL_DISABLE);
				break;
			}
		}
		SDL_Delay(0);
	}
	if (thread)
	{
		PlayCtx.quit = 1;
		SDL_WaitThread(thread, NULL);
		thread = NULL;
	}
Quit:
	if (window)
	{
		SDL_DestroyWindow(window);
		window = NULL;
	}
	SDL_Quit();

	return 0;
}