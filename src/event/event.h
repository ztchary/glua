#pragma once
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>
#include "keyboard/keys.h"

void glua_event_handle(lua_State *L, SDL_Event *e);

int glua_event_link(lua_State *);

