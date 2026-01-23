#pragma once
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>

int glua_graphics_draw_ellipse(lua_State *);
int glua_graphics_draw_ellipses(lua_State *);
int glua_graphics_fill_ellipse(lua_State *);
int glua_graphics_fill_ellipses(lua_State *);

