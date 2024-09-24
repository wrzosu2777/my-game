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
 *  Sub     : Implementation of HGfx functions
 *
 *  File    : HGfx.cpp
 *  
 *  Projekt : HAllegro
 *  
 *  Autor   : Bernard Lidicky [berny@prog.cz]
 *
 *  Datum   : 5.7.2000
 *
 */      

#include "halleg.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

// pro switching a problem ve win32
HScreen *hscreen_self_pointer;


/* ::hload_bitmap() : 
 * nacte bitmapu - resi problem palet
 */
BITMAP *hload_bitmap(const char *filename) {
	BITMAP   *bmp;
	PALETTE   pal;
	
	bmp = load_bitmap(filename, pal);
	if (bmp == NULL) {
		char filename2[255];
		sprintf(filename2, "/usr/local/share/racer/%s", filename);
		bmp = load_bitmap(filename2, pal);
		
		if (bmp == NULL) {
			sprintf(filename2, "/usr/share/racer/%s", filename);
			bmp = load_bitmap(filename2, pal);
		}
	}
	
	if (bmp == NULL) return NULL;
	
	if (screen)
		if (bitmap_color_depth(screen) == 8) {
			PALETTE   glob; // Globlani paleta

			get_palette(glob);
			if (pal == glob) 
				return bmp;
		
	// bestfit_color je pomale
			int       i,j;
			unsigned char   *c;

			j = bmp->w * bmp->h;
			c = bmp->line[0];
		
			if (rgb_map == NULL) rgb_map =  &hscreen_self_pointer->m_rgb_map;
			for (i = 0; i < j; i++) {
				//*c = bestfit_color(glob, pal[*c].r, pal[*c].g, pal[*c].b); // strasne pomale
				if ((pal[*c].r == 63) && (pal[*c].g == 0) && (pal[*c].b == 63)) 
					*c = 0;
				else 
					*c = rgb_map->data[pal[*c].r/2][pal[*c].g/2][pal[*c].b/2];
				
				c++;
			}
	}

	return bmp;
}

/* ::hprepare_pal() : 
 * nastavi paletu, kde je na 0 ruzova barva
 */
void hprepare_pal() {
	PALETTE   glob; // bloblani paleta
	int       i;
	
	get_palette(glob);

	for (i = 0; i < 255; i++)
		if (glob[i].r == 63 && glob[i].g == 0 && glob[i].b == 63) {
			glob[i].r = 0;
			glob[i].g = 0;
			glob[i].b = 0;
		}

	glob[0].r = 63;
	glob[0].g = 0;
	glob[0].b = 63;
	
	set_palette(glob);
}
