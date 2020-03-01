// scan_jpg.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "scan.h"

#include "Image.h"

#include <sstream>
#include <setjmp.h>

//	QR code generator
#include "../qrgen/QR-Code-generator/c/qrcodegen.h"
#include "../qrgen/qrgen.h"

//	QR code decoder
#include "../QUIRC/quirc/lib/quirc_internal.h"
#include "../QUIRC/quirc_api.h"

//	Global data
extern std::vector<SCAN> global_scans;



/*---------------------------------------------------------------------------

   wpng - simple PNG-writing program                             writepng.c

  ---------------------------------------------------------------------------

	  Copyright (c) 1998-2007 Greg Roelofs.  All rights reserved.

	  This software is provided "as is," without warranty of any kind,
	  express or implied.  In no event shall the author or contributors
	  be held liable for any damages arising in any way from the use of
	  this software.

	  The contents of this file are DUAL-LICENSED.  You may modify and/or
	  redistribute this software according to the terms of one of the
	  following two licenses (at your option):


	  LICENSE 1 ("BSD-like with advertising clause"):

	  Permission is granted to anyone to use this software for any purpose,
	  including commercial applications, and to alter it and redistribute
	  it freely, subject to the following restrictions:

	  1. Redistributions of source code must retain the above copyright
		 notice, disclaimer, and this list of conditions.
	  2. Redistributions in binary form must reproduce the above copyright
		 notice, disclaimer, and this list of conditions in the documenta-
		 tion and/or other materials provided with the distribution.
	  3. All advertising materials mentioning features or use of this
		 software must display the following acknowledgment:

			This product includes software developed by Greg Roelofs
			and contributors for the book, "PNG: The Definitive Guide,"
			published by O'Reilly and Associates.


	  LICENSE 2 (GNU GPL v2 or later):

	  This program is free software; you can redistribute it and/or modify
	  it under the terms of the GNU General Public License as published by
	  the Free Software Foundation; either version 2 of the License, or
	  (at your option) any later version.

	  This program is distributed in the hope that it will be useful,
	  but WITHOUT ANY WARRANTY; without even the implied warranty of
	  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	  GNU General Public License for more details.

	  You should have received a copy of the GNU General Public License
	  along with this program; if not, write to the Free Software Foundation,
	  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  ---------------------------------------------------------------------------*/


#include <stdlib.h>     /* for exit() prototype */

#include "zlib.h"

#include "png.h"        /* libpng header; includes zlib.h and setjmp.h */
#include "writepng.h"   /* typedefs, common macros, public prototypes */


  /* local prototype */

static void writepng_error_handler(png_structp png_ptr, png_const_charp msg);



void writepng_version_info(void)
{
	fprintf(stderr, "   Compiled with libpng %s; using libpng %s.\n",
		PNG_LIBPNG_VER_STRING, png_libpng_ver);
	fprintf(stderr, "   Compiled with zlib %s; using zlib %s.\n",
		ZLIB_VERSION, zlib_version);
}


/* returns 0 for success, 2 for libpng problem, 4 for out of memory, 11 for
 *  unexpected pnmtype; note that outfile might be stdout */

