/*  Hippo Games - 2000 (C) - http://www.prog.cz/hippo/
 *    __________________________________________________
 *   /\     __  __    _____   ______   ______   ______  \
 *   \ \   /\ \|\ \  /\_  _\ /\  __ \ /\  __ \ /\  __ \  \
 *    \ \  \ \ \_\ \ \//\ \/ \ \ \_\ \\ \ \_\ \\ \ \ \ \  \
 *     \ \  \ \  __ \  \ \ \  \ \  __/ \ \  __/ \ \ \ \ \  \
 *      \ \  \ \ \/\ \  \_\ \_ \ \ \/   \ \ \/   \ \ \_\ \  \
 *       \ \  \ \_\ \_\ /\____\ \ \_\    \ \_\    \ \_____\  \
 *        \ \  \/_/\/_/ \/____/  \/_/     \/_/     \/_____/   \
 *         \ \_________________________________________________\
 *          \/_________________________________________________/
 *                           
 *  
 *  Sub     : hfade_sprite function
 *
 *  File    : HFader.cpp
 *  
 *  Projekt : HAllegro
 *  
 *  Autor   : Bernard Lidicky [berny@prog.cz]
 *
 *  Datum   : 6.7.2000
 *
 */      

#include "halleg.h"
#include <string.h>
#include <stdio.h>

/* ::hfade_sprite() : 
 * ztmavi obrazek o 50%  (nemeni paletu)
 */
void hfade_sprite(BITMAP   *bmp) {

	if (bmp == NULL) return;

	int       i,j;
	unsigned char   *c; // 8b
	unsigned short  *s, s2; // 16b
	uint32_t   *l, l2; // 32b
	unsigned int   col; // transparent color

	j = bmp->w * bmp->h; // velikost obrazku


	switch (bitmap_color_depth(bmp)) {
	case 8:
		c = bmp->line[0];
		PALETTE pal;
		get_palette(pal);
		if (rgb_map == NULL) rgb_map =  &hscreen_self_pointer->m_rgb_map;

		for (i = 0; i < j; i++) {
			if (*c != 0) {  // aby se nepremalovavaly pruhledne pixely
				*c = rgb_map->data[pal[*c].r >> 2][pal[*c].g >> 2][pal[*c].b >> 2];
			}
			c++;
		}
		break;
	case 15:
		s   = (unsigned short *) bmp->line[0];
		col = makecol15(255, 0, 255);
		for (i = 0; i < j; i++) {
			if (*s != col) {  // aby se nepremalovavaly pruhledne pixely
				s2 = *s;
				// r,g,b
				*s = makecol15( getr15(s2) >> 1, getg15(s2) >> 1, getb15(s2) >> 1);
			}
			s++;
		}
		break;
	case 16 :
		s   = (unsigned short *) bmp->line[0];
		col = makecol16(255, 0, 255);
		for (i = 0; i < j; i++) {
			if (*s != col) {  // aby se nepremalovavaly pruhledne pixely
				s2 = *s;
				// r,g,b
				*s = makecol16( getr16(s2) >> 1, getg16(s2) >> 1, getb16(s2) >> 1);
			}
			s++;
		}
		break;
	case 24 :
		c   = bmp->line[0];
		col = makecol24(255, 0, 255);
		for (i = 0; i < j; i++) {
			l =     (uint32_t*) c;
			l2 = (*((uint32_t*) c) & 0xFFFFFF);
			if (l2 != col) {  // aby se nepremalovavaly pruhledne pixely
				// r,g,b
				*l = (*l & 0xFF000000) | makecol24( getr24(l2) >> 1, getg24(l2) >> 1, getb24(l2) >> 1);
			}
			c  += 3;
		}
		break;
	case 32:
		l   = (uint32_t *) bmp->line[0];
		col = makecol32(255, 0, 255);
		for (i = 0; i < j; i++) {
			if (*l != col) {  // aby se nepremalovavaly pruhledne pixely
				l2 = *l;
				// r,g,b
				*l = makecol32( getr32(l2) >> 1, getg32(l2) >> 1, getb32(l2) >> 1);
			}
			l++;
		}
		break;
	}
}

