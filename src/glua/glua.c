#include "glua/glua.h"

const char *project_path;

bool glua_init() {
	SDL_SetHint(SDL_HINT_VIDEODRIVER, "x11"); // fixes weird resize issues with wayland
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;
	if (!glua_window_init()) return false;
	if (!glua_graphics_init()) return false;
	if (!glua_audio_init()) return false;
	return true;
}

void glua_quit() {
	glua_audio_quit();
	glua_graphics_quit();
	glua_window_quit();
	SDL_Quit();
}

int glua_exit(lua_State *L) {
	(void)L;
	SDL_PushEvent(&(SDL_Event){ .type = SDL_QUIT });
	return 0;
}

static const luaL_Reg glua_functions[] = {
	{ "exit", glua_exit },
	{ NULL, NULL }
};

static const luaL_Reg glua_modules[] = {
	{ "keyboard", glua_keyboard_link },
	{ "graphics", glua_graphics_link },
	{ "window",   glua_window_link },
	{ "audio",    glua_audio_link },
	{ "event",    glua_event_link },
	{ "data",     glua_data_link },
	{ NULL, NULL }
};

void glua_add_path(lua_State* L, const char* path) {
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "path");
	const char* cur_path = lua_tostring(L, -1);

	char new_path[512];
	snprintf(new_path, sizeof(new_path), "%s;%s/?.lua", cur_path, path);
	lua_pushstring(L, new_path);
	lua_setfield(L, -3, "path");
	lua_pop(L, 1);
}

int glua_link(lua_State *L, const char *path) {
	project_path = path;
	glua_add_path(L, project_path);
	luaL_newlib(L, glua_functions);
	for (int i = 0; glua_modules[i].name; i++) {
		glua_modules[i].func(L);
		lua_setfield(L, -2, glua_modules[i].name);
	}
	lua_setglobal(L, "glua");
	return 0;
}

