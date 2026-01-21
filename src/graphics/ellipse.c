#include "graphics/graphics.h"
#include <SDL2/SDL2_gfxPrimitives.h>

SDL_Rect *glua_graphics_get_ellipses(lua_State *L, int *n) {
	*n = lua_rawlen(L, 1);
	SDL_Rect *ellipses = malloc(*n * sizeof(SDL_Rect));
	for (int i = 0; i < *n; i++) {
		lua_rawgeti(L, 1, i + 1);
		SDL_Rect *ellipse = luaL_checkudata(L, 2, "glua_rect");
		if (ellipse == NULL) {
			free(ellipses);
			return NULL;
		}
		ellipses[i] = *ellipse;
		lua_pop(L, 1);
	}
	return ellipses;
}

int glua_graphics_draw_ellipse(lua_State *L) {
	SDL_Rect *ellipse = luaL_checkudata(L, 1, "glua_rect");
	ellipseRGBA(renderer, ellipse->x, ellipse->y, ellipse->w, ellipse->h, color.r, color.g, color.b, color.a);
	return 0;
}

int glua_graphics_draw_ellipses(lua_State *L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	int n;
	SDL_Rect *ellipses = glua_graphics_get_ellipses(L, &n);
	if (!ellipses) return 0;
	for (int i = 0; i < n; i++) {
		SDL_Rect e = ellipses[i];
		ellipseRGBA(renderer, e.x, e.y, e.w, e.h, color.r, color.g, color.b, color.a);
	}
	free(ellipses);
	return 0;
}

int glua_graphics_fill_ellipse(lua_State *L) {
	SDL_Rect *ellipse = luaL_checkudata(L, 1, "glua_rect");
	filledEllipseRGBA(renderer, ellipse->x, ellipse->y, ellipse->w, ellipse->h, color.r, color.g, color.b, color.a);
	return 0;
}

int glua_graphics_fill_ellipses(lua_State *L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	int n;
	SDL_Rect *ellipses = glua_graphics_get_ellipses(L, &n);
	if (!ellipses) return 0;
	for (int i = 0; i < n; i++) {
		SDL_Rect e = ellipses[i];
		filledEllipseRGBA(renderer, e.x, e.y, e.w, e.h, color.r, color.g, color.b, color.a);
	}
	free(ellipses);
	return 0;
}

