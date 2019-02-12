//waveview.c
/*

febonachi: waveview.c
	gcc -Wall -o febonachi waveview.c -L/usr/lib/ -lSDL2

*/

#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <math.h>

#define coord(x,y) ((y)*W_width)+(x)

const double pi = 3.14159265;		//value of pi
const double pi2 = 6.28318530718; 	//The double of pi

const int W_height = 480;			//Window height
const int W_width = 640;			//Window width
uint32_t gBrushColor = 0;		//My draw function use this color to draw

typedef struct {
	int x;				//x value
	int y;				//y value
	int z;				//z value
} vec3d;

typedef struct {
	int x;				//x value
	int y;				//y value
} vec2d;

typedef struct {
	vec2d c;			//center
	int r;				//radius
} circle;

#define _V2D(x,y) (vec2d) {x,y}
void drawCircle(uint32_t* pixelMap, const size_t pixelSize, const circle cir);
void drawLine(uint32_t * pixelMap, const size_t pixelSize, const vec2d v1, const vec2d v2);
void drawFormula(uint32_t * pixelMap, const size_t pixelSize, const vec2d zero,const double sx, const double sy, double (*f)(double));
void protectPutPixel(uint32_t * pixelMap, const size_t pixelSize, const int x, const int y);
double taylorSined(double rad);
double taylorCossined(double rad);

void clear_map(uint32_t * pixelMap) {
	memset(pixelMap,0,W_width*W_height*sizeof(uint32_t));
}

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
		clear_map(myPixels);

		// UPDATE
		int i =0;
		/*for(i=0;i<200;i++)
		{
		//gBrushColor = 0x00FF0000;
			gBrushColor = rand() % 0x01000000;
			circle tc = {{rand()%640,rand()%480},rand()%100};
			drawCircle(myPixels, sizeof(uint32_t), tc );
		}*/
		gBrushColor = 0x00FFFF00;
	//	circle tc2 = {{300,300},50};
	//	drawCircle(myPixels, sizeof(uint32_t), tc2 );
		drawFormula(myPixels, sizeof(uint32_t),_V2D(300,240), 0.01, 0.005, taylorSined);
		gBrushColor = 0x0000FF55;
		drawFormula(myPixels, sizeof(uint32_t), _V2D(300,240), 0.01, 0.005, sin);
		gBrushColor = 0x00FF5500;
		drawLine(myPixels, sizeof(uint32_t), _V2D(0,240), _V2D(640,240));
		drawLine(myPixels, sizeof(uint32_t), _V2D(300,0), _V2D(300,480));

		//Change the texture to DRAW
		SDL_UpdateTexture(sdlTexture, NULL, myPixels, W_width * sizeof (Uint32) );
		//DRAW
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
		SDL_RenderPresent(renderer);
//		SDL_Delay(3000);
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
		protectPutPixel(pixelMap, pixelSize, cir.c.x+p.x,cir.c.y+p.y);
		protectPutPixel(pixelMap, pixelSize, cir.c.x+p.y,cir.c.y+p.x);
		protectPutPixel(pixelMap, pixelSize, cir.c.x-p.y,cir.c.y+p.x);
		protectPutPixel(pixelMap, pixelSize, cir.c.x-p.x,cir.c.y+p.y);
		protectPutPixel(pixelMap, pixelSize, cir.c.x-p.x,cir.c.y-p.y);
		protectPutPixel(pixelMap, pixelSize, cir.c.x-p.y,cir.c.y-p.x);
		protectPutPixel(pixelMap, pixelSize, cir.c.x+p.y,cir.c.y-p.x);
		protectPutPixel(pixelMap, pixelSize, cir.c.x+p.x,cir.c.y-p.y);

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

