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

void glua_graphics_present();

bool glua_graphics_init();

int glua_graphics_draw_rect(lua_State *L);
int glua_graphics_draw_rects(lua_State *L);
int glua_graphics_fill_rect(lua_State *L);
int glua_graphics_fill_rects(lua_State *L);
int glua_graphics_draw_ellipse(lua_State *L);
int glua_graphics_draw_ellipses(lua_State *L);
int glua_graphics_fill_ellipse(lua_State *L);
int glua_graphics_fill_ellipses(lua_State *L);

int glua_graphics_link(lua_State *L);

