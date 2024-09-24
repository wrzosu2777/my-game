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
 *  Sub     : hmasked_sprite function
 *
 *  File    : HMaskBlit.cpp
 *  
 *  Projekt : HAllegro
 *  
 *  Autor   : Bernard Lidicky [berny@prog.cz]
 *
 *  Datum   : 18.7.2000
 *
 */      

#include "halleg.h"

void hmasked_sprite(BITMAP *dst, BITMAP *src, BITMAP *mask, int x, int y)
{
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
	unsigned char   *cs, *cd, *cm; // 8b  & 24b
	unsigned short  *ss, *sd;      // 16b & 15b
	uint32_t   *ls, *ld;      // 32b

	uint32_t  uls, uld;
	unsigned short uss, usd;
	unsigned char  ucm;

	unsigned int sr,sg,sb, r,g,b;

	unsigned int col;

	switch (bitmap_color_depth(dst)) {
	case 8:
		break;

	case 15:

		col = makecol15(255, 0, 255);
		for (i = 0; i < h; i++) {
			ss = (unsigned short *) src->line[sy + i] + sx;
			sd = (unsigned short *) dst->line[dy + i] + dx;
			cm = mask->line[sy+i] + sx;
			for (j = 0; j < w; j++) {
				if ((ucm = *(cm++)) == 256 || (uss = *(ss++)) == col){
					sd++;
					continue;
				}
				usd = *sd;
				
				sr = (uss >> 10) & 0x1F;
				sg = (uss >>  5) & 0x1F;
				sb =  uss        & 0x1F;
				r = (ucm * (((usd >> 10) & 0x1F) - sr) >> 8) + sr;
				g = (ucm * (((usd >> 5)  & 0x1F) - sg) >> 8) + sg;
				b = (ucm * (( usd        & 0x1F) - sb) >> 8) + sb;

				*(sd++) =  (r << 10) | (g << 5) | b;
			}
		}



		break;
	case 16 :
	
		col = makecol16(255, 0, 255);
		for (i = 0; i < h; i++) {
			ss = (unsigned short *) src->line[sy + i] + sx;
			sd = (unsigned short *) dst->line[dy + i] + dx;
			cm = mask->line[sy+i] + sx;
			for (j = 0; j < w; j++) {
				if ((ucm = *(cm++)) == 256 || (uss = *(ss++)) == col){
					sd++;
					continue;
				}
				usd = *sd;
				
				sr = (uss >> 11) & 0x1F;
				sg = (uss >>  5) & 0x3F;
				sb =  uss        & 0x1F;
				r = ((ucm * (((usd >> 11) & 0x1F) - sr) >> 8) + sr);
				g = ((ucm * (((usd >> 5)  & 0x3F) - sg) >> 8) + sg);
				b = ((ucm * (( usd        & 0x1F) - sb) >> 8) + sb);

				*(sd++) =  (r << 11) | (g << 5) | b;
			}
		}

		break;
	case 24 :
		col = makecol24(255, 0, 255);
		for (i = 0; i < h; i++) {
			cs = (unsigned char *) src->line[sy + i] + sx*3;
			cd = (unsigned char *) dst->line[dy + i] + dx*3;
			cm = mask->line[sy+i] + sx;
			for (j = 0; j < w; j++) {
				if ((ucm = *(cm++)) == 256 || (uls = *((uint32_t *)cs)) == col){
					cd += 3;
					cs += 3;
					continue;
				}
				uld = *((uint32_t *)cd);
				
				sr = (uls >> 16) & 0xFF;
				sg = (uls >>  8) & 0xFF;
				sb =  uls        & 0xFF;
				r = ((ucm * (((uld >> 16) & 0xFF) - sr) >> 8) + sr);
				g = ((ucm * (((uld >> 8)  & 0xFF) - sg) >> 8) + sg);
				b = ((ucm * (( uld        & 0xFF) - sb) >> 8) + sb);

				*((uint32_t *)cd) = (uld & (0xFF000000)) | (r << 16) | (g << 8) | b;
				cd += 3;
				cs += 3;
			}
		}
		break;

	case 32:
		col = makecol32(255, 0, 255);
		for (i = 0; i < h; i++) {
			ls = (uint32_t *) src->line[sy + i] + sx;
			ld = (uint32_t *) dst->line[dy + i] + dx;
			cm = mask->line[sy+i] + sx;
			for (j = 0; j < w; j++) {
				if ((ucm = *(cm++)) == 255 || (uls = *(ls++)) == col){
					ld++;
					continue;
				}
				uld = *ld;
				
				sr = (uls >> 16) & 0xFF;
				sg = (uls >>  8) & 0xFF;
				sb =  uls        & 0xFF;
				r = ((ucm * (((uld >> 16) & 0xFF) - sr) >> 8) + sr);
				g = ((ucm * (((uld >> 8)  & 0xFF) - sg) >> 8) + sg);
				b = ((ucm * (( uld        & 0xFF) - sb) >> 8) + sb);

				*(ld++) =  (r << 16) | (g << 8) | b;
			}
		}
		break;
	}

}


