#pragma once
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdint.h>
#include "window/window.h"

extern SDL_Renderer *renderer;
extern uint8_t color[4];

bool glua_graphics_init();

int glua_graphics_link(lua_State *L);

