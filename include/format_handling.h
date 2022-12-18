#pragma once

#include <libdragon.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Configure the render mode based on the sprite format.
 * 		  Uses copy mode whenever possible for speed.
 *
 * @param sprite
 *        The sprite to use.
 * @param mirroring
 * 		  Whether or not to utilize mirroring (enabling certain types of mirroring requires mode
 * change). Set as false if not being used for performance.
 * @return
 *        The format of the sprite
 */
inline tex_format_t format_set_render_mode(sprite_t *sprite, bool mirroring) {
	tex_format_t sprite_format = sprite_get_format(sprite);

	if (mirroring) {
		rdpq_set_mode_standard();
		rdpq_mode_alphacompare(ALPHACOMPARE_THRESHOLD);
		rdpq_set_blend_color(RGBA16(0, 0, 0, 1));
	}

	switch (sprite_format) {
		case FMT_CI4:
			if (!mirroring)
				rdpq_set_mode_copy(true);
			rdpq_mode_tlut(TLUT_RGBA16);
			rdpq_tex_load_tlut(sprite_get_palette(sprite), 0, 16);
			break;
		case FMT_RGBA16:
			rdpq_mode_tlut(TLUT_NONE);
			if (!mirroring)
				rdpq_set_mode_copy(true);
			break;
		default:
			break;
	}
	return sprite_format;
}

#ifdef __cplusplus
}
#endif