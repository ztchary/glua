#pragma once
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>

int glua_graphics_draw_rect(lua_State *);
int glua_graphics_draw_rects(lua_State *);
int glua_graphics_fill_rect(lua_State *);
int glua_graphics_fill_rects(lua_State *);

