#pragma once
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>

extern SDL_Window *window;

bool glua_window_init();

int glua_window_link(lua_State *);

