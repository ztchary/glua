#include "data/data.h"

const char *rect_fields[] = { "x", "y", "w", "h" };

int glua_data_rect_index(lua_State *L) {
	SDL_Rect *rect = (SDL_Rect *)luaL_checkudata(L, 1, "glua.data.rect");
	
	const char *index = luaL_checkstring(L, 2);

	for (int i = 0; i < 4; i++) {
		if (strcmp(index, rect_fields[i]) == 0) {
			lua_pushnumber(L, ((int *)rect)[i]);
			return 1;
		}
	}
	lua_pushnil(L);
	return 1;
}

int glua_data_rect_newindex(lua_State *L) {
	SDL_Rect *rect = (SDL_Rect *)luaL_checkudata(L, 1, "glua.data.rect");
	
	const char *index = luaL_checkstring(L, 2);

	int value = luaL_checknumber(L, 3);
	for (int i = 0; i < 4; i++) {
		if (strcmp(index, rect_fields[i]) == 0) {
			((int *)rect)[i] = value;
			return 0;
		}
	}
	return 0;
}

int glua_data_rect(lua_State *L) {
	int x = (int)luaL_checknumber(L, 1);
	int y = (int)luaL_checknumber(L, 2);
	int w = (int)luaL_checknumber(L, 3);
	int h = (int)luaL_checknumber(L, 4);
	SDL_Rect *rect = lua_newuserdata(L, sizeof(SDL_Rect));

	luaL_getmetatable(L, "glua.data.rect");
	lua_setmetatable(L, -2);

	*rect = (SDL_Rect){ x, y, w, h };

	return 1;
}

static const luaL_Reg glua_data_rect_functions[] = {
	{ "__index",    glua_data_rect_index },
	{ "__newindex", glua_data_rect_newindex },
	{ NULL, NULL }
};

int glua_data_rect_link(lua_State *L) {
	luaL_newmetatable(L, "glua.data.rect");
	luaL_setfuncs(L, glua_data_rect_functions, 0);
	lua_pop(L, 1);
	lua_pushcfunction(L, glua_data_rect);
	return 0;
}

