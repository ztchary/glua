#include "graphics/graphics.h"

SDL_FRect *glua_graphics_get_rects(lua_State *L, int *n) {
	*n = lua_rawlen(L, 1);
	SDL_FRect *rects = malloc(*n * sizeof(SDL_FRect));
	for (int i = 0; i < *n; i++) {
		lua_rawgeti(L, 1, i + 1);
		if (!lua_istable(L, 2)) {
			free(rects);
			luaL_error(L, "rects must be tables");
			return NULL;
		}
		if (lua_rawlen(L, 2) != 4) {
			free(rects);
			luaL_error(L, "rects must have four values");
			return NULL;
		}
		float *rect = (float *)&rects[i];
		for (int j = 0; j < 4; j++) {
			lua_rawgeti(L, 2, j + 1);
			if (!lua_isnumber(L, 3)) {
				free(rects);
				luaL_error(L, "rects must contain numbers");
				return NULL;
			}
			rect[j] = lua_tonumber(L, 3);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}
	return rects;
}

int glua_graphics_draw_rect(lua_State *L) {
	SDL_FRect rect;
	rect.x = luaL_checknumber(L, 1);
	rect.y = luaL_checknumber(L, 2);
	rect.w = luaL_checknumber(L, 3);
	rect.h = luaL_checknumber(L, 4);
	SDL_RenderDrawRectF(renderer, &rect);
	return 0;
}

int glua_graphics_draw_rects(lua_State *L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	int n;
	SDL_FRect *rects = glua_graphics_get_rects(L, &n);
	if (!rects) return 0;
	SDL_RenderDrawRectsF(renderer, rects, n);
	free(rects);
	return 0;
}

int glua_graphics_fill_rect(lua_State *L) {
	SDL_FRect rect;
	rect.x = luaL_checknumber(L, 1);
	rect.y = luaL_checknumber(L, 2);
	rect.w = luaL_checknumber(L, 3);
	rect.h = luaL_checknumber(L, 4);
	SDL_RenderFillRectF(renderer, &rect);
	return 0;
}

int glua_graphics_fill_rects(lua_State *L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	int n;
	SDL_FRect *rects = glua_graphics_get_rects(L, &n);
	if (!rects) return 0;
	SDL_RenderFillRectsF(renderer, rects, n);
	free(rects);
	return 0;
}

