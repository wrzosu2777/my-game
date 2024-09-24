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
 *  Sub     : functions for shadows and lights
 *
 *  File    : HLights.cpp
 *  
 *  Projekt : HAllegro
 *  
 *  Autor   : Bernard Lidicky [berny@prog.cz]
 *
 *  Datum   : 9.7.2000
 *
 */      

#include "halleg.h"

#include <string.h>
#include <stdio.h>
/* hdraw_light_sprite:
 * Zesvetli bitmapu dst podle masky src
 */
void hdraw_light_sprite(BITMAP *dst, BITMAP *src, int x, int y){

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
	dx   = x;
	dy   = y;

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
	unsigned char   *sr; // 8b // zdrojova bitmapa

	// dest bitmapa
	unsigned char   *c; // 8b
	unsigned short  *s, s2; // 16b
	uint32_t   *l, l2; // 32b

	unsigned int alp, r,g,b;
	unsigned int col; // bila barva

	switch (bitmap_color_depth(dst)) {
	case 8:
		PALETTE pal;
		get_palette(pal);

		if (rgb_map == NULL) rgb_map =  &hscreen_self_pointer->m_rgb_map;

		col = rgb_map->data[31][31][31];
		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			c =  dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (*sr != 0 && *c != 0 && *c != col) {
					alp = ((*sr)+64);
					if ((pal[*c].r > 250) || ((r = (pal[*c].r * alp >> 7)) > 31)) r = 31;
					if ((pal[*c].g > 250) || ((g = (pal[*c].g * alp >> 7)) > 31)) g = 31;
					if ((pal[*c].g > 250) || ((b = (pal[*c].b * alp >> 7)) > 31)) b = 31;
					*c = rgb_map->data[r][g][b];
				}
				c++;
				sr++;
			}
		}
		break;
	case 15:
		col = makecol15(255, 255, 255);
		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			s =  (unsigned short *) dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (*s != 0 && *sr != 0 && *s != col) {
					s2 = *s;
					alp = ((*sr)+64);
					if ((getr15(s2) > 250) || ((r = (getr15(s2) * alp >> 6)) > 255)) r = 255;
					if ((getg15(s2) > 250) || ((g = (getg15(s2) * alp >> 6)) > 255)) g = 255;
					if ((getb15(s2) > 250) || ((b = (getb15(s2) * alp >> 6)) > 255)) b = 255;
					*s = makecol15(r, g, b);
				}

				s++;
				sr++;
			}
		}
		break;
	case 16 :
		col = makecol16(255,255,255);
		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			l  = (uint32_t*)dst->line[dy + i] + dx/2;
			for (j = 0; j < w/2; j++) {

				l2 = *l;

				alp = (*sr + 64);
				b =  l2        & 0x1F;
				g = (l2 >> 5)  & 0x3F;
				r = (l2 >> 11) & 0x1F;
				if ((r = (r * alp >> 6)) > 0x1F) r = 0x1F;
				if ((g = (g * alp >> 6)) > 0x3F) g = 0x3F;
				if ((b = (b * alp >> 6)) > 0x1F) b = 0x1F;
				col = (r << 11) | (g << 5) | (b);

				alp = (*(sr+1) + 64);
				b = (l2 >> 16) & 0x1F;
				g = (l2 >> 21) & 0x3F;
				r = (l2 >> 27) & 0x1F;
				if ((r = (r * alp >> 6)) > 0x1F) r = 0x1F;
				if ((g = (g * alp >> 6)) > 0x3F) g = 0x3F;
				if ((b = (b * alp >> 6)) > 0x1F) b = 0x1F;
				col |= ((r << 11) | (g << 5) | (b)) << 16;

				*l = col;


				l++;
				sr += 2;

			}
		}
		break;
	case 24 :

		col = makecol24(255, 255, 255);
		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			c =  dst->line[dy + i] + dx*3;
			for (j = 0; j < w; j++) {
				l2 = (*((uint32_t*) c) & 0xFFFFFF);
				if (l2 != 0 && *sr != 0 && l2 != col) {
					l =     (uint32_t*) c;
					alp = ((*sr)+64);
					if ((getr24(l2) > 250) || ((r = (getr24(l2) * alp >> 6)) > 255)) r = 255;
					if ((getg24(l2) > 250) || ((g = (getg24(l2) * alp >> 6)) > 255)) g = 255;
					if ((getb24(l2) > 250) || ((b = (getb24(l2) * alp >> 6)) > 255)) b = 255;
					*l = (*l & 0xFF000000) | makecol24( r,g,b);
				}

				c += 3;
				sr++;
			}
		}


		break;
	case 32:
		col = makecol32(255, 255, 255);
		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			l   = (uint32_t *) dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (*sr != 0 && *l != 0 && *l != col) {
					l2 = *l;
					alp = ((*sr)+64);
					if ((getr32(l2) > 250) || ((r = (getr32(l2) * alp >> 6)) > 255)) r = 255;
					if ((getg32(l2) > 250) || ((g = (getg32(l2) * alp >> 6)) > 255)) g = 255;
					if ((getb32(l2) > 250) || ((b = (getb32(l2) * alp >> 6)) > 255)) b = 255;
					*l = makecol32( r,g,b);
				}
				l++;
				sr++;
			}
		}
		break;
	}
}


