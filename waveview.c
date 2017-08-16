#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <math.h>

#define coord(x,y) y*width+x

const int height = 480;
const int width = 640;

int main(int argc, char* argv[])
{
	//Argument handling
	
	SDL_Init(SDL_INIT_VIDEO);
	
//	SDL_SetVideoMode(800,600,0,SDL_SWSURFACE|SDL_DOUBLEBUF);

	SDL_Window *screen = SDL_CreateWindow("Test",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width, height,
			0);

	SDL_Renderer *renderer;
	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture *sdlTexture = SDL_CreateTexture(renderer,
						SDL_PIXELFORMAT_ARGB8888,
						SDL_TEXTUREACCESS_STREAMING,
						width, height);
	int quit = 0;
	uint32_t * myPixels = malloc(width*height*sizeof(uint32_t));

	while(!quit)
	{
		//Handling EVENT
		SDL_Event e;
		while( SDL_PollEvent(&e) !=0)
		{
			switch(e.type) {
				case SDL_QUIT:
					quit =1;
					break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					break;
				
			}
		}

		// UPDATE
		int i =0;
		for(i=0;i<width;i++) {
			myPixels[coord(i,240)] = 0x0000FF00;
		}

		//Change the texture to DRAW
		SDL_UpdateTexture(sdlTexture, NULL, myPixels, 640 * sizeof (Uint32) );
		//DRAW
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
		SDL_RenderPresent(renderer);

	}

	free(myPixels);
	SDL_DestroyWindow(screen);
	SDL_Quit();
	return 0;
}
