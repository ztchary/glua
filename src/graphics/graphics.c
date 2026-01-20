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

int glua_graphics_draw_rect(lua_State *L);
int glua_graphics_draw_rects(lua_State *L);
int glua_graphics_fill_rect(lua_State *L);
int glua_graphics_fill_rects(lua_State *L);

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

