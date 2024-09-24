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
 *  Sub     : hanti_alias function
 *
 *  File    : HAntiAlias.cpp
 *  
 *  Projekt : HAllegro
 *  
 *  Autor   : Bernard Lidicky [berny@prog.cz]
 *
 *  Datum   : 18.7.2000
 *
 */      

#include "halleg.h"

#include <iostream>

/* ::hanti_alias(BITMAP* dst, BITMAP* src) : 
 * roztahne bitmapu src na bitmapu dst - src musi mit polovicni velikost vuci dst
 * a provede VYHLAZENI
 */

void hanti_alias(BITMAP* dst, BITMAP* src)
{
	int h = src->h;
	int w = src->w;

	unsigned short  s1A, s1B, s2B, s2A = 0;
	unsigned short *d1, *d2, *s1, *s2;

//	unsigned char  c1A, cs1B, cs2B, cs2A;
	unsigned char *cd1, *cd2, *cs1, *cs2;

	uint32_t  ls1A, ls1B, ls2B, ls2A;
	uint32_t *ld1, *ld2, *ls1, *ls2;

	// slozky barev
	int s1Br, s1Bg, s1Bb;
	int s1Ar, s1Ag, s1Ab;
	int s2Br, s2Bg, s2Bb;
	int s2Ar, s2Ag, s2Ab;
	int r,g,b;

	int i,j;

	switch (bitmap_color_depth(src)) {
	case 8 :
		// unsupported : using normal stretch
		stretch_sprite(dst, src, 0, 0, dst->w, dst->h);
		break;

	case 15 :
		for (i = 0; i < h-1; i++) {
			s1 = (unsigned short*) src->line[i];
			s2 = (unsigned short*) src->line[i+1];
			d1 = (unsigned short*) dst->line[2*i];
			d2 = (unsigned short*) dst->line[2*i+1];

			s1A = *(s1++);
			s2A = *(s2++);

			for (j = 0; j < w-1; j++) {

				s1B = s1A;
				s1A = *(s1++);

				s2B = s2A;
				s2A = *(s2++);

				// nacteni rgb
				s1Bb =  s1B          & 0x1F;
				s1Bg = (s1B >>  5)   & 0x1F;
				s1Br = (s1B >> 10)   & 0x1F;

				s1Ab =  s1A          & 0x1F;
				s1Ag = (s1A >>  5)   & 0x1F;
				s1Ar = (s1A >> 10)   & 0x1F;

				// nacteni 2. rgb
				s2Bb =  s2B          & 0x1F;
				s2Bg = (s2B >>  5)   & 0x1F;
				s2Br = (s2B >> 10)   & 0x1F;

				s2Ab =  s2A          & 0x1F;
				s2Ag = (s2A >>  5)   & 0x1F;
				s2Ar = (s2A >> 10)   & 0x1F;

				// 1. pixel
				*(d1++) = s1B;

				// 2. pixel
				r = (s1Ar + s1Br) >> 1;
				g = (s1Ag + s1Bg) >> 1;
				b = (s1Ab + s1Bb) >> 1;
				*(d1++) = (r << 10) | (g << 5) | (b);

				// 3. pixel
				r = ((s1Br  + s2Br) >> 1);
				g = ((s1Bg  + s2Bg) >> 1);
				b = ((s1Bb  + s2Bb) >> 1);
				*(d2++) = (r << 10) | (g << 5) | (b);

				// 4. pixel
				r = (s1Br + s2Ar) >> 1;
				g = (s1Bg + s2Ag) >> 1;
				b = (s1Bb + s2Ab) >> 1;
				*(d2++) = (r << 10) | (g << 5) | (b);
				
			}
			// posledni pixely
			*(d1++) = s1A;
			*(d1++) = s1A;
			*(d2++) = s2A;
			*(d2++) = s2A;
		}
		// posledni radek v 16
		s1 = (unsigned short*) src->line[i];
		d1 = (unsigned short*) dst->line[2*i];
		d2 = (unsigned short*) dst->line[2*i+1];

		s1A = *(s1++);

		for (j = 0; j < w-1; j++) {

			s1B = s1A;
			s1A = *(s1++);

			// nacteni rgb
			s1Bb =  s1B          & 0x1F;
			s1Bg = (s1B >>  5)   & 0x1F;
			s1Br = (s1B >> 10)   & 0x1F;

			s1Ab =  s1A          & 0x1F;
			s1Ag = (s1A >>  5)   & 0x1F;
			s1Ar = (s1A >> 10)   & 0x1F;

			// 1. pixel
			*(d1++) = s1B;
			*(d2++) = s1B;

			// 2. pixel
			r = (s1Ar + s1Br) >> 1;
			g = (s1Ag + s1Bg) >> 1;
			b = (s1Ab + s1Bb) >> 1;
			*(d1++) = (r << 10) | (g << 5) | (b);
			*(d2++) = (r << 10) | (g << 5) | (b);
		}
		// posledni pixely
		*(d1++) = s1A;
		*(d1++) = s1A;
		*(d2++) = s2A;
		*(d2++) = s2A;
		break;

	case 16 :
		for (i = 0; i < h-1; i++) {
			s1 = (unsigned short*) src->line[i];
			s2 = (unsigned short*) src->line[i+1];
			d1 = (unsigned short*) dst->line[2*i];
			d2 = (unsigned short*) dst->line[2*i+1];

			s1A = *(s1++);
			s2A = *(s2++);

			for (j = 0; j < w-1; j++) {

				s1B = s1A;
				s1A = *(s1++);

				s2B = s2A;
				s2A = *(s2++);

				// nacteni rgb
				s1Bb =  s1B          & 0x1F;
				s1Bg = (s1B >>  5)   & 0x3F;
				s1Br = (s1B >> 11)   & 0x1F;

				s1Ab =  s1A          & 0x1F;
				s1Ag = (s1A >>  5)   & 0x3F;
				s1Ar = (s1A >> 11)   & 0x1F;

				// nacteni 2. rgb
				s2Bb =  s2B          & 0x1F;
				s2Bg = (s2B >>  5)   & 0x3F;
				s2Br = (s2B >> 11)   & 0x1F;

				s2Ab =  s2A          & 0x1F;
				s2Ag = (s2A >>  5)   & 0x3F;
				s2Ar = (s2A >> 11)   & 0x1F;

				// 1. pixel
				*(d1++) = s1B;

				// 2. pixel
				r = (s1Ar + s1Br) >> 1;
				g = (s1Ag + s1Bg) >> 1;
				b = (s1Ab + s1Bb) >> 1;
				*(d1++) = (r << 11) | (g << 5) | (b);

				// 3. pixel
				r = ((s1Br  + s2Br) >> 1);
				g = ((s1Bg  + s2Bg) >> 1);
				b = ((s1Bb  + s2Bb) >> 1);
				*(d2++) = (r << 11) | (g << 5) | (b);

				// 4. pixel
				r = (s1Br + s2Ar) >> 1;
				g = (s1Bg + s2Ag) >> 1;
				b = (s1Bb + s2Ab) >> 1;
				*(d2++) = (r << 11) | (g << 5) | (b);
				
			}
			// posledni pixely
			*(d1++) = s1A;
			*(d1++) = s1A;
			*(d2++) = s2A;
			*(d2++) = s2A;
		}

		// posledni radek v 16
		s1 = (unsigned short*) src->line[i];
		d1 = (unsigned short*) dst->line[2*i];
		d2 = (unsigned short*) dst->line[2*i+1];

		s1A = *(s1++);

		for (j = 0; j < w-1; j++) {

			s1B = s1A;
			s1A = *(s1++);

			// nacteni rgb
			s1Bb =  s1B          & 0x1F;
			s1Bg = (s1B >>  5)   & 0x3F;
			s1Br = (s1B >> 11)   & 0x1F;

			s1Ab =  s1A          & 0x1F;
			s1Ag = (s1A >>  5)   & 0x3F;
			s1Ar = (s1A >> 11)   & 0x1F;

			// 1. pixel
			*(d1++) = s1B;
			*(d2++) = s1B;

			// 2. pixel
			r = (s1Ar + s1Br) >> 1;
			g = (s1Ag + s1Bg) >> 1;
			b = (s1Ab + s1Bb) >> 1;
			*(d1++) = (r << 11) | (g << 5) | (b);
			*(d2++) = (r << 11) | (g << 5) | (b);
		}
		// posledni pixely
		*(d1++) = s1A;
		*(d1++) = s1A;
		*(d2++) = s2A;
		*(d2++) = s2A;

		break;

	case 24:
		for (i = 0; i < h-1; i++) {
			cs1 = (unsigned char*) src->line[i];
			cs2 = (unsigned char*) src->line[i+1];
			cd1 = (unsigned char*) dst->line[2*i];
			cd2 = (unsigned char*) dst->line[2*i+1];

			ls1A = 0xFFFFFF & *((uint32_t*)(cs1));
			cs1 += 3;
			ls2A = 0xFFFFFF & *((uint32_t*)(cs2));
			cs2 += 3;

			for (j = 0; j < w-1; j++) {

				ls1B = ls1A;
				ls1A = 0xFFFFFF & *((uint32_t*)(cs1));
				cs1 += 3;

				ls2B = ls2A;
				ls2A = 0xFFFFFF & *((uint32_t*)(cs2));
				cs2 += 3;

				// nacteni rgb
				s1Bb =  ls1B          & 0xFF;
				s1Bg = (ls1B >>  8)   & 0xFF;
				s1Br = (ls1B >> 16)   & 0xFF;

				s1Ab =  ls1A          & 0xFF;
				s1Ag = (ls1A >>  8)   & 0xFF;
				s1Ar = (ls1A >> 16)   & 0xFF;

				// nacteni 2. rgb
				s2Bb =  ls2B          & 0xFF;
				s2Bg = (ls2B >>  8)   & 0xFF;
				s2Br = (ls2B >> 16)   & 0xFF;

				s2Ab =  ls2A          & 0xFF;
				s2Ag = (ls2A >>  8)   & 0xFF;
				s2Ar = (ls2A >> 16)   & 0xFF;

				// 1. pixel
				*((uint32_t*)(cd1)) = ls1B;
				cd1 += 3;

				// 2. pixel
				r = (s1Ar + s1Br) >> 1;
				g = (s1Ag + s1Bg) >> 1;
				b = (s1Ab + s1Bb) >> 1;
				*((uint32_t*)(cd1)) = (r << 16) | (g << 8) | (b);
				cd1 += 3;

				// 3. pixel
				r = ((s1Br  + s2Br) >> 1);
				g = ((s1Bg  + s2Bg) >> 1);
				b = ((s1Bb  + s2Bb) >> 1);
				*((uint32_t*)(cd2)) = (r << 16) | (g << 8) | (b);
				cd2 += 3;

				// 4. pixel
				r = (s1Br + s2Ar) >> 1;
				g = (s1Bg + s2Ag) >> 1;
				b = (s1Bb + s2Ab) >> 1;
				*((uint32_t*)(cd2)) = (r << 16) | (g << 8) | (b);
				cd2 += 3;
			}
			// poslendni pixely radku
			*((uint32_t*)(cd1)) = ls1A;
			cd1 += 3;
			*((uint32_t*)(cd1)) = ls1A;
			cd1 += 3;
			*((uint32_t*)(cd2)) = ls2A;
			cd2 += 3;
			*((uint32_t*)(cd2)) = ls2A;
			cd2 += 3;
		}

		// posledni radek
		cs1 = (unsigned char*) src->line[i];
		cd1 = (unsigned char*) dst->line[2*i];
		cd2 = (unsigned char*) dst->line[2*i+1];

		ls1A = 0xFFFFFF & *((uint32_t*)(cs1));
		cs1 += 3;

		for (j = 0; j < w-1; j++) {

			ls1B = ls1A;
			ls1A = 0xFFFFFF & *((uint32_t*)(cs1));
			cs1 += 3;

			// nacteni rgb
			s1Bb =  ls1B          & 0xFF;
			s1Bg = (ls1B >>  8)   & 0xFF;
			s1Br = (ls1B >> 16)   & 0xFF;

			s1Ab =  ls1A          & 0xFF;
			s1Ag = (ls1A >>  8)   & 0xFF;
			s1Ar = (ls1A >> 16)   & 0xFF;

			// 1. pixel
			*((uint32_t*)(cd1)) = ls1B;
			cd1 += 3;
			*((uint32_t*)(cd2)) = ls1B;
			cd2 += 3;

			// 2. pixel
			r = (s1Ar + s1Br) >> 1;
			g = (s1Ag + s1Bg) >> 1;
			b = (s1Ab + s1Bb) >> 1;
			*((uint32_t*)(cd1)) = (r << 16) | (g << 8) | (b);
			cd1 += 3;
			*((uint32_t*)(cd2)) = (r << 16) | (g << 8) | (b);
			cd2 += 3;
		}
		*((uint32_t*)(cd1)) = ls1A;
		cd1 += 3;
		*((uint32_t*)(cd1)) = ls1A;
		cd1 += 3;
		*((uint32_t*)(cd2)) = ls1A;
		cd2 += 3;
		*((uint32_t*)(cd2)) = ls1A;
		cd2 += 3;

		break;
	case 32:
		for (i = 0; i < h-1; i++) {
			ls1 = (uint32_t*) src->line[i];
			ls2 = (uint32_t*) src->line[i+1];
			ld1 = (uint32_t*) dst->line[2*i];
			ld2 = (uint32_t*) dst->line[2*i+1];

			ls1A = *(ls1++);
			ls2A = *(ls2++);

			for (j = 0; j < w-1; j++) {

				ls1B = ls1A;
				ls1A = *(ls1++);

				ls2B = ls2A;
				ls2A = *(ls2++);

				// nacteni rgb
				s1Bb =  ls1B          & 0xFF;
				s1Bg = (ls1B >>  8)   & 0xFF;
				s1Br = (ls1B >> 16)   & 0xFF;

				s1Ab =  ls1A          & 0xFF;
				s1Ag = (ls1A >>  8)   & 0xFF;
				s1Ar = (ls1A >> 16)   & 0xFF;

				// nacteni 2. rgb
				s2Bb =  ls2B          & 0xFF;
				s2Bg = (ls2B >>  8)   & 0xFF;
				s2Br = (ls2B >> 16)   & 0xFF;

				s2Ab =  ls2A          & 0xFF;
				s2Ag = (ls2A >>  8)   & 0xFF;
				s2Ar = (ls2A >> 16)   & 0xFF;

				// 1. pixel
				*(ld1++) = ls1B;

				// 2. pixel
				r = (s1Ar + s1Br) >> 1;
				g = (s1Ag + s1Bg) >> 1;
				b = (s1Ab + s1Bb) >> 1;
				*(ld1++) = (r << 16) | (g << 8) | (b);

				// 3. pixel
				r = ((s1Br  + s2Br) >> 1);
				g = ((s1Bg  + s2Bg) >> 1);
				b = ((s1Bb  + s2Bb) >> 1);
				*(ld2++) = (r << 16) | (g << 8) | (b);

				// 4. pixel
				r = (s1Br + s2Ar) >> 1;
				g = (s1Bg + s2Ag) >> 1;
				b = (s1Bb + s2Ab) >> 1;
				*(ld2++) = (r << 16) | (g << 8) | (b);
			}
			*(ld2++) = ls2A;
			*(ld1++) = ls1A;
			*(ld2++) = ls2A;
			*(ld1++) = ls1A;
		}

		// posledni radek
		ls1 = (uint32_t*) src->line[h-1];
		ld1 = (uint32_t*) dst->line[2*(h-1)];
		ld2 = (uint32_t*) dst->line[2*(h-1)+1];
		ls1A = *(ls1++);
		for (j = 0; j < w-1; j++) {
			ls1B = ls1A;
			ls1A = *(ls1++);

			// nacteni rgb
			s1Bb =  ls1B          & 0xFF;
			s1Bg = (ls1B >>  8)   & 0xFF;
			s1Br = (ls1B >> 16)   & 0xFF;

			s1Ab =  ls1A          & 0xFF;
			s1Ag = (ls1A >>  8)   & 0xFF;
			s1Ar = (ls1A >> 16)   & 0xFF;

			// 1. pixel
			*(ld1++) = ls1B;
			*(ld2++) = ls1B;

		// 2. pixel
			r = (s1Ar + s1Br) >> 1;
			g = (s1Ag + s1Bg) >> 1;
			b = (s1Ab + s1Bb) >> 1;
			*(ld1++) = (r << 16) | (g << 8) | (b);
			*(ld2++) = (r << 16) | (g << 8) | (b);

		}
		*(ld1++) = ls1A;
		*ld1 = ls1A;
		*(ld2++) = ls1A;
		*ld2 = ls1A;

		break;
	}
}

