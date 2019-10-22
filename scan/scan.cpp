// scan.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "scan.h"

#include "Subsys/CodeGeneration.h"
#include "System/RaptorConfig.h"

#include "ToolBox/Imaging.h"
#include "System/Image.h"

RAPTOR_NAMESPACE


const uint8_t TRESH = 192;

const float TABLE_Y_START = 13.38f;
const float TABLE_Y_END = 45.38f;
const float TABLE_X_START = 12.94f;
const float TABLE_X_END = 26.41f;

const float NUM_Y_START = 8.87f;
const float NUM_Y_END = 10.30f;
const float NUM_X_START = 21.89f;
const float NUM_X_END = 24.41f;


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


SCAN_API bool open_scan(const char* scan)
{
	CRaptorConfig config;
	config.m_bRelocation = true;
	config.m_uiPolygons = 10000;
	config.m_uiVertices = 40000;
	config.m_logFile = "Scan.log";
	Raptor::glInitRaptor(config);

	CImaging::installImagers();

	CImage image;

	CImage::IImageOP::operation_param_t param;
	CVaArray<CImage::IImageOP::OP_KIND> iops;
	
	std::string scanfile(scan);
	if (image.loadImage(scanfile, iops, param))
	{
		if (!extracttable(image, "table.tga"))
			return false;
		else if (!extractnum(image, "num.tiff"))
			return false;
		else
			return true;
	}
	else
		return false;
}

SCAN_API bool close_scan()
{
	return false;
}

