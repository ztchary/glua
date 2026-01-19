#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "keys.h"

SDL_Window *window;
SDL_Renderer *renderer;
bool quit = false;
int glua_draw_func = LUA_REFNIL;
int glua_key_func = LUA_REFNIL;
bool sdl_init = false;

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

void assert_initd(lua_State *L) {
	if (!window || !renderer) {
		luaL_error(L, "glua must be initialized");
	}
}

void crash_if(lua_State *L, bool do_crash) {
	if (!do_crash) return;
	const char *err = lua_tostring(L, -1);
	fprintf(stderr, "%s\n", err);
	lua_close(L);
	exit(1);
}

int glua_quit(lua_State *L) {
	quit = true;
	return 0;
}

int glua_init(lua_State *L) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		lua_pushboolean(L, 0);
		return 1;
	}

	int width = luaL_checknumber(L, 1);
	int height = luaL_checknumber(L, 2);

	window = SDL_CreateWindow("glua", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (renderer) SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	lua_pushboolean(L, window && renderer);
	return 1;
}

int glua_set_draw(lua_State *L) {
	if (!lua_isfunction(L, 1)) {
		return 0;
	}

	if (glua_draw_func != LUA_REFNIL) {
		luaL_unref(L, LUA_REGISTRYINDEX, glua_draw_func);
	}

	glua_draw_func = luaL_ref(L, LUA_REGISTRYINDEX);
	return 0;
}

int glua_set_color(lua_State *L) {
	assert_initd(L);
	int r = (int)(luaL_checknumber(L, 1) * 255.0);
	int g = (int)(luaL_checknumber(L, 2) * 255.0);
	int b = (int)(luaL_checknumber(L, 3) * 255.0);
	int a = (int)(luaL_checknumber(L, 4) * 255.0);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	return 0;
}

int glua_on_key(lua_State *L) {
	if (!lua_isfunction(L, 1)) {
		return 0;
	}

	if (glua_key_func != LUA_REFNIL) {
		luaL_unref(L, LUA_REGISTRYINDEX, glua_key_func);
	}

	glua_key_func = luaL_ref(L, LUA_REGISTRYINDEX);
	return 0;
}

int glua_clear(lua_State *L) {
	assert_initd(L);
	SDL_RenderClear(renderer);
	return 0;
}

int glua_draw_rect(lua_State *L) {
	assert_initd(L);
	SDL_FRect rect;
	rect.x = luaL_checknumber(L, 1);
	rect.y = luaL_checknumber(L, 2);
	rect.w = luaL_checknumber(L, 3);
	rect.h = luaL_checknumber(L, 4);
	SDL_RenderDrawRectF(renderer, &rect);
	return 0;
}

int glua_draw_rects(lua_State *L) {
	assert_initd(L);
	luaL_checktype(L, 1, LUA_TTABLE);
	int n;
	SDL_FRect *rects = lua_get_rects(L, &n);
	if (!rects) return 0;
	SDL_RenderDrawRectsF(renderer, rects, n);
	free(rects);
	return 0;
}

int glua_fill_rect(lua_State *L) {
	assert_initd(L);
	SDL_FRect rect;
	rect.x = luaL_checknumber(L, 1);
	rect.y = luaL_checknumber(L, 2);
	rect.w = luaL_checknumber(L, 3);
	rect.h = luaL_checknumber(L, 4);
	SDL_RenderFillRectF(renderer, &rect);
	return 0;
}

int glua_fill_rects(lua_State *L) {
	assert_initd(L);
	luaL_checktype(L, 1, LUA_TTABLE);
	int n;
	SDL_FRect *rects = lua_get_rects(L, &n);
	if (!rects) return 0;
	SDL_RenderFillRectsF(renderer, rects, n);
	free(rects);
	return 0;
}

struct luaL_Reg glua_api[] = {
	{"quit",       glua_quit},
	{"init",       glua_init},
	{"set_color",  glua_set_color},
	{"set_draw",   glua_set_draw},
	{"on_key",     glua_on_key},
	{"clear",      glua_clear},
	{"draw_rect",  glua_draw_rect},
	{"draw_rects", glua_draw_rects},
	{"fill_rect",  glua_fill_rect},
	{"fill_rects", glua_fill_rects},
	{NULL, NULL},
};

void init_glua_api(lua_State *L) {
	luaL_newlib(L, glua_api);
	lua_pushstring(L, "keycode");
	lua_newtable(L);
	for (int i = 0; i < sizeof(keys) / sizeof(*keys); i++) {
		lua_pushstring(L, keys[i].key);
		lua_pushnumber(L, keys[i].value);
		lua_settable(L, -3);
	}
	lua_settable(L, -3);
	
	lua_setglobal(L, "glua");
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Provide a lua file\n");
		return 1;
	}

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	init_glua_api(L);

	crash_if(L, luaL_dofile(L, argv[1]) != 0);

	unsigned int last_tick = SDL_GetTicks();
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = true;
			if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
				if (glua_key_func == LUA_REFNIL) continue;
				lua_rawgeti(L, LUA_REGISTRYINDEX, glua_key_func);
				lua_pushnumber(L, e.key.keysym.scancode);
				lua_pushboolean(L, e.type == SDL_KEYDOWN);
				crash_if(L, lua_pcall(L, 2, 0, 0) != LUA_OK);
			}
		}
		if (glua_draw_func == LUA_REFNIL) continue;
		unsigned int cur_tick = SDL_GetTicks();
		lua_rawgeti(L, LUA_REGISTRYINDEX, glua_draw_func);
		lua_pushnumber(L, (float)(cur_tick - last_tick) / 1000.0);
		crash_if(L, lua_pcall(L, 1, 0, 0) != LUA_OK);
		SDL_RenderPresent(renderer);
		last_tick = cur_tick;
	}

	lua_close(L);
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	SDL_Quit();
}

