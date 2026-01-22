#pragma once
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

bool glua_audio_init();
void glua_audio_quit();

int glua_audio_link(lua_State *);

