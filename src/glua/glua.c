#include "glua/glua.h"

bool quit = false;

bool glua_init() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) return false;
	if (!glua_window_init()) return false;
	if (!glua_graphics_init()) return false;
	if (!glua_audio_init()) return false;
	return true;
}

int glua_quit(lua_State *L) {
	quit = true;
	return 0;
}

static const luaL_Reg glua_functions[] = {
	{ "quit", glua_quit },
	{ NULL, NULL }
};

static const luaL_Reg glua_modules[] = {
	{ "keyboard", glua_keyboard_link },
	{ "graphics", glua_graphics_link },
	{ "window",   glua_window_link },
	{ "audio",    glua_audio_link },
	{ "event",    glua_event_link },
	{ NULL, NULL }
};

int glua_link(lua_State *L) {
	luaL_newlib(L, glua_functions);
	for (int i = 0; glua_modules[i].name; i++) {
		glua_modules[i].func(L);
		lua_setfield(L, -2, glua_modules[i].name);
	}
	lua_setglobal(L, "glua");
	return 0;
}