int writepng_init(mainprog_info *mainprog_ptr)
{
	png_structp  png_ptr;       /* note:  temporary variables! */
	png_infop  info_ptr;
	int color_type, interlace_type;


	/* could also replace libpng warning-handler (final NULL), but no need: */

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, mainprog_ptr, writepng_error_handler, NULL);
	if (!png_ptr)
		return 4;   /* out of memory */

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_write_struct(&png_ptr, NULL);
		return 4;   /* out of memory */
	}


	/* setjmp() must be called in every function that calls a PNG-writing
	 * libpng function, unless an alternate error handler was installed--
	 * but compatible error handlers must either use longjmp() themselves
	 * (as in this program) or exit immediately, so here we go: */

	if (setjmp(mainprog_ptr->jmpbuf))
	{
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return 2;
	}


	/* make sure outfile is (re)opened in BINARY mode */

	png_init_io(png_ptr, mainprog_ptr->outfile);


	/* set the compression levels--in general, always want to leave filtering
	 * turned on (except for palette images) and allow all of the filters,
	 * which is the default; want 32K zlib window, unless entire image buffer
	 * is 16K or smaller (unknown here)--also the default; usually want max
	 * compression (NOT the default); and remaining compression flags should
	 * be left alone */

	png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
	/*
		>> this is default for no filtering; Z_FILTERED is default otherwise:
		png_set_compression_strategy(png_ptr, Z_DEFAULT_STRATEGY);
		>> these are all defaults:
		png_set_compression_mem_level(png_ptr, 8);
		png_set_compression_window_bits(png_ptr, 15);
		png_set_compression_method(png_ptr, 8);
	 */


	 /* set the image parameters appropriately */

	if (mainprog_ptr->pnmtype == 5)
		color_type = PNG_COLOR_TYPE_GRAY;
	else if (mainprog_ptr->pnmtype == 6)
		color_type = PNG_COLOR_TYPE_RGB;
	else if (mainprog_ptr->pnmtype == 8)
		color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	else {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return 11;
	}

	interlace_type = mainprog_ptr->interlaced ? PNG_INTERLACE_ADAM7 :
		PNG_INTERLACE_NONE;

	png_set_IHDR(png_ptr, info_ptr, mainprog_ptr->width, mainprog_ptr->height,
		mainprog_ptr->sample_depth, color_type, interlace_type,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	if (mainprog_ptr->gamma > 0.0)
		png_set_gAMA(png_ptr, info_ptr, mainprog_ptr->gamma);

	if (mainprog_ptr->have_bg) {   /* we know it's RGBA, not gray+alpha */
		png_color_16  background;

		background.red = mainprog_ptr->bg_red;
		background.green = mainprog_ptr->bg_green;
		background.blue = mainprog_ptr->bg_blue;
		png_set_bKGD(png_ptr, info_ptr, &background);
	}

	if (mainprog_ptr->have_time) {
		png_time  modtime;

		png_convert_from_time_t(&modtime, mainprog_ptr->modtime);
		png_set_tIME(png_ptr, info_ptr, &modtime);
	}

	if (mainprog_ptr->have_text) {
		png_text  text[6];
		int  num_text = 0;

		if (mainprog_ptr->have_text & TEXT_TITLE) {
			text[num_text].compression = PNG_TEXT_COMPRESSION_NONE;
			text[num_text].key = (char*)"Title";
			text[num_text].text = mainprog_ptr->title;
			++num_text;
		}
		if (mainprog_ptr->have_text & TEXT_AUTHOR) {
			text[num_text].compression = PNG_TEXT_COMPRESSION_NONE;
			text[num_text].key = (char*)"Author";
			text[num_text].text = mainprog_ptr->author;
			++num_text;
		}
		if (mainprog_ptr->have_text & TEXT_DESC) {
			text[num_text].compression = PNG_TEXT_COMPRESSION_NONE;
			text[num_text].key = (char*)"Description";
			text[num_text].text = mainprog_ptr->desc;
			++num_text;
		}
		if (mainprog_ptr->have_text & TEXT_COPY) {
			text[num_text].compression = PNG_TEXT_COMPRESSION_NONE;
			text[num_text].key = (char*)"Copyright";
			text[num_text].text = mainprog_ptr->copyright;
			++num_text;
		}
		if (mainprog_ptr->have_text & TEXT_EMAIL) {
			text[num_text].compression = PNG_TEXT_COMPRESSION_NONE;
			text[num_text].key = (char*)"E-mail";
			text[num_text].text = mainprog_ptr->email;
			++num_text;
		}
		if (mainprog_ptr->have_text & TEXT_URL) {
			text[num_text].compression = PNG_TEXT_COMPRESSION_NONE;
			text[num_text].key = (char*)"URL";
			text[num_text].text = mainprog_ptr->url;
			++num_text;
		}
		png_set_text(png_ptr, info_ptr, text, num_text);
	}


	/* write all chunks up to (but not including) first IDAT */

	png_write_info(png_ptr, info_ptr);


	/* if we wanted to write any more text info *after* the image data, we
	 * would set up text struct(s) here and call png_set_text() again, with
	 * just the new data; png_set_tIME() could also go here, but it would
	 * have no effect since we already called it above (only one tIME chunk
	 * allowed) */


	 /* set up the transformations:  for now, just pack low-bit-depth pixels
	  * into bytes (one, two or four pixels per byte) */

	png_set_packing(png_ptr);
	/*  png_set_shift(png_ptr, &sig_bit);  to scale low-bit-depth values */


		/* make sure we save our pointers for use in writepng_encode_image() */

	mainprog_ptr->png_ptr = png_ptr;
	mainprog_ptr->info_ptr = info_ptr;


	/* OK, that's all we need to do for now; return happy */

	return 0;
}





