#include "audio/audio.h"
#include "glua/glua.h"
#include "path.h"

struct glua_audio_source {
	Mix_Chunk *chunk;
	int channel;
	int pause;
};

int glua_audio_source_new(lua_State *L) {
	const char *rel_path = luaL_checkstring(L, 1);
	char path[128];
	path_join(path, project_path, rel_path);

	Mix_Chunk *chunk = Mix_LoadWAV(path);
	if (chunk == NULL) {
		luaL_error(L, "Failed to open audio file");
		lua_pushnil(L);
		return 1;
	}

	struct glua_audio_source *source = (struct glua_audio_source *)lua_newuserdata(L, sizeof(struct glua_audio_source));
	*source = (struct glua_audio_source){
		.chunk = chunk,
		.channel = -1,
	};

	luaL_getmetatable(L, "glua.audio.Source");
	lua_setmetatable(L, -2);

	return 1;
}

int glua_audio_source_gc(lua_State *L) {
	struct glua_audio_source *source = (struct glua_audio_source *)luaL_checkudata(L, 1, "glua.audio.Source");

	Mix_FreeChunk(source->chunk);

	return 0;
}

int glua_audio_source_play(lua_State *L) {
	struct glua_audio_source *source = (struct glua_audio_source *)luaL_checkudata(L, 1, "glua.audio.Source");
	int loops = lua_tonumber(L, 2);

	if (source->pause) {
		Mix_Resume(source->channel);
		source->pause = 0;
		return 0;
	}

	if (source->channel != -1) {
		Mix_HaltChannel(source->channel);
	}

	source->channel = Mix_PlayChannel(-1, source->chunk, loops);

	return 0;
}

int glua_audio_source_pause(lua_State *L) {
	struct glua_audio_source *source = (struct glua_audio_source *)luaL_checkudata(L, 1, "glua.audio.Source");

	if (source->channel == -1) return 0;

	Mix_Pause(source->channel);
	source->pause = 1;

	return 0;
}


int glua_audio_source_stop(lua_State *L) {
	struct glua_audio_source *source = (struct glua_audio_source *)luaL_checkudata(L, 1, "glua.audio.Source");

	if (source->channel != -1) Mix_HaltChannel(source->channel);
	source->channel = -1;

	return 0;
}

int glua_audio_source_set_volume(lua_State *L) {
	struct glua_audio_source *source = (struct glua_audio_source *)luaL_checkudata(L, 1, "glua.audio.Source");

	float vol = luaL_checknumber(L, 2);
	luaL_argcheck(L, vol >= 0.0 && vol <= 1.0, 1, "volume must be between 0.0 and 1.0");
	Mix_VolumeChunk(source->chunk, (int)((float)MIX_MAX_VOLUME * vol));

	return 0;
}

static const luaL_Reg glua_audio_source_methods[] = {
	{ "__gc", glua_audio_source_gc },
	{ "play", glua_audio_source_play },
	{ "pause", glua_audio_source_pause },
	{ "stop", glua_audio_source_stop },
	{ "set_volume", glua_audio_source_set_volume },
	{ NULL, NULL }
};

int glua_audio_source_link(lua_State *L) {
	luaL_newmetatable(L, "glua.audio.Source");
	luaL_setfuncs(L, glua_audio_source_methods, 0);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_pop(L, 1);
	lua_pushcfunction(L, glua_audio_source_new);
	return 1;
}

