// scan_pdf.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "process.h"
#include <math.h>
#include "scan.h"

#include "Subsys/CodeGeneration.h"
#include "System/RaptorConfig.h"
#include "System/RaptorErrorManager.h"

#include "ToolBox/Imaging.h"
#include "System/Image.h"

//	Global data
extern std::vector<SCAN> global_scans;
const uint8_t TRESH_HIGH = 192;
const uint8_t TRESH = 128;
const uint8_t TRESH_LOW = 64;

//	Margins for document data to skip artefacts on page borders.
const size_t MARGIN_LEFT = 30;
const size_t MARGIN_RIGHT = 30;
const size_t MARGIN_TOP = 1700;
const size_t MARGIN_BOTTOM = 30;

//	Line threshold to identify a line
const float LINE_THRESHOLD = 3.5f;

//	Identification lines
const float CORNER1_X = 86.0f / 1700.0f;
const float CORNER1_YMIN = (133.0f - 10.0f) / 2338.0f;
const float CORNER1_Y = 133.0f / 2338.0f;
const float CORNER1_YMAX = (133.0f + 10.0f) / 2338.0f;
const float CORNER2_YMIN = (190.0f - 10.0f) / 2338.0f;
const float CORNER2_Y = 190.0f / 2338.0f;
const float CORNER2_YMAX = (190.0f + 10.0f) / 2338.0f;
const float CORNER3_YMIN = (260.0f - 10.0f) / 2338.0f;
const float CORNER3_Y = 260.0f / 2338.0f;
const float CORNER3_YMAX = (260.0f + 10.0f) / 2338.0f;

static CImage *current_scan = NULL;


SCAN_API int open_doc(const char* doc)
{
	if (NULL == doc)
		return false;

	stringstream cmd;
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
		stringstream doc;
		doc << "scan_p" << num_scan++ << ".jpg" << ends;
		rt = rt | _unlink(doc.str().c_str());
	}

	rt = rt | _unlink("rectified.jpg");
	rt = rt | _unlink("resized.jpg");

	return (0 == rt);
}



/*

img = ChunkyPNG::Image.from_file('engine.png')

sobel_x = [[-1,0,1],
[-2,0,2],
[-1,0,1]]

sobel_y = [[-1,-2,-1],
[0,0,0],
[1,2,1]]

edge = ChunkyPNG::Image.new(img.width, img.height, ChunkyPNG::Color::TRANSPARENT)

for x in 1..img.width-2
for y in 1..img.height-2
pixel_x =	(sobel_x[0][0] * img.at(x-1,y-1)) + (sobel_x[0][1] * img.at(x,y-1)) + (sobel_x[0][2] * img.at(x+1,y-1)) +
(sobel_x[1][0] * img.at(x-1,y))   + (sobel_x[1][1] * img.at(x,y))   + (sobel_x[1][2] * img.at(x+1,y)) +
(sobel_x[2][0] * img.at(x-1,y+1)) + (sobel_x[2][1] * img.at(x,y+1)) + (sobel_x[2][2] * img.at(x+1,y+1))

pixel_y =	(sobel_y[0][0] * img.at(x-1,y-1)) + (sobel_y[0][1] * img.at(x,y-1)) + (sobel_y[0][2] * img.at(x+1,y-1)) +
(sobel_y[1][0] * img.at(x-1,y))   + (sobel_y[1][1] * img.at(x,y))   + (sobel_y[1][2] * img.at(x+1,y)) +
(sobel_y[2][0] * img.at(x-1,y+1)) + (sobel_y[2][1] * img.at(x,y+1)) + (sobel_y[2][2] * img.at(x+1,y+1))

val = Math.sqrt((pixel_x * pixel_x) + (pixel_y * pixel_y)).ceil
edge[x,y] = ChunkyPNG::Color.grayscale(val)
end
end

edge.save('engine_edge.png')

*/

