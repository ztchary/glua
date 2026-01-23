#include "texture/texture.h"
#include "texture/type.h"

bool glua_texture_init() { return true; }
void glua_texture_quit() {}

static const luaL_Reg glua_texture_functions[] = {
	{ "load_file", glua_texture_type_new },
	{ NULL, NULL }
};

int glua_texture_link(lua_State *L) {
	luaL_newlib(L, glua_texture_functions);
	glua_texture_type_link(L);
	return 0;
}

