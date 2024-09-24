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
 *  Sub     : For fadedown 
 *
 *  File    : HScreenFader.cpp
 *  
 *  Projekt : HAllegro
 *  
 *  Autor   : Bernard Lidicky [berny@prog.cz]
 *
 *  Datum   : 11.7.2000
 *
 */      
#include "halleg.h"

// pouziva se pro fade screen

void fade_bitmap(BITMAP *dst, BITMAP *src, int x, int y, int cnt) {
	int sx, sy;
	int dx, dy;
	int w,h;

	w = src->w;
	h = src->h;

	// uplne vedle
	if ((x > dst->w) || (y > dst->h)) return;
	if ((x < -w) || (y < -h)) return;

	sx   = 0;
	sy   = 0;

	if (x+w > dst->w) w = dst->w-x;
	if (y+h > dst->h) h = dst->h-y;
	if (x   < 0) {
		w  += x; // x je zaporne (jinak samo w-= abs(x) )
		sx = -x; // sx = abs (x);
		dx = 0;
	}
	else dx = x;
	if (y   < 0) {
		h  += y; // x je zaporne (jinak samo w-= abs(x) )
		sy = -y; // sx = abs (x);
		dy = 0;
	}
	else dy = y;
	
	int       i,j;


	// dest bitmapa
	unsigned char   *c, *sr, sr1; // 8b
	uint32_t   *l, l2, *lsr; // 32b

	unsigned int r, g, b;

	cnt = 256 - cnt;

	switch (bitmap_color_depth(dst)) {
	case 8:
		PALETTE pal;
		get_palette(pal);

		if (rgb_map == NULL) rgb_map =  &hscreen_self_pointer->m_rgb_map;

		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			c =  dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				sr1 = *(sr++);
				*(c++)  = rgb_map->data[(pal[sr1].r * cnt) >> 9]
										     [(pal[sr1].g * cnt) >> 9]
											  [(pal[sr1].b * cnt) >> 9];
			}
		}
		break;

	case 15:
		for (i = 0; i < h; i++) {
			lsr =  (uint32_t *) src->line[sy + i] + sx;
			l   =  (uint32_t *) bmp_read_line(dst, dy+i) + dx/2;
			for (j = 0; j < w/2; j++) {

				l2 = *(lsr++);

				b =  ((cnt * (l2 & 0x001F001F)) >> 8) & 0x001F001F;
				g = (((cnt * ((l2 >> 5)  & 0x001F001F)) >> 8) & 0x001F001F) << 5;
				r = (((cnt * ((l2 >> 10) & 0x001F001F)) >> 8) & 0x001F001F) << 10;

				*(l++) = b | g | r;
			}
		}
		bmp_unwrite_line(dst);
		break;
		
	case 16 :
		for (i = 0; i < h; i++) {
			lsr =  (uint32_t *) src->line[sy + i] + sx;
			l   =  (uint32_t *) bmp_read_line(dst, dy+i) + dx/2;
			for (j = 0; j < w/2; j++) {

				l2 = *(lsr++);

				b =  ((cnt * (l2 & 0x001F001F)) >> 8) & 0x001F001F;
				g = (((cnt * ((l2 >> 5)  & 0x003F003F)) >> 8) & 0x003F003F) << 5;
				r = (((cnt * ((l2 >> 11) & 0x001F001F)) >> 8) & 0x001F001F) << 11;

				*(l++) = b | g | r;
			}
		}
		bmp_unwrite_line(dst);
		break;
		
	case 24 :
		for (i = 0; i < h; i++) {
			sr  =  (unsigned char *) src->line[sy + i] + sx*3;
			c   =  (unsigned char *) bmp_read_line(dst, dy+i) + dx*3;
			for (j = 0; j < w; j++) {

				l2 = (uint32_t)(*sr);

				b = ((cnt * ((*(sr++)) & 0xFF)) >> 8) & 0xFF;
				g = ((cnt * ((*(sr++)) & 0xFF)) >> 8) & 0xFF;
				r = ((cnt * ((*(sr++)) & 0xFF)) >> 8) & 0xFF;

				*(c++) = b;
				*(c++) = g; 
				*(c++) = r; 
			}
		}
		bmp_unwrite_line(dst);
		break;

	case 32:
		for (i = 0; i < h; i++) {
			lsr  =  (uint32_t *) src->line[sy + i] + sx;
			l    =  (uint32_t *) bmp_read_line(dst, dy+i) + dx;
			for (j = 0; j < w; j++) {

				l2 = (uint32_t)(*(lsr++));

				b = (((cnt * ((l2      ) & 0xFF)) >> 8) & 0xFF);
				g = (((cnt * ((l2 >> 8 ) & 0xFF)) >> 8) & 0xFF) << 8;
				r = (((cnt * ((l2 >> 16) & 0xFF)) >> 8) & 0xFF) << 16;

				*(l++) = r | g | b;
			}
		}
		bmp_unwrite_line(dst);
		break;
	}
}
