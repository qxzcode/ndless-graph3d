#include <string>

//#include <os.h>
#include <SDL/SDL.h>
//#include <SDL/SDL_gfxPrimitives.h>

#include "grid.h"
#include "3D.h"


double func(double x, double y) {
	//return sin(x)*sin(y) / 8;
	double foo = hypot(x, y);
	return 0.6 / (1 + foo*foo);
}

int main() {
	// initialize SDL
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* screen = SDL_SetVideoMode(320, 240, has_colors ? 16 : 8, SDL_SWSURFACE);
	nSDL_Font* font = nSDL_LoadFont(NSDL_FONT_TINYTYPE, 29, 43, 61);
	
	SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 128, 0));
	SDL_Flip(screen);
	
	Grid grid(func);
	Camera cam;
	
	SDLKey curKey = SDLK_UNKNOWN;
	Uint8 curKeySC = 0;
	Uint32 lastFrame = SDL_GetTicks();
	double time = 0.0;
	bool running = true;
	while (running) {
		// compute delta time
		Uint32 curFrame = SDL_GetTicks();
		double dt = (curFrame-lastFrame)/1000.0;
		time += dt;
		lastFrame = curFrame;
		
		// handle input
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				curKey = event.key.keysym.sym;
				curKeySC = event.key.keysym.scancode;
				if (curKeySC == 53) running = false;
				break;
			case SDL_KEYUP:
				curKey = SDLK_UNKNOWN;
				curKeySC = 0;
				break;
			}
		}
		
		//cam.x = 0.35*cos(time) + 0.5;
		//cam.y = 0.35*sin(time) + 0.5;
		cam.yaw += dt*1.0;//sin(time*2.0)*0.6;
		cam.pitch = 0.6;
		cam.calcMatrix();
		
		// clear the screen
		SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 184, 200, 222));
		
		// draw the geometry
		grid.draw(screen, cam);
		
		// debug text
		#define ts std::to_string
		std::string str;
		//str += ts(curKeySC)+"\n";
		//str += ts(curKey)+" : "+SDL_GetKeyName(curKey)+"\n";
		str += "xInner="+ts(cam.xInner)+"  aRev="+ts(cam.aRev)+"  bRev="+ts(cam.bRev)+"\n";
		str += "FPS: "+ts(int(1/dt));
		nSDL_DrawString(screen, font, 30, 30, str.c_str());
		#undef ts
		
		// update the screen
		SDL_Flip(screen);
	}
	
	//wait_key_pressed();
	
	SDL_Quit();
	return 0;
}