/* hdraw_light_sprite:
 * Zesvetli bitmapu dst podle masky src
 */
void hdraw_light_spritetcm(BITMAP *dst, BITMAP *src, int x, int y){

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
	dx   = x;
	dy   = y;

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
	unsigned char   *sr; // 8b // zdrojova bitmapa
	unsigned short *sr16;
	uint32_t  *sr32;

	// dest bitmapa
	unsigned char   *c; // 8b
	unsigned short  *s, s2; // 16b
	uint32_t   *l, l2; // 32b

	unsigned int alp, r,g,b;
	unsigned int col; // bila barva

	switch (bitmap_color_depth(dst)) {
	case 8:
		PALETTE pal;
		get_palette(pal);

		if (rgb_map == NULL) rgb_map =  &hscreen_self_pointer->m_rgb_map;

		col = rgb_map->data[31][31][31];
		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			c =  dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (*sr != 0 && *c != 0 && *c != col) {
					alp = ((*sr)+64);
					if ((pal[*c].r > 250) || ((r = (pal[*c].r * alp >> 7)) > 31)) r = 31;
					if ((pal[*c].g > 250) || ((g = (pal[*c].g * alp >> 7)) > 31)) g = 31;
					if ((pal[*c].g > 250) || ((b = (pal[*c].b * alp >> 7)) > 31)) b = 31;
					*c = rgb_map->data[r][g][b];
				}
				c++;
				sr++;
			}
		}
		break;
	case 15:
		col = makecol15(255, 255, 255);
		for (i = 0; i < h; i++) {
			sr16   =  (unsigned short *) src->line[sy + i] + sx;
			s    =  (unsigned short *) dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (*s != 0 && getb15(*((unsigned short*)sr16)) != 0 && *s != col) {
					s2 = *s;
					alp = (getb15(*((unsigned short*)sr16))+64);
					if ((getr15(s2) > 250) || ((r = (getr15(s2) * alp >> 6)) > 255)) r = 255;
					if ((getg15(s2) > 250) || ((g = (getg15(s2) * alp >> 6)) > 255)) g = 255;
					if ((getb15(s2) > 250) || ((b = (getb15(s2) * alp >> 6)) > 255)) b = 255;
					*s = makecol15(r, g, b);
				}

				s++;
				sr16++;
			}
		}
		break;
	case 16 :
		col = makecol16(255,255,255);
		for (i = 0; i < h; i++) {
			sr16 = (unsigned short*) src->line[sy + i] + sx;
			l    = (uint32_t*)dst->line[dy + i] + dx/2;
			for (j = 0; j < w/2; j++) {

				l2 = *l;

				alp = (getb16(*sr16++) + 64);
				b =  l2        & 0x1F;
				g = (l2 >> 5)  & 0x3F;
				r = (l2 >> 11) & 0x1F;
				if ((r = (r * alp >> 6)) > 0x1F) r = 0x1F;
				if ((g = (g * alp >> 6)) > 0x3F) g = 0x3F;
				if ((b = (b * alp >> 6)) > 0x1F) b = 0x1F;
				col = (r << 11) | (g << 5) | (b);

				alp = (getb16(*sr16++) + 64);
				b = (l2 >> 16) & 0x1F;
				g = (l2 >> 21) & 0x3F;
				r = (l2 >> 27) & 0x1F;
				if ((r = (r * alp >> 6)) > 0x1F) r = 0x1F;
				if ((g = (g * alp >> 6)) > 0x3F) g = 0x3F;
				if ((b = (b * alp >> 6)) > 0x1F) b = 0x1F;
				col |= ((r << 11) | (g << 5) | (b)) << 16;

				*l = col;


				l++;
			}
		}
		break;
	case 24 :

		col = makecol24(255, 255, 255);
		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx*3;
			c =  dst->line[dy + i] + dx*3;
			for (j = 0; j < w; j++) {
				l2 = (*((uint32_t*) c) & 0xFFFFFF);
				if (l2 != 0 && *sr != 0 && l2 != col) {
					l =     (uint32_t*) c;
					alp = ((*sr)+64);
					if ((getr24(l2) > 250) || ((r = (getr24(l2) * alp >> 6)) > 255)) r = 255;
					if ((getg24(l2) > 250) || ((g = (getg24(l2) * alp >> 6)) > 255)) g = 255;
					if ((getb24(l2) > 250) || ((b = (getb24(l2) * alp >> 6)) > 255)) b = 255;
					*l = (*l & 0xFF000000) | makecol24( r,g,b);
				}

				c += 3;
				sr += 3;
			}
		}


		break;
	case 32:
		col = makecol32(255, 255, 255);
		for (i = 0; i < h; i++) {
			sr32 = (uint32_t *) src->line[sy + i] + sx;
			l    = (uint32_t *) dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (getb32(*sr32) != 0 && *l != 0 && *l != col) {
					l2 = *l;
					alp = (getb32(*sr32)+64);
					if ((getr32(l2) > 250) || ((r = (getr32(l2) * alp >> 6)) > 255)) r = 255;
					if ((getg32(l2) > 250) || ((g = (getg32(l2) * alp >> 6)) > 255)) g = 255;
					if ((getb32(l2) > 250) || ((b = (getb32(l2) * alp >> 6)) > 255)) b = 255;
					*l = makecol32( r,g,b);
				}
				l++;
				sr32++;
			}
		}
		break;
	}
}


