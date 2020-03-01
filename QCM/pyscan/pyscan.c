#define PY_SSIZE_T_CLEAN
#include <Python.h>

//! Modules interface Interface
#include "zlib.h"
#include "png.h"
#include "qrgen.h"
#include "scan.h"

// Function 1: A simple 'hello world' function
static PyObject* helloworld(PyObject* self, PyObject* args)
{
	printf("Hello World from pyscan extension module\n");
	return Py_None;
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

static PyObject* qrread(PyObject* self, PyObject* args)
{
    const char *path = NULL;
    Py_ssize_t pathlen = 0;

    if(!PyArg_ParseTuple(args, "s#", &path, &pathlen))
    {
        return NULL;
    }
    if (NULL == path)
    {
        return NULL;
    }

	char *payload = read_qrcode(path);
	if (NULL != payload)
	{
		printf("QR code %s décodé: [%s].\n", path, payload);
		return Py_None;
	}
	else
	{
		printf("Echec de l'import du QR code\n");
		return NULL;
	}
}

// Function 3: A qrcode generator
static PyObject* qrgen(PyObject* self, PyObject* args)
{
    const char *text = NULL;
    Py_ssize_t textlen = 0;
    const char *path = NULL;
    Py_ssize_t pathlen = 0;

    if(!PyArg_ParseTuple(args, "s#s#", &text, &textlen, &path, &pathlen))
    {
        return NULL;
    }
    if ((NULL == text) || (NULL == path))
    {
        return NULL;
    }

    if (1 == export_png_qrcode(text, path))
    {
        printf("QR code %s généré.\n", path);
        return Py_None;
    }
    else
    {
        printf("Echec de l'export du QR code\n");
        return NULL;
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
    fprintf(stderr, "   Compiled with qrgen %s; using qrgen %s.\n",
					QRGEN_VERSION, qrgen_version());

    if (0 == init_scan())
    {
        fprintf(stderr, "   Failed to initialize scan library. Please restart.\n");
    }

	return PyModule_Create(&myModule);
}
