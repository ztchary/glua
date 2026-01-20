#include "audio/audio.h"

SDL_AudioDeviceID audio_id;

bool glua_audio_init() {
	SDL_AudioSpec spec = {
		.freq = 44100,
		.format = AUDIO_S16SYS,
		.channels = 1,
		.samples = 1024,
		.callback = NULL,
	};

	audio_id = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);

	return true;
}

int glua_audio_play_samples(lua_State *L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	int n = lua_rawlen(L, 1);
	short *samples = malloc(sizeof(short) * n);
	for (int i = 0; i < n; i++) {
		lua_rawgeti(L, 1, i + 1);
		if (!lua_isnumber(L, 2)) {
			free(samples);
			luaL_error(L, "samples must be numbers");
			return 0;
		}
		samples[i] = (short)(lua_tonumber(L, 2) * 32767.0);
		lua_pop(L, 1);
	}
	SDL_QueueAudio(audio_id, samples, sizeof(short) * n);
	SDL_PauseAudioDevice(audio_id, 0);
	free(samples);
	return 0;
}

static const luaL_Reg glua_audio_functions[] = {
	{ "play_samples", glua_audio_play_samples },
	{ NULL, NULL }
};

int glua_audio_link(lua_State *L) {
	luaL_newlib(L, glua_audio_functions);
	return 0;
}