/*
 * hdraw_shadow_sprite
 * Ztmavi  bitmapu dst podle masky src
 */
void hdraw_shadow_sprite(BITMAP *dst, BITMAP *src, int x, int y){

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

	unsigned char   *sr; // 8b // zdrojova bitmapa

	// dest bitmapa
	unsigned char   *c; // 8b
	unsigned short  *s, s2; // 16b
	uint32_t   *l, l2; // 32b

	unsigned int alp;

	switch (bitmap_color_depth(dst)) {
	case 8:
		PALETTE pal;
		get_palette(pal);

		if (rgb_map == NULL) rgb_map =  &hscreen_self_pointer->m_rgb_map;

		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			c =  dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (*sr != 0 && *c != 0) {
					alp = ((*sr)+64);
				*c  = rgb_map->data[(pal[*c].r<<5)/alp]
					                [(pal[*c].g<<5)/alp]
										 [(pal[*c].b<<5)/alp];
				}
				c++;
				sr++;
			}
		}
		break;
	case 15:
		for (i = 0; i < h; i++) {
			sr   = src->line[sy + i] + sx;
			s    = (unsigned short *) dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (*s != 0 && *sr != 0) {
					s2 = *s;
					alp = ((*sr)+64);
					*s = makecol15((getr15(s2) << 6) / alp, 
										(getg15(s2) << 6) / alp,
										(getb15(s2) << 6) / alp);
				}

				s++;
				sr++;
			}
		}
		break;
	case 16 :
	
		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			s =  (unsigned short *) dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (*sr != 0 && *s != 0) {
					s2 = *s;
					alp = ((*sr)+64);
					*s = makecol16((getr16(s2) << 6) / alp, 
										(getg16(s2) << 6) / alp,
										(getb16(s2) << 6) / alp);
				}
				s++;
				sr++;
			}
		}

		break;
	case 24 :

		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			c =  dst->line[dy + i] + dx*3;
			for (j = 0; j < w; j++) {
				l2 = (*((uint32_t*) c) & 0xFFFFFF);
				if (*sr != 0 && l2 != 0) {
					l =     (uint32_t*) c;
					alp = ((*sr)+64);
					*l = (*l & 0xFF000000) | makecol24((getr24(l2) << 6) / alp, 
																  (getg24(l2) << 6) / alp,
																  (getb24(l2) << 6) / alp);
				}

				c += 3;
				sr++;
			}
		}


		break;
	case 32:
		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			l   = (uint32_t *) dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (*sr != 0 && *l != 0) {
					l2 = *l;
					alp = (((*sr))+64);
					*l = makecol32((getr32(l2) << 6) / alp, 
										(getg32(l2) << 6) / alp,
										(getb32(l2) << 6) / alp);
				}
				l++;
				sr++;
			}
		}
		break;
	}
}


/*
 * hdraw_shadow_sprite
 * Ztmavi  bitmapu dst podle masky src
 */
