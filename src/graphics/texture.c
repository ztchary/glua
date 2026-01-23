#include "graphics/graphics.h"
#include "graphics/texture.h"
#include "texture/type.h"

int glua_graphics_draw_texture(lua_State *L) {
	SDL_Texture *texture = *(SDL_Texture **)luaL_checkudata(L, 1, "glua.texture");

	SDL_Rect *rect = (SDL_Rect *)luaL_checkudata(L, 2, "glua.data.rect");

	SDL_RenderCopy(renderer, texture, NULL, rect);
	return 0;
}

