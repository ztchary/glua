#pragma once
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>

bool glua_audio_init();

int glua_audio_link(lua_State *L);

