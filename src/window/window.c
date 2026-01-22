#include "window/window.h"

SDL_Window *window;

bool glua_window_init() {
	window = SDL_CreateWindow("glua project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_ALWAYS_ON_TOP);
	return window != NULL;
}

void glua_window_quit() {
	SDL_DestroyWindow(window);
}

int glua_window_set_title(lua_State *L) {
	const char *title = luaL_checkstring(L, 1);
	SDL_SetWindowTitle(window, title);
	return 0;
}

int glua_window_set_size(lua_State *L) {
	int width = luaL_checknumber(L, 1);
	int height = luaL_checknumber(L, 2);
	SDL_SetWindowSize(window, width, height);
	return 0;
}

struct luaL_Reg glua_window_functions[] = {
	{ "set_title", glua_window_set_title },
	{ "set_size", glua_window_set_size },
	{ NULL, NULL }
};

int glua_window_link(lua_State *L) {
	luaL_newlib(L, glua_window_functions);
	return 0;
}
