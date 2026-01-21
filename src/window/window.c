#include "window/window.h"

SDL_Window *window;
const char *name = "glua project";
int width = 800;
int height = 600;

bool glua_window_init() {
	window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	return window != NULL;
}

int glua_window_set_name(lua_State *L) {
	name = luaL_checkstring(L, 1);
	return 0;
}

int glua_window_set_size(lua_State *L) {
	width = luaL_checknumber(L, 1);
	height = luaL_checknumber(L, 2);
	return 0;
}

struct luaL_Reg glua_window_functions[] = {
	{ "set_name", glua_window_set_name },
	{ "set_size", glua_window_set_size },
	{ NULL, NULL }
};

int glua_window_link(lua_State *L) {
	luaL_newlib(L, glua_window_functions);
	return 0;
}
