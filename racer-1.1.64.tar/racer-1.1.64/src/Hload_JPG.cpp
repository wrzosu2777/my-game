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
 *  Sub     : Implementation of load_jpg - jpg images loader
 *
 *  File    : HLoad_JPG.cpp
 *  
 *  Projekt : HAllegro
 *  
 *  Autor   : Bernard Lidicky [berny@prog.cz]
 *
 *  Datum   : 5.7.2000
 *
 */      

#include "halleg.h"
#include <stdio.h>

#include <iostream>

using namespace std;


#ifdef _HALLEG_JPG_SUPPORT_

extern "C" 
{
	#include <jpeglib.h>
	#include <setjmp.h>
}


struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr  myerr = (my_error_ptr) cinfo->err;
	char          buffer[JMSG_LENGTH_MAX];
   
	(*cinfo->err->format_message) (cinfo, buffer);
	
	cout << buffer << endl;

	longjmp(myerr->setjmp_buffer, 1);
}

/*
 * Sample routine for JPEG decompression.  We assume that the source file name
 * is passed in.  We want to return 1 on success, 0 on error.
 */

BITMAP *load_24BPP_jpg(const char *filename, int *w, int *h)
{
  /* This struct contains the JPEG decompression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   */
  struct jpeg_decompress_struct cinfo;
  /* We use our private extension JPEG error handler.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct my_error_mgr jerr;
  /* More stuff */
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */

  /* In this example we want to open the input file before doing anything else,
   * so that the setjmp() error recovery below can assume the file is open.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to read binary files.
   */

  if ((infile = fopen(filename, "rb")) == NULL) {
    cout << "Error reading file " << filename << endl;
    return NULL;
  }

  /* Step 1: allocate and initialize JPEG decompression object */

  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(jerr.setjmp_buffer)) {
    /* If we get here, the JPEG code has signaled an error.
     * We need to clean up the JPEG object, close the input file, and return.
     */
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return NULL;
  }
  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&cinfo);

  /* Step 2: specify data source (eg, a file) */

  jpeg_stdio_src(&cinfo, infile);

  /* Step 3: read file parameters with jpeg_read_header() */

  jpeg_read_header(&cinfo, TRUE);
  /* We can ignore the return value from jpeg_read_header since
   *   (a) suspension is not possible with the stdio data source, and
   *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
   * See libjpeg.doc for more info.
   */

  /* Step 4: set parameters for decompression */

  /* In this example, we don't need to change any of the defaults set by
   * jpeg_read_header(), so we do nothing here.
   */

  /* Step 5: Start decompressor */

  jpeg_start_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* We may need to do some setup of our own at this point before reading
   * the data.  After jpeg_start_decompress() we have the correct scaled
   * output image dimensions available, as well as the output colormap
   * if we asked for color quantization.
   * In this example, we need to make an output work buffer of the right size.
   */ 
  /* JSAMPLEs per row in output buffer */
  row_stride = cinfo.output_width * cinfo.output_components;
  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)
      ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */

    // how big is this thing gonna be?
    *w  = cinfo.output_width;
    *h  = cinfo.output_height;

	 // create bitmap
	 BITMAP *tmp = create_bitmap_ex(cinfo.out_color_components << 3, cinfo.output_width, cinfo.output_height);
	 if (tmp == NULL) {
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return NULL;
	 }

	   /* Here we use the library's state variable cinfo.output_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   */
 // while (cinfo.output_scanline < cinfo.output_height) {
 //   (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    /* Assume put_scanline_someplace wants a pointer and sample count. */
    // put_scanline_someplace(buffer[0], row_stride);
 // }
	for (unsigned int i = 0; i < cinfo.output_height; i++) {
		jpeg_read_scanlines(&cinfo, &tmp->line[i], 1);
	}

  /* Step 7: Finish decompression */

  jpeg_finish_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);

  /* After finish_decompress, we can close the input file.
   * Here we postpone it until after no more JPEG errors are possible,
   * so as to simplify the setjmp error logic above.  (Actually, I don't
   * think that jpeg_destroy can do an error exit, but why assume anything...)
   */
  fclose(infile);


  /* At this point you may want to check to see whether any corrupt-data
   * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
   */

  /* And we're done! */
  return tmp;
}

/*
 * Our loading function
 */
BITMAP *load_jpg(const char *filename, RGB *pal)
{

	int w, h;
	int i;

	BITMAP *tmp = load_24BPP_jpg(filename, &w, &h);
	if (tmp == NULL) {
		return NULL;
	}

	// tvoreni palety
	if (bitmap_color_depth(tmp) == 8) {  
		RGB *rgb = pal;  // cernobila paleta
		for (i = 0; i < 256; i++) {
			rgb->r = i/4;
			rgb->g = i/4;
			rgb->b = i/4;
			rgb++;
		}
	}
	else {
		get_palette(pal); 
	}

	// prevedeni do zpravneho formatu BPP
	if (screen != NULL && bitmap_color_depth(tmp) != bitmap_color_depth(screen)) {
		// bitmapa se spravnym formatem
		BITMAP *bmp = create_bitmap(w, h);
		if (bmp == NULL) {
			destroy_bitmap(tmp);
			return NULL;
		}
	
		if (bitmap_color_depth(bmp) != 8) {
			select_palette(pal);
			blit(tmp, bmp, 0, 0, 0, 0, w, h);
			unselect_palette();
		}
		else {
			// blit je pro prevod do 8BPP kod prilis pomale bez rgb_map
			if (rgb_map ==  NULL) rgb_map =  &hscreen_self_pointer->m_rgb_map;
			blit(tmp, bmp, 0, 0, 0, 0, w, h);
		}

		destroy_bitmap(tmp);
		return bmp;
	}

	// prevod 24 BRG na 24RGB
	if (bitmap_color_depth(tmp) == 24) {
		int       i,j;
		unsigned char   *c, *c3, cc; // c2 (prostredni) se nemeni
		j = tmp->w * tmp->h;
		c   = tmp->line[0];
		c3  = c+2;
		for (i = 0; i < j; i++) {
			cc  = *c;
			*c  = *c3;
			*c3 = cc;

			c  += 3;
			c3 += 3;
		}
	}

	return tmp;
}

#else //_HALLEG_JPG_SUPPORT_
BITMAP *load_jpg(const char *filename, RGB *pal)
{
	cout << "loading JPG is not working" << endl;
	return NULL;
}

#endif //_HALLEG_JPG_SUPPORT_
