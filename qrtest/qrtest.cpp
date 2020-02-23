/* quirc -- QR-code recognition library
 * Copyright (C) 2010-2012 Daniel Beer <dlbeer@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include "stdafx.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>

#include <setjmp.h>
#include <time.h>
#include "dbgutil.h"

#include <quirc/lib/quirc.h>
#include <quirc_api.h>
#include <QR-Code-generator/c/qrcodegen.h>
#include <qrgen.h>
#include <scan.h>
extern "C"
{
	#include "writepng.h"
}

#include <malloc.h>


static int want_verbose = 0;
static int want_cell_dump = 0;

static struct quirc *decoder;

struct result_info
{
	int		file_count;
	int		id_count;
	int		decode_count;

	unsigned int	load_time;
	unsigned int	identify_time;
	unsigned int	total_time;
};

static void print_result(const char *name, struct result_info *info)
{
	puts("----------------------------------------"
	     "---------------------------------------");
	printf("%s: %d files, %d codes, %d decoded (%d failures)",
	       name, info->file_count, info->id_count, info->decode_count,
	       (info->id_count - info->decode_count));
	if (info->id_count)
		printf(", %d%% success rate",
		       (info->decode_count * 100 + info->id_count / 2) /
			info->id_count);
	printf("\n");
	printf("Total time [load: %u, identify: %u, total: %u]\n",
	       info->load_time,
	       info->identify_time,
	       info->total_time);
	if (info->file_count)
		printf("Average time [load: %u, identify: %u, total: %u]\n",
		       info->load_time / info->file_count,
		       info->identify_time / info->file_count,
		       info->total_time / info->file_count);
}

static void add_result(struct result_info *sum, struct result_info *inf)
{
	sum->file_count += inf->file_count;
	sum->id_count += inf->id_count;
	sum->decode_count += inf->decode_count;

	sum->load_time += inf->load_time;
	sum->identify_time += inf->identify_time;
	sum->total_time += inf->total_time;
}

static int scan_file(const char *path, const char *filename,
		     struct result_info *info)
{
	int (*loader)(struct quirc *, const char *);
	int len = strlen(filename);
	const char *ext;
	
	int ret;
	int i;

	while (len >= 0 && filename[len] != '.')
		len--;
	ext = filename + len + 1;
	if (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0)
		loader = load_jpeg;
	else if (strcmp(ext, "png") == 0)
		loader = load_png;
	else
		return 0;

	ret = loader(decoder, path);
	if (ret < 0) {
		fprintf(stderr, "%s: load failed\n", filename);
		return -1;
	}

	quirc_end_api(decoder);

	info->id_count = quirc_count_api(decoder);
	for (i = 0; i < info->id_count; i++)
	{
		struct quirc_code code;
		struct quirc_data data;

		quirc_extract_api(decoder, i, &code);

		if (!quirc_decode_api(&code, &data))
			info->decode_count++;
	}

	printf("  %-30s: %5u %5u %5u %5d %5d\n", filename,
	       info->load_time,
	       info->identify_time,
	       info->total_time,
	       info->id_count, info->decode_count);

	if (want_cell_dump || want_verbose)
	{
		for (i = 0; i < info->id_count; i++) {
			struct quirc_code code;

			quirc_extract_api(decoder, i, &code);
			if (want_cell_dump) {
				dump_cells(&code);
				printf("\n");
			}

			if (want_verbose) {
				struct quirc_data data;
				quirc_decode_error_t err =
					quirc_decode_api(&code, &data);

				if (err) {
					printf("  ERROR: %s\n\n",
					       quirc_strerror_api(err));
				} else {
					printf("  Decode successful:\n");
					dump_data(&data);
					printf("\n");
				}
			}
		}
	}

	info->file_count = 1;
	return 1;
}

static int test_scan(const char *path, struct result_info *info)
{
	int len = strlen(path);
	struct stat st;
	const char *filename;

	memset(info, 0, sizeof(*info));

	while (len >= 0 && path[len] != '/')
		len--;
	filename = path + len + 1;

	if (stat(path, &st) < 0) {
		fprintf(stderr, "%s: lstat: %s\n", path, strerror(errno));
		return -1;
	}

	return scan_file(path, filename, info);
}

static int run_tests(int argc, char **argv)
{
	struct result_info sum;
	int count = 0;
	int i;

	decoder = quirc_new_api();
	if (!decoder) {
		perror("quirc_new");
		return -1;
	}

	printf("  %-30s  %17s %11s\n", "", "Time (ms)", "Count");
	printf("  %-30s  %5s %5s %5s %5s %5s\n",
	       "Filename", "Load", "ID", "Total", "ID", "Dec");
	puts("----------------------------------------"
	     "---------------------------------------");

	memset(&sum, 0, sizeof(sum));
	for (i = 1; i < argc; i++) {
		struct result_info info;

		if (test_scan(argv[i], &info) > 0) {
			add_result(&sum, &info);
			count++;
		}
	}

	if (count > 1)
		print_result("TOTAL", &sum);

	quirc_destroy_api(decoder);
	return 0;
}

void printQr(const uint8_t qrcode[])
{
	int size = qrgen_getSize(qrcode);
	int border = 4;
	for (int y = -border; y < size + border; y++)
	{
		for (int x = -border; x < size + border; x++)
		{
			fputs((qrgen_getModule(qrcode, x, y) ? "##" : "  "), stdout);
		}
		fputs("\n", stdout);
	}
	fputs("\n", stdout);
}

void gen_qrcode(const char *text, unsigned char* qrcode)
{
	// Error correction level
	enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_LOW;  

	// Make and print the QR Code symbol
	//uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	bool ok = qrgen_encodeText(text, tempBuffer, qrcode, 
								   errCorLvl,
								   qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, 
								   qrcodegen_Mask_AUTO, true);
	
	if (ok)
		printQr(qrcode);
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


void wpng_cleanup(mainprog_info &wpng_info)
{
	if (wpng_info.outfile)
	{
		fclose(wpng_info.outfile);
		wpng_info.outfile = NULL;
	}

	if (wpng_info.infile)
	{
		fclose(wpng_info.infile);
		wpng_info.infile = NULL;
	}

	if (wpng_info.image_data)
	{
		free(wpng_info.image_data);
		wpng_info.image_data = NULL;
	}

	if (wpng_info.row_pointers)
	{
		free(wpng_info.row_pointers);
		wpng_info.row_pointers = NULL;
	}
}

int write_png(mainprog_info& wpng_info, uint8_t qrcode[])
{
	char *bgstr = NULL;
	ulg rowbytes;
	int rc, len = 0;
	int error = 0;
	double LUT_exponent;                /* just the lookup table */
	double CRT_exponent = 2.2;          /* just the monitor */
	double default_display_exponent;    /* whole display system */
	double default_gamma = 0.0;

	
	LUT_exponent = 1.0;   /* assume no LUT:  most PCs */

	/* the defaults above give 1.0, 1.3, 1.5 and 2.2, respectively: */
	default_display_exponent = LUT_exponent * CRT_exponent;

	if (default_gamma == 0.0)
		default_gamma = 1.0 / default_display_exponent;


	/* open the input and output files, or register an error and abort */

	//wpng_info.infile = fdopen(fileno(stdin), "rb");
	wpng_info.filter = TRUE;

	/* allocate libpng stuff, initialize transformations, write pre-IDAT data */

	if ((rc = writepng_init(&wpng_info)) != 0)
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


	/* calculate rowbytes on basis of image type; note that this becomes much
	* more complicated if we choose to support PBM type, ASCII PNM types, or
	* 16-bit-per-sample binary data [currently not an official NetPBM type] */

	if (wpng_info.pnmtype == 5)
		rowbytes = wpng_info.width;
	else if (wpng_info.pnmtype == 6)
		rowbytes = wpng_info.width * 3;
	else /* if (wpng_info.pnmtype == 8) */
		rowbytes = wpng_info.width * 4;


	/* read and write the image, either in its entirety (if writing interlaced
	* PNG) or row by row (if non-interlaced) */

	fprintf(stderr, "Encoding image data...\n");
	fflush(stderr);

	if (wpng_info.interlaced)
	{
		long i;
		ulg image_bytes = rowbytes * wpng_info.height;   /* overflow? */

		wpng_info.image_data = (uch *)malloc(image_bytes);
		wpng_info.row_pointers = (uch **)malloc(wpng_info.height*sizeof(uch *));
		
		for (i = 0; i < wpng_info.height; ++i)
			wpng_info.row_pointers[i] = wpng_info.image_data + i*rowbytes;
		
		//bytes = fread(wpng_info.image_data, 1, image_bytes, wpng_info.infile);
		extractQr(qrcode, wpng_info.image_data);
		
		if (writepng_encode_image(&wpng_info) != 0)
		{
			fprintf(stderr, "writepng_encode_image Error:  libpng problem (longjmp) while writing image data\n");
			writepng_cleanup(&wpng_info);
			wpng_cleanup(wpng_info);
			return -2;
		}
	}

	/* OK, we're done (successfully):  clean up all resources and quit */

	fprintf(stderr, "Done.\n");
	fflush(stderr);

	writepng_cleanup(&wpng_info);
	wpng_cleanup(wpng_info);

	return 0;
}




