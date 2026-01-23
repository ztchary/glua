#include "audio/audio.h"
#include "glua/glua.h"
#include "path.h"

struct glua_audio_type {
	Mix_Chunk *chunk;
	int channel;
	int pause;
};

int glua_audio_type_new(lua_State *L) {
	const char *rel_path = luaL_checkstring(L, 1);
	char path[128];
	path_join(path, project_path, rel_path);

	Mix_Chunk *chunk = Mix_LoadWAV(path);
	if (chunk == NULL) {
		luaL_error(L, "Failed to open audio file");
		lua_pushnil(L);
		return 1;
	}

	struct glua_audio_type *audio = (struct glua_audio_type *)lua_newuserdata(L, sizeof(struct glua_audio_type));
	*audio = (struct glua_audio_type){
		.chunk = chunk,
		.channel = -1,
	};

	luaL_getmetatable(L, "glua.audio");
	lua_setmetatable(L, -2);

	return 1;
}

int glua_audio_type_gc(lua_State *L) {
	struct glua_audio_type *audio = (struct glua_audio_type *)luaL_checkudata(L, 1, "glua.audio");

	Mix_FreeChunk(audio->chunk);

	return 0;
}

int glua_audio_type_play(lua_State *L) {
	struct glua_audio_type *audio = (struct glua_audio_type *)luaL_checkudata(L, 1, "glua.audio");
	int loops = lua_tonumber(L, 2);

	if (audio->pause) {
		Mix_Resume(audio->channel);
		audio->pause = 0;
		return 0;
	}

	if (audio->channel != -1) {
		Mix_HaltChannel(audio->channel);
	}

	audio->channel = Mix_PlayChannel(-1, audio->chunk, loops);

	return 0;
}

int glua_audio_type_pause(lua_State *L) {
	struct glua_audio_type *audio = (struct glua_audio_type *)luaL_checkudata(L, 1, "glua.audio");

	if (audio->channel == -1) return 0;

	Mix_Pause(audio->channel);
	audio->pause = 1;

	return 0;
}


int glua_audio_type_stop(lua_State *L) {
	struct glua_audio_type *audio = (struct glua_audio_type *)luaL_checkudata(L, 1, "glua.audio");

	if (audio->channel != -1) Mix_HaltChannel(audio->channel);
	audio->channel = -1;

	return 0;
}

int glua_audio_type_set_volume(lua_State *L) {
	struct glua_audio_type *audio = (struct glua_audio_type *)luaL_checkudata(L, 1, "glua.audio");

	float vol = luaL_checknumber(L, 2);
	luaL_argcheck(L, vol >= 0.0 && vol <= 1.0, 1, "volume must be between 0.0 and 1.0");
	Mix_VolumeChunk(audio->chunk, (int)((float)MIX_MAX_VOLUME * vol));

	return 0;
}

static const luaL_Reg glua_audio_type_methods[] = {
	{ "__gc", glua_audio_type_gc },
	{ "play", glua_audio_type_play },
	{ "pause", glua_audio_type_pause },
	{ "stop", glua_audio_type_stop },
	{ "set_volume", glua_audio_type_set_volume },
	{ NULL, NULL }
};

int glua_audio_type_link(lua_State *L) {
	luaL_newmetatable(L, "glua.audio");
	luaL_setfuncs(L, glua_audio_type_methods, 0);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_pop(L, 1);
	return 1;
}

