#pragma once
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

bool glua_texture_init();
void glua_texture_quit();

int glua_texture_link(lua_State *);

