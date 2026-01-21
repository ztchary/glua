#pragma once
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <SDL2/SDL.h>
#include "keyboard/keyboard.h"
#include "graphics/graphics.h"
#include "window/window.h"
#include "audio/audio.h"
#include "event/event.h"
#include "data/data.h"

extern bool quit;

bool glua_init();

int glua_link(lua_State *, const char *);

