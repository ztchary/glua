#include "data/data.h"
#include "data/rect.h"
#include "data/color.h"

static const luaL_Reg glua_data_functions[] = {
	{ NULL, NULL }
};

static const luaL_Reg glua_data_modules[] = {
	{ "rect", glua_data_rect_link },
	{ "color", glua_data_color_link },
	{ NULL, NULL }
};

int glua_data_link(lua_State *L) {
	luaL_newlib(L, glua_data_functions);
	for (int i = 0; glua_data_modules[i].name; i++) {
		glua_data_modules[i].func(L);
		lua_setfield(L, -2, glua_data_modules[i].name);
	}
	return 0;
}

