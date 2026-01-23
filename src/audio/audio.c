#include "audio/audio.h"
#include "audio/type.h"

#define AUDIO_FORMAT_INIT MIX_INIT_MP3 | MIX_INIT_OGG

bool glua_audio_init() {
	if (Mix_Init(AUDIO_FORMAT_INIT) == 0) return false;
	if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096) < 0) return false;

	Mix_AllocateChannels(16);

	return true;
}

void glua_audio_quit() {
	Mix_Quit();
}

static const luaL_Reg glua_audio_functions[] = {
	{ "load_file", glua_audio_type_new },
	{ NULL, NULL }
};

int glua_audio_link(lua_State *L) {
	luaL_newlib(L, glua_audio_functions);
	glua_audio_type_link(L);
	return 0;
}