void hdraw_shadow_spritetcm(BITMAP *dst, BITMAP *src, int x, int y){

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
	unsigned char   *sr; // 8b // zdrojova bitmapa

	unsigned short *sr16;
	uint32_t  *sr32;
	uint32_t  sr24;

	// dest bitmapa
	unsigned char   *c, *c2; // 8b
	unsigned short  *s, s2; // 16b
	uint32_t   *l, l2; // 32b

	unsigned int alp;
	unsigned int alpr;
	unsigned int alpg;
	unsigned int alpb;

	switch (bitmap_color_depth(dst)) {
	case 8:
		PALETTE pal;
		get_palette(pal);

		if (rgb_map == NULL) rgb_map =  &hscreen_self_pointer->m_rgb_map;

		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			c =  dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (*sr != 0 && *c != 0) {
					alp = ((*sr)+64);
				*c  = rgb_map->data[(pal[*c].r<<5)/alp]
					                [(pal[*c].g<<5)/alp]
										 [(pal[*c].b<<5)/alp];
				}
				c++;
				sr++;
			}
		}
		break;
	case 15:
		for (i = 0; i < h; i++) {
			sr16 = (unsigned short *) src->line[sy + i] + sx;
			s =    (unsigned short *) dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (*s != 0 && *sr16 != 0) {
					s2 = *s;
					alpr = (getr15(*((unsigned short*)sr16))+64);
					alpg = (getg15(*((unsigned short*)sr16))+64);
					alpb = (getb15(*((unsigned short*)sr16))+64);
					*s = makecol15((getr15(s2) << 6) / alpr, 
										(getg15(s2) << 6) / alpg,
										(getb15(s2) << 6) / alpb);
				}

				s++;
				sr16++;
			}
		}
		break;
	case 16 :
	
		for (i = 0; i < h; i++) {
			sr16 =  (unsigned short *) src->line[sy + i] + sx;
			s =     (unsigned short *) dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (*s != 0 && *sr16 != 0) {
					s2 = *s;
					alpr = (getr16(*((unsigned short*)sr16))+64);
					alpg = (getg16(*((unsigned short*)sr16))+64);
					alpb = (getb16(*((unsigned short*)sr16))+64);
					*s = makecol15((getr16(s2) << 6) / alpr, 
										(getg16(s2) << 6) / alpg,
										(getb16(s2) << 6) / alpb);
				}

				s++;
				sr16++;
			}
		}

		break;
	case 24 :

		for (i = 0; i < h; i++) {
			c2 = src->line[sy + i] + sx;
			c  =  dst->line[dy + i] + dx*3;
			for (j = 0; j < w; j++) {
				l2   = (*((uint32_t*) c) & 0xFFFFFF);
				sr24 = (*((uint32_t*) c2) & 0xFFFFFF);
				if (sr24 != 0 && l2 != 0) {
					l =     (uint32_t*) c;
					alpr = (getr24(sr24)+64);
					alpg = (getg24(sr24)+64);
					alpb = (getb24(sr24)+64);
					*l = (*l & 0xFF000000) | makecol24((getr24(l2) << 6) / alpr, 
																  (getg24(l2) << 6) / alpg,
																  (getb24(l2) << 6) / alpb);
				}

				c += 3;
				c2 += 3;
			}
		}


		break;
	case 32:
		for (i = 0; i < h; i++) {
			sr32  = (uint32_t *) src->line[sy + i] + sx;
			l   = (uint32_t *) dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (*sr32 != 0 && *l != 0) {
					l2 = *l;
					alpr = (((getr32(*sr32)))+64);
					alpg = (((getr32(*sr32)))+64);
					alpb = (((getr32(*sr32)))+64);
					*l = makecol32((getr32(l2) << 6) / alpr, 
										(getg32(l2) << 6) / alpg,
										(getb32(l2) << 6) / alpb);
				}
				l++;
				sr32++;
			}
		}
		break;
	}
}

/* ::hload_shadow_bitmap() : 
 * nacte 8BPP bitmapu s askou stinu (svetla)
 */
BITMAP *hload_shadow_bitmap(char *filename) {
	BITMAP   *bmp;
	PALETTE   pal;
	

	bmp = load_bitmap(filename, pal);
	if (bmp == NULL) {
		char filename2[255];
		sprintf(filename2, "/usr/local/share/kulic/%s", filename);
		bmp = load_bitmap(filename2, pal);
		
		if (bmp == NULL) {
			sprintf(filename2, "/usr/share/kulic/%s", filename);
			bmp = load_bitmap(filename2, pal);
		}
	}
	
	if (bmp == NULL) return NULL;
	
	if (bitmap_color_depth(screen) != 8) {
		BITMAP   *tmp;
		tmp = create_bitmap_ex(8, bmp->w, bmp->h);
		
		unsigned char *c = tmp->line[0];

		for (int y = 0; y < bmp->h; y++)
			for (int x = 0; x < bmp->w; x++)	{
				*c = getr(getpixel(bmp, x, y));
				c++;
			}
		destroy_bitmap(bmp);
		return tmp;
	}
	return bmp;
}
