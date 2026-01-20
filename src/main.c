#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_filesystem.h>
#include "glua/glua.h"
#include "path.h"

void crash_if(lua_State *L, bool do_crash) {
	if (!do_crash) return;
	const char *err = lua_tostring(L, -1);
	fprintf(stderr, "Error: %s\n", err);
	lua_close(L);
	exit(1);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Provide a project path\n");
		return 1;
	}

	const char *project_path = argv[1];
	char main_lua[128];
	char link_path[128];

	path_join(main_lua, project_path, "main.lua");
	path_join(link_path, project_path, "?.lua");

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	glua_link(L, link_path);

	crash_if(L, luaL_dofile(L, main_lua) != 0);

	lua_getglobal(L, "glua");
	lua_getfield(L, -1, "init");

	if(!lua_isfunction(L, -1)) {
		fprintf(stderr, "Couldn't find glua.init\n");
		return 1;
	}

	crash_if(L, lua_pcall(L, 0, 0, 0) != LUA_OK);

	if (!glua_init()) {
		fprintf(stderr, "Failed to initialize\n");
		return 1;
	}

	unsigned int last_tick = SDL_GetTicks();
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = true;
			glua_event_handle(L, &e);
		}
		unsigned int cur_tick = SDL_GetTicks();
		lua_getglobal(L, "glua");
		lua_getfield(L, -1, "draw");
		if (!lua_isfunction(L, -1)) {
			fprintf(stderr, "Couldn't find glua.draw\n");
			return 1;
		}
		lua_pushnumber(L, (float)(cur_tick - last_tick) / 1000.0);
		crash_if(L, lua_pcall(L, 1, 0, 0) != LUA_OK);
		lua_pop(L, 1);
		last_tick = cur_tick;
	}

	lua_close(L);
	SDL_Quit();
}