void hmasked_spritetcm(BITMAP *dst, BITMAP *src, BITMAP *mask, int x, int y)
{
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
	unsigned char   *cs, *cd, *cm; // 8b  & 24b
	unsigned short  *ss, *sd;      // 16b & 15b
	uint32_t   *ls, *ld;      // 32b

	uint32_t  uls, uld;
	unsigned short uss, usd;
	unsigned char  ucm;

	unsigned int sr,sg,sb, r,g,b;

	unsigned int col;

	switch (bitmap_color_depth(dst)) {
	case 8:
		break;

	case 15:

		col = makecol15(255, 0, 255);
		for (i = 0; i < h; i++) {
			ss = (unsigned short *) src->line[sy + i] + sx;
			sd = (unsigned short *) dst->line[dy + i] + dx;
			cm = mask->line[sy+i] + sx*2;
			for (j = 0; j < w; j++) {
				if ((ucm = getb15(*((unsigned short*)cm))) == 255 || (uss = *(ss++)) == col){
					cm += 2;
					sd++;
					continue;
				}
				cm += 2;
				usd = *sd;
				
				sr = (uss >> 10) & 0x1F;
				sg = (uss >>  5) & 0x1F;
				sb =  uss        & 0x1F;
				r = (ucm * (((usd >> 10) & 0x1F) - sr) >> 8) + sr;
				g = (ucm * (((usd >> 5)  & 0x1F) - sg) >> 8) + sg;
				b = (ucm * (( usd        & 0x1F) - sb) >> 8) + sb;

				*(sd++) =  (r << 10) | (g << 5) | b;
			}
		}



		break;
	case 16 :
	
		col = makecol16(255, 0, 255);
		for (i = 0; i < h; i++) {
			ss = (unsigned short *) src->line[sy + i] + sx;
			sd = (unsigned short *) dst->line[dy + i] + dx;
			cm = mask->line[sy+i] + sx*2;
			for (j = 0; j < w; j++) {
				if ((uss = *(ss++)) == col || (ucm = getb16(*((unsigned short*)cm))) == 255){
					cm += 2;
					sd++;
					continue;
				}
				cm += 2;
				usd = *sd;
				
				sr = (uss >> 11) & 0x1F;
				sg = (uss >>  5) & 0x3F;
				sb =  uss        & 0x1F;
				r = ((ucm * (((usd >> 11) & 0x1F) - sr) >> 8) + sr);
				g = ((ucm * (((usd >> 5)  & 0x3F) - sg) >> 8) + sg);
				b = ((ucm * (( usd        & 0x1F) - sb) >> 8) + sb);

				*(sd++) =  (r << 11) | (g << 5) | b;
			}
		}

		break;
	case 24 :
		col = makecol24(255, 0, 255);
		for (i = 0; i < h; i++) {
			cs = (unsigned char *) src->line[sy + i] + sx*3;
			cd = (unsigned char *) dst->line[dy + i] + dx*3;
			cm = mask->line[sy+i] + sx*3;
			for (j = 0; j < w; j++) {
				if ((ucm = getb24(*((unsigned short*)cm))) == 255 || (uls = *((uint32_t *)cs)) == col){
					cm += 3;
					cd += 3;
					cs += 3;
					continue;
				}
				cm += 3;
				uld = *((uint32_t *)cd);
				
				sr = (uls >> 16) & 0xFF;
				sg = (uls >>  8) & 0xFF;
				sb =  uls        & 0xFF;
				r = ((ucm * (((uld >> 16) & 0xFF) - sr) >> 8) + sr);
				g = ((ucm * (((uld >> 8)  & 0xFF) - sg) >> 8) + sg);
				b = ((ucm * (( uld        & 0xFF) - sb) >> 8) + sb);

				*((uint32_t *)cd) = (uld & (0xFF000000)) | (r << 16) | (g << 8) | b;
				cd += 3;
				cs += 3;
			}
		}
		break;

	case 32:
		col = makecol32(255, 0, 255);
		for (i = 0; i < h; i++) {
			ls = (uint32_t *) src->line[sy + i] + sx;
			ld = (uint32_t *) dst->line[dy + i] + dx;
			cm = mask->line[sy+i] + sx*4;
			for (j = 0; j < w; j++) {
				if ((uls = *(ls++)) == col || (ucm = getb32(*((unsigned short*)cm))) == 255){
					cm += 4;
					ld++;
					continue;
				}
				cm += 4;
				uld = *ld;
				
				sr = (uls >> 16) & 0xFF;
				sg = (uls >>  8) & 0xFF;
				sb =  uls        & 0xFF;
				r = ((ucm * (((uld >> 16) & 0xFF) - sr) >> 8) + sr);
				g = ((ucm * (((uld >> 8)  & 0xFF) - sg) >> 8) + sg);
				b = ((ucm * (( uld        & 0xFF) - sb) >> 8) + sb);

				*(ld++) =  (r << 16) | (g << 8) | b;
			}
		}
		break;
	}

}
