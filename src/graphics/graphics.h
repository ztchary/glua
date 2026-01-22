#pragma once
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdint.h>

extern SDL_Renderer *renderer;
extern SDL_Color color;

bool glua_graphics_init();
void glua_graphics_quit();

int glua_graphics_link(lua_State *);

