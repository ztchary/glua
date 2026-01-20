#include "graphics/graphics.h"

SDL_Renderer *renderer;

void glua_graphics_present() {
	SDL_RenderPresent(renderer);
}

bool glua_graphics_init() {
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (renderer) SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	return renderer != NULL;
}

SDL_FRect *lua_get_rects(lua_State *L, int *n) {
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

int glua_graphics_set_color(lua_State *L) {
	int r = (int)(luaL_checknumber(L, 1) * 255.0);
	int g = (int)(luaL_checknumber(L, 2) * 255.0);
	int b = (int)(luaL_checknumber(L, 3) * 255.0);
	int a = (int)(luaL_checknumber(L, 4) * 255.0);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	return 0;
}

int glua_graphics_clear(lua_State *L) {
	SDL_RenderClear(renderer);
	return 0;
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
	SDL_FRect *rects = lua_get_rects(L, &n);
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
	SDL_FRect *rects = lua_get_rects(L, &n);
	if (!rects) return 0;
	SDL_RenderFillRectsF(renderer, rects, n);
	free(rects);
	return 0;
}

static const luaL_Reg glua_graphics_functions[] = {
	{ "set_color",  glua_graphics_set_color },
	{ "clear",      glua_graphics_clear },
	{ "draw_rect",  glua_graphics_draw_rect },
	{ "draw_rects", glua_graphics_draw_rects },
	{ "fill_rect",  glua_graphics_fill_rect },
	{ "fill_rects", glua_graphics_fill_rects },
	{ NULL, NULL }
};

int glua_graphics_link(lua_State *L) {
	luaL_newlib(L, glua_graphics_functions);
	return 0;
}