/* returns 0 for success, 2 for libpng (longjmp) problem */

int writepng_encode_image(mainprog_info *mainprog_ptr)
{
	png_structp png_ptr = (png_structp)mainprog_ptr->png_ptr;
	png_infop info_ptr = (png_infop)mainprog_ptr->info_ptr;


	/* as always, setjmp() must be called in every function that calls a
	 * PNG-writing libpng function */

	if (setjmp(mainprog_ptr->jmpbuf)) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		mainprog_ptr->png_ptr = NULL;
		mainprog_ptr->info_ptr = NULL;
		return 2;
	}


	/* and now we just write the whole image; libpng takes care of interlacing
	 * for us */

	png_write_image(png_ptr, mainprog_ptr->row_pointers);


	/* since that's it, we also close out the end of the PNG file now--if we
	 * had any text or time info to write after the IDATs, second argument
	 * would be info_ptr, but we optimize slightly by sending NULL pointer: */

	png_write_end(png_ptr, NULL);

	return 0;
}





/* returns 0 if succeeds, 2 if libpng problem */

int writepng_encode_row(mainprog_info *mainprog_ptr)  /* NON-interlaced only! */
{
	png_structp png_ptr = (png_structp)mainprog_ptr->png_ptr;
	png_infop info_ptr = (png_infop)mainprog_ptr->info_ptr;


	/* as always, setjmp() must be called in every function that calls a
	 * PNG-writing libpng function */

	if (setjmp(mainprog_ptr->jmpbuf)) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		mainprog_ptr->png_ptr = NULL;
		mainprog_ptr->info_ptr = NULL;
		return 2;
	}


	/* image_data points at our one row of image data */

	png_write_row(png_ptr, mainprog_ptr->image_data);

	return 0;
}





/* returns 0 if succeeds, 2 if libpng problem */

int writepng_encode_finish(mainprog_info *mainprog_ptr)   /* NON-interlaced! */
{
	png_structp png_ptr = (png_structp)mainprog_ptr->png_ptr;
	png_infop info_ptr = (png_infop)mainprog_ptr->info_ptr;


	/* as always, setjmp() must be called in every function that calls a
	 * PNG-writing libpng function */

	if (setjmp(mainprog_ptr->jmpbuf)) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		mainprog_ptr->png_ptr = NULL;
		mainprog_ptr->info_ptr = NULL;
		return 2;
	}


	/* close out PNG file; if we had any text or time info to write after
	 * the IDATs, second argument would be info_ptr: */

	png_write_end(png_ptr, NULL);

	return 0;
}





void writepng_cleanup(mainprog_info *mainprog_ptr)
{
	png_structp png_ptr = (png_structp)mainprog_ptr->png_ptr;
	png_infop info_ptr = (png_infop)mainprog_ptr->info_ptr;

	if (png_ptr && info_ptr)
		png_destroy_write_struct(&png_ptr, &info_ptr);
}





