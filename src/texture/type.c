#include "texture/texture.h"
#include "texture/type.h"
#include "graphics/graphics.h"
#include "glua/glua.h"
#include "path.h"

int glua_texture_type_new(lua_State *L) {
	const char *rel_path = luaL_checkstring(L, 1);
	char path[128];
	path_join(path, project_path, rel_path);

	SDL_Texture *img = IMG_LoadTexture(renderer, path);
	if (img == NULL) {
		luaL_error(L, "Failed to open image file");
		lua_pushnil(L);
		return 1;
	}

	SDL_Texture **texture = (SDL_Texture **)lua_newuserdata(L, sizeof(SDL_Texture *));
	*texture = img;

	luaL_getmetatable(L, "glua.texture");
	lua_setmetatable(L, -2);

	return 1;
}

int glua_texture_type_gc(lua_State *L) {
	SDL_Texture *texture = *(SDL_Texture **)luaL_checkudata(L, 1, "glua.texture");

	SDL_DestroyTexture(texture);

	return 0;
}

int glua_texture_type_get_size(lua_State *L) {
	SDL_Texture *texture = *(SDL_Texture **)luaL_checkudata(L, 1, "glua.texture");

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	lua_pushnumber(L, w);
	lua_pushnumber(L, h);

	return 2;
}

static const luaL_Reg glua_texture_type_methods[] = {
	{ "__gc", glua_texture_type_gc },
	{ "get_size", glua_texture_type_get_size },
	{ NULL, NULL }
};

int glua_texture_type_link(lua_State *L) {
	luaL_newmetatable(L, "glua.texture");
	luaL_setfuncs(L, glua_texture_type_methods, 0);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_pop(L, 1);
	return 0;
}

