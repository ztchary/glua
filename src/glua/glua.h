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
#include "texture/texture.h"

extern const char *project_path;

bool glua_init();
void glua_quit();

int glua_link(lua_State *, const char *);

