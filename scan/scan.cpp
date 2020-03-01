// scan.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <sstream>
#include <cstring>
#include <setjmp.h>
#include <time.h>

#include "scan.h"

#include "TGAImaging.h"
#include "JPGImaging.h"
//#include "TIFFImaging.h"
#include "PNGImaging.h"
#include "Image.h"

//	QR code generator
#include "../qrgen/QR-Code-generator/c/qrcodegen.h"
#include "../qrgen/qrgen.h"

//	QR code decoder
#include "../QUIRC/quirc/lib/quirc_internal.h"
#include "../QUIRC/quirc_api.h"


#include "writepng.h"
extern int load_jpeg(struct quirc *q, const char *filename);
extern int write_png(mainprog_info *wpng_info, uint8_t qrcode[]);
extern int load_png(struct quirc *q, const char *filename);


extern bool extracttable(CImage &in, const std::string &out);
extern bool extractnum(CImage &in, const std::string &out);

//	Global variable to hold a set of results for a set of scan (typically a classroom)
std::vector<SCAN>	global_scans;
//	Global variable to hold an image for successive operations
CImage				*current_scan = NULL;


extern "C" void __cxa_pure_virtual()
{
	exit(-1);
}

extern SCAN_API int scan_version(void)
{
	return 12345;
}

SCAN_API int init_scan(void)
{
	CImage::setImageKindIO(new CTGAImaging());
	CImage::setImageKindIO(new CJPGImaging());
	//CImage::setImageKindIO(new CTIFFImaging());
	CImage::setImageKindIO(new CPNGImaging());

	return 1;
}

SCAN_API int release_scan(void)
{
	int c = close_scan();

	return ((1 == c) ? 1 : 0);
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
		return 0;
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
		wpng_info.title = (char*)"Fanny Ferrand";
		wpng_info.have_text |= TEXT_TITLE;
		wpng_info.author = (char*)"Fabrice Ferrand";
		wpng_info.have_text |= TEXT_AUTHOR;
		wpng_info.desc = (char*)"QCM QR Code";
		wpng_info.have_text |= TEXT_DESC;
		wpng_info.copyright = (char*)"Rafale Soft Inc. (c)";
		wpng_info.have_text |= TEXT_COPY;
		wpng_info.email = (char*)"fabfer@wanadoo.fr";
		wpng_info.have_text |= TEXT_EMAIL;
		wpng_info.url = (char*)"http://github.com/RafaleSoft";
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


static const char *data_type_str(int dt)
{
	switch (dt) {
	case QUIRC_DATA_TYPE_NUMERIC: return "NUMERIC";
	case QUIRC_DATA_TYPE_ALPHA:   return "ALPHA";
	case QUIRC_DATA_TYPE_BYTE:    return "BYTE";
	case QUIRC_DATA_TYPE_KANJI:   return "KANJI";
	}

	return "unknown";
}

void dump_data(const struct quirc_data *data)
{
	printf("    Version: %d\n", data->version);
	printf("    ECC level: %c\n", "MLHQ"[data->ecc_level]);
	printf("    Mask: %d\n", data->mask);
	printf("    Data type: %d (%s)\n",
		data->data_type, data_type_str(data->data_type));
	printf("    Length: %d\n", data->payload_len);
	printf("    Payload: %s\n", data->payload);

	if (data->eci)
		printf("    ECI: %d\n", data->eci);
}

void dump_cells(const struct quirc_code *code)
{
	int u, v;

	printf("    %d cells, corners:", code->size);
	for (u = 0; u < 4; u++)
		printf(" (%d,%d)", code->corners[u].x,
			code->corners[u].y);
	printf("\n");

	for (v = 0; v < code->size; v++) {
		printf("    ");
		for (u = 0; u < code->size; u++) {
			int p = v * code->size + u;

			if (code->cell_bitmap[p >> 3] & (1 << (p & 7)))
				printf("[]");
			else
				printf("  ");
		}
		printf("\n");
	}
}

static int qrdecode(struct quirc *decoder,
	const char *path,
	struct result_info *info)
{
	size_t len = strlen(path);
	while (len >= 0 && path[len] != '/')
		len--;
	const char *filename = path + len + 1;

	struct stat st;
	if (stat(path, &st) < 0)
	{
		fprintf(stderr, "%s: lstat: %s\n", path, strerror(errno));
		return -1;
	}

	int(*loader)(struct quirc *, const char *);
	len = strlen(filename);
	const char *ext = NULL;

	while (len >= 0 && filename[len] != '.')
		len--;
	ext = filename + len + 1;
	if (0 == strcmp(ext, "jpg") ||
		0 == strcmp(ext, "JPG") ||
		0 == strcmp(ext, "jpeg") ||
		0 == strcmp(ext, "JPEG"))
		loader = load_jpeg;
	else if (0 == strcmp(ext, "png") || 0 == strcmp(ext, "PNG"))
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

SCAN_API char* read_qrcode(const char *path)
{
	struct quirc *decoder = quirc_new_api();
	if (NULL == decoder)
	{
		fprintf(stderr, "   Unable to create a quirc decoder.\n");
		return NULL;
	}

	printf("Décodage QR-code pour: %s\n", path);

	struct result_info info;
	memset(&info, 0, sizeof(info));
	if (qrdecode(decoder, path, &info) <= 0)
		fprintf(stderr, "   Unable to decode QRCode.\n");

	quirc_destroy_api(decoder);

	return NULL;
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
			fprintf(stderr, "open_scan Error:  impossible to extract response table\n");
			return 0;
		}
		else if (!extractnum(image, "num.tiff"))
		{
			fprintf(stderr, "open_scan Error:  impossible to extract identifier\n");
			return 0;
		}
		else
			return 1;
	}
	else
	{
		fprintf(stderr, "open_scan Error:  impossible to open source image\n");
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
		rt = rt | UNLINK(doc.str().c_str());
	}

	rt = rt | UNLINK("rectified.jpg");
	rt = rt | UNLINK("resized.jpg");

	return (0 == rt);
}