bool sobel(CImage& in)
{
	uint8_t *pixels = in.getPixels();
	if (NULL != pixels)
	{
		size_t w = in.getWidth();
		size_t h = in.getHeight();

		float *Gx = new float[w * h];
		memset(Gx, 0, w * h * sizeof(float));
		float *Gy = new float[w * h];
		memset(Gy, 0, w * h * sizeof(float));

		for (size_t j = 1; j < w - 1; j++)
		{
			for (size_t i = 1; i < h - 1; i++)
			{
				int offset1 = (((i - 1) * w) + j) * 4;

				uint8_t r1 = pixels[offset1];
				uint8_t g1 = pixels[offset1 + 1];
				uint8_t b1 = pixels[offset1 + 2];

				int offset2 = ((i * w) + j) * 4;

				uint8_t r2 = pixels[offset2];
				uint8_t g2 = pixels[offset2 + 1];
				uint8_t b2 = pixels[offset2 + 2];

				int offset3 = (((i + 1) * w) + j) * 4;

				uint8_t r3 = pixels[offset3];
				uint8_t g3 = pixels[offset3 + 1];
				uint8_t b3 = pixels[offset3 + 2];


			}
		}


		CImage tmp;
		tmp.allocatePixels(w, h);
		uint8_t *pixels2 = tmp.getPixels();
		if (NULL != pixels2)
		{
			for (size_t i = 0; i < w * h * 4; i += 4)
			{
				uint8_t n = Gx[i];
				uint8_t m = Gy[i];

				pixels2[i] = n;
				pixels2[i + 1] = m;
				pixels2[i + 2] = sqrt(n*n + m*m);
				pixels2[i + 3] = 255;
			}
		}

		CImage::IImageIO *io = in.getImageKindIO("tga");
		io->storeImageFile("sobel.tga", &tmp);

		delete[] Gx;
		delete[] Gy;

		return true;
	}
	else
		return false;

	return false;
}


SCAN_API int resize_scan(CImage& image, size_t newW, size_t newH)
{
	uint8_t *pixels = image.getPixels();
	if (NULL != pixels)
	{
		//CImage resize;
		if (NULL == current_scan)
			current_scan = new CImage();
		current_scan->allocatePixels(newW, newH);
		uint8_t *dst = current_scan->getPixels();
		memset(dst, 255, newW * newH * 4);

		size_t w = image.getWidth();
		size_t h = image.getHeight();

		for (size_t j = 0; j < newH; j++)
		{
			for (size_t i = 0; i < newW; i++)
			{
				float dx = (float)i / (float)newW * float(w);
				float dy = (float)j * (float)h / (float)newH;

				uint16_t r = 0;
				uint16_t g = 0;
				uint16_t b = 0;
				uint16_t a = 0;

				int I = floor(dx);
				int J = floor(dy);
				float factx = (dx - I);
				float facty = (dy - J);
										
				int offset_src = ((J * w) + I) * 4;
				r += (1 - factx) * (1 - facty) * pixels[offset_src];
				g += (1 - factx) * (1 - facty) * pixels[offset_src + 1];
				b += (1 - factx) * (1 - facty) * pixels[offset_src + 2];
				a += (1 - factx) * (1 - facty) * pixels[offset_src + 3];
					
				if (I < w - 1)
				{
					offset_src = ((J * w) + I + 1) * 4;
					r += factx * (1 - facty) * pixels[offset_src];
					g += factx * (1 - facty) * pixels[offset_src + 1];
					b += factx * (1 - facty) * pixels[offset_src + 2];
					a += factx * (1 - facty) * pixels[offset_src + 3];
				}

				if (J < h - 1)
				{
					offset_src = (((J + 1) * w) + I) * 4;
					r += (1 - factx) * facty * pixels[offset_src];
					g += (1 - factx) * facty * pixels[offset_src + 1];
					b += (1 - factx) * facty * pixels[offset_src + 2];
					a += (1 - factx) * facty * pixels[offset_src + 3];
				}

				if ((J < h - 1) && (I < w - 1))
				{
					offset_src = (((J + 1) * w) + I + 1) * 4;
					r += factx * facty * pixels[offset_src];
					g += factx * facty * pixels[offset_src + 1];
					b += factx * facty * pixels[offset_src + 2];
					a += factx * facty * pixels[offset_src + 3];
				}

				int offset_dst = ((j * newW) + i) * 4;

				dst[offset_dst] = r;
				dst[offset_dst + 1] = g;
				dst[offset_dst + 2] = b;
				dst[offset_dst + 3] = a;
			}
		}

		//CImage::IImageIO *io = current_scan->getImageKindIO("jpg");
		//io->storeImageFile("resized.jpg", current_scan);

		return 1;
	}
	else
		return 0;
}

