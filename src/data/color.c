#include "data/color.h"

const char *color_fields[] = { "r", "g", "b", "a" };

int glua_data_color_index(lua_State *L) {
	SDL_Color *color = (SDL_Color *)luaL_checkudata(L, 1, "glua_color");
	
	const char *index = luaL_checkstring(L, 2);

	for (int i = 0; i < 4; i++) {
		if (strcmp(index, color_fields[i]) == 0) {
			lua_pushnumber(L, (float)((unsigned char *)color)[i] / 255.0);
			return 1;
		}
	}
	lua_pushnil(L);
	return 1;
}

int glua_data_color_newindex(lua_State *L) {
	SDL_Color *color = (SDL_Color *)luaL_checkudata(L, 1, "glua_color");
	
	const char *index = luaL_checkstring(L, 2);

	int value = (int)(luaL_checknumber(L, 3) * 255.0);
	for (int i = 0; i < 4; i++) {
		if (strcmp(index, color_fields[i]) == 0) {
			((unsigned char *)color)[i] = value;
			return 0;
		}
	}
	return 0;
}

int glua_data_color(lua_State *L) {
	int r = (int)(luaL_checknumber(L, 1) * 255.0);
	int g = (int)(luaL_checknumber(L, 2) * 255.0);
	int b = (int)(luaL_checknumber(L, 3) * 255.0);
	int a = (int)(luaL_checknumber(L, 4) * 255.0);
	SDL_Color *color = lua_newuserdata(L, sizeof(SDL_Color));

	luaL_getmetatable(L, "glua_color");
	lua_setmetatable(L, -2);

	*color = (SDL_Color){ r, g, b, a };

	return 1;
}

static const luaL_Reg glua_data_color_functions[] = {
	{ "__index",    glua_data_color_index },
	{ "__newindex", glua_data_color_newindex },
	{ NULL, NULL }
};

int glua_data_color_link(lua_State *L) {
	luaL_newmetatable(L, "glua_color");
	luaL_setfuncs(L, glua_data_color_functions, 0);
	lua_pop(L, 1);
	lua_pushcfunction(L, glua_data_color);
	return 0;
}

