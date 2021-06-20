/**
 * Original code by:
 * FlappyBird-N64 - audio.h
 * Copyright 2017, Christopher Bonhage
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Audio definitions */

#define AUDIO_FREQUENCY 32000
#define AUDIO_BUFFERS 4
#define MONO_PCM_SAMPLE_SIZE sizeof(int16_t)
#define STEREO_PCM_SAMPLE_SIZE (MONO_PCM_SAMPLE_SIZE << 1)
#define AUDIO_MAX_SFX 10

/* Sound FX definitions */

// Number of sfx it can play at a time. Last one is reserved for BGM.
#define SFX_NUM_CHANNELS AUDIO_BUFFERS

typedef struct {
	uint16_t sample_rate;
	uint8_t channels;
	uint32_t frames;
	uint32_t samples;
	int16_t *data;
	bool loop;
} PcmSound;

typedef struct {
	uint32_t cursor;
	PcmSound *sfx;
} SfxChannel;

typedef struct {
	// Setup state
	uint16_t sample_rate;
	uint32_t frames;
	int16_t *buffer;
	PcmSound *sfx_cache[AUDIO_MAX_SFX];

	uint8_t current_bgm;
	PcmSound *bgm_sound;
	// Playback state
	SfxChannel channels[SFX_NUM_CHANNELS];
} Audio;

/* Audio functions */

Audio *audio_setup(const uint16_t sample_rate);
void audio_free(Audio *audio);

void audio_load_sfx(Audio *audio, const char *sfx_files[], uint8_t sfx_count);
void audio_unload_all_sfx(Audio *audio);

void audio_tick(Audio *audio);
void audio_play_sfx(Audio *audio, const uint8_t sfx_sound);
void audio_load_and_play_bgm(Audio *audio, uint8_t bgm_sound_id, const char *bgm_path);

PcmSound *read_dfs_pcm_sound(const char *file, uint16_t sample_rate, uint8_t channels, bool loop);

#define PLAY_AUDIO(audio) audio_play_sfx(audio_player, audio)