//http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
void drawLine(uint32_t * pixelMap, const size_t pixelSize, const vec2d v1, const vec2d v2) 
{
	vec2d pen;
	int deltaX = abs(v2.x - v1.x);
	int deltaY = abs(v2.y - v1.y);

	int sx = v1.x<v2.x ? 1 : -1;
	int sy = v1.y<v2.y ? 1 : -1;

	int err = (deltaX>deltaY ? deltaX : -deltaY)/2;
	int e2;

	pen.y = v1.y;
	pen.x = v1.x;
	for(;;)
	{
		//printf("%d %d %d %d\n",pen.x, pen.y, v2.x, v2.y);
		protectPutPixel(pixelMap, pixelSize,pen.x, pen.y);
		if (pen.x == v2.x && pen.y == v2.y) { break; }
		e2 = err;
		if(e2 > -deltaX) { err -= deltaY; pen.x += sx; }
		if(e2 < deltaY) { err += deltaX; pen.y += sy; }
	}
}
void drawTriangle(uint32_t * pixelMap, const size_t pixelSize, const vec2d v1, const vec2d v2, const vec2d v3) 
{
	drawLine(pixelMap, pixelSize, v1, v2);
	drawLine(pixelMap, pixelSize, v3, v2);
	drawLine(pixelMap, pixelSize, v1, v3);
}
//http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
void fillTriangle(uint32_t * pixelMap, const size_t pixelSize, const vec2d v1, const vec2d v2, const vec2d v3) 
{
	vec2d pen1, pen2;
	vec2d* obj1, obj2;
	obj1 = &v2;
	obj2 = &v3;

	int deltaX = abs(v2.x - v1.x);
	int deltaY = abs(v2.y - v1.y);
	int deltaX2 = abs(v3.x - v1.x);
	int deltaY2 = abs(v3.y - v1.y);

	int sx = v1.x<v2.x ? 1 : -1;
	int sy = v1.y<v2.y ? 1 : -1;
	int sx2 = v1.x<v3.x ? 1 : -1;
	int sy2 = v1.y<v3.y ? 1 : -1;

	int err = (deltaX>deltaY ? deltaX : -deltaY)/2;
	int err2 = (deltaX2>deltaY2 ? deltaX2 : -deltaY2)/2;
	int e2;

	pen1.y = v1.y;
	pen1.x = v1.x;

	for(;;)
	{
		//printf("%d %d %d %d\n",pen.x, pen.y, v2.x, v2.y);
		protectPutPixel(pixelMap, pixelSize,pen1.x, pen1.y);
		if (pen1.x == obj1->x && pen.y == obj1->y) { break; }
		e2 = err;
		if(e2 > -deltaX) { err -= deltaY; pen1.x += sx; }
		if(e2 < deltaY) { 
			err += deltaX; 
			pen1.y += sy; 
			for(;;) {

			}
		}
	}
}



double taylorSined(double rad)
{
/*	if(rad>1.57 || rad<-1.57)
		fprintf(stderr, "\rtaylorSined : %f \n", rad);*/
	const double reverse[5] = {(1.0/6.0),(1.0/120.0),(1.0/5040.0),(1.0/362880.0),(1.0/39916800.0)};
	const double square = rad * rad;
	double total = rad * square;
	double ret = rad - total*reverse[0];
	total *= square;
	ret += total*reverse[1];
	total *= square;
	ret -= total*reverse[2];
	total *= square;
	ret += total*reverse[3];
//	total *= square;
//	ret -= total*reverse[4];
	return ret;
}

double taylorCossined(double rad) 
{
//	return rad - (pow(rad,3)/6) + (pow(rad,5)/120) - (pow(rad,7)/5040);
	return 1 - (pow(rad,2)/2) + (pow(rad,4)/24) - (pow(rad,6)/720);
}

void drawFormula(uint32_t * pixelMap, const size_t pixelSize, const vec2d zero,const double sx, const double sy, double (*f)(double))
{
	double x;
	vec2d last, cur;
	last.x=0;
	x=(last.x - zero.x)*sx;
	last.y=f(x) / sy + zero.y;
	const int increment = 5;
	for(cur.x=5;cur.x<W_width;cur.x+=increment) {
		x=(cur.x - zero.x)*sx;
		//y=(zero.y - f(x))*sy;
		cur.y=f(x) / sy + zero.y;
		
		//protectPutPixel(pixelMap,pixelSize,i,y);
		drawLine(pixelMap,pixelSize,last,cur);	
		last = cur;
	}
}

//Sanitize draw function
void protectPutPixel(uint32_t * pixelMap, const size_t pixelSize, const int x, const int y)
{
	if(x < W_width && y < W_height && x >= 0 && y >= 0)
	{
		pixelMap[coord(x,y)] = gBrushColor;
	}
}