static void writepng_error_handler(png_structp png_ptr, png_const_charp msg)
{
	mainprog_info  *mainprog_ptr;

	/* This function, aside from the extra step of retrieving the "error
	 * pointer" (below) and the fact that it exists within the application
	 * rather than within libpng, is essentially identical to libpng's
	 * default error handler.  The second point is critical:  since both
	 * setjmp() and longjmp() are called from the same code, they are
	 * guaranteed to have compatible notions of how big a jmp_buf is,
	 * regardless of whether _BSD_SOURCE or anything else has (or has not)
	 * been defined. */

	fprintf(stderr, "writepng libpng error: %s\n", msg);
	fflush(stderr);

	mainprog_ptr = (mainprog_info*)png_get_error_ptr(png_ptr);
	if (mainprog_ptr == NULL) {         /* we are completely hosed now */
		fprintf(stderr,
			"writepng severe error:  jmpbuf not recoverable; terminating.\n");
		fflush(stderr);
		exit(99);
	}

	longjmp(mainprog_ptr->jmpbuf, 1);
}


void extractQr(const uint8_t qrcode[], unsigned char *image_data)
{
	int size = qrgen_getSize(qrcode);
	int border = 4;

	unsigned char* image_pos = image_data;

	for (int y = -border; y < size + border; y++)
	{
		for (int t = 0; t < 4; t++)
			for (int x = -border; x < size + border; x++)
			{
				bool pixel = qrgen_getModule(qrcode, x, y);
				for (int z = 0; z < 4; z++)
				{
					if (pixel)
						*image_pos = 0;
					else
						*image_pos = 255;
					image_pos++;
				}
			}
	}
}


void wpng_cleanup(mainprog_info *wpng_info)
{
	if (wpng_info->outfile)
	{
		fclose(wpng_info->outfile);
		wpng_info->outfile = NULL;
	}

	if (wpng_info->infile)
	{
		fclose(wpng_info->infile);
		wpng_info->infile = NULL;
	}

	if (wpng_info->image_data)
	{
		free(wpng_info->image_data);
		wpng_info->image_data = NULL;
	}

	if (wpng_info->row_pointers)
	{
		free(wpng_info->row_pointers);
		wpng_info->row_pointers = NULL;
	}
}


int write_png(mainprog_info *wpng_info, uint8_t qrcode[])
{
	char *bgstr = NULL;
	ulg rowbytes;
	int rc, len = 0;
	int error = 0;
	double LUT_exponent;                // just the lookup table
	double CRT_exponent = 2.2;          // just the monitor
	double default_display_exponent;    // whole display system
	double default_gamma = 0.0;

	LUT_exponent = 1.0;   // assume no LUT:  most PCs

	// the defaults above give 1.0, 1.3, 1.5 and 2.2, respectively:
	default_display_exponent = LUT_exponent * CRT_exponent;

	if (default_gamma == 0.0)
		default_gamma = 1.0 / default_display_exponent;


	// open the input and output files, or register an error and abort

	wpng_info->filter = TRUE;

	// allocate libpng stuff, initialize transformations, write pre-IDAT data

	if ((rc = writepng_init(wpng_info)) != 0)
	{
		switch (rc)
		{
		case 2:
			fprintf(stderr, "writepng_init Error:  libpng initialization problem (longjmp)\n");
			break;
		case 4:
			fprintf(stderr, "writepng_init Error:  insufficient memory\n");
			break;
		case 11:
			fprintf(stderr, "writepng_init Error:  internal logic error (unexpected PNM type)\n");
			break;
		default:
			fprintf(stderr, "writepng_init Error:  unknown writepng_init() error\n");
			break;
		}

		return -rc;
	}

	// calculate rowbytes on basis of image type; note that this becomes much
	// more complicated if we choose to support PBM type, ASCII PNM types, or
	// 16-bit-per-sample binary data [currently not an official NetPBM type]

	if (wpng_info->pnmtype == 5)
		rowbytes = wpng_info->width;
	else if (wpng_info->pnmtype == 6)
		rowbytes = wpng_info->width * 3;
	else /* if (wpng_info.pnmtype == 8) */
		rowbytes = wpng_info->width * 4;


	// read and write the image, either in its entirety (if writing interlaced
	// PNG) or row by row (if non-interlaced)

	fprintf(stderr, "Encoding image data...\n");
	fflush(stderr);

	if (wpng_info->interlaced)
	{
		long i;
		ulg image_bytes = rowbytes * wpng_info->height;   // overflow?

		wpng_info->image_data = (uch *)malloc(image_bytes);
		wpng_info->row_pointers = (uch **)malloc(wpng_info->height * sizeof(uch *));

		for (i = 0; i < wpng_info->height; ++i)
			wpng_info->row_pointers[i] = wpng_info->image_data + i * rowbytes;

		extractQr(qrcode, wpng_info->image_data);

		if (writepng_encode_image(wpng_info) != 0)
		{
			fprintf(stderr, "writepng_encode_image Error:  libpng problem (longjmp) while writing image data\n");
			writepng_cleanup(wpng_info);
			wpng_cleanup(wpng_info);
			return -2;
		}
	}

	/* OK, we're done (successfully):  clean up all resources and quit */

	fprintf(stderr, "Done.\n");
	fflush(stderr);

	wpng_cleanup(wpng_info);
	writepng_cleanup(wpng_info);

	fprintf(stderr, "Cleanup.\n");
	fflush(stderr);

	return 0;
}


