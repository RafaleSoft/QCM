// scan.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "scan.h"
#include <sstream>

#include "TGAImaging.h"
#include "JPGImaging.h"
#include "TIFFImaging.h"
#include "PNGImaging.h"
#include "Image.h"

#include "../../qrgen/QR-Code-generator/c/qrcodegen.h"
#include "../../qrgen/qrgen.h"

#include <setjmp.h>
#include "writepng.h"
#include <time.h>

extern bool extracttable(CImage &in, const std::string &out);
extern bool extractnum(CImage &in, const std::string &out);

//	Global variable to hold a set of results for a set of scan (typically a classroom)
std::vector<SCAN>	global_scans;
//	Global variable to hold an image for successive operations
CImage				*current_scan = NULL;


extern SCAN_API int scan_version(void)
{
	return 12345;
}

SCAN_API int init_scan(void)
{
	CImage::setImageKindIO(new CTGAImaging());
	CImage::setImageKindIO(new CJPGImaging());
	CImage::setImageKindIO(new CTIFFImaging());
	CImage::setImageKindIO(new CPNGImaging());

	return 1;
}

SCAN_API int release_scan(void)
{
	int c = close_scan();

	return ((1 == c) ? 1 : 0);
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

SCAN_API int export_png_qrcode(const char *payload, const char *path)
{
	if ((NULL == payload) || (NULL == path))
		return 0;

	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];

	// Make and print the QR Code symbol
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	bool ok = qrgen_encodeText(	payload,
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
		wpng_info.outfile = fopen(path, "wb");
		wpng_info.pnmtype = 5;

		if (0 == write_png(&wpng_info, qrcode))
		{
			printf("QR code %s généré.\n", path);
			return 1;
		}
		else
		{
			printf("Echec de l'export du QR code\n");
			return 0;
		}
	}
}

SCAN_API int get_scan_answers(const SCAN* scan)
{
	if (NULL == scan)
		return 0;

	size_t nb_scans = global_scans.size();
	size_t index = nb_scans;
	for (size_t i = 0; (i < nb_scans) && (index == nb_scans); i++)
		if (global_scans[i].scan_id == scan->scan_id)
			index = i;

	if (index == global_scans.size())
	{
		std::stringstream msg;
		msg << "Questionnaire incorrect : scan_id ";
		msg << scan->scan_id << " introuvable";
		msg << std::ends;
		/*
		CRaptorErrorManager *mgr = Raptor::GetErrorManager();
		mgr->generateRaptorError(CPersistence::CPersistenceClassID::GetClassId(),
								 CRaptorErrorManager::RAPTOR_ERROR, msg.str());
								 */
		return 0;
	}

	SCAN& g_scan = global_scans[index];
	for (size_t i = 0; i < g_scan.num_answers; i++)
		if (i < scan->num_answers)
			scan->answers[i] = g_scan.answers[i];

	return 1;
}

SCAN_API int open_scan(const char* scan)
{
	CImage image;
	const std::vector<CImage::IImageOP*>& ops = {};
	if (image.loadImage(scan, ops))
	{
		if (!extracttable(image, "table.tga"))
		{
			/*
			mgr->generateRaptorError(CPersistence::CPersistenceClassID::GetClassId(),
									 CRaptorErrorManager::RAPTOR_ERROR,
									 "Impossible d'extraire la table des réponses");
									 */
			return 0;
		}
		else if (!extractnum(image, "num.tiff"))
		{
			/*
			mgr->generateRaptorError(CPersistence::CPersistenceClassID::GetClassId(),
									 CRaptorErrorManager::RAPTOR_ERROR,
									 "Impossible d'extraire l'identifiant");
									 */
			return 0;
		}
		else
			return 1;
	}
	else
	{
		/*
		mgr->generateRaptorError(CPersistence::CPersistenceClassID::GetClassId(),
								 CRaptorErrorManager::RAPTOR_FATAL,
								 "Impossible d'ouvrir l'image source");
								 */
		return 0;
	}
}


SCAN_API int open_doc(const char* doc)
{
	if (NULL == doc)
		return false;

	std::stringstream cmd;
	cmd << "F:\\QCM\\ghostscript\\gswin32c.exe -dNOPAUSE -dBATCH -sDEVICE=jpeg -sOutputFile=scan_p%d.jpg -r300x300 ";
	cmd << doc;
	cmd << std::ends;
	int ex = system(cmd.str().c_str());

	return ex;
}

SCAN_API int close_doc()
{
	for (size_t i = 0; i < global_scans.size(); i++)
	{
		SCAN& g_scan = global_scans[i];
		if (g_scan.answers != NULL)
			delete[] g_scan.answers;
	}

	if (NULL != current_scan)
		delete current_scan;

	global_scans.clear();

	int num_scan = 1;
	int rt = 0;
	while (0 == rt)
	{
		std::stringstream doc;
		doc << "scan_p" << num_scan++ << ".jpg" << std::ends;
		rt = rt | _unlink(doc.str().c_str());
	}

	rt = rt | _unlink("rectified.jpg");
	rt = rt | _unlink("resized.jpg");

	return (0 == rt);
}

