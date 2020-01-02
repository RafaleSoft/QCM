#define PY_SSIZE_T_CLEAN
#include <Python.h>

//! Scan interfaces
#include "../../scan/scan.h"

extern "C"
{
	#include "../../qrgen/writepng.h"
}

//! QR code interfaces
#include "../../qrgen/QR-Code-generator/c/qrcodegen.h"
#include "../../qrgen/qrgen.h"

// Function 1: A simple 'hello world' function
static PyObject* helloworld(PyObject* self, PyObject* args)
{
	printf("Hello World from pyscan extension module\n");
	return Py_None;
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


bool export_qrcode(uint8_t *qrcode)
{
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
}


// Function 3: A qrcode generator
static PyObject* qrgen(PyObject* self, PyObject* args)
{
    // if our `n` value
    const char *text = NULL;
    Py_ssize_t textlen = 0;

    if(!PyArg_ParseTuple(args, "s#", &text, &textlen))
        return NULL;
    if (NULL == text)
        return NULL;

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
        return Py_None;
    }
}

// Our Module's Function Definition struct
// We require this `NULL` to signal the end of our method
// definition
static PyMethodDef myMethods[] = {
	{ "helloworld", helloworld, METH_NOARGS, "Prints Hello World" },
	{ "qrgen", qrgen, METH_VARARGS, "Generates a qrcode with provided data" },
	{ NULL, NULL, 0, NULL }
};

// Our Module Definition struct
static struct PyModuleDef myModule = {
	PyModuleDef_HEAD_INIT,
	"PyScan",               // Module name
	"Py Scan module",       // Module documentation
	-1,                     // Size of module per-interpreter state
	myMethods
};

// Initializes our module using our above struct
PyMODINIT_FUNC PyInit_pyscan(void)
{
	return PyModule_Create(&myModule);
}
