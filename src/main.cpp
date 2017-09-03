#include <string>

//#include <os.h>
#include <SDL/SDL.h>
//#include <SDL/SDL_gfxPrimitives.h>

#include "grid.h"
#include "3D.h"
#include "input.h"


double func(double x, double y) {
	//return 1/x + 1/y;
	return sin(x)*sin(y) / 4;
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
		if (input::exit()) {
			running = false;
			break;
		}
		
		constexpr double SPIN_RATE = 1.4;
		double spin = SPIN_RATE*dt;
		if (input::left())  cam.yaw -= spin;
		if (input::right()) cam.yaw += spin;
		if (input::up())   cam.pitch -= spin;
		if (input::down())   cam.pitch += spin;
		if (cam.pitch >  M_PI/2) cam.pitch =  M_PI/2;
		if (cam.pitch < -M_PI/2) cam.pitch = -M_PI/2;
		cam.calcMatrix();
		
		// clear the screen
		SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 184, 200, 222));
		
		// draw the geometry
		grid.draw(screen, cam);
		
		// debug text
		#define ts std::to_string
		std::string str;
		str += ts(isKeyPressed(KEY_NSPIRE_LEFTUP))+ts(isKeyPressed(KEY_NSPIRE_UP))+ts(isKeyPressed(KEY_NSPIRE_UPRIGHT))+"\n";
		str += ts(isKeyPressed(KEY_NSPIRE_LEFT))+ts(isKeyPressed(KEY_NSPIRE_CLICK))+ts(isKeyPressed(KEY_NSPIRE_RIGHT))+"\n";
		str += ts(isKeyPressed(KEY_NSPIRE_DOWNLEFT))+ts(isKeyPressed(KEY_NSPIRE_DOWN))+ts(isKeyPressed(KEY_NSPIRE_RIGHTDOWN))+"\n";
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