int load_png(struct quirc *q, const char *filename)
{
	int width, height, rowbytes, interlace_type, number_passes = 1;
	png_uint_32 trns;
	png_byte color_type, bit_depth;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	FILE *infile = NULL;
	uint8_t *image;
	int ret = -1;
	int pass;

	if ((infile = fopen(filename, "rb")) == NULL)
		goto out;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		goto out;

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		goto out;

	//if (setjmp(png_jmpbuf(png_ptr)))
	//	goto out;

	png_init_io(png_ptr, infile);

	png_read_info(png_ptr, info_ptr);

	color_type = png_get_color_type(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	interlace_type = png_get_interlace_type(png_ptr, info_ptr);

	// Read any color_type into 8bit depth, Grayscale format.
	// See http://www.libpng.org/pub/png/libpng-manual.txt

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png_ptr);

	if ((trns = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)))
		png_set_tRNS_to_alpha(png_ptr);

	if (bit_depth == 16)
#if PNG_LIBPNG_VER >= 10504
		png_set_scale_16(png_ptr);
#else
		png_set_strip_16(png_ptr);
#endif

	if ((trns) || color_type & PNG_COLOR_MASK_ALPHA)
		png_set_strip_alpha(png_ptr);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	if (color_type == PNG_COLOR_TYPE_PALETTE ||
		color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		png_set_rgb_to_gray_fixed(png_ptr, 1, -1, -1);
	}

	if (interlace_type != PNG_INTERLACE_NONE)
		number_passes = png_set_interlace_handling(png_ptr);

	png_read_update_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	if (rowbytes != width) {
		fprintf(stderr,
			"load_png: expected rowbytes to be %u but got %u\n",
			width, rowbytes);
		goto out;
	}

	if (quirc_resize_api(q, width, height) < 0)
		goto out;

	image = quirc_begin_api(q, NULL, NULL);

	for (pass = 0; pass < number_passes; pass++) {
		int y;

		for (y = 0; y < height; y++) {
			png_bytep row_pointer = image + y * width;
			png_read_rows(png_ptr, &row_pointer, NULL, 1);
		}
	}

	png_read_end(png_ptr, info_ptr);

	ret = 0;
	/* FALLTHROUGH */
out:
	/* cleanup */
	if (png_ptr) {
		if (info_ptr)
			png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		else
			png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
	}
	if (infile)
		fclose(infile);
	return (ret);
}
