#include "graphics/graphics.h"
#include <SDL2/SDL2_gfxPrimitives.h>

SDL_Rect *glua_graphics_get_ellipses(lua_State *L, int *n) {
	*n = lua_rawlen(L, 1);
	SDL_Rect *ellipses = malloc(*n * sizeof(SDL_Rect));
	for (int i = 0; i < *n; i++) {
		lua_rawgeti(L, 1, i + 1);
		if (!lua_istable(L, 2)) {
			free(ellipses);
			luaL_error(L, "ellipses must be tables");
			return NULL;
		}
		if (lua_rawlen(L, 2) != 4) {
			free(ellipses);
			luaL_error(L, "ellipses must have four values");
			return NULL;
		}
		float *ellipse = (float *)&ellipses[i];
		for (int j = 0; j < 4; j++) {
			lua_rawgeti(L, 2, j + 1);
			if (!lua_isnumber(L, 3)) {
				free(ellipses);
				luaL_error(L, "ellipses must contain numbers");
				return NULL;
			}
			ellipse[j] = lua_tonumber(L, 3);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}
	return ellipses;
}

int glua_graphics_draw_ellipse(lua_State *L) {
	int x = (int)luaL_checknumber(L, 1);
	int y = (int)luaL_checknumber(L, 2);
	int w = (int)luaL_checknumber(L, 3);
	int h = (int)luaL_checknumber(L, 4);
	ellipseRGBA(renderer, x, y, w, h, color[0], color[1], color[2], color[3]);
	return 0;
}

int glua_graphics_draw_ellipses(lua_State *L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	int n;
	SDL_Rect *ellipses = glua_graphics_get_ellipses(L, &n);
	if (!ellipses) return 0;
	for (int i = 0; i < n; i++) {
		SDL_Rect e = ellipses[i];
		ellipseRGBA(renderer, e.x, e.y, e.w, e.h, color[0], color[1], color[2], color[3]);
	}
	free(ellipses);
	return 0;
}

int glua_graphics_fill_ellipse(lua_State *L) {
	int x = (int)luaL_checknumber(L, 1);
	int y = (int)luaL_checknumber(L, 2);
	int w = (int)luaL_checknumber(L, 3);
	int h = (int)luaL_checknumber(L, 4);
	filledEllipseRGBA(renderer, x, y, w, h, color[0], color[1], color[2], color[3]);
	return 0;
}

int glua_graphics_fill_ellipses(lua_State *L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	int n;
	SDL_Rect *ellipses = glua_graphics_get_ellipses(L, &n);
	if (!ellipses) return 0;
	for (int i = 0; i < n; i++) {
		SDL_Rect e = ellipses[i];
		filledEllipseRGBA(renderer, e.x, e.y, e.w, e.h, color[0], color[1], color[2], color[3]);
	}
	free(ellipses);
	return 0;
}