int main(int argc, char **argv)
{
	printf("quirc test program\n");
	printf("Copyright (C) 2010-2012 Daniel Beer <dlbeer@gmail.com>\n");
	printf("Library version: %s\n", quirc_version_api());
	printf("\n");

	want_verbose = 1;
	want_cell_dump = 1;
	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
	gen_qrcode("Fanny Ferrand",qrcode);

	int size = 0;
	size = qrgen_getSize(qrcode);

	mainprog_info wpng_info;   /* lone global */
	memset(&wpng_info, 0, sizeof(mainprog_info));
	wpng_info.interlaced = TRUE;
	wpng_info.modtime = time(NULL);
	wpng_info.have_time = TRUE;
	wpng_info.sample_depth = 8;
	wpng_info.width = 4 * (size + 2 * 4); // + border
	wpng_info.height = 4 * (size + 2 * 4); // + border
	wpng_info.title = "Fanny Ferrand";
	wpng_info.have_text |= TEXT_TITLE;
	wpng_info.author = "Fabrice Ferrand";
	wpng_info.have_text |= TEXT_AUTHOR;
	wpng_info.desc = "QCM QR Code";
	wpng_info.have_text |= TEXT_DESC;
	wpng_info.copyright = "Rafale Soft Inc. (c)";
	wpng_info.have_text |= TEXT_COPY;
	wpng_info.email = "fabfer@wanadoo.fr";
	wpng_info.have_text |= TEXT_EMAIL;
	wpng_info.url = "http://github.com/RafaleSoft";
	wpng_info.have_text |= TEXT_URL;
	wpng_info.outfile = fopen("qr3.png", "wb");
	wpng_info.pnmtype = 5;

	write_png(wpng_info, qrcode);

	return run_tests(argc, argv);
}
