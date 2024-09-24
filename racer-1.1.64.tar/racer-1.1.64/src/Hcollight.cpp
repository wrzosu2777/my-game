/*
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
	unsigned char   *sr, sr1, sr2; // 8b // zdrojova bitmapa

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

		col = rgb_map->data[64][64][64];
		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			c =  dst->line[dy + i] + dx;
			for (j = 0; j < w; j++) {
				if (*sr != 0 && *c != 0 && *c != col) {
					alp = ((*sr)+64);
					if ((pal[*c].r > 250) || ((r = (pal[*c].r * alp >> 7)) > 255)) r = 255;
					if ((pal[*c].g > 250) || ((g = (pal[*c].g * alp >> 7)) > 255)) g = 255;
					if ((pal[*c].g > 250) || ((b = (pal[*c].b * alp >> 7)) > 255)) b = 255;
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

		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			l  = (uint32_t*)dst->line[dy + i] + dx;
			for (j = 0; j < w/2; j++) {
				l2 = *l;
				sr1 = *sr;
				sr2 = *(sr+1);

//				b =  ((cnt * (l2 & 0x001F001F)) >> 8) & 0x001F001F;
//				g = (((cnt * ((l2 >> 5)  & 0x003F003F)) >> 8) & 0x003F003F) << 5;
//				r = (((cnt * ((l2 >> 11) & 0x001F001F)) >> 8) & 0x001F001F) << 11;

				r = ((l2 & 0x001F001F) + sr1 + (sr2 << 16)) & 0x001F001F;
				g = ((((l2 >> 5)  & 0x003F003F) + sr1 + (sr2 << 16)) & 0x003F003F) << 5;
				b = ((((l2 >> 11) & 0x001F001F) + sr1 + (sr2 << 16)) & 0x001F001F) << 11;

				*l = r | g | b;

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

				sTemp = *((DWORD*)lpSprite);
				dTemp = *((DWORD*)lpDest);

				sb = sTemp & 0x1f;
				db = dTemp & 0x1f;
				sg = (sTemp >> 5) & 0x3f;
				dg = (dTemp >> 5) & 0x3f;
				sr = (sTemp >> 11) & 0x1f;
				dr = (dTemp >> 11) & 0x1f;

				Result = (DWORD)((ALPHA * (db - sb) >> 8) + sb |
					((ALPHA * (dg - sg) >> 8) + sg) << 5 |
					((ALPHA * (dr - sr) >> 8) + sr) << 11);

				sb = (sTemp >> 16) & 0x1f;
				db = (dTemp >> 16) & 0x1f;
				sg = (sTemp >> 21) & 0x3f;
				dg = (dTemp >> 21) & 0x3f;
				sr = (sTemp >> 27) & 0x1f;
				dr = (dTemp >> 27) & 0x1f;

				Result |= (DWORD)( (ALPHA * (db - sb) >> 8) + sb |
					((ALPHA * (dg - sg) >> 8) + sg) << 5 |
					((ALPHA * (dr - sr) >> 8) + sr) << 11 ) << 16;

				*((DWORD*)lpDest) = Result;

				lpDest += 4;
				lpSprite +=4;
*/
/*
		for (i = 0; i < h; i++) {
			sr = src->line[sy + i] + sx;
			l  = (uint32_t*)dst->line[dy + i] + dx/2;
			for (j = 0; j < w/2; j++) {
				l2 = *l;

				sr1 = 255 - (*sr);
				b =  l2        & 0x1F;
				g = (l2 >> 5)  & 0x3F;
				r = (l2 >> 11) & 0x1F;
				col = (unsigned int)((((sr1 * (b - 0x1F) >> 8) + 0x1F |
					((sr1 * (g - 0x3F) >> 8) + 0x3F) << 5 |
					((sr1 * (r - 0x1F) >> 8) + 0x1F) << 11)) & 0x0000FFFF);

				sr1 = 255 - (*(sr+1));
				b = (l2 >> 16) & 0x1F;
				g = (l2 >> 21)  & 0x3F;
				r = (l2 >> 27) & 0x1F;
				col |= (((unsigned int)((sr1 * (b - 0x1F) >> 8) + 0x1F |
					((sr1 * (g - 0x3F) >> 8) + 0x3F) << 5 |
					((sr1 * (r - 0x1F) >> 8) + 0x1F) << 11)) << 16) & 0xFFFF0000;

				*l = col;

				l++;
				sr += 2;
			}
		}
		break;
  */
