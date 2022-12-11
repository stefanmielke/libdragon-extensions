#pragma once

#include <libdragon.h>

#ifdef __cplusplus
extern "C"{
#endif


/**
 * @brief Configure the render mode based on the sprite format.
 * 		  Uses copy mode whenever possible.
 *
 * @param sprite
 *        The sprite to use.
 * @return 
 *        The format of the sprite
 */
inline tex_format_t format_set_render_mode(sprite_t *sprite){
    tex_format_t sprite_format = sprite_get_format(sprite);
    
    switch(sprite_format){
		case FMT_I4:
		case FMT_I8:
			rdpq_set_mode_standard();
			break;
		case FMT_IA4:
			rdpq_set_mode_standard();
			rdpq_mode_alphacompare(ALPHACOMPARE_THRESHOLD);
			rdpq_set_blend_color(RGBA16(0,0,0,1));
			break;
		case FMT_CI4:
			rdpq_set_mode_copy(true);
			rdpq_mode_tlut(TLUT_RGBA16);
			rdpq_tex_load_tlut(sprite_get_palette(sprite), 0, 16);
			break;
		case FMT_CI8:
			rdpq_set_mode_copy(true);
			rdpq_mode_tlut(TLUT_RGBA16);
			rdpq_tex_load_tlut(sprite_get_palette(sprite), 0, 256);
			break;
		case FMT_IA8:
		case FMT_IA16:
		case FMT_RGBA16:
		case FMT_RGBA32:
			rdpq_set_mode_copy(true);
			break;
	}

    return sprite_format;
}

#ifdef __cplusplus
}
#endif