// scan_jpg.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "process.h"
#include "scan.h"

#include "Subsys/CodeGeneration.h"
#include "System/RaptorConfig.h"
#include "System/RaptorErrorManager.h"

#include "ToolBox/Imaging.h"
#include "System/Image.h"

#include "baseapi.h"
#include "allheaders.h"

RAPTOR_NAMESPACE

//	Global data
extern std::vector<SCAN> global_scans;


//	Full table extraction normaliser threshold (below: black, above: white)
const uint8_t TRESH = 192;
//	Table cell filled ratio: above 1/FILL, cell is filled, empty otherwise
const uint8_t FILL = 9;

const float TABLE_Y_START = 13.17f;
const float TABLE_Y_END = 45.20f;
const float TABLE_X_START = 13.35f;
const float TABLE_X_END = 26.70f;

const float NUM_Y_START = 8.95f;
const float NUM_Y_END = 10.30f;
const float NUM_X_START = 22.20f;
const float NUM_X_END = 24.45f;

const size_t NUM_COLUMNS = 5;


bool normalize(CImage &in, const std::string &out)
{
	uint8_t *pixels = in.getPixels();
	if (NULL != pixels)
	{
		size_t w = in.getWidth();
		size_t h = in.getHeight();

		uint8_t *normalized = new uint8_t[w*h];
		memset(normalized, 0, w*h);

		for (size_t i = 0, j = 0; i < w*h * 4; i += 4, j++)
		{
			uint8_t r = pixels[i];
			uint8_t g = pixels[i + 1];
			uint8_t b = pixels[i + 2];

			uint8_t &n = normalized[j];

			if ((r >= TRESH) || (g >= TRESH) || (b >= TRESH))
				n = 255;
			else
				n = 0;
		}

		CImage tmp;
		tmp.allocatePixels(w, h);
		uint8_t *pixels2 = tmp.getPixels();
		if (NULL != pixels2)
		{
			for (size_t i = 0, j = 0; i < w*h * 4; i += 4, j++)
			{
				uint8_t n = normalized[j];

				pixels2[i] = n;
				pixels2[i + 1] = n;
				pixels2[i + 2] = n;
				pixels2[i + 3] = 255;
			}
		}

		CImage::IImageIO *io = in.getImageKindIO("tga");
		io->storeImageFile(out, &tmp);

		delete[] normalized;

		return true;
	}
	else
		return false;
}


