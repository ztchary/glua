#include "graphics/graphics.h"
#include "graphics/rect.h"
#include "graphics/ellipse.h"

SDL_Renderer *renderer;
SDL_Color color;

bool glua_graphics_init() {
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (renderer) SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	return renderer != NULL;
}

int glua_graphics_set_color(lua_State *L) {
	color = *(SDL_Color *)luaL_checkudata(L, 1, "glua_color");
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
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