int translate_scan(int tx, int ty)
{
	if (NULL != current_scan)
	{
		uint8_t *pixels = current_scan->getPixels();

		if (NULL != pixels)
		{
			size_t w = current_scan->getWidth();
			size_t h = current_scan->getHeight();

			CImage image;
			image.allocatePixels(w, h);
			uint8_t *dst = image.getPixels();
			memset(dst, 255, w * h * 4);

			for (size_t j = 0; j < h; j++)
			{
				for (size_t i = 0; i < w; i++)
				{
					int offset_src = (((j + ty) * w) + (i - tx)) * 4;
					int offset_dst = ((j * w) + i) * 4;

					if ((offset_src > 0) && (offset_src < w*h * 4))
					{
						uint8_t r = pixels[offset_src];
						uint8_t g = pixels[offset_src + 1];
						uint8_t b = pixels[offset_src + 2];
						uint8_t a = pixels[offset_src + 3];

						dst[offset_dst] = r;
						dst[offset_dst + 1] = g;
						dst[offset_dst + 2] = b;
						dst[offset_dst + 3] = a;
					}
				}
			}

			memcpy(pixels, dst, w*h * 4);

			//CImage::IImageIO *io = image.getImageKindIO("jpg");
			//io->storeImageFile("translate.jpg", &image);

			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}


bool findLeftTopCorner(CImage& in, int &x, int &y)
{
	uint8_t *pixels = in.getPixels();
	if (NULL != pixels)
	{
		size_t w = in.getWidth();
		size_t h = in.getHeight();

		for (size_t j = MARGIN_LEFT; j < w - MARGIN_RIGHT; j++)
		{
			for (size_t i = MARGIN_TOP; i < h - MARGIN_BOTTOM; i++)
			{
				int offset = ((i * w) + j) * 4;

				uint8_t r = pixels[offset];
				uint8_t g = pixels[offset + 1];
				uint8_t b = pixels[offset + 2];

				if ((r <= TRESH) && (g <= TRESH) && (b <= TRESH))
				{
					x = j;
					//y = (h - i - 1);
					y = i;
					return true;
				}
			}
		}
		return false;
	}
	else
		return false;
}


int getPixelDiffs(uint8_t *pixels, size_t w, size_t h, float l1, int x2, int y2)
{
	int offset = (x2 + (w * y2)) * 4;

	uint8_t r = pixels[offset];
	uint8_t g = pixels[offset + 1];
	uint8_t b = pixels[offset + 2];
	CColor::RGBA c2 = CColor::RGBA((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);

	//float l1 = c1.operator raptor::CColor::YUVA().y;
	float l2 = c2.operator raptor::CColor::YUVA().y;

	return 255 * (l2 - l1);
}

bool checkIsLine(std::vector<int> &X, std::vector<int> &Y)
{
	size_t nb = X.size();
	if (nb != Y.size())
		return false;

	bool is_line = true;
	float deltaMax = 0.0f;

	float one_over_dx = 1.0f / (float)(X[nb - 1] - X[0]);
	int dy = Y[nb - 1] - Y[0];

	for (size_t i = 1; i < nb; i++)
	{
		float x = (float)(X[i] - X[0]);
		float y = (float)(Y[i] - Y[0]);

		float delta = fabs(x * dy * one_over_dx - y);
		deltaMax = max(delta, deltaMax);

		if (LINE_THRESHOLD < delta)
		{
			is_line = false;
			CRaptorErrorManager *mgr = Raptor::GetErrorManager();
			mgr->generateRaptorError(CPersistence::CPersistenceClassID::GetClassId(),
									 CRaptorErrorManager::RAPTOR_FATAL,
									 "Axe de ligne supérieur à la limite.");
		}
	}

	return is_line;
}

bool findLine(CImage& in, int x, int y, int &x2, int &y2)
{
	uint8_t *pixels = in.getPixels();
	if (NULL != pixels)
	{
		size_t w = in.getWidth();
		size_t h = in.getHeight();

		std::vector<int> X;
		std::vector<int> Y;
		X.push_back(x);
		Y.push_back(y);

		bool b = false;

		int cur_x = x;
		int cur_y = y;
		int prev_x = x;
		int prev_y = y;
		int dx[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };
		int dy[8] = { 0, 1, -1, 1, -1, 1, 0, -1 };

		int maxM = 0;

		while (!b)
		{
			int M = 255;
			int next_x = cur_x + 1;
			int next_y = y;
			int d[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

			int offset = (cur_x + (w * cur_y)) * 4;
			uint8_t r1 = pixels[offset];
			uint8_t g1 = pixels[offset + 1];
			uint8_t b1 = pixels[offset + 2];
			CColor::RGBA c1 = CColor::RGBA((float)r1 / 255.0f, (float)g1 / 255.0f, (float)b1 / 255.0f, 1.0f);
			float l1 = c1.operator raptor::CColor::YUVA().y;

			// do not go backwards: limit to 5 possible steps.
			for (size_t i = 0; i < 5; i++)
			{
				int sx = cur_x + dx[i];
				int sy = cur_y + dy[i];
				d[i] = getPixelDiffs(pixels, w, h, l1, sx, sy);
				if ((d[i] < M) && ((sx != prev_x) || (sy != prev_y)))
				{
					M = d[i];
					next_x = sx;
					next_y = sy;
				}
			}

			maxM = max(M, maxM);

			if ((M > 50) || (X.size() > in.getWidth()))
				b = true;
			else
			{
				//int offset = (cur_x + (w * cur_y)) * 4;
				//pixels[offset] = 255;
				//pixels[offset + 1] = 0;
				//pixels[offset + 2] = 0;

				prev_x = cur_x;
				prev_y = cur_y;
				cur_x = next_x;
				cur_y = next_y;
				X.push_back(cur_x);
				Y.push_back(cur_y);
			}
		}

		if (checkIsLine(X, Y))
		{
			x2 = X[X.size() - 1];
			y2 = Y[Y.size() - 1];
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

//
// Rotate Top Left/Right: angle is < 0
//
int rotate(CImage &in, CImage &out, float angle, float centerx, float centery)
{
	if (fabs(angle) > 10.0f * PI / 180.0f)
		return 0;

	uint8_t *pixels = in.getPixels();
	if (NULL != pixels)
	{
		size_t w = in.getWidth();
		size_t h = in.getHeight();

		out.releasePixels();
		out.allocatePixels(w, h);
		uint8_t *dst = out.getPixels();
		memset(dst, 255, w*h * 4);

		float C = cos(angle);
		float S = sin(angle);

		for (size_t j = 0; j < h; j++)
		{
			for (size_t i = 0; i < w; i++)
			{
				float x = (float)i - centerx;
				float y = (float)j - centery;
				
				float dX = (C * x - S * y + centerx);
				float dY = (S * x + C * y + centery);

				//	Todo: bilinear interpolation
				if ((dY >= 0) && (dY < h))
				{
					uint16_t r = 0;
					uint16_t g = 0;
					uint16_t b = 0;
					uint16_t a = 0;

					int I = floor(dX);
					int J = floor(dY);
					float factx = (dX - I);
					float facty = (dY - J);

					int offset_src = (((h - J - 1) * w) + I) * 4;
					r += (1 - factx) * (1 - facty) * pixels[offset_src];
					g += (1 - factx) * (1 - facty) * pixels[offset_src + 1];
					b += (1 - factx) * (1 - facty) * pixels[offset_src + 2];
					a += (1 - factx) * (1 - facty) * pixels[offset_src + 3];

					if (I < w - 1)
					{
						offset_src = (((h - J - 1) * w) + I + 1) * 4;
						r += factx * (1 - facty) * pixels[offset_src];
						g += factx * (1 - facty) * pixels[offset_src + 1];
						b += factx * (1 - facty) * pixels[offset_src + 2];
						a += factx * (1 - facty) * pixels[offset_src + 3];
					}

					if (J < h - 1)
					{
						offset_src = ((((h - J - 1) - 1) * w) + I) * 4;
						r += (1 - factx) * facty * pixels[offset_src];
						g += (1 - factx) * facty * pixels[offset_src + 1];
						b += (1 - factx) * facty * pixels[offset_src + 2];
						a += (1 - factx) * facty * pixels[offset_src + 3];
					}

					if ((J < h - 1) && (I < w - 1))
					{
						offset_src = ((((h - J - 1) - 1) * w) + I + 1) * 4;
						r += factx * facty * pixels[offset_src];
						g += factx * facty * pixels[offset_src + 1];
						b += factx * facty * pixels[offset_src + 2];
						a += factx * facty * pixels[offset_src + 3];
					}

					int offset_dst = (i + (w * (h - j - 1))) * 4;	// image is upside down
					dst[offset_dst] = r;
					dst[offset_dst + 1] = g;
					dst[offset_dst + 2] = b;
					dst[offset_dst + 3] = a;
				}
			}
		}

		return 1;
	}
	else
		return 0;
}

int histogram_scan(size_t px, size_t py, size_t width, size_t height)
{
	if (NULL != current_scan)
	{
		size_t w = current_scan->getWidth();
		size_t h = current_scan->getHeight();
		uint8_t *pixels = current_scan->getPixels();

		CImage normalized;
		normalized.allocatePixels(1700, 2338);
		uint8_t *dst = normalized.getPixels();
		memset(dst, 255, 1700 * 2338 * 4);

		for (size_t j = py; j < height; j++)
		{
			for (size_t i = px; i < width; i++)
			{
				int offset_src = (((h - j - 1) * w) + i) * 4;
				int offset_dst = (((2338 - j - 1) * 1700) + i) * 4;

				uint8_t r = 255;
				uint8_t g = 255;
				uint8_t b = 255;
				uint8_t a = 255;

				if ((offset_src > 0) && (offset_src < w * h * 4))
				{
					r = pixels[offset_src];
					g = pixels[offset_src + 1];
					b = pixels[offset_src + 2];
					//a = pixels[offset_src + 3];
				}

				if ((offset_dst > 0) && (offset_dst < 1700 * 2338 * 4))
				{
					r = (r > 230 ? 255 : r);
					g = (g > 230 ? 255 : g);
					b = (b > 230 ? 255 : b);
					dst[offset_dst] = ( r < 25 ? 0 : r);
					dst[offset_dst + 1] = (g < 25 ? 0 : g);
					dst[offset_dst + 2] = (b < 25 ? 0 : b);
					dst[offset_dst + 3] = a;
				}
			}
		}

		return (normalized.getImageKindIO("jpg")->storeImageFile("normalized.jpg", &normalized) ? 1 : 0);
	}
	else
		return 0;
}


SCAN_API int rectify_scan(const char* scan)
{
	CRaptorErrorManager *mgr = Raptor::GetErrorManager();

	CImage image;
	CImage::IImageOP::operation_param_t param;
	CVaArray<CImage::IImageOP::OP_KIND> iops;

	std::string scanfile(scan);
	if (image.loadImage(scanfile, iops, param))
	{
		int x = 0;
		int y = 0;

		//! Beware: y coordinate is reversed, image is upside down.
		if (findLeftTopCorner(image, x, y))
		{
			int rectify = 0;

			//sobel(image);
			int x2 = x;
			int y2 = y;
			if (findLine(image, x, y, x2, y2))
			{
				float d = sqrt((float)(y2 - y) * (float)(y2 - y) + (float)(x2 - x) * (float)(x2 - x));
				float fact = 1435.0 / d;
				if ((fact > 2.0f) || (fact < 0.5f))
					return 0;

				CImage result;

				if (y2 > y)
				{
					float angle = atan2((float)(y2 - y), (float)(x2 - x));
					rectify = rotate(image, result, - angle, 0, 0);
				}
				else
				{
					float angle = atan2((float)(y2 - y), (float)(x2 - x));
					rectify = rotate(image, result, angle, 0, 0);
				}

				if (1 == rectify)
				{
					rectify = resize_scan(result, fact * image.getWidth(), fact * image.getHeight()); //  1700, 2338))
					if (0 == rectify)
						std::cout << "Erreur de redimensionnement de scan" << std::endl;
				}

				if (1 == rectify)
				{
					int ty = 0;
					int tx = 0;

					float dy = (float)(image.getHeight() - y) / (float)image.getHeight();
					if ((dy > CORNER1_YMIN) && (dy < CORNER1_YMAX))
					{
						tx = 6 + floor(0.5f + CORNER1_X * (float)current_scan->getWidth() - fact * x);
						ty = 3 + floor(0.5f + CORNER1_Y * (float)current_scan->getHeight() - fact * (float)(image.getHeight() - y));
					}
					else if ((dy > CORNER2_YMIN) && (dy < CORNER2_YMAX))
					{
						tx = 7 + floor(0.5f + CORNER1_X * (float)current_scan->getWidth() - fact * x);
						ty = 4 + floor(0.5f + CORNER2_Y * (float)current_scan->getHeight() - fact * (float)(image.getHeight() - y));
					}
					else if ((dy > CORNER3_YMIN) && (dy < CORNER3_YMAX))
					{
						tx = 8 + floor(0.5f + CORNER1_X * (float)current_scan->getWidth() - fact * x);
						ty = 7 + floor(0.5f + CORNER3_Y * (float)current_scan->getHeight() - fact * (float)(image.getHeight() - y));
					}
					else
					{
						std::cout << "Format de page de scan non reconnu" << std::endl;
						rectify = 0;
					}
					
					if (1 == rectify)
						rectify = translate_scan(tx, ty);
					if (0 == rectify)
						std::cout << "Erreur de translation de scan" << std::endl;
				}

				if (1 == rectify)
				{
					rectify = histogram_scan(20, 20, 1500, 2200);
					if (0 == rectify)
						std::cout << "Erreur de normalisation de scan" << std::endl;
				}

				if (NULL != current_scan)
				{
					current_scan->releasePixels();
					delete current_scan;
					current_scan = NULL;
				}

				return rectify;
			}
			else
				return 0;
		}
		else
			return 0;
	}
	else
	{
		mgr->generateRaptorError(CPersistence::CPersistenceClassID::GetClassId(),
								 CRaptorErrorManager::RAPTOR_FATAL,
								 "Impossible d'ouvrir l'image source");
		return 0;
	}
}
