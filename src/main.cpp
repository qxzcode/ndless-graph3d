#include <os.h>
#include <SDL/SDL_config.h>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>


struct point {
	int x, y;
};

constexpr double P_SCALE = 200.0;

struct Camera {
	
	point transform(double x, double y, double z) {
		// view transform...
		
		return point{
			static_cast<int>(P_SCALE*x/z),
			static_cast<int>(P_SCALE*y/z)
		};
	}
	
};


double func(double x, double y) {
	double foo = x + y;
	return 1 / (1 + foo*foo);
}

constexpr int GRID_SIZE = 5;
constexpr double SCALE = 4.0 / GRID_SIZE;

struct Grid {
	
	double hmap[GRID_SIZE][GRID_SIZE];
	
	Grid() {
		for (int x = 0; x < GRID_SIZE; x++) {
			for (int y = 0; y < GRID_SIZE; y++) {
				hmap[x][y] = func(x*SCALE, y*SCALE);
			}
		}
	}
	
};


int main() {
	SDL_Surface *screen;
	nSDL_Font *font;
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(320, 240, has_colors ? 16 : 8, SDL_SWSURFACE);
	font = nSDL_LoadFont(NSDL_FONT_TINYTYPE, 29, 43, 61);
	
	SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 128, 128, 128));
	SDL_Flip(screen);
	SDL_Delay(1000);
	
	Grid grid;
	Camera cam;
	
	SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 184, 200, 222));
	filledTrigonRGBA(screen, 100,100, 50,130, 230,125, 0,0,255,255);
	trigonRGBA      (screen, 100,100, 50,130, 230,125, 0,0,0,255);
	nSDL_DrawString(screen, font, 30, 30, "Hello, world! \x1");
	SDL_Flip(screen);
	
	wait_key_pressed();
	
	SDL_Quit();
	return 0;
}

