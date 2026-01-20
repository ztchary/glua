#pragma once
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "window/window.h"

void glua_graphics_present();

bool glua_graphics_init();

int glua_graphics_link(lua_State *L);

