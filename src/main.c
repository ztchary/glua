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
	glua_quit();
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

	path_join(main_lua, project_path, "main.lua");

	if (!glua_init()) {
		fprintf(stderr, "Failed to initialize\n");
		return 1;
	}

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	glua_link(L, project_path);

	crash_if(L, luaL_dofile(L, main_lua) != 0);

	unsigned int last_tick = SDL_GetTicks();
	SDL_Event e;
	bool run = true;
	while (run) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) run = false;
			if (e.type == SDL_WINDOWEVENT_SIZE_CHANGED) {
				printf("resize\n");
			}
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
	glua_quit();
}

