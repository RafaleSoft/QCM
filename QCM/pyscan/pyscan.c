#define PY_SSIZE_T_CLEAN
#include <Python.h>

//! Scan interfaces
#include "../../scan/scan.h"

//! Png Interface
#include <setjmp.h>
#include "../../zlib-1.2.11/include/zlib.h"
#include "../../libpng-1.6.36/include/png.h"

//! QR code interfaces
#include "../../qrgen/QR-Code-generator/c/qrcodegen.h"
#include "../../qrgen/qrgen.h"
#include "../../QUIRC/quirc/lib/quirc_internal.h"
#include "../../QUIRC/quirc_api.h"

#include "writepng.h"
#include "dbgutil.h"


// Function 1: A simple 'hello world' function
static PyObject* helloworld(PyObject* self, PyObject* args)
{
	printf("Hello World from pyscan extension module\n");
	return Py_None;
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
		wpng_info->row_pointers = (uch **)malloc(wpng_info->height*sizeof(uch *));

		for (i = 0; i < wpng_info->height; ++i)
			wpng_info->row_pointers[i] = wpng_info->image_data + i*rowbytes;

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

	writepng_cleanup(wpng_info);
	wpng_cleanup(wpng_info);

	return 0;
}

struct result_info
{
	int		file_count;
	int		id_count;
	int		decode_count;

	unsigned int	load_time;
	unsigned int	identify_time;
	unsigned int	total_time;
};

static int qrdecode(struct quirc *decoder,
                    const char *path,
                    struct result_info *info)
{
	int len = strlen(path);
	while (len >= 0 && path[len] != '/')
		len--;
	const char *filename = path + len + 1;

    struct stat st;
	if (stat(path, &st) < 0)
	{
		fprintf(stderr, "%s: lstat: %s\n", path, strerror(errno));
		return -1;
	}

	int (*loader)(struct quirc *, const char *);
	len = strlen(filename);
	const char *ext = NULL;

	while (len >= 0 && filename[len] != '.')
		len--;
	ext = filename + len + 1;
	if (strcmp(ext, "jpg") == 0 ||
	    strcmp(ext, "JPG") == 0 ||
	    strcmp(ext, "jpeg") == 0 ||
	    strcmp(ext, "JPEG") == 0)
		loader = load_jpeg;
	else if (strcmp(ext, "png") == 0 || strcmp(ext, "PNG") == 0)
		loader = load_png;
	else
		return 0;

	int ret = loader(decoder, path);
	if (ret < 0)
	{
		fprintf(stderr, "%s: load failed\n", filename);
		return -1;
	}

	quirc_end_api(decoder);

	info->id_count = quirc_count_api(decoder);
	for (int i = 0; i < info->id_count; i++)
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
	       info->id_count,
	       info->decode_count);

    for (int i = 0; i < info->id_count; i++)
    {
        struct quirc_code code;

        quirc_extract_api(decoder, i, &code);
        struct quirc_data data;
        quirc_decode_error_t err = quirc_decode_api(&code, &data);

        printf("  Decode successful:\n");
        dump_data(&data);
        printf("\n");
    }

	info->file_count = 1;
	return 1;
}

static PyObject* qrread(PyObject* self, PyObject* args)
{
    const char *text = NULL;
    Py_ssize_t textlen = 0;

    if(!PyArg_ParseTuple(args, "s#", &text, &textlen))
        return NULL;
    if (NULL == text)
        return NULL;

	struct quirc *decoder = quirc_new_api();
	if (NULL == decoder)
	{
	    fprintf(stderr, "   Unable to create a quirc decoder.\n");
		return NULL;
	}

    printf("Décodage QR-code pour: %s\n",text);

	struct result_info info;
	memset(&info, 0, sizeof(info));
	if (qrdecode(decoder, text, &info) <= 0)
	    fprintf(stderr, "   Unable to decode QRCode.\n");

	quirc_destroy_api(decoder);

	return Py_None;
}

// Function 3: A qrcode generator
static PyObject* qrgen(PyObject* self, PyObject* args)
{
    const char *text = NULL;
    Py_ssize_t textlen = 0;

    if(!PyArg_ParseTuple(args, "s#", &text, &textlen))
        return NULL;
    if (NULL == text)
        return NULL;

    printf("Generation QR-code pour: %s\n",text);

    uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];

	// Make and print the QR Code symbol
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	bool ok = qrgen_encodeText( text,
	                            tempBuffer,
	                            qrcode,
							    qrcodegen_Ecc_LOW,
								qrcodegen_VERSION_MIN,
								qrcodegen_VERSION_MAX,
								qrcodegen_Mask_AUTO,
								true);
    if (!ok)
    {
        printf("Echec de génération du QR code\n");
        return NULL;
    }
    else
    {
        printf("Génération du QR code réussie\n");

	    int size = qrgen_getSize(qrcode);

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

        if (0 == write_png(&wpng_info, qrcode))
        {
            printf("QR code qr3.png généré.\n");
            return Py_None;
        }
        else
        {
            printf("Echec de l'export du QR code\n");
            return NULL;
        }
    }
}

// Our Module's Function Definition struct
// We require this `NULL` to signal the end of our method
// definition
static PyMethodDef myMethods[] = {
	{ "helloworld", helloworld, METH_NOARGS, "Prints Hello World" },
	{ "qrgen", qrgen, METH_VARARGS, "Generates a qrcode with provided data" },
	{ "qrread", qrread, METH_VARARGS, "Decodes a qrcode with provided filename" },
	{ NULL, NULL, 0, NULL }
};

// Our Module Definition struct
static struct PyModuleDef myModule =
{
	PyModuleDef_HEAD_INIT,
	"PyScan",               // Module name
	"Py Scan module",       // Module documentation
	-1,                     // Size of module per-interpreter state
	myMethods
};

// Initializes our module using our above struct
PyMODINIT_FUNC PyInit_pyscan(void)
{
    fprintf(stderr, "   Compiled with libpng %s; using libpng %d.\n",
                    PNG_LIBPNG_VER_STRING, png_access_version_number());
    fprintf(stderr, "   Compiled with zlib %s; using zlib %s.\n",
                    ZLIB_VERSION, zlibVersion());
    fprintf(stderr, "   Compiled with scan %d; using scan %d.\n",
                    1, scan_version());
    fprintf(stderr, "   Compiled with qrgen %d; using qrgen %d.\n",
                    1, qrgen_version());

    if (0 == init_scan())
        fprintf(stderr, "   Failed to initialize scan library.\n");

	return PyModule_Create(&myModule);
}
