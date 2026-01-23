#pragma once
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int glua_texture_type_new(lua_State *);
int glua_texture_type_link(lua_State *);

