/**
 * Original code by:
 * FlappyBird-N64 - audio.c
 * Copyright 2017, Christopher Bonhage
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/audio.h"

#include <libdragon.h>

Audio *audio_setup(const uint16_t sample_rate) {
	const uint16_t buffer_length = audio_get_buffer_length();
	int16_t *buffer = malloc(buffer_length * STEREO_PCM_SAMPLE_SIZE);
	Audio *audio = malloc(sizeof(Audio));
	audio->sample_rate = sample_rate;
	audio->frames = buffer_length << 1;
	audio->buffer = buffer;
	audio->current_bgm = 0;

	/* Setup the sound effects channels */
	for (uint8_t i = 0; i < SFX_NUM_CHANNELS; i++) {
		audio->channels[i].cursor = 0;
		audio->channels[i].sfx = NULL;
	}
	return audio;
}

void audio_load_sfx(Audio *audio, const char *sfx_files[], uint8_t sfx_count) {
	PcmSound *sfx;
	for (uint8_t i = 0; i < sfx_count; i++) {
		sfx = read_dfs_pcm_sound(sfx_files[i], audio->sample_rate, 1, false);
		audio->sfx_cache[i] = sfx;
	}
}

void audio_unload_all_sfx(Audio *audio) {
	for (uint8_t i = 0; i < AUDIO_MAX_SFX; i++) {
		if (!audio->sfx_cache[i])
			continue;
		free(audio->sfx_cache[i]->data);
		free(audio->sfx_cache[i]);
		audio->sfx_cache[i] = NULL;
	}
}

void audio_load_and_play_bgm(Audio *audio, uint8_t bgm_sound_id, const char *bgm_path) {
	return;
	if (bgm_sound_id == audio->current_bgm || bgm_sound_id == 0)
		return;

	if (audio->current_bgm != 0) {
		free(audio->channels[SFX_NUM_CHANNELS - 1].sfx->data);
		free(audio->channels[SFX_NUM_CHANNELS - 1].sfx);
	}

	PcmSound *sfx = read_dfs_pcm_sound(bgm_path, audio->sample_rate, 2, true);
	audio->current_bgm = bgm_sound_id;
	audio->channels[SFX_NUM_CHANNELS - 1].sfx = sfx;
	audio->channels[SFX_NUM_CHANNELS - 1].cursor = 0;
}

void audio_free(Audio *audio) {
	/* Clear the sound effects cache */
	for (uint8_t i = 0; i < AUDIO_MAX_SFX; i++) {
		free(audio->sfx_cache[i]->data);
		free(audio->sfx_cache[i]);
		audio->sfx_cache[i] = NULL;
	}
	/* Clear sound effects pointers from playback channels */
	for (uint8_t i = 0; i < SFX_NUM_CHANNELS; i++) {
		audio->channels[i].sfx = NULL;
	}

	/* BGM */
	if (audio->current_bgm != 0) {
		free(audio->channels[SFX_NUM_CHANNELS - 1].sfx->data);
		free(audio->channels[SFX_NUM_CHANNELS - 1].sfx);
	}

	/* Shut down the audio subsystem */
	free(audio->buffer);
	free(audio);
}

inline static int16_t mix_pcm_samples(int32_t mix, uint8_t num) {
	return (num > 1) ? mix / num : mix;
}

void audio_tick(Audio *audio) {
	if (audio && audio_can_write()) {
		SfxChannel channel;
		PcmSound *sfx;
		int32_t left_mix, right_mix;
		uint8_t left_num, right_num;
		/* Fill the audio buffer with stereo sample frames */
		for (uint16_t frame = 0; frame < audio->frames;
			 left_mix = left_num = right_mix = right_num = 0) {
			/* Accumulate all currently playing sound effects samples */
			for (uint8_t i = 0; i < SFX_NUM_CHANNELS; i++) {
				channel = audio->channels[i];
				sfx = channel.sfx;
				if (sfx && channel.cursor < sfx->samples) {
					left_mix += sfx->data[channel.cursor++];
					left_num++;
					/* Play mono sound effects in both speakers */
					if (sfx->channels == 1) {
						right_mix += sfx->data[channel.cursor - 1];
						right_num++;
					}
					/* Play stereo sound effects in separate speakers */
					else if (channel.cursor < sfx->samples) {
						right_mix += sfx->data[channel.cursor++];
						right_num++;
					}

					/* Reset channels that have finished playing */
					if (channel.cursor >= sfx->samples) {
						channel.cursor = 0;
						if (!sfx->loop)
							channel.sfx = NULL;
					}
				}
				audio->channels[i] = channel;
			}
			/* Mix down all of the samples as an average */
			audio->buffer[frame++] = mix_pcm_samples(left_mix, left_num);
			audio->buffer[frame++] = mix_pcm_samples(right_mix, right_num);
		}
		audio_write(audio->buffer);
	}
}

void audio_play_sfx(Audio *audio, const uint8_t sfx_sound) {
	if (audio) {
		PcmSound *sfx = audio->sfx_cache[sfx_sound];
		if (sfx) {
			for (uint8_t i = 0; i < SFX_NUM_CHANNELS - 1; i++) {
				if (!audio->channels[i].sfx) {
					audio->channels[i].sfx = sfx;
					audio->channels[i].cursor = 0;
					break;
				}
			}
		}
	}
}

PcmSound *read_dfs_pcm_sound(const char *file, uint16_t sample_rate, uint8_t channels, bool loop) {
	int fp = dfs_open(file);
	const uint32_t size = dfs_size(fp);
	int16_t *data = malloc(size);
	dfs_read(data, 1, size, fp);
	dfs_close(fp);
	uint32_t frames = size / MONO_PCM_SAMPLE_SIZE;
	PcmSound *sound = malloc(sizeof(PcmSound));
	sound->sample_rate = sample_rate;
	sound->channels = channels;
	sound->frames = frames;
	sound->samples = frames / channels;
	sound->data = data;
	sound->loop = loop;
	return sound;
}
