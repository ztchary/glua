#include "graphics/graphics.h"

SDL_Rect *glua_graphics_get_rects(lua_State *L, int *n) {
	*n = lua_rawlen(L, 1);
	SDL_Rect *rects = malloc(*n * sizeof(SDL_Rect));
	for (int i = 0; i < *n; i++) {
		lua_rawgeti(L, 1, i + 1);
		SDL_Rect *rect = luaL_checkudata(L, 2, "glua.data.Rect");
		if (rect == NULL) {
			free(rects);
			return NULL;
		}
		rects[i] = *rect;
		lua_pop(L, 1);
	}
	return rects;
}

int glua_graphics_draw_rect(lua_State *L) {
	SDL_Rect *rect = luaL_checkudata(L, 1, "glua.data.Rect");
	SDL_RenderDrawRect(renderer, rect);
	return 0;
}

int glua_graphics_draw_rects(lua_State *L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	int n;
	SDL_Rect *rects = glua_graphics_get_rects(L, &n);
	if (!rects) return 0;
	SDL_RenderDrawRects(renderer, rects, n);
	free(rects);
	return 0;
}

int glua_graphics_fill_rect(lua_State *L) {
	SDL_Rect *rect = luaL_checkudata(L, 1, "glua.data.Rect");
	SDL_RenderFillRect(renderer, rect);
	return 0;
}

int glua_graphics_fill_rects(lua_State *L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	int n;
	SDL_Rect *rects = glua_graphics_get_rects(L, &n);
	if (!rects) return 0;
	SDL_RenderFillRects(renderer, rects, n);
	free(rects);
	return 0;
}

