#include "event/event.h"

void glua_call_event(lua_State *L, const char *name, int n_args) {
	lua_getglobal(L, "glua");
	lua_getfield(L, -1, "event");
	lua_remove(L, -2);
	lua_getfield(L, -1, name);
	lua_remove(L, -2);
	if (!lua_isfunction(L, -1)) {
		lua_pop(L, 1 + n_args);
		return;
	}
	lua_insert(L, -1 - n_args);
	lua_pcall(L, n_args, 0, 0);
}

void glua_event_handle(lua_State *L, SDL_Event *e) {
	switch (e->type) {
	case SDL_KEYDOWN:
		if (e->key.repeat != 0) break;
		lua_pushstring(L, glua_keyboard_keys[e->key.keysym.scancode - 4]);
		glua_call_event(L, "on_keydown", 1);
		break;
	case SDL_KEYUP:
		lua_pushstring(L, glua_keyboard_keys[e->key.keysym.scancode - 4]);
		glua_call_event(L, "on_keyup", 1);
		break;
	case SDL_QUIT:
		glua_call_event(L, "on_quit", 0);
		break;
	default:
		break;
	}
}

static const luaL_Reg glua_event_functions[] = {
	{ NULL, NULL }
};

int glua_event_link(lua_State *L) {
	luaL_newlib(L, glua_event_functions);
	return 0;
}
