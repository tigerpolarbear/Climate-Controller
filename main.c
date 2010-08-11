#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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
	
	screen = SDL_SetVideoMode(640, 480, 16, SDL_FULLSCREEN);
	//screen = SDL_SetVideoMode(x, y, 16, 0);
	
	// Load the actual game
	if (luaL_dofile(L,"lulululu.lua")) {
		printf("Uh oh!\n");
		lua_error(L);
		exit(252);
	}
	
	// Main loop
	SDL_Event event;
	uint8_t killswitch = 0;
	// it's a joke! get it? it kills these switch statements!
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
		//lua_getglobal(L, "mainloop");
		//if (lua_pcall(L, 0, 1, 0) != 0)
		//	luaL_error(L, "error running function: 'mainloop': ");
		SDL_Flip(screen);
	}
	exit(0);
}

