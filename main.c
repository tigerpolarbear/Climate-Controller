#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

// Functions for the Lua environment
static const struct luaL_reg cc [] = {
	{NULL, NULL} // form {"<fname>", fpointer}
};

// Globals
lua_State * L;
SDL_Surface * screen;

int main(int argc, char * argv[]) {
	screen = NULL;
	
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_EnableUNICODE(1); //important
	
	// Initialize Lua
	L = luaL_newstate();
	luaL_openlibs(L);
	luaL_register (L, "cc", cc);
	
	// Load the lua library
	if (luaL_dofile(L,"climatecontroller.lua")) {
		printf("Uh oh!\n");
		lua_error(L);
		exit(1);
	}

	// Try to load the actual game
	if (argc != 2) {
		printf("provide one argument, the game to play\n");
		exit(1);
	}
	if (chdir(argv[1])) {
		printf("%s doesn't exist!\n",argv[1]);
		exit(1);
	}
	
	if (luaL_dofile(L,"index.lua")) {
		printf("Uh oh!\n");
		lua_error(L);
		exit(1);
	}
	
	screen = SDL_SetVideoMode(640, 480, 16, 0);

	// Main loop
	SDL_Event event;
	uint8_t killswitch = 0;
	Uint32 lastflip = SDL_GetTicks();
	Uint32 lastmainloop= SDL_GetTicks();
	uint8_t fps = 0;
	Uint32 now = SDL_GetTicks();
	Uint32 delta = 0; //used for both lastflip and lastmainloop

	lua_getglobal(L, "desiredfps");
	if (!lua_isnumber(L, -1)) {
		printf("Whoa. desiredfps wasn't a number.\n");
		exit(1);
	}
	fps = lua_tonumber(L,-1);
	if (!fps) lua_error(L);
	printf("FPS: %i\n", fps);

	lua_getglobal(L, "initialize");
	if (lua_pcall(L, 0, 1, 0) != 0)
		lua_error(L);


	while (!killswitch) {
		while( SDL_PollEvent( &event ) ){
			switch( event.type ){
				case SDL_KEYDOWN:
					//lua_getglobal(L, "keypress");
					//lua_pushnumber(L, event.key.keysym.unicode);
					//if (lua_pcall(L, 1, 0, 0) != 0) 
					//	luaL_error(L, "error calling keypress");
					switch ( event.key.keysym.sym ) {
						case SDLK_ESCAPE:
							killswitch = 1;
							break;
						default:
							break;
					}
				case SDL_KEYUP:
					break;
			}
		}
		// This is called every frame
		now = SDL_GetTicks();
		delta = now - lastmainloop;
		lua_getglobal(L, "mainloop");
		lua_pushnumber(L,delta);
		lastmainloop = now;
		if (lua_pcall(L, 1, 1, 0) != 0)
			lua_error(L);
		now = SDL_GetTicks();
		delta = now - lastflip;
		if (delta < (1000/fps))
			SDL_Delay((1000/fps) - delta);
		lastflip = now;
		SDL_Flip(screen);
	}
	exit(0);
}