/*
bool extracttable(CImage &in, const std::string &out)
{
	uint8_t *pixels = in.getPixels();
	if (NULL != pixels)
	{
		size_t w = in.getWidth();
		size_t h = in.getHeight();

		//	Image is upside down.
		size_t y_end = h - (size_t)(TABLE_Y_START * h * 0.01f);
		size_t y_start = h - (size_t)(TABLE_Y_END * h * 0.01f);

		size_t x_start = (size_t)(TABLE_X_START * w * 0.01f);
		size_t x_end = (size_t)(TABLE_X_END * w * 0.01f);

		uint8_t *table = new uint8_t[(x_end - x_start) * (y_end - y_start)];
		memset(table, 0, (x_end - x_start)*(y_end - y_start));

		for (size_t v = y_start, j = 0; v < y_end; v++)
		{
			for (size_t o = (w * v + x_start) * 4; o < (w * v + x_end) * 4; o += 4, j++)
			{
				uint8_t r = pixels[o];
				uint8_t g = pixels[o + 1];
				uint8_t b = pixels[o + 2];

				uint8_t &n = table[j];

				if ((r >= TRESH) || (g >= TRESH) || (b >= TRESH))
					n = 255;
				else
					n = 0;
			}
		}

		CImage tmp;
		tmp.allocatePixels((x_end - x_start), (y_end - y_start));
		uint8_t *pixels2 = tmp.getPixels();
		if (NULL != pixels2)
		{
			for (size_t i = 0, j = 0; j < (x_end - x_start) * (y_end - y_start); i += 4, j++)
			{
				uint8_t n = table[j];

				pixels2[i] = n;
				pixels2[i + 1] = n;
				pixels2[i + 2] = n;
				pixels2[i + 3] = 255;
			}
		}

		CImage::IImageIO *io = in.getImageKindIO("tga");
		io->storeImageFile(out, &tmp);

		delete[] table;

		return true;
	}
	else
		return false;
}
*/
/*
bool extractnum(CImage &in, const std::string &out)
{
	uint8_t *pixels = in.getPixels();
	if (NULL != pixels)
	{
		size_t w = in.getWidth();
		size_t h = in.getHeight();

		//	Image is upside down.
		size_t y_end = h - (size_t)(NUM_Y_START * h * 0.01f);
		size_t y_start = h - (size_t)(NUM_Y_END * h * 0.01f);

		size_t x_start = (size_t)(NUM_X_START * w * 0.01f);
		size_t x_end = (size_t)(NUM_X_END * w * 0.01f);

		uint8_t *table = new uint8_t[(x_end - x_start) * (y_end - y_start)];
		memset(table, 0, (x_end - x_start)*(y_end - y_start));

		for (size_t v = y_start, j = 0; v < y_end; v++)
		{
			for (size_t o = (w * v + x_start) * 4; o < (w * v + x_end) * 4; o += 4, j++)
			{
				uint8_t r = pixels[o];
				uint8_t g = pixels[o + 1];
				uint8_t b = pixels[o + 2];

				uint8_t &n = table[j];

				if ((r >= TRESH) || (g >= TRESH) || (b >= TRESH))
					n = 255;
				else
					n = 0;
			}
		}

		CImage tmp;
		tmp.allocatePixels((x_end - x_start), (y_end - y_start));
		uint8_t *pixels2 = tmp.getPixels();
		if (NULL != pixels2)
		{
			for (size_t i = 0, j = 0; j < (x_end - x_start) * (y_end - y_start); i += 4, j++)
			{
				uint8_t n = table[j];

				pixels2[i] = n;
				pixels2[i + 1] = n;
				pixels2[i + 2] = n;
				pixels2[i + 3] = 255;
			}
		}

		CImage::IImageIO *io = in.getImageKindIO("tiff");
		io->storeImageFile(out, &tmp);

		delete[] table;

		return true;
	}
	else
		return false;
}
*/


SCAN_API uint32_t extract_scan_id(const char* scan)
{
	if (NULL == scan)
		return 0;

	char *outText = NULL;
	uint32_t id = 0;

	int ex = system("F:\\QCM\\Tesseract-OCR\\tesseract.exe num.tiff scan -psm 7");

	FILE *txt = fopen("scan.txt", "r");
	if (NULL != txt)
	{
		char buffer[255];
		memset(buffer, 0, 255);

		size_t sz = fread(buffer, 1, 255, txt);
		if (sz > 0)
		{
			for (size_t i = 0; i < sz; i++)
			{
				if (buffer[i] == 'o')
					buffer[i] = '0';
				else if (buffer[i] == 'O')
					buffer[i] = '0';
				else if (buffer[i] == 'I')
					buffer[i] = '1';
				else if (buffer[i] == 'l')
					buffer[i] = '1';
			}
			id = atoi(buffer);
		}

		fclose(txt);
	}

	/*
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init(NULL, "eng"))
	{
	fprintf(stderr, "Could not initialize tesseract.\n");
	exit(1);
	}

	// Open input image with leptonica library
	Pix *image = pixRead("/usr/src/tesseract/testing/phototest.tif");
	api->SetImage(image);
	// Get OCR result
	outText = api->GetUTF8Text();
	printf("OCR output:\n%s", outText);

	// Destroy used object and release memory
	api->End();
	delete[] outText;
	pixDestroy(&image);
	*/

	return id;
}

