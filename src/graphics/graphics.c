#include "graphics/graphics.h"

SDL_Renderer *renderer;
uint8_t color[4];

bool glua_graphics_init() {
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (renderer) SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	return renderer != NULL;
}

int glua_graphics_set_color(lua_State *L) {
	if (lua_istable(L, 1)) {
		if (lua_rawlen(L, 1) != 4) {
			luaL_error(L, "Color must have 4 values");
			return 0;
		}
		for (int i = 0; i < 4; i++) {
			lua_rawgeti(L, 1, i + 1);
			color[i] = (int)(lua_tonumber(L, 2) * 255.0);
			lua_pop(L, 1);
		}
	} else {
		color[0] = (int)(luaL_checknumber(L, 1) * 255.0);
		color[1] = (int)(luaL_checknumber(L, 2) * 255.0);
		color[2] = (int)(luaL_checknumber(L, 3) * 255.0);
		color[3] = (int)(luaL_checknumber(L, 4) * 255.0);
	}
	SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
	return 0;
}

int glua_graphics_clear(lua_State *L) {
	SDL_RenderClear(renderer);
	return 0;
}

int glua_graphics_show(lua_State *L) {
	SDL_RenderPresent(renderer);
	return 0;
}

int glua_graphics_draw_rect(lua_State *L);
int glua_graphics_draw_rects(lua_State *L);
int glua_graphics_fill_rect(lua_State *L);
int glua_graphics_fill_rects(lua_State *L);
int glua_graphics_draw_ellipse(lua_State *L);
int glua_graphics_draw_ellipses(lua_State *L);
int glua_graphics_fill_ellipse(lua_State *L);
int glua_graphics_fill_ellipses(lua_State *L);

static const luaL_Reg glua_graphics_functions[] = {
	{ "set_color",     glua_graphics_set_color },
	{ "clear",         glua_graphics_clear },
	{ "show",          glua_graphics_show },
	{ "draw_rect",     glua_graphics_draw_rect },
	{ "draw_rects",    glua_graphics_draw_rects },
	{ "fill_rect",     glua_graphics_fill_rect },
	{ "fill_rects",    glua_graphics_fill_rects },
	{ "draw_ellipse",  glua_graphics_draw_ellipse },
	{ "draw_ellipses", glua_graphics_draw_ellipses },
	{ "fill_ellipse",  glua_graphics_fill_ellipse },
	{ "fill_ellipses", glua_graphics_fill_ellipses },
	{ NULL, NULL }
};

int glua_graphics_link(lua_State *L) {
	luaL_newlib(L, glua_graphics_functions);
	return 0;
}

