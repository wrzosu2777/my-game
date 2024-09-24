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
 *  Sub     : funkce na vlneni
 *
 *  File    : HWarping.cpp
 *  
 *  Projekt : HAllegro
 *  
 *  Autor   : Bernard Lidicky [berny@prog.cz]
 *
 *  Datum   : 24.9.2000
 *
 */      

#include "halleg.h"

/////////////// BUMPING
int b_sila;
double b_perioda;

int getpos(int y, int maxy, int id, int dst, int sd)
{
/*	switch (id) {
	case 0 :
		return 0;
		break;
	case 1 :
		return sin((y+sd)*b_perioda)*b_sila + dst/3;
		break;
	case 2 :
		return cos((y+sd)*b_perioda)*b_sila + 2*dst/3;
		break;
	case 3 :
		return dst;
		break;
	}
	return 0;
	*/
	return 0;
/*	
	switch (id) {
	case 0 :
		return 0;
		break;
	case 1 :
//		return b_sila*sin(y*128/maxy) + dst/3;
		return sin((y+sd)*b_perioda)*b_sila*sin(y*128/maxy) + dst/3;
		break;
	case 2 :
		return cos((y+sd)*b_perioda)*b_sila*sin(y*128/maxy) + 2*dst/3;
		break;
	case 3 :
		return dst;
		break;
	}
	return 0;
	*/
}

int getposN(int id, int dst)
{
	switch (id) {
	case 0 :
		return 0;
		break;
	case 1 :
		return dst/3;
		break;
	case 2 :
		return 2*dst/3;
		break;
	case 3 :
		return dst;
		break;
	}
	return 0;
}

void hwarp(BITMAP *dst, BITMAP *bmp, int sx, int sy, int w, int h, int sd, double perioda, int sila)
{
	BITMAP *tmp;	
	tmp = create_bitmap(w, h);
	int i;

	b_perioda = perioda;
	b_sila    = sila;

	// bumping po X
	for (int y = 0; y < h; y++) {
		for (i = 0; i < 3; i++) {
			int pos = getpos(y, h, i, w, sd);
			stretch_blit(bmp, tmp, getposN(i, w)+sx, y+sy, getposN(i+1, w)-getposN(i, w), 1, pos, y, getpos(y, h, i+1, w, sd) - pos, 1);
		}
	}
		

	// bumping po Y
	for (int x = 0; x < w; x++) {
		for (i = 0; i < 3; i++)
			stretch_blit(tmp, dst, x, getposN(i, h), 1, getposN(i+1, h)-getposN(i,h), x+sx, getpos(x, w, i,h, sd)+sy, 1, getpos(x, w, i+1,h, sd) - getpos(x, w, i,h, sd));
	}


	destroy_bitmap(tmp);
}

void hwarp2(BITMAP *dst, BITMAP *bmp, int sx, int sy, int w, int h, int sd, double perioda, int sila)
{
	BITMAP *tmp;	
	tmp = create_bitmap(w, h);
	int i;

	b_perioda = perioda;
	b_sila    = sila;

	// bumping po X
	for (int y = 0; y < h; y++) {
		for (i = 0; i < 3; i++) {
			int pos = getpos(y, h, i, w, sd);
			stretch_blit(bmp, tmp, getposN(i, w)+sx, y+sy, getposN(i+1, w)-getposN(i, w), 1, pos, y, getpos(y, h, i+1, w, sd) - pos, 1);
		}
	}
		

	// bumping po Y
	for (int x = 0; x < w; x++) {
		for (i = 0; i < 3; i++)
			stretch_blit(tmp, dst, x, getposN(i, h), 1, getposN(i+1, h)-getposN(i,h), x+sx, getpos(x, w, i,h, sd)+sy, 1, getpos(x, w, i+1,h, sd) - getpos(x, w, i,h, sd));
	}


	destroy_bitmap(tmp);
}