SCAN_API int extract_scan_fills(size_t num_lines, size_t num_columns)
{
	if ((0 == num_lines) || (0 == num_columns))
		return 0;

	uint32_t scan_id = extract_scan_id("num.tiff");
	if (0 == scan_id)
		return 0;

	CImage::IImageOP::operation_param_t param;
	CVaArray<CImage::IImageOP::OP_KIND> iops;

	CImage image;
	if (image.loadImage("table.tga", iops, param))
	{
		SCAN scan;
		scan.num_answers = num_lines;
		scan.answers = new QCM_Answer[num_lines];

		uint8_t *pixels = image.getPixels();
		if (NULL != pixels)
		{
			size_t w = image.getWidth();
			size_t h = image.getHeight();

			// Loop for every line
			for (size_t l = 0; l < num_lines; l++)
			{
				size_t answer = 0;

				//	Image is upside down.
				size_t y_start = h - (size_t)((l + 1) * h / num_lines);
				size_t y_end = h - (size_t)((l + 0) * h / num_lines);

				// loop for every cell in line
				size_t max_value = 0;
				size_t max_ref = 0;

				for (size_t c = 0; c < num_columns; c++)
				{
					size_t x_start = (size_t)((c + 0) * w / num_columns);
					size_t x_end = (size_t)((c + 1) * w / num_columns);

					size_t num_pixels = 1;
					size_t value = 0;
					for (size_t v = y_start; v < y_end; v++)
					{
						for (size_t o = (w * v + x_start) * 4; o < (w * v + x_end) * 4; o += 4, num_pixels++)
						{
							uint8_t r = pixels[o];
							uint8_t g = pixels[o + 1];
							uint8_t b = pixels[o + 2];

							if ((r >= TRESH) || (g >= TRESH) || (b >= TRESH))
								;
							else
								value++;
						}
					}

					max_value = max(value, max_value);
					max_ref = max(num_pixels / FILL, max_ref);
					bool is_filled = (value > (num_pixels / FILL));
					if (is_filled)
					{
						if (answer > 0)
						{
							std::stringstream msg;
							msg << "Questionnaire " << scan_id << " incorrect: plusieurs reponses donnees à la question ";
							msg << l + 1 << ends;

							CRaptorErrorManager *mgr = Raptor::GetErrorManager();
							mgr->generateRaptorError(CPersistence::CPersistenceClassID::GetClassId(),
													 CRaptorErrorManager::RAPTOR_ERROR, msg.str());

							answer = 0;
							//	If multiple answers, abort current line.
							break;
						}
						else
							answer = c + 1;
					}
					else
						;	// answer not modified.
				}

				if (0 == answer)
				{
					std::stringstream msg;
					msg << "Questionnaire " << scan_id << " incomplet, ";
					msg << "Pas de reponse trouvée à la question ";
					msg << l + 1;
					msg << ". Estimation max: " << max_value << " / " << max_ref << ends;

					CRaptorErrorManager *mgr = Raptor::GetErrorManager();
					mgr->generateRaptorError(CPersistence::CPersistenceClassID::GetClassId(),
											 CRaptorErrorManager::RAPTOR_ERROR, msg.str());
				}

				// Store question
				QCM_Answer qanswer;
				qanswer.question_id = l;
				qanswer.question_value = answer;

				scan.scan_id = scan_id;
				scan.answers[l] = qanswer;
			}

			// Store QCM
			global_scans.push_back(scan);

			return 1;
		}
		else
			return 1;
	}
	else
		return 0;
}


SCAN_API int close_scan()
{
	for (size_t i = 0; i < global_scans.size(); i++)
	{
		SCAN& g_scan = global_scans[i];
		if (g_scan.answers != NULL)
			delete[] g_scan.answers;
	}

	global_scans.clear();

	int rt = _unlink("table.tga");
	int rn = _unlink("num.tiff");
	int rs = _unlink("scan.txt");

	if ((0 == rs) && (0 == rt) && (0 == rn))
		return 1;
	else
		return 0;
}

