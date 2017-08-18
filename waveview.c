//waveview.c
/*

febonachi: waveview.c
	gcc -Wall -o febonachi waveview.c -L/usr/lib/ -lSDL2

*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <math.h>

#define coord(x,y) ((y)*W_width)+(x)

const double pi = 3.14159265;

int W_height = 480;
int W_width = 640;
uint32_t gBrushColor = 0;

typedef struct {
	int x;
	int y;
} vec2d;

typedef struct {
	vec2d c; //center
	int r; //radius
} circle;

void drawCircle(uint32_t* pixelMap, const size_t pixelSize, const circle cir);
void drawLine(uint32_t * pixelMap, const size_t pixelSize, const vec2d v1, const vec2d v2);
double taylorSined(double rad);

int main(int argc, char* argv[])
{
	//Argument handling
	
	SDL_Init(SDL_INIT_VIDEO);
	
//	SDL_SetVideoMode(800,600,0,SDL_SWSURFACE|SDL_DOUBLEBUF);

	SDL_Window *screen = SDL_CreateWindow("Test",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			W_width, W_height,
			0);

	SDL_Renderer *renderer;
	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture *sdlTexture = SDL_CreateTexture(renderer,
						SDL_PIXELFORMAT_ARGB8888,
						SDL_TEXTUREACCESS_STREAMING,
						W_width, W_height);
	int quit = 0;
	uint32_t * myPixels = malloc(W_width*W_height*sizeof(uint32_t));

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
		gBrushColor = 0x0000FF00;
		for(i=0;i<W_width;i++) {
			myPixels[coord(i,240)] = gBrushColor;
		}
		gBrushColor = 0x00FF0000;
		circle tc = {{199,199},100};
		drawCircle(myPixels, sizeof(uint32_t), tc );

		//Change the texture to DRAW
		SDL_UpdateTexture(sdlTexture, NULL, myPixels, W_width * sizeof (Uint32) );
		//DRAW
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
		SDL_RenderPresent(renderer);

	}

	//free(myPixels);
	SDL_DestroyWindow(screen);
	SDL_Quit();
	return 0;
}


void drawCircle(uint32_t * pixelMap, const size_t pixelSize, const circle cir)
{
	vec2d p = {cir.r - 1,0};
	int dx = 1;int dy = 1;
	int err = dx - (cir.r << 1);

	while(p.x >= p.y) {
		pixelMap[coord(cir.c.x+p.x,cir.c.y+p.y)] = gBrushColor;
		pixelMap[coord(cir.c.x+p.y,cir.c.y+p.x)] = gBrushColor;
		pixelMap[coord(cir.c.x-p.y,cir.c.y+p.x)] = gBrushColor;
		pixelMap[coord(cir.c.x-p.x,cir.c.y+p.y)] = gBrushColor;
		pixelMap[coord(cir.c.x-p.x,cir.c.y-p.y)] = gBrushColor;
		pixelMap[coord(cir.c.x-p.y,cir.c.y-p.x)] = gBrushColor;
		pixelMap[coord(cir.c.x+p.y,cir.c.y-p.x)] = gBrushColor;
		pixelMap[coord(cir.c.x+p.x,cir.c.y-p.y)] = gBrushColor;

		if (err <= 0)
		{
			p.y++;
			err += dy;
			dy += 2;
		}
		if ( err > 0 )
		{
			p.x--;
			dx += 2;
			err += (-cir.r << 1) + dx;
		}
			
	}
}
void drawLine(uint32_t * pixelMap, const size_t pixelSize, const vec2d v1, const vec2d v2) 
{

}

double taylorSined(double rad)
{
	return rad - (pow(rad,3)/6) + (pow(rad,5)/120) - (pow(rad,7)/5040);
}
