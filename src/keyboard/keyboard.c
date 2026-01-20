#include "keyboard/keyboard.h"

int glua_keyboard_is_pressed(lua_State *L) {
	const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
	const char *keyname = luaL_checkstring(L, 1);
	for (int i = 0; i < 100; i++) {
		if (strcmp(keyname, glua_keyboard_keys[i]) == 0) {
			lua_pushboolean(L, keyboard_state[i+4]);
			return 1;
		}
	}
	return 0;
}

static const luaL_Reg glua_keyboard_functions[] = {
	{ "is_pressed", glua_keyboard_is_pressed },
	{ NULL, NULL }
};

int glua_keyboard_link(lua_State *L) {
	luaL_newlib(L, glua_keyboard_functions);
	return 0;
}
