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
 *  Sub     : Include funkci
 *
 *  File    : HGFX.cpp
 *  
 *  Projekt : HAllegro
 *  
 *  Autor   : Bernard Lidicky [berny@prog.cz]
 *
 *  Datum   : 9.7.2000
 *
 */      

#ifndef _HALLEG_GFX_
#define _HALLEG_GFX_

#include <allegro.h>  // allegro - pro jistotu


BITMAP *hload_bitmap(const char *filename); // nacte bitmapu a v 8bpp mody konvertne na unipaletu

void    hprepare_pal();  // upravi paletu tak, aby barva 0 bylo ruzova

BITMAP *load_jpg(const char *filename, RGB *pal); // nacte jpg (s knihovnou JpegLib)

void    hfade_sprite(BITMAP *bmp);   // ztmavi bitmapu na 50% svetlosti (podporuje 8, 15, 16, 24, 32 BPP)
                                     // pruhlednou nechava bezezmeny (pla[0], nebo ruzova)


void    hdraw_light_sprite(BITMAP *dst, BITMAP *src, int x, int y);
void    hdraw_light_spritetcm(BITMAP *dst, BITMAP *src, int x, int y);

void    hdraw_shadow_sprite(BITMAP *dst, BITMAP *src, int x, int y);
void    hdraw_shadow_spritetcm(BITMAP *dst, BITMAP *src, int x, int y);

BITMAP *hload_shadow_bitmap(char *filename);

// tmavici fce
void fade_screen(int speed);
void fade_bitmap(BITMAP *dst, BITMAP *src, int x, int y, int cnt);

// kresleni s maskou
void hmasked_sprite(BITMAP *dest, BITMAP *src, BITMAP *mask, int x, int y);
void hmasked_spritetcm(BITMAP *dst, BITMAP *src, BITMAP *mask, int x, int y);

// aa kresleni
void hanti_alias(BITMAP* dst, BITMAP* src);

// efekty vlneni
void hwarp(BITMAP *dst, BITMAP *bmp, int sx, int sy, int w, int h, int sd, double perioda, int sila);
void hwarp2(BITMAP *dst, BITMAP *bmp, int sx, int sy, int w, int h, int sd, double perioda, int sila);


#endif
