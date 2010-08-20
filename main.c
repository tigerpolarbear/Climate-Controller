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
// static int functionname (lua_State *L);
static int ccframe_new(lua_State *L);
static int ccframe_setx(lua_State *L);
static int ccframe_sety(lua_State *L);
static int ccframe_setw(lua_State *L);
static int ccframe_seth(lua_State *L);
static int ccframe_getx(lua_State *L);
static int ccframe_gety(lua_State *L);
static int ccframe_getw(lua_State *L);
static int ccframe_geth(lua_State *L);

static int ccimage_new(lua_State * L);

static int ccblit(lua_State * L);

static const struct luaL_reg cc [] = {
	{"frame_new", ccframe_new},
	{"frame_setx", ccframe_setx},
	{"frame_sety", ccframe_sety},
	{"frame_setw", ccframe_setw},
	{"frame_seth", ccframe_seth},
	{"frame_getx", ccframe_getx},
	{"frame_gety", ccframe_gety},
	{"frame_getw", ccframe_getw},
	{"frame_geth", ccframe_geth},

	{"image_new", ccimage_new},

	{"blit", ccblit},

	{NULL, NULL} // form {"<fname>", fpointer}
};

// Globals
lua_State * L;
SDL_Surface * screen;

int main(int argc, char * argv[]) {

	screen = NULL;
	
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	
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

	// SDLK Mappings
	if (luaL_dofile(L,"sdlk.lua")) {
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
	
	// Get a display! Throw it at the lua environment!
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
					lua_getglobal(L, "keystroke");
					lua_pushnumber(L, event.key.keysym.sym);
					if (lua_pcall(L, 1, 0, 0) != 0) 
						luaL_error(L, "error calling keystroke");
					switch ( event.key.keysym.sym ) {
						case SDLK_ESCAPE:
							killswitch = 1;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					lua_getglobal(L, "keyrelease");
					lua_pushnumber(L, event.key.keysym.sym);
					if (lua_pcall(L, 1, 0, 0) != 0) 
						luaL_error(L, "error calling keyrelease");
					break;
				default:
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

static int ccframe_new(lua_State *L) {
	SDL_Rect * r = lua_newuserdata (L, sizeof(SDL_Rect));
	r->x = lua_tonumber(L, 1);
	r->y = lua_tonumber(L, 2);
	r->w = lua_tonumber(L, 3);
	r->h = lua_tonumber(L, 4);
	return 1;
}

static int ccframe_setx(lua_State *L) {
	SDL_Rect * r = lua_touserdata(L, 1);
	int x = lua_tonumber(L, 2);
	r->x = x;
	return 0;
}

static int ccframe_sety(lua_State *L) {
	SDL_Rect * r = lua_touserdata(L, 1);
	int y = lua_tonumber(L, 2);
	r->y = y;
	return 0;
}

static int ccframe_setw(lua_State *L) {
	SDL_Rect * r = lua_touserdata(L, 1);
	int w = lua_tonumber(L, 2);
	r->w = w;
	return 0;
}

static int ccframe_seth(lua_State *L) {
	SDL_Rect * r = lua_touserdata(L, 1);
	int h = lua_tonumber(L, 2);
	r->h = h;
	return 0;
}

static int ccframe_getx(lua_State *L) {
	SDL_Rect * r = lua_touserdata(L, 1);
	lua_pushnumber(L, r->x);
	return 1;
}

static int ccframe_gety(lua_State *L) {
	SDL_Rect * r = lua_touserdata(L, 1);
	lua_pushnumber(L, r->y);
	return 1;
}

static int ccframe_getw(lua_State *L) {
	SDL_Rect * r = lua_touserdata(L, 1);
	lua_pushnumber(L, r->w);
	return 1;
}

static int ccframe_geth(lua_State *L) {
	SDL_Rect * r = lua_touserdata(L, 1);
	lua_pushnumber(L, r->h);
	return 1;
}

static int ccimage_new(lua_State * L) {
	SDL_Surface ** r = lua_newuserdata(L, sizeof(SDL_Surface *));
	SDL_Surface * s = IMG_Load(lua_tostring (L, 1));
	if (!s) {
		printf("Tried to load an image and it didn't work!!!!\n");
		exit(1);
	}
	*r = s;
	printf("loaded image: %p\n", *r); 
	return 1;
}

static int ccblit(lua_State * L) {
	SDL_Surface ** src = lua_touserdata(L, 1);
	SDL_Rect * srcr = lua_touserdata(L, 2);
	//SDL_Surface ** dst = lua_touserdata(L, 3);
	SDL_Surface * dst = SDL_GetVideoSurface(); 
	SDL_Rect * dstr = lua_touserdata(L, 3);
	printf("src: %p\n", *src, srcr, dst, dstr);
	SDL_BlitSurface(*src, srcr, dst, dstr);
	printf("blitted?\n");
	return 0;
}

